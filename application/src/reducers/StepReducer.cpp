#include "StepReducer.h"

#include "singletones/Store.h"
#include "entropyencryptor.h"
#include "inheritance.h"
#include "keyfile.h"
#include "keygenerator.h"
#include "misc/Visitor.h"
#include "freedomdatabase.h"

namespace XWallet::Reducers {

Data::Step StepReducer::reduce(const Data::Step& step, const StepActions::Action& action)
{
    const auto& visitor = Visitor {
        [step](StepActions::SetInitScreen action) {
        auto newStep = step;

        auto screensStack = step.screensStack();
        screensStack.clear();
        screensStack.push(action.screen);

        newStep.setScreensStack(screensStack);
        newStep.setScreen(screensStack.top());
        newStep.setDirection(Data::Step::Direction::ForwardDirection);

        return newStep;
    },
        [step](const StepActions::FinishScreen& action) {
        return finishScreen(step, action);
    },
        [step](StepActions::NextScreen action) {
        auto newStep = step;
        newStep.setDirection(Data::Step::Direction::UnknownDirection);

        qsizetype index = step.screensStack().indexOf(step.screen());

        if (index < 0 || index + 1 >= step.screensStack().count())
        {
            return newStep;
        }

        newStep.setScreen(step.screensStack().at(index + 1));
        newStep.setDirection(Data::Step::Direction::ForwardDirection);

        return newStep;
    },
        [step](StepActions::PreviousScreen action) {
        auto newStep = step;
        newStep.setDirection(Data::Step::Direction::UnknownDirection);

        qsizetype index = step.screensStack().indexOf(step.screen());

        if (index <= 0)
        {
            return newStep;
        }

        newStep.setScreen(step.screensStack().at(index - 1));
        newStep.setDirection(Data::Step::Direction::BackwardDirection);

        return newStep;
    },

    };

    auto result = std::visit(visitor, action);

    return result;
}

Data::Step StepReducer::finishScreen(const Data::Step& step, const StepActions::FinishScreen& action)
{
    Data::Step newStep = step;

    newStep.setDirection(Data::Step::Direction::UnknownDirection);

    const auto& pushNextScreenOnStack = [&newStep, action]() {
        auto screensStack = newStep.screensStack();

        screensStack.resize(screensStack.indexOf(action.currentScreen) + 1);
        screensStack.push(action.nextScreen);

        newStep.setScreensStack(screensStack);
        newStep.setScreen(screensStack.top());
        newStep.setDirection(Data::Step::Direction::ForwardDirection);
    };

    const auto& setNextScreenOnCleanStack = [&newStep, action]() {
        QStack<Data::Step::Screen> screensStack;

        screensStack.push(action.nextScreen);

        newStep.setScreensStack(screensStack);
        newStep.setScreen(screensStack.top());
        newStep.setDirection(Data::Step::Direction::ForwardDirection);
    };

    Data::Step::Errors               stepErrors        = Data::Step::Error::None;
    Data::Step::InheritanceKeyErrors inheritanceErrors = Data::Step::InheritanceKeyError::None;

    switch (action.currentScreen)
    {
        case Data::Step::Screen::Unknown:
        {
            qCritical() << Q_FUNC_INFO << "Current screen can't be unknown";

            stepErrors.setFlag(Data::Step::Error::WrongStep);
            break;
        }
        case Data::Step::Screen::Unlock:
        {
            if (std::holds_alternative<StepActions::PasswordData>(action.stepData))
            {
                const auto& data = std::get<StepActions::PasswordData>(action.stepData);

                if (data.password.isEmpty())
                {
                    stepErrors.setFlag(Data::Step::Error::EmptyPassword);
                }
                else if (!KeyFile::LoadKeyFile(data.password))
                {
                    stepErrors.setFlag(Data::Step::Error::InvalidPassword);
                }
                else
                {
                    Store::instance().dispatch(WalletServiceActions::UnlockWallet {});

                    InheritanceKeepAlivePing::Ping();
                }
            }
            else
            {
                stepErrors.setFlag(Data::Step::Error::WrongData);
            }

            break;
        }
        case Data::Step::Screen::Welcome:
        {
            if (action.nextScreen == Data::Step::Screen::CreatePassword ||
                action.nextScreen == Data::Step::Screen::SuggestRecoveringVariants)
            {
                newStep.setRestorationMethod(Data::Step::RestorationMethod::Unknown);

                pushNextScreenOnStack();
            }
            else
            {
                stepErrors.setFlag(Data::Step::Error::WrongStep);
            }

            break;
        }
        case Data::Step::Screen::CreatePassword:
        {
            if (std::holds_alternative<StepActions::PasswordData>(action.stepData))
            {
                const auto& data = std::get<StepActions::PasswordData>(action.stepData);

                if (data.password.isEmpty())
                {
                    stepErrors.setFlag(Data::Step::Error::EmptyPassword);
                    break;
                }

                freedom::core::database::FreedomDatabase database;
                const std::string hashedUserPw = QCryptographicHash::hash(data.password.toStdString(), QCryptographicHash::Md5).toHex().toStdString();

                if(database.isUserPwDuplicate(hashedUserPw))
                {
                    stepErrors.setFlag(Data::Step::Error::PasswordAlreadyExists);
                    break;
                }

                QString mnemonicPassphrase;

                switch (step.restorationMethod())
                {
                    case Data::Step::RestorationMethod::FromBiometric:
                    {
                        break;
                    }
                    case Data::Step::RestorationMethod::FromInheritance:
                    {
                        mnemonicPassphrase = InheritanceUnlocker::Unlock(step.inheritanceKey());

                        if (!KeyFile::SaveRestoredKeyFile(data.password, mnemonicPassphrase, true))
                        {
                            stepErrors.setFlag(Data::Step::Error::InvalidPassword);
                            stepErrors.setFlag(Data::Step::Error::InvalidMnemonicPassphrase);
                        }

                        break;
                    }
                    case Data::Step::RestorationMethod::FromMnemonicPassphrase:
                    {
                        mnemonicPassphrase = step.mnemonicPassphrase().join(QStringLiteral(" "));

                        if (!KeyFile::SaveRestoredKeyFile(data.password, mnemonicPassphrase, true))
                        {
                            stepErrors.setFlag(Data::Step::Error::InvalidPassword);
                            stepErrors.setFlag(Data::Step::Error::InvalidMnemonicPassphrase);
                        }

                        break;
                    }
                    case Data::Step::RestorationMethod::Unknown:
                    {
                        if (!KeyFile::SaveNewKeyFile(data.password, true))
                        {
                            stepErrors.setFlag(Data::Step::Error::InvalidPassword);
                            break;
                        }

                        mnemonicPassphrase = QString::fromStdString(freedom::KeyGenerator::Instance().GetPrivateData().mnemonic);

                        break;
                    }
                    default:
                    {
                        qWarning() << Q_FUNC_INFO << "Unknown restoration method:" << step.restorationMethod();

                        stepErrors.setFlag(Data::Step::Error::WrongData);

                        break;
                    }
                }

                if (stepErrors)
                {
                    break;
                }

                if (action.nextScreen == Data::Step::Screen::ShowMnemonicPassphrase ||
                    action.nextScreen == Data::Step::Screen::SuggestBiometricCreation)
                {
                    QList<QString> mnemonicPassphraseList = mnemonicPassphrase.split(QStringLiteral(" "), Qt::SkipEmptyParts);

                    std::string mnemonicPassphraseData = mnemonicPassphrase.toStdString();

                    std::vector<unsigned char> passphrase(mnemonicPassphraseData.begin(), mnemonicPassphraseData.end());

                    const auto& encryptedPassphrase = freedom::EntropyEncryptor::Encrypt(passphrase, data.password.toStdString());

                    newStep.setPassword(data.password);
                    newStep.setMnemonicPassphrase(mnemonicPassphraseList);
                    newStep.setEncryptedSeed(encryptedPassphrase);

                    if (action.nextScreen == Data::Step::Screen::ShowMnemonicPassphrase)
                    {
                        pushNextScreenOnStack();
                    }
                    else
                    {
                        setNextScreenOnCleanStack();
                    }
                }
                else
                {
                    stepErrors.setFlag(Data::Step::Error::WrongStep);
                }
            }
            else
            {
                stepErrors.setFlag(Data::Step::Error::WrongData);
            }

            break;
        }
        case Data::Step::Screen::ShowMnemonicPassphrase:
        {
            if (action.nextScreen == Data::Step::Screen::CheckMnemonicPassphrase)
            {
                pushNextScreenOnStack();
            }
            else
            {
                stepErrors.setFlag(Data::Step::Error::WrongStep);
            }

            break;
        }
        case Data::Step::Screen::CheckMnemonicPassphrase:
        {
            if (action.nextScreen == Data::Step::Screen::SuggestBiometricCreation ||
                action.nextScreen == Data::Step::Screen::SuggestInheritance)
            {
                setNextScreenOnCleanStack();
            }
            else
            {
                stepErrors.setFlag(Data::Step::Error::WrongStep);
            }

            break;
        }
        case Data::Step::Screen::SuggestBiometricCreation:
        {
            if (action.nextScreen == Data::Step::Screen::BiometricCreation)
            {
                pushNextScreenOnStack();
            }
            else if (action.nextScreen == Data::Step::Screen::SuggestInheritance)
            {
                setNextScreenOnCleanStack();
            }
            else if (action.nextScreen == Data::Step::Screen::Unknown)
            {
                Store::instance().dispatch(WalletServiceActions::UnlockWallet {});
            }
            else
            {
                stepErrors.setFlag(Data::Step::Error::WrongStep);
            }

            break;
        }
        case Data::Step::Screen::BiometricCreation:
        {
            freedom::core::database::FreedomDatabase database;

            const std::string hashedUserPw = QCryptographicHash::hash(step.password().toStdString(), QCryptographicHash::Md5).toHex().toStdString();

            if (!database.addBiometricData( hashedUserPw, step.encryptedSeed()))
            {
                stepErrors.setFlag(Data::Step::Error::DatabaseError);
                break;
            }

            if (action.nextScreen == Data::Step::Screen::SuggestInheritance)
            {
                setNextScreenOnCleanStack();
            }
            else if (action.nextScreen == Data::Step::Screen::Unknown)
            {
                Store::instance().dispatch(WalletServiceActions::UnlockWallet {});
            }
            else
            {
                stepErrors.setFlag(Data::Step::Error::WrongStep);
            }

            break;
        }
        case Data::Step::Screen::SuggestInheritance:
        {
            if (action.nextScreen == Data::Step::Screen::Inheritance)
            {
                QString inheritanceKey = InheritanceMaker::MakeNewInheritanceKey();

                newStep.setInheritanceKey(inheritanceKey);

                pushNextScreenOnStack();
            }
            else if (action.nextScreen == Data::Step::Screen::Unknown)
            {
                Store::instance().dispatch(WalletServiceActions::UnlockWallet {});
            }
            else
            {
                stepErrors.setFlag(Data::Step::Error::WrongStep);
            }

            break;
        }
        case Data::Step::Screen::Inheritance:
        {
            if (std::holds_alternative<StepActions::InheritanceInactivityDurationData>(action.stepData))
            {
                const auto& data = std::get<StepActions::InheritanceInactivityDurationData>(action.stepData);

                MaxInactivityDuration maxInactivityDuration = static_cast<MaxInactivityDuration>(data.maxInactivityDuration);

                InheritanceMaker::SetMaxInactivityDuration(maxInactivityDuration);
                InheritanceMaker::RegisterInheritance();

                // Prepare next inheritance key in case user chooses to make another inheritance
                QString inheritanceKey = InheritanceMaker::MakeNewInheritanceKey();

                newStep.setInheritanceKey(inheritanceKey);

                if (action.nextScreen == Data::Step::Screen::Unknown)
                {
                    Store::instance().dispatch(WalletServiceActions::UnlockWallet {});
                }
                else if (action.nextScreen == Data::Step::Screen::Inheritance)
                {
                    // Handle multiple iheritance cases
                    QString inheritanceKey = InheritanceMaker::MakeNewInheritanceKey();
                    newStep.setInheritanceKey(inheritanceKey);

                    pushNextScreenOnStack();
                }
                else
                {
                    stepErrors.setFlag(Data::Step::Error::WrongStep);
                }
            }
            else
            {
                stepErrors.setFlag(Data::Step::Error::WrongData);
                break;
            }

            break;
        }
        case Data::Step::Screen::SuggestRecoveringVariants:
        {
            if (action.nextScreen == Data::Step::Screen::RecoverFromMnemonicPassphrase ||
                action.nextScreen == Data::Step::Screen::RecoverFromBiometric ||
                action.nextScreen == Data::Step::Screen::RecoverFromInheritance)
            {
                pushNextScreenOnStack();
            }
            else
            {
                stepErrors.setFlag(Data::Step::Error::WrongStep);
            }

            break;
        }
        case Data::Step::Screen::RecoverFromMnemonicPassphrase:
        {
            if (std::holds_alternative<StepActions::PassphraseData>(action.stepData))
            {
                const auto& data = std::get<StepActions::PassphraseData>(action.stepData);

                QString mnemonicPassphrase = data.passphrase.join(QStringLiteral(" "));

                if (mnemonicPassphrase.isEmpty() || !freedom::KeyGenerator::Instance().IsMnemonicValid(mnemonicPassphrase.toStdString()))
                {
                    stepErrors.setFlag(Data::Step::Error::InvalidMnemonicPassphrase);
                }

                if (stepErrors)
                {
                    break;
                }

                newStep.setMnemonicPassphrase(data.passphrase);

                if (action.nextScreen == Data::Step::Screen::CreatePassword)
                {
                    newStep.setRestorationMethod(Data::Step::RestorationMethod::FromMnemonicPassphrase);

                    pushNextScreenOnStack();
                }
                else
                {
                    stepErrors.setFlag(Data::Step::Error::WrongStep);
                }
            }
            else
            {
                stepErrors.setFlag(Data::Step::Error::WrongData);
            }

            break;
        }
        case Data::Step::Screen::RecoverFromBiometric:
        {
            if (std::holds_alternative<StepActions::PasswordData>(action.stepData))
            {
                const auto& data = std::get<StepActions::PasswordData>(action.stepData);

                if (data.password.isEmpty())
                {
                    stepErrors.setFlag(Data::Step::Error::EmptyPassword);
                    break;
                }

                freedom::core::database::FreedomDatabase database;
                std::optional<std::vector<std::vector<unsigned char>>> encryptedPassphrases;

                const std::string hashedUserPw = QCryptographicHash::hash(data.password.toStdString(), QCryptographicHash::Md5).toHex().toStdString();

                if (!database.getBiometricData(hashedUserPw, encryptedPassphrases))
                {
                    stepErrors.setFlag(Data::Step::Error::DatabaseError);
                    break;
                }

                if (!encryptedPassphrases.has_value())
                {
                    stepErrors.setFlag(Data::Step::Error::DatabaseError);
                    break;
                }

                const auto& decryptedPassphraseVector = freedom::EntropyEncryptor::Decrypt(encryptedPassphrases.value().back(), data.password.toStdString());

                auto decryptedPassphrase = QString::fromStdString(std::string(decryptedPassphraseVector.begin(), decryptedPassphraseVector.end()));
                decryptedPassphrase.remove('\0');

                if (decryptedPassphrase.isEmpty())
                {
                    stepErrors.setFlag(Data::Step::Error::InvalidBiometricData);
                    break;
                }

                if (!freedom::KeyGenerator::Instance().IsMnemonicValid(decryptedPassphrase.toStdString()))
                {
                    stepErrors.setFlag(Data::Step::Error::InvalidMnemonicPassphrase);
                    break;
                }

                newStep.setMnemonicPassphrase(decryptedPassphrase.split(QStringLiteral(" "), Qt::SkipEmptyParts));

                if (!KeyFile::SaveRestoredKeyFile(data.password, decryptedPassphrase, true))
                {
                    stepErrors.setFlag(Data::Step::Error::InvalidPassword);
                }
                else
                {
                    if (action.nextScreen == Data::Step::Screen::ShowMnemonicPassphrase)
                    {
                        newStep.setRestorationMethod(Data::Step::RestorationMethod::FromBiometric);

                        pushNextScreenOnStack();
                    }
                    else
                    {
                        stepErrors.setFlag(Data::Step::Error::WrongStep);
                    }
                }
            }
            else
            {
                stepErrors.setFlag(Data::Step::Error::WrongData);
            }

            break;
        }
        case Data::Step::Screen::RecoverFromInheritance:
        {
            if (std::holds_alternative<StepActions::InheritanceKeyData>(action.stepData))
            {
                const auto& data = std::get<StepActions::InheritanceKeyData>(action.stepData);

                InheritanceValidator::ValidationResult keyValidationResult = InheritanceValidator::ValidateInheritanceKey(data.key);

                if (data.key.isEmpty())
                {
                    inheritanceErrors.setFlag(Data::Step::InheritanceKeyError::KeyEmptyError);
                    break;
                }

                switch (keyValidationResult)
                {
                    case InheritanceValidator::ValidationResult::KeyFormatError:
                        inheritanceErrors.setFlag(Data::Step::InheritanceKeyError::KeyFormatError);
                        break;
                    case InheritanceValidator::ValidationResult::NotReadyToInherit:
                        inheritanceErrors.setFlag(Data::Step::InheritanceKeyError::KeyNotMatureError);
                        break;
                    case InheritanceValidator::ValidationResult::InheritanceNotSetup:
                        inheritanceErrors.setFlag(Data::Step::InheritanceKeyError::InheritanceNotSetup);
                        break;
                    case InheritanceValidator::ValidationResult::ReadyToInherit:
                        newStep.setInheritanceKey(data.key);
                        break;
                    default:
                        qWarning() << Q_FUNC_INFO << "Unhandled inheritance key validation result" << static_cast<int>(keyValidationResult);
                        break;
                }
            }
            else
            {
                stepErrors.setFlag(Data::Step::Error::WrongData);
            }

            if (stepErrors || inheritanceErrors)
            {
                break;
            }

            if (action.nextScreen == Data::Step::Screen::CreatePassword)
            {
                newStep.setRestorationMethod(Data::Step::RestorationMethod::FromInheritance);

                pushNextScreenOnStack();
            }
            else
            {
                stepErrors.setFlag(Data::Step::Error::WrongStep);
            }

            break;
        }
        default:
        {
            qCritical() << Q_FUNC_INFO << "Unhandled current screen branch:" << action.currentScreen;

            stepErrors.setFlag(Data::Step::Error::WrongStep);

            break;
        }
    }

    newStep.setErrors(stepErrors);
    newStep.setInheritanceErrors(inheritanceErrors);

    return newStep;
}

} // namespace XWallet::Reducers
