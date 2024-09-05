#pragma once

#include "data/Asset.h"
#include "data/Quote.h"
#include "models/AxesModel.h"

#include <QQuickWindow>
#include <QtQmlIntegration>

namespace XWallet::ViewModels {

class LineChartViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_DISABLE_COPY_MOVE(LineChartViewModel)

    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged FINAL)

    Q_PROPERTY(QString linePath READ linePath NOTIFY linePathChanged FINAL)
    Q_PROPERTY(QPointF linePointer READ linePointer NOTIFY linePointerChanged FINAL)
    Q_PROPERTY(Data::Quote quote READ quote NOTIFY quoteChanged FINAL)
    Q_PROPERTY(QList<QPointF> axis READ axis WRITE setAxis NOTIFY axisChanged FINAL)

    Q_PROPERTY(Period period READ period WRITE setPeriod NOTIFY periodChanged FINAL)
    Q_PROPERTY(QSizeF chartSize READ chartSize WRITE setChartSize NOTIFY chartSizeChanged REQUIRED FINAL)
    Q_PROPERTY(QPointF hoverPoint READ hoverPoint WRITE setHoverPoint NOTIFY hoverPointChanged REQUIRED FINAL)
    Q_PROPERTY(Data::Asset::Type assetType READ assetType WRITE setAssetType NOTIFY assetTypeChanged REQUIRED FINAL)

    Q_PROPERTY(QAbstractItemModel* horizontalLabelsModel READ horizontalLabelsModel CONSTANT FINAL)
    Q_PROPERTY(QAbstractItemModel* verticalLabelsModel READ verticalLabelsModel CONSTANT FINAL)

public:
    enum class Period
    {
        Unknown           = 0,
        DayPeriod         = 1,
        WeekPeriod        = 7,
        MonthPeriod       = 30,
        ThreeMonthsPeriod = 90,
        SixMonthsPeriod   = 180,
        YearPeriod        = 365,
    };
    Q_ENUM(Period)

    enum class State
    {
        UnknownState,
        LoadingState,
        CompletedState,
    };
    Q_ENUM(State)

public:
    explicit LineChartViewModel(QObject* parent = nullptr);
    ~LineChartViewModel() override;

private:
    void updateLineChart();

    void animateLineChart(const QList<QPointF>& newPoints);

    static QList<Data::Quote> limitQuotesList(const QList<Data::Quote>& quotes, const QDateTime& fromDate);
    static QList<QPointF>     normalizeQuoteList(const QList<Data::Quote>& quotes, QSizeF chartSize);
    static QList<QPointF>     resizePointsList(const QList<QPointF>& points, qsizetype size);

    static QList<Models::Item::Label> updateHorizontalLabels(const QList<Data::Quote>& quotes, QSizeF chartSize, Period period);
    static QList<Models::Item::Label> updateVerticalLabels(const QList<Data::Quote>& quotes, QSizeF chartSize);

    static qreal makeNiceNumber(qreal value, bool ceiling = false);

    State state() const { return m_state; }
    void  setState(State state);

    Period period() const { return m_period; }
    void   setPeriod(Period period);

    QString linePath() const { return m_linePath; }
    void    setLinePath(const QString& linePath);

    QPointF linePointer() const { return m_linePointer; }
    void    setLinePointer(QPointF linePointer);

    Data::Quote quote() const { return m_quote; }
    void        setQuote(const Data::Quote& quote);

    QList<QPointF> axis() const { return m_axis; }
    void           setAxis(const QList<QPointF>& axis);

    QSizeF chartSize() const { return m_chartSize; }
    void   setChartSize(QSizeF chartSize);

    QPointF hoverPoint() const { return m_hoverPoint; }
    void    setHoverPoint(QPointF hoverPoint);

    Data::Asset::Type assetType() const { return m_assetType; }
    void              setAssetType(Data::Asset::Type assetType);

    QAbstractItemModel* horizontalLabelsModel() const { return m_horizontalLabelsModel.data(); }
    QAbstractItemModel* verticalLabelsModel() const { return m_verticalLabelsModel.data(); }

signals:
    void periodChanged();

    void linePathChanged();
    void linePointerChanged();
    void quoteChanged();
    void axisChanged();

    void chartSizeChanged();
    void hoverPointChanged();
    void assetTypeChanged();

    void stateChanged();

private:
    State m_state { State::UnknownState };

    Period m_period { Period::DayPeriod };

    QString        m_linePath;
    QPointF        m_linePointer;
    Data::Quote    m_quote;
    QList<QPointF> m_axis;

    QSizeF            m_chartSize;
    QPointF           m_hoverPoint;
    Data::Asset::Type m_assetType { Data::Asset::Type::Unknown };

    QList<Data::Quote> m_quotes;
    QList<QPointF>     m_points;

    QMetaObject::Connection m_animationConnection;

    QScopedPointer<Models::AxesModel> m_horizontalLabelsModel;
    QScopedPointer<Models::AxesModel> m_verticalLabelsModel;
};

} // namespace XWallet::ViewModels
