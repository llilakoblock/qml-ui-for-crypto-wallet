#pragma once

#include "Currency.h"

#include <QQmlEngine>
#include <QSharedDataPointer>

namespace Data {

class HistoryEntryData;

class HistoryEntry
{
    Q_GADGET
    QML_VALUE_TYPE(historyEntry)

    Q_PROPERTY(Direction direction READ direction CONSTANT FINAL)

    Q_PROPERTY(Data::Currency currency READ currency CONSTANT FINAL)

    Q_PROPERTY(QString hash READ hash CONSTANT FINAL)
    Q_PROPERTY(QString amount READ amount CONSTANT FINAL)
    Q_PROPERTY(QString fee READ fee CONSTANT FINAL)

    Q_PROPERTY(QList<QString> from READ from CONSTANT FINAL)
    Q_PROPERTY(QList<QString> to READ to CONSTANT FINAL)

    Q_PROPERTY(QDateTime timestamp READ timestamp CONSTANT FINAL)

    Q_PROPERTY(QString note READ note CONSTANT FINAL)

    Q_PROPERTY(bool success READ success CONSTANT FINAL)

public:
    enum class Direction : quint8
    {
        Unknown,
        Income,
        Outgo,
    };
    Q_ENUM(Direction)

public:
    explicit HistoryEntry();

    HistoryEntry(HistoryEntry&& other) noexcept;
    HistoryEntry(const HistoryEntry& other) noexcept;
    ~HistoryEntry();

    HistoryEntry& operator= (const HistoryEntry& other) noexcept;
    bool          operator== (const HistoryEntry& other) const;

    Direction direction() const;
    void      setDirection(Direction direction);

    Data::Currency currency() const;
    void           setCurrency(const Data::Currency& currency);

    QString hash() const;
    void    setHash(const QString& hash);

    QString amount() const;
    void    setAmount(const QString& amount);

    QString fee() const;
    void    setFee(const QString& fee);

    QList<QString> from() const;
    void           setFrom(const QList<QString>& from);

    QList<QString> to() const;
    void           setTo(const QList<QString>& to);

    QDateTime timestamp() const;
    void      setTimestamp(const QDateTime& timestamp);

    QString note() const;
    void    setNote(const QString& note);

    bool success() const;
    void setSuccess(bool success);

private:
    QSharedDataPointer<HistoryEntryData> data;
};

class HistoryEntryDerived : public HistoryEntry
{
    Q_GADGET
};

namespace HistoryEntryDerivedForeign {
Q_NAMESPACE
QML_NAMED_ELEMENT(HistoryEntry)
QML_FOREIGN_NAMESPACE(Data::HistoryEntryDerived)
}; // namespace HistoryEntryDerivedForeign

} // namespace Data

Q_DECLARE_TYPEINFO(Data::HistoryEntry, Q_MOVABLE_TYPE);
