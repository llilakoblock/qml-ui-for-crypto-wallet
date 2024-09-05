#pragma once

#include <QQmlEngine>
#include <QSharedDataPointer>
#include <QStack>

namespace Data {

class SwapLogData;

class SwapLog
{
    Q_GADGET
    QML_VALUE_TYPE(swapLog)

    Q_PROPERTY(State state READ state CONSTANT FINAL)
    Q_PROPERTY(QString text READ text CONSTANT FINAL)
    Q_PROPERTY(QDateTime time READ time CONSTANT FINAL)

public:
    enum class State
    {
        UnknownState,
        InProgressState,
        DoneState,
        FailedState
    };

public:
    explicit SwapLog();

    SwapLog(SwapLog&& other) noexcept;
    SwapLog(const SwapLog& other) noexcept;
    ~SwapLog();

    SwapLog& operator= (const SwapLog& other) noexcept;
    bool     operator== (const SwapLog& other) const;

    State state() const;
    void  setState(State state);

    QString text() const;
    void    setText(const QString& text);

    QDateTime time() const;
    void      setTime(const QDateTime& time);

private:
    QSharedDataPointer<SwapLogData> data;
};

class SwapLogDerived : public SwapLog
{
    Q_GADGET
};

namespace SwapLogDerivedForeign {
Q_NAMESPACE
QML_NAMED_ELEMENT(SwapLog)
QML_FOREIGN_NAMESPACE(Data::SwapLogDerived)
}; // namespace SwapLogDerivedForeign

} // namespace Data

Q_DECLARE_TYPEINFO(Data::SwapLog, Q_MOVABLE_TYPE);
