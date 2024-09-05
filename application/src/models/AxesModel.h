#pragma once

#include <QAbstractItemModel>
#include <QtQmlIntegration>

namespace XWallet::Models {

namespace Item {

class Label
{
    Q_GADGET
    QML_VALUE_TYPE(label)

    Q_PROPERTY(QString text MEMBER text CONSTANT)
    Q_PROPERTY(QPointF point MEMBER point CONSTANT)

public:
    explicit Label(const QString& text, QPointF point)
        : text(text)
        , point(point)
    {}

    Label() = default;

    bool operator== (const Label& other) const
    {
        return text == other.text && point == other.point;
    }

private:
    QString text;
    QPointF point;
};

} // namespace Item

class AxesModel final : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)

public:
    enum Roles
    {
        LabelRole = Qt::UserRole + 1,
    };

public:
    explicit AxesModel(QObject* parent = nullptr);
    ~AxesModel() final = default;

    void setList(const QList<Item::Label>& list);

protected:
    int                    rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant               data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void rowCountChanged();

private:
    QList<Item::Label> m_list;
};

} // namespace XWallet::Models
