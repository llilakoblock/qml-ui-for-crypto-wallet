#pragma once

#include <QAbstractListModel>

template<class Gadget>
class GadgetListModel : public QAbstractListModel
{
public:
    explicit GadgetListModel(QObject* parent = nullptr)
        : QAbstractListModel(parent)
    {}
    ~GadgetListModel() override = default;

    void setList(const QList<Gadget>& list)
    {
        if (m_list == list)
        {
            return;
        }

        beginResetModel();
        m_list = list;
        endResetModel();
    }

protected:
    int rowCount(const QModelIndex& parent) const override
    {
        Q_UNUSED(parent);

        return m_list.count();
    }

    QVariant data(const QModelIndex& index, int role) const override
    {
        if (index.row() < 0 || index.row() >= m_list.count() || role != Qt::UserRole + 1)
        {
            return QVariant();
        }

        const auto& item = m_list.at(index.row());

        return QVariant::fromValue(item);
    }

    QHash<int, QByteArray> roleNames() const override
    {
        return {
            { Qt::UserRole + 1, "gadget" }
        };
    }

private:
    QList<Gadget> m_list;
};
