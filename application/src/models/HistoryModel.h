#pragma once

#include "data/HistoryEntry.h"

#include <QAbstractItemModel>

namespace XWallet::Models {

class HistoryModel final : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int length READ length NOTIFY lengthChanged)

public:
    enum Roles
    {
        HistoryEntryRole = Qt::UserRole + 1,
        DateRole,
        ExpandedRole,
    };

public:
    explicit HistoryModel(QObject* parent = nullptr);
    ~HistoryModel() final = default;

    void setList(const QList<Data::HistoryEntry>& list);

protected:
    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool     setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    QHash<int, QByteArray> roleNames() const override;

private:
    int length() const { return m_list.count(); }

signals:
    void lengthChanged();

private:
    QList<Data::HistoryEntry> m_list;

    QHash<QString, bool> m_expandedItems;
};

} // namespace XWallet::Models
