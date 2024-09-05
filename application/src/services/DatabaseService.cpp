#include "DatabaseService.h"

#include <QLoggingCategory>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtConcurrent>

Q_DECLARE_LOGGING_CATEGORY(database)
Q_LOGGING_CATEGORY(database, "database")

namespace XWallet::Services {

using namespace Qt::Literals::StringLiterals;

constexpr auto DriverName   = "QSQLITE"_L1;
constexpr auto DatabaseName = "ApplicationDatabase"_L1;

constexpr auto DatabaseInitialSchemeFile = ":/database/initial/database.sql"_L1;
constexpr auto DatabaseMigrationsPath    = ":/database/migrations"_L1;

constexpr auto DatabaseDateTimeFormat = "yyyy-MM-dd HH:mm:ss.zzz t"_L1;

struct DatabaseServicePrivate
{
    QSqlDatabase              db;
    QHash<QString, QSqlQuery> cache;
};

DatabaseService::DatabaseService(QObject* parent)
    : QObject(parent)
    , d(new DatabaseServicePrivate)
    , m_threadPool(new QThreadPool)
{
    m_threadPool->setMaxThreadCount(1);
    m_threadPool->setExpiryTimeout(-1);
}

DatabaseService::~DatabaseService() = default;

QString DatabaseService::datetimeToString(const QDateTime& datetime)
{
    return datetime.toString(DatabaseDateTimeFormat);
}

QDateTime DatabaseService::stringToDatetime(const QString& string)
{
    return QDateTime::fromString(string, DatabaseDateTimeFormat);
}

QFuture<bool> DatabaseService::establishConnection(const QString& databasePath)
{
    return QtConcurrent::run(m_threadPool.data(), [this, databasePath]() {
        QSqlDatabase db = QSqlDatabase::addDatabase(DriverName, DatabaseName);
        db.setDatabaseName(databasePath);

        d->db = db;

        bool isDatabaseExits = QFile::exists(databasePath);

        if (db.open())
        {
            if (!isDatabaseExits)
            {
                return applyScheme(DatabaseInitialSchemeFile);
            }
            else
            {
                return checkAndApplyMigrations();
            }

            return true;
        }

        qCCritical(database) << "Failed to open database:" << databasePath
                             << "Error:" << db.lastError().text();
        return false;
    });
}

QFuture<QVariantMap> DatabaseService::loadItem(const QString& table, const QStringList& selections, const QVariantMap& bindables) const
{
    return QtConcurrent::run(m_threadPool.data(), [this, table, selections, bindables]() -> QVariantMap {
        if (!d->db.isValid() || !d->db.isOpen())
        {
            qCWarning(database) << "Database in wrong state:"
                                << "is opened:" << d->db.isOpen()
                                << "is valid:" << d->db.isValid();

            return {};
        }

        QStringList keysPrepeared;

        const auto& keys = bindables.keys();
        keysPrepeared.reserve(keys.count());

        for (const auto& key : keys)
        {
            keysPrepeared.append(QStringLiteral("%1 = ?").arg(key));
        }

        QString keysString       = keysPrepeared.join(QStringLiteral(" AND "));
        QString selectionsString = selections.join(QStringLiteral(", "));

        QString queryString = QStringLiteral("SELECT %1 FROM %2 WHERE %3").arg(selectionsString, table, keysString);

        auto queryOptional = prepareQuery(queryString);
        if (!queryOptional)
        {
            return {};
        }

        auto& query = queryOptional.value();

        for (const auto& bindable : bindables)
        {
            query.addBindValue(bindable);
        }

        if (query.exec())
        {
            if (query.first())
            {
                QVariantMap result;

                for (const auto& selection : selections)
                {
                    result.insert(selection, query.value(selection));
                }

                return result;
            }
            else
            {
                qCWarning(database) << "failed to find result." << query.lastError().text();
            }
        }
        else
        {
            qCWarning(database) << "failed to execute query." << query.lastError().text();
        }

        return {};
    });
}

QFuture<QList<QVariantMap>> DatabaseService::loadItemsList(const QString& table, const QStringList& selections, const QVariantMap& bindables) const
{
    return QtConcurrent::run(m_threadPool.data(), [this, table, selections, bindables]() -> QList<QVariantMap> {
        if (!d->db.isValid() || !d->db.isOpen())
        {
            qCWarning(database) << "Database in wrong state:"
                                << "is opened:" << d->db.isOpen()
                                << "is valid:" << d->db.isValid();

            return {};
        }

        QString selectionsString = selections.join(QStringLiteral(", "));
        QString queryString      = QStringLiteral("SELECT %1 FROM %2").arg(selectionsString, table);

        if (!bindables.isEmpty())
        {
            QStringList keysPrepeared;

            const auto& keys = bindables.keys();
            keysPrepeared.reserve(keys.count());

            for (const auto& key : keys)
            {
                keysPrepeared.append(QStringLiteral("%1 = ?").arg(key));
            }

            QString keysString = keysPrepeared.join(QStringLiteral(" AND "));

            queryString.append(QStringLiteral(" WHERE %1").arg(keysString));
        }

        auto queryOptional = prepareQuery(queryString);
        if (!queryOptional)
        {
            return {};
        }

        auto& query = queryOptional.value();

        for (const auto& bindable : bindables)
        {
            query.addBindValue(bindable);
        }

        if (query.exec())
        {
            QList<QVariantMap> resultsList;

            while (query.next())
            {
                QVariantMap result;

                for (const auto& selection : selections)
                {
                    result.insert(selection, query.value(selection));
                }

                resultsList.append(result);
            }

            return resultsList;
        }
        else
        {
            qCWarning(database) << "failed to execute query." << query.lastError().text();
        }

        return {};
    });
}

QFuture<bool> DatabaseService::upsertItem(const QString& table, const QList<QString>& indexedColumns, const QVariantMap& itemMap) const
{
    return QtConcurrent::run(m_threadPool.data(), [this, table, indexedColumns, itemMap]() -> bool {
        if (!d->db.isValid() || !d->db.isOpen())
        {
            qCWarning(database) << "Database in wrong state:"
                                << "is opened:" << d->db.isOpen()
                                << "is valid:" << d->db.isValid();

            return false;
        }

        if (!d->db.transaction())
        {
            qCWarning(database) << "failed to start transation. " << d->db.lastError().text();
            return false;
        }

        QStringList updateValues;

        const auto& keys = itemMap.keys();
        updateValues.reserve(keys.count());

        for (const auto& key : keys)
        {
            updateValues.append(QStringLiteral("%1=excluded.%1").arg(key));
        }

        QStringList bindableQuestions;
        bindableQuestions.reserve(itemMap.count());

        for (int i = 0; i < itemMap.count(); ++i)
        {
            bindableQuestions.append(QStringLiteral("?"));
        }

        QString keysString              = itemMap.keys().join(QStringLiteral(", "));
        QString excludedKeys            = updateValues.join(QStringLiteral(", "));
        QString bindableQuestionsString = bindableQuestions.join(QStringLiteral(", "));
        QString indexedColumnsString    = indexedColumns.join(QStringLiteral(", "));

        QString queryString = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3) ON CONFLICT(%4) DO UPDATE SET %5").arg(table, keysString, bindableQuestionsString, indexedColumnsString, excludedKeys);

        auto queryOptional = prepareQuery(queryString);
        if (!queryOptional)
        {
            return {};
        }

        auto& query = queryOptional.value();

        for (const auto& bindable : itemMap)
        {
            query.addBindValue(bindable);
        }

        if (!query.exec())
        {
            qCWarning(database) << "failed to execute query. " << query.lastQuery() << query.lastError().text() << " aborting.";

            d->db.rollback();

            return false;
        }

        d->db.commit();

        return true;
    });
}

QFuture<bool> DatabaseService::upsertItemsList(const QString& table, const QList<QString>& indexedColumns, const QList<QVariantMap>& itemsList) const
{
    return QtConcurrent::run(m_threadPool.data(), [this, table, indexedColumns, itemsList]() -> bool {
        if (!d->db.isValid() || !d->db.isOpen())
        {
            qCWarning(database) << "Database in wrong state:"
                                << "is opened:" << d->db.isOpen()
                                << "is valid:" << d->db.isValid();

            return false;
        }

        if (!d->db.transaction())
        {
            qCWarning(database) << "failed to start transation. " << d->db.lastError().text();
            return false;
        }

        QMap<QString, QVariantList> mapOfItemsFields;
        for (const auto& itemMap : itemsList)
        {
            const auto& itemFields = itemMap.keys();
            for (const auto& itemField : itemFields)
            {
                if (mapOfItemsFields.contains(itemField))
                {
                    mapOfItemsFields[itemField].append(itemMap.value(itemField));
                }
                else
                {
                    QVariantList listOfItemsFields { itemMap.value(itemField) };
                    mapOfItemsFields.insert(itemField, listOfItemsFields);
                }
            }
        }

        QStringList updateValues;
        updateValues.reserve(mapOfItemsFields.count());
        const auto& keys = mapOfItemsFields.keys();
        for (const auto& key : keys)
        {
            updateValues.append(QStringLiteral("%1=excluded.%1").arg(key));
        }

        QStringList bindableQuestions;
        bindableQuestions.reserve(mapOfItemsFields.count());
        for (int i = 0; i < mapOfItemsFields.count(); ++i)
        {
            bindableQuestions.append(QStringLiteral("?"));
        }

        QString keysString              = mapOfItemsFields.keys().join(QStringLiteral(", "));
        QString excludedKeys            = updateValues.join(QStringLiteral(", "));
        QString bindableQuestionsString = bindableQuestions.join(QStringLiteral(", "));
        QString indexedColumnsString    = indexedColumns.join(QStringLiteral(", "));

        QString queryString = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3) ON CONFLICT(%4) DO UPDATE SET %5").arg(table, keysString, bindableQuestionsString, indexedColumnsString, excludedKeys);

        auto queryOptional = prepareQuery(queryString);
        if (!queryOptional)
        {
            return {};
        }

        auto& query = queryOptional.value();

        for (const auto& bindableList : std::as_const(mapOfItemsFields))
        {
            query.addBindValue(bindableList);
        }

        if (!query.execBatch())
        {
            qCWarning(database) << "failed to execute query. " << query.lastQuery() << query.lastError().text() << " aborting.";

            d->db.rollback();

            return false;
        }

        d->db.commit();

        return true;
    });
}

QFuture<bool> DatabaseService::replaceItem(const QString& table, const QVariantMap& itemMap) const
{
    return QtConcurrent::run(m_threadPool.data(), [this, table, itemMap]() -> bool {
        if (!d->db.isValid() || !d->db.isOpen())
        {
            qCWarning(database) << "Database in wrong state:"
                                << "is opened:" << d->db.isOpen()
                                << "is valid:" << d->db.isValid();

            return false;
        }

        if (!d->db.transaction())
        {
            qCWarning(database) << "failed to start transation. " << d->db.lastError().text();
            return false;
        }

        QStringList bindableQuestions;
        bindableQuestions.reserve(itemMap.count());
        for (int i = 0; i < itemMap.count(); ++i)
        {
            bindableQuestions.append(QStringLiteral("?"));
        }

        QString keysString              = itemMap.keys().join(QStringLiteral(", "));
        QString bindableQuestionsString = bindableQuestions.join(QStringLiteral(", "));

        QString queryString = QStringLiteral("INSERT OR REPLACE INTO %1 (%2) VALUES (%3)").arg(table, keysString, bindableQuestionsString);

        auto queryOptional = prepareQuery(queryString);
        if (!queryOptional)
        {
            return {};
        }

        auto& query = queryOptional.value();

        for (const auto& bindable : itemMap)
        {
            query.addBindValue(bindable);
        }

        if (!query.exec())
        {
            qCWarning(database) << "failed to execute query. " << query.lastQuery() << query.lastError().text() << " aborting.";

            d->db.rollback();

            return false;
        }

        d->db.commit();

        return true;
    });
}

QFuture<bool> DatabaseService::replaceItemsList(const QString& table, const QList<QVariantMap>& itemsList) const
{
    return QtConcurrent::run(m_threadPool.data(), [this, table, itemsList]() -> bool {
        if (!d->db.isValid() || !d->db.isOpen())
        {
            qCWarning(database) << "Database in wrong state:"
                                << "is opened:" << d->db.isOpen()
                                << "is valid:" << d->db.isValid();

            return false;
        }

        if (!d->db.transaction())
        {
            qCWarning(database) << "failed to start transation. " << d->db.lastError().text();
            return false;
        }

        QMap<QString, QVariantList> mapOfItemsFields;
        for (const auto& itemMap : itemsList)
        {
            const auto& itemFields = itemMap.keys();
            for (const auto& itemField : itemFields)
            {
                if (mapOfItemsFields.contains(itemField))
                {
                    mapOfItemsFields[itemField].append(itemMap.value(itemField));
                }
                else
                {
                    QVariantList listOfItemsFields { itemMap.value(itemField) };
                    mapOfItemsFields.insert(itemField, listOfItemsFields);
                }
            }
        }

        QStringList bindableQuestions;
        bindableQuestions.reserve(mapOfItemsFields.count());
        for (int i = 0; i < mapOfItemsFields.count(); ++i)
        {
            bindableQuestions.append(QStringLiteral("?"));
        }

        QString keysString              = mapOfItemsFields.keys().join(QStringLiteral(", "));
        QString bindableQuestionsString = bindableQuestions.join(QStringLiteral(", "));

        QString queryString = QStringLiteral("INSERT OR REPLACE INTO %1 (%2) VALUES (%3)").arg(table, keysString, bindableQuestionsString);

        auto queryOptional = prepareQuery(queryString);
        if (!queryOptional)
        {
            return {};
        }

        auto& query = queryOptional.value();

        for (const auto& bindableList : std::as_const(mapOfItemsFields))
        {
            query.addBindValue(bindableList);
        }

        if (!query.execBatch())
        {
            qCWarning(database) << "failed to execute query. " << query.lastQuery() << query.lastError().text() << " aborting.";

            d->db.rollback();

            return false;
        }

        d->db.commit();

        return true;
    });
}

QFuture<bool> DatabaseService::removeItem(const QString& table, const QVariantMap& bindables) const
{
    return QtConcurrent::run(m_threadPool.data(), [this, table, bindables]() -> bool {
        if (!d->db.isValid() || !d->db.isOpen())
        {
            qCWarning(database) << "Database in wrong state:"
                                << "is opened:" << d->db.isOpen()
                                << "is valid:" << d->db.isValid();

            return false;
        }

        QStringList keysPrepeared;

        const auto& keys = bindables.keys();
        keysPrepeared.reserve(keys.count());

        for (const auto& key : keys)
        {
            keysPrepeared.append(QStringLiteral("%1 = ?").arg(key));
        }

        QString keysString = keysPrepeared.join(QStringLiteral(" AND "));

        QString queryString = QStringLiteral("DELETE FROM %1 WHERE %2").arg(table, keysString);

        auto queryOptional = prepareQuery(queryString);
        if (!queryOptional)
        {
            return {};
        }

        auto& query = queryOptional.value();

        for (const auto& bindable : bindables)
        {
            query.addBindValue(bindable);
        }

        if (!query.exec())
        {
            qCWarning(database) << "failed to execute query. " << query.lastError().text();

            return false;
        }

        return true;
    });
}

QFuture<bool> DatabaseService::removeItems(const QString& table, const QVariantMap& andBindables, const QMultiMap<QString, QVariant>& orBindables) const
{
    return QtConcurrent::run(m_threadPool.data(), [this, table, andBindables, orBindables]() -> bool {
        if (!d->db.isValid() || !d->db.isOpen())
        {
            qCWarning(database) << "Database in wrong state:"
                                << "is opened:" << d->db.isOpen()
                                << "is valid:" << d->db.isValid();

            return false;
        }

        QStringList andKeysPrepeared;
        QStringList orKeysPrepeared;

        const auto& andKeys = andBindables.keys();
        const auto& orKeys  = orBindables.keys();

        andKeysPrepeared.reserve(andKeys.count());
        orKeysPrepeared.reserve(orKeys.count());

        for (const auto& key : andKeys)
        {
            andKeysPrepeared.append(QStringLiteral("%1 = ?").arg(key));
        }

        for (const auto& key : orKeys)
        {
            orKeysPrepeared.append(QStringLiteral("%1 = ?").arg(key));
        }

        QString andKeysString = andKeysPrepeared.join(QStringLiteral(" AND "));
        QString orKeysString  = orKeysPrepeared.join(QStringLiteral(" OR "));

        QString queryString;

        if (andKeysString.isEmpty())
        {
            queryString = QStringLiteral("DELETE FROM %1 WHERE (%2)").arg(table, orKeysString);
        }
        else if (orKeysString.isEmpty())
        {
            queryString = QStringLiteral("DELETE FROM %1 WHERE (%2)").arg(table, andKeysString);
        }
        else
        {
            queryString = QStringLiteral("DELETE FROM %1 WHERE (%2) AND (%3)").arg(table, andKeysString, orKeysString);
        }

        auto queryOptional = prepareQuery(queryString);
        if (!queryOptional)
        {
            return {};
        }

        auto& query = queryOptional.value();

        for (const auto& bindable : andBindables)
        {
            query.addBindValue(bindable);
        }

        for (const auto& bindable : orBindables)
        {
            query.addBindValue(bindable);
        }

        if (!query.exec())
        {
            qCWarning(database) << "failed to execute query. " << query.lastError().text();

            return false;
        }

        return true;
    });
}

QFuture<bool> DatabaseService::clearTable(const QString& table) const
{
    return QtConcurrent::run(m_threadPool.data(), [this, table]() -> bool {
        if (!d->db.isValid() || !d->db.isOpen())
        {
            qCWarning(database) << "Database in wrong state:"
                                << "is opened:" << d->db.isOpen()
                                << "is valid:" << d->db.isValid();

            return false;
        }

        if (!d->db.transaction())
        {
            qCWarning(database) << "failed to start transation. " << d->db.lastError().text();
            return false;
        }

        QString queryString = QStringLiteral("DELETE FROM %1").arg(table);

        auto queryOptional = prepareQuery(queryString);
        if (!queryOptional)
        {
            return {};
        }

        auto& query = queryOptional.value();

        if (!query.exec())
        {
            qCWarning(database) << "failed to execute query. " << query.lastError().text();

            d->db.rollback();

            return false;
        }

        d->db.commit();

        return true;
    });
}

bool DatabaseService::checkAndApplyMigrations() const
{
    QDir dir(DatabaseMigrationsPath);

    dir.setNameFilters(QStringList(QStringLiteral("*.sql")));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    dir.setSorting(QDir::Name);

    const auto& migrationFileNames = dir.entryList();

    if (migrationFileNames.isEmpty())
    {
        return true;
    }

    QVersionNumber currentDbVersion = databaseVersion();

    if (currentDbVersion.isNull())
    {
        qCCritical(database) << "Can't apply migrations cause current DB version is null";
        return false;
    }

    bool allMigrationApplyed = true;

    for (const auto& migrationFileName : migrationFileNames)
    {
        QVersionNumber migrationVersion = QVersionNumber::fromString(migrationFileName);

        if (migrationVersion.isNull())
        {
            qCCritical(database) << "Migrations contains file, that can not be parced to version number:" << migrationFileName;
            return false;
        }

        if (migrationVersion <= currentDbVersion)
        {
            continue;
        }

        qCDebug(database) << "Current DB version:" << currentDbVersion
                          << "Migration version:" << migrationVersion;

        allMigrationApplyed &= applyScheme(QStringLiteral("%1/%2").arg(DatabaseMigrationsPath, migrationFileName));
    }

    return allMigrationApplyed;
}

bool DatabaseService::applyScheme(const QString& schemePath) const
{
    QFile file(schemePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCCritical(database) << "Can't open db scheme file:" << schemePath;
        return false;
    }

    QByteArray ba = file.readAll();

    QStringList queryStrings = QString(ba).split(";\n"_L1, Qt::SkipEmptyParts);

    if (!d->db.isValid() || !d->db.isOpen())
    {
        qCCritical(database) << "Database isn't ready:" << d->db.lastError().text();
        return false;
    }

    if (!d->db.transaction())
    {
        qCCritical(database) << "Begin transaction error:" << d->db.lastError().text();
        return false;
    }

    for (const auto& queryString : queryStrings)
    {
        QString queryStringTrimmed = queryString.trimmed();

        if (queryStringTrimmed.isEmpty() || queryStringTrimmed.startsWith("--"_L1))
        {
            continue;
        }

        auto queryOptional = prepareQuery(queryStringTrimmed);

        if (!queryOptional)
        {
            return false;
        }

        auto& qr = queryOptional.value();

        bool result = qr.exec(queryStringTrimmed);

        qCDebug(database) << queryStringTrimmed << "---" << result;

        if (!result)
        {
            qCCritical(database) << "Apply query error:" << qr.lastError().text();

            if (!d->db.rollback())
            {
                qCFatal(database) << "Rollback transaction error:" << d->db.lastError().text();
                return false;
            }

            return false;
        }
    }

    if (!d->db.commit())
    {
        qCCritical(database) << "Commit transaction error:" << d->db.lastError().text();
        return false;
    }

    return true;
}

std::optional<QSqlQuery> DatabaseService::prepareQuery(const QString& sqlQuery) const
{
    qCDebug(database) << "Running" << sqlQuery;

    if (d->cache.contains(sqlQuery))
    {
        return d->cache.value(sqlQuery);
    }

    QSqlQuery query(d->db);

    if (!query.prepare(sqlQuery))
    {
        qCCritical(database) << "Query preparation error:" << query.lastError().text();
        return {};
    }

    d->cache.insert(sqlQuery, query);

    return query;
}

QVersionNumber DatabaseService::databaseVersion() const
{
    auto queryOptional = prepareQuery("SELECT major, minor, micro FROM version;"_L1);

    if (!queryOptional)
    {
        return QVersionNumber();
    }

    auto& query = queryOptional.value();

    bool result = query.exec();

    if (!result || !query.first())
    {
        qCritical() << Q_FUNC_INFO << "Can't get db version:" << query.lastError().text();
        return QVersionNumber();
    }

    qint32 major = query.value("major"_L1).toInt();
    qint32 minor = query.value("minor"_L1).toInt();
    qint32 micro = query.value("micro"_L1).toInt();

    const auto& version = QVersionNumber(major, minor, micro);

    return version;
}

} // namespace XWallet::Services
