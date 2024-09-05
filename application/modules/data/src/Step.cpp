#include "data/Step.h"

#include "StepData.h"

namespace Data {

Step::Step()
    : data(new StepData)
{
}

Step::Step(Step&& other) noexcept      = default;
Step::Step(const Step& other) noexcept = default;
Step::~Step()                          = default;

Step& Step::operator= (const Step& other) noexcept
{
    if (this != &other)
    {
        data = other.data;
    }

    return *this;
}

bool Step::operator== (const Step& other) const
{
    return data == other.data || *data == *other.data;
}

Step::Screen Step::screen() const
{
    return data->screen;
}

void Step::setScreen(Screen screen)
{
    data->screen = screen;
}

Step::Direction Step::direction() const
{
    return data->direction;
}

void Step::setDirection(Direction direction)
{
    data->direction = direction;
}

QStack<Step::Screen> Step::screensStack() const
{
    return data->screensStack;
}

void Step::setScreensStack(const QStack<Screen>& screensStack)
{
    data->screensStack = screensStack;
}

Step::Errors Step::errors() const
{
    return data->errors;
}

void Step::setErrors(Errors errors)
{
    data->errors = errors;
}

Step::RestorationMethod Step::restorationMethod() const
{
    return data->restorationMethod;
}

void Step::setRestorationMethod(RestorationMethod method)
{
    data->restorationMethod = method;
}

QString Step::password() const
{
    return data->password;
}

void Step::setPassword(const QString& password)
{
    data->password = password;
}

QList<QString> Step::mnemonicPassphrase() const
{
    return data->mnemonicPassphrase;
}

void Step::setMnemonicPassphrase(const QList<QString>& mnemonicPassphrase)
{
    data->mnemonicPassphrase = mnemonicPassphrase;
}

std::vector<unsigned char> Step::encryptedSeed() const
{
    return data->encryptedSeed;
}

void Step::setEncryptedSeed(const std::vector<unsigned char>& encryptedSeed)
{
    data->encryptedSeed = encryptedSeed;
}

QString Step::inheritanceKey() const
{
    return data->inheritanceKey;
}

void Step::setInheritanceKey(const QString& key)
{
    data->inheritanceKey = key;
}

Step::InheritanceKeyErrors Step::inheritanceErrors() const
{
    return data->inheritanceErrors;
}

void Step::setInheritanceErrors(InheritanceKeyErrors error)
{
    data->inheritanceErrors = error;
}

} // namespace Data
