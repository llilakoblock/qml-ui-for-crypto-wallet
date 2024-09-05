#pragma once

#include <QConcatenateTablesProxyModel>

namespace XWallet::Models {

class ConcatenatedSwapsModel final : public QConcatenateTablesProxyModel
{
    Q_OBJECT

    Q_PROPERTY(int length READ length NOTIFY lengthChanged)

public:
    enum Roles
    {
        SwapRole = Qt::UserRole + 1,
        BidAssetRole,
        AskAssetRole,
        BidCoinAmountRole,
        BidFiatAmountRole,
        AskCoinAmountRole,
        AskFiatAmountRole,
        ExpandedRole,
        ShowLogsRole,
    };

public:
    explicit ConcatenatedSwapsModel(QObject* parent = nullptr);
    ~ConcatenatedSwapsModel() final = default;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    int length() const { return rowCount(); }

signals:
    void lengthChanged();
};

} // namespace XWallet::Models
