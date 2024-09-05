#pragma once

#include "actions/RootActions.h"
#include "services/DatabaseService.h"

#include <QObject>

namespace XWallet {

class Worker;

namespace Middleware {
class DatabaseMiddleware : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(DatabaseMiddleware)

public:
    DatabaseMiddleware(QObject* parent = nullptr);

private:
    RootActions::Action apply(const RootActions::Action& action) const;

private:
    QScopedPointer<Services::DatabaseService> m_databaseService;

    bool m_isConnected = false;

    friend class XWallet::Worker;
};

} // namespace Middleware
} // namespace XWallet
