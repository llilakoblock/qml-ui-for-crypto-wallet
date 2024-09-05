#pragma once

#include <QFuture>
#include <QObject>

class QThreadPool;
class QVersionNumber;
class QSqlQuery;
class QSqlDatabase;

namespace XWallet::Services {

struct DatabaseServicePrivate;

class DatabaseService final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(DatabaseService)

public:
    explicit DatabaseService(QObject* parent = nullptr);
    ~DatabaseService() override;

    static QString   datetimeToString(const QDateTime& datetime);
    static QDateTime stringToDatetime(const QString& string);

    QFuture<bool> establishConnection(const QString& databasePath);

    QFuture<QVariantMap>        loadItem(const QString& table, const QStringList& selections, const QVariantMap& bindables = {}) const;
    QFuture<QList<QVariantMap>> loadItemsList(const QString& table, const QStringList& selections, const QVariantMap& bindables = {}) const;

    QFuture<bool> upsertItem(const QString& table, const QList<QString>& indexedColumns, const QVariantMap& itemMap) const;
    QFuture<bool> upsertItemsList(const QString& table, const QList<QString>& indexedColumns, const QList<QVariantMap>& itemsList) const;

    QFuture<bool> replaceItem(const QString& table, const QVariantMap& itemMap) const;
    QFuture<bool> replaceItemsList(const QString& table, const QList<QVariantMap>& itemsList) const;

    QFuture<bool> removeItem(const QString& table, const QVariantMap& bindables = {}) const;
    QFuture<bool> removeItems(const QString& table, const QVariantMap& andBindables = {}, const QMultiMap<QString, QVariant>& orBindables = {}) const;

    QFuture<bool> clearTable(const QString& table) const;

private:
    QVersionNumber databaseVersion() const;

    bool checkAndApplyMigrations() const;

    bool applyScheme(const QString& schemePath) const;

    std::optional<QSqlQuery> prepareQuery(const QString& sqlQuery) const;

private:
    QScopedPointer<DatabaseServicePrivate> d;

    QScopedPointer<QThreadPool> m_threadPool;
};

} // namespace XWallet::Services
