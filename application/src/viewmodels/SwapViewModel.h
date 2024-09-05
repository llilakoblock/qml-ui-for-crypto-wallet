#pragma once

#include "misc/SortFilterProxyModelByCondition.h"
#include "models/AssetsModel.h"

#include <QtQmlIntegration>

namespace XWallet::ViewModels {

class SwapViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(Status status READ status NOTIFY statusChanged FINAL)
    Q_PROPERTY(Errors errors READ errors NOTIFY errorsChanged FINAL)

    Q_PROPERTY(Data::Asset bidAsset READ bidAsset WRITE setBidAsset NOTIFY bidAssetChanged FINAL)
    Q_PROPERTY(Data::Asset askAsset READ askAsset WRITE setAskAsset NOTIFY askAssetChanged FINAL)

    Q_PROPERTY(QString bidValue READ bidValue WRITE setBidValue NOTIFY bidValueChanged FINAL)
    Q_PROPERTY(QString askValue READ askValue WRITE setAskValue NOTIFY askValueChanged FINAL)

    Q_PROPERTY(QString fiatValue READ fiatValue NOTIFY fiatValueChanged FINAL)

    Q_PROPERTY(FilterAsset filterAsset READ filterAsset WRITE setFilterAsset NOTIFY filterAssetChanged FINAL)
    Q_PROPERTY(QString searchQuery READ searchQuery WRITE setSearchQuery NOTIFY searchQueryChanged FINAL)

    Q_PROPERTY(QAbstractItemModel* assetsModel READ assetsModel CONSTANT FINAL)

public:
    enum class Status
    {
        IdleStatus,
        CreatingOrderStatus,
    };
    Q_ENUM(Status)

    enum class ChangeReason
    {
        Unknown,
        UserInput,
        Calculations,
        StateChanged,
    };
    Q_ENUM(ChangeReason)

    enum class FilterAsset
    {
        UnknownAsset,
        BidAsset,
        AskAsset,
    };
    Q_ENUM(FilterAsset)

    enum class FocusedField
    {
        UnknownField,
        BidField,
        AskField,
    };
    Q_ENUM(FocusedField)

    enum class Error
    {
        NoError                 = 0,
        InsufficientAmountError = 1 << 0,
        WrongFeeAmountError     = 1 << 1,
        WrongAddressError       = 1 << 2,
    };
    Q_DECLARE_FLAGS(Errors, Error)
    Q_FLAG(Error)

public:
    explicit SwapViewModel(QObject* parent = nullptr);

    Q_INVOKABLE void interchangeAssets();
    Q_INVOKABLE void makeSwap();

    Q_INVOKABLE void cancelOrder(const QString& orderId);

    Q_INVOKABLE void setFocusedField(FocusedField focusedField);

private:
    static QString prepareUserInput(const QString& userInput);

    Status status() const { return m_status; }
    void   setStatus(Status status);

    Errors errors() const { return m_errors; }
    void   setErrors(Errors errors);

    FocusedField focusedField() const { return m_focusedField; }

    Data::Asset bidAsset() const { return m_bidAsset; }
    void        setBidAsset(const Data::Asset& bidAsset);

    Data::Asset askAsset() const { return m_askAsset; }
    void        setAskAsset(const Data::Asset& askAsset);

    QString bidValue() const { return m_bidValue; }
    void    setBidValue(const QString& bidValue, ChangeReason changeReason = ChangeReason::UserInput);

    QString askValue() const { return m_askValue; }
    void    setAskValue(const QString& askValue, ChangeReason changeReason = ChangeReason::UserInput);

    QString fiatValue() const { return m_fiatValue; }
    void    setFiatValue(const QString& fiatValue);

    FilterAsset filterAsset() const { return m_filterAsset; }
    void        setFilterAsset(FilterAsset filterAsset);

    QString searchQuery() const { return m_searchQuery; }
    void    setSearchQuery(const QString& searchQuery);

    QAbstractItemModel* assetsModel() const { return m_sortedAssetsModel.data(); }

signals:
    void statusChanged();

    void errorsChanged();

    void focusedFieldChanged();

    void bidAssetChanged();
    void askAssetChanged();

    void fiatValueChanged();

    void bidValueChanged(const QString& bidValue, ChangeReason changeReason);
    void askValueChanged(const QString& bidValue, ChangeReason changeReason);

    void searchQueryChanged();

    void filterAssetChanged();

private:
    Status m_status { Status::IdleStatus };

    Errors m_errors { Error::NoError };

    FocusedField m_focusedField { FocusedField::UnknownField };

    Data::Asset m_bidAsset;
    Data::Asset m_askAsset;

    QString m_bidValue;
    QString m_askValue;

    QString m_fiatValue;

    FilterAsset m_filterAsset { FilterAsset::UnknownAsset };
    QString     m_searchQuery;

    QScopedPointer<Models::AssetsModel> m_assetsModel;

    QScopedPointer<SortFilterProxyModelByCondition> m_sortedAssetsModel;
};

} // namespace XWallet::ViewModels
