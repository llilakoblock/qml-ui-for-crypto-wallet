#pragma once

#include <QQmlEngine>
#include <QSharedDataPointer>
#include <QStack>

namespace Data {

class StepData;

class Step
{
    Q_GADGET
    QML_VALUE_TYPE(step)

    Q_PROPERTY(Screen screen READ screen CONSTANT FINAL)
    Q_PROPERTY(Direction direction READ direction CONSTANT FINAL)

    Q_PROPERTY(QStack<Screen> screensStack READ screensStack CONSTANT FINAL)

    Q_PROPERTY(Errors errors READ errors CONSTANT FINAL)

    Q_PROPERTY(RestorationMethod restorationMethod READ restorationMethod CONSTANT FINAL)

    Q_PROPERTY(InheritanceKeyErrors inheritanceErrors READ inheritanceErrors CONSTANT FINAL)
    Q_PROPERTY(QString inheritanceKey READ inheritanceKey CONSTANT FINAL)

    Q_PROPERTY(QString password READ password CONSTANT FINAL)
    Q_PROPERTY(QList<QString> mnemonicPassphrase READ mnemonicPassphrase CONSTANT FINAL)

public:
    enum class Screen
    {
        Unknown,
        Unlock,
        Welcome,
        CreatePassword,
        ShowMnemonicPassphrase,
        CheckMnemonicPassphrase,
        SuggestBiometricCreation,
        BiometricCreation,
        SuggestInheritance,
        Inheritance,
        SuggestRecoveringVariants,
        RecoverFromMnemonicPassphrase,
        RecoverFromBiometric,
        RecoverFromInheritance,
    };
    Q_ENUM(Screen)

    enum class Direction
    {
        UnknownDirection,
        ForwardDirection,
        BackwardDirection,
    };
    Q_ENUM(Direction)

    enum class RestorationMethod
    {
        Unknown,
        FromMnemonicPassphrase,
        FromInheritance,
        FromBiometric,
    };
    Q_ENUM(RestorationMethod)

    enum class Error
    {
        None                      = 0,
        EmptyPassword             = 1 << 0,
        InvalidPassword           = 1 << 1,
        InvalidMnemonicPassphrase = 1 << 2,
        InvalidKeyFilePath        = 1 << 3,
        InvalidKeyFile            = 1 << 4,
        InvalidKeyFilePassword    = 1 << 5,
        InvalidBiometricData      = 1 << 6,
        WrongStep                 = 1 << 7,
        WrongData                 = 1 << 8,
        DatabaseError             = 1 << 9,
        PasswordAlreadyExists     = 1 << 10
    };
    Q_DECLARE_FLAGS(Errors, Error)
    Q_FLAG(Error)

    enum class InheritanceKeyError
    {
        None                = 0,
        InheritanceNotSetup = 1 << 0,
        KeyNotMatureError   = 1 << 1,
        KeyEmptyError       = 1 << 2,
        KeyFormatError      = 1 << 3,
    };
    Q_DECLARE_FLAGS(InheritanceKeyErrors, InheritanceKeyError)
    Q_FLAG(InheritanceKeyError)

public:
    explicit Step();

    Step(Step&& other) noexcept;
    Step(const Step& other) noexcept;
    ~Step();

    Step& operator= (const Step& other) noexcept;
    bool  operator== (const Step& other) const;

    Screen screen() const;
    void   setScreen(Screen screen);

    Direction direction() const;
    void      setDirection(Direction direction);

    QStack<Screen> screensStack() const;
    void           setScreensStack(const QStack<Screen>& screensStack);

    Errors errors() const;
    void   setErrors(Errors errors);

    RestorationMethod restorationMethod() const;
    void              setRestorationMethod(RestorationMethod method);

    QString password() const;
    void    setPassword(const QString& password);

    QList<QString> mnemonicPassphrase() const;
    void           setMnemonicPassphrase(const QList<QString>& mnemonicPassphrase);

    std::vector<unsigned char> encryptedSeed() const;
    void                       setEncryptedSeed(const std::vector<unsigned char>& encryptedSeed);

    QString inheritanceKey() const;
    void    setInheritanceKey(const QString& key);

    InheritanceKeyErrors inheritanceErrors() const;
    void                 setInheritanceErrors(InheritanceKeyErrors error);

private:
    QSharedDataPointer<StepData> data;
};

class StepDerived : public Step
{
    Q_GADGET
};

namespace StepDerivedForeign {
Q_NAMESPACE
QML_NAMED_ELEMENT(Step)
QML_FOREIGN_NAMESPACE(Data::StepDerived)
}; // namespace StepDerivedForeign

} // namespace Data

Q_DECLARE_OPERATORS_FOR_FLAGS(Data::Step::Errors)
Q_DECLARE_TYPEINFO(Data::Step, Q_MOVABLE_TYPE);
