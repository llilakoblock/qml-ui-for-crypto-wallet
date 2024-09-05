#pragma once

#include "misc/SortFilterProxyModelByCondition.h"
#include "models/AssetsModel.h"

#include <QtQmlIntegration>

namespace XWallet::ViewModels {

class WalletViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString searchQuery READ searchQuery WRITE setSearchQuery NOTIFY searchQueryChanged FINAL)

    Q_PROPERTY(qint32 currentIndex READ currentIndex NOTIFY currentIndexChanged FINAL)
    Q_PROPERTY(Data::Asset currentAsset READ currentAsset NOTIFY currentAssetChanged FINAL)

    Q_PROPERTY(QAbstractItemModel* assetsModel READ assetsModel CONSTANT FINAL)

public:
    enum class Filtering
    {
        All,
        Sent,
        Received,
        Swapped
    };
    Q_ENUM(Filtering)

public:
    explicit WalletViewModel(QObject* parent = nullptr);

    Q_INVOKABLE void selectIndex(qint32 index);

private:
    QString searchQuery() const { return m_searchQuery; }
    void    setSearchQuery(const QString& searchQuery);

    qint32 currentIndex() const { return m_currentIndex; }
    void   setCurrentIndex(qint32 currentIndex);

    Data::Asset currentAsset() const { return m_currentAsset; }
    void        setCurrentAsset(const Data::Asset& asset);

    QAbstractItemModel* assetsModel() const { return m_sortedAssetsModel.data(); }

signals:
    void searchQueryChanged();

    void currentAssetChanged();
    void currentIndexChanged();

private:
    QString m_searchQuery;

    Data::Asset m_currentAsset;
    qint32      m_currentIndex { 0 };

    QScopedPointer<SortFilterProxyModelByCondition> m_sortedAssetsModel;

    QScopedPointer<Models::AssetsModel> m_assetsModel;
};

} // namespace XWallet::ViewModels
