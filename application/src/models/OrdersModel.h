#pragma once

#include "data/Order.h"
#include "data/Swap.h"

#include <QAbstractItemModel>

namespace XWallet::Models {

class OrdersModel final : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)

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
    explicit OrdersModel(QObject* parent = nullptr);
    ~OrdersModel() final = default;

    void setList(const QList<Data::Order>& list);

    void setAssets(const Data::Asset::Hash& assets);

protected:
    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool     setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    QHash<int, QByteArray> roleNames() const override;

signals:
    void rowCountChanged();

private:
    QList<Data::Swap> m_list;
    Data::Asset::Hash m_assets;

    QHash<QString, bool> m_expandedItems;
    QHash<QString, bool> m_showLogsItems;
};

} // namespace XWallet::Models
