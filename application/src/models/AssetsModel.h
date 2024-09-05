#pragma once

#include "data/Asset.h"

#include <QAbstractItemModel>

namespace XWallet::Models {

class AssetsModel final : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)

public:
    enum Roles
    {
        AssetRole = Qt::UserRole + 1,
    };

public:
    explicit AssetsModel(QObject* parent = nullptr);
    ~AssetsModel() final = default;

    void               setList(const QList<Data::Asset>& list);
    QList<Data::Asset> list() const { return m_list; }

protected:
    int                    rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant               data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void rowCountChanged();

private:
    QList<Data::Asset> m_list;
};

} // namespace XWallet::Models
