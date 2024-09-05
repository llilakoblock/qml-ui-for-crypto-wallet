#pragma once

#include <QQmlEngine>
#include <QSharedDataPointer>

namespace Data {

class ApplicationData;

class Application
{
    Q_GADGET
    QML_VALUE_TYPE(application)

    Q_PROPERTY(State state READ state CONSTANT FINAL)

public:
    enum class State
    {
        Unknown,
        Initializing,
        Loading,
        Start,
        Ready,
        Error,
        Closing
    };
    Q_ENUM(State)

public:
    explicit Application();

    Application(Application&& other) noexcept;
    Application(const Application& other) noexcept;
    ~Application();

    Application& operator= (const Application& other) noexcept;
    bool         operator== (const Application& other) const;

    State state() const;
    void  setState(State state);

private:
    QSharedDataPointer<ApplicationData> data;
};

class ApplicationDerived : public Application
{
    Q_GADGET
};

namespace ApplicationDerivedForeign {
Q_NAMESPACE
QML_NAMED_ELEMENT(WalletApplication)
QML_FOREIGN_NAMESPACE(Data::ApplicationDerived)
}; // namespace ApplicationDerivedForeign

} // namespace Data

Q_DECLARE_TYPEINFO(Data::Application, Q_MOVABLE_TYPE);
