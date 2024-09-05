#include "AxesModel.h"

namespace XWallet::Models {

AxesModel::AxesModel(QObject* parent)
    : QAbstractListModel(parent)
{
    connect(this, &AxesModel::modelReset, this, &AxesModel::rowCountChanged);
    connect(this, &AxesModel::rowsInserted, this, &AxesModel::rowCountChanged);
    connect(this, &AxesModel::rowsRemoved, this, &AxesModel::rowCountChanged);
}

void AxesModel::setList(const QList<Item::Label>& list)
{
    beginResetModel();

    m_list = list;

    endResetModel();
}

int AxesModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_list.count();
}

QVariant AxesModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
    {
        return QVariant();
    }

    const auto& item = m_list.at(index.row());

    switch (role)
    {
        case LabelRole:
            return QVariant::fromValue(item);
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> AxesModel::roleNames() const
{
    return {
        { LabelRole, "label" },
    };
}

} // namespace XWallet::Models
