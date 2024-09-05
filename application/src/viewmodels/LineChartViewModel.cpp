#include "LineChartViewModel.h"

#include "singletones/Store.h"

#include <QEasingCurve>
#include <QGuiApplication>
#include <QtConcurrent>

namespace XWallet::ViewModels {

constexpr quint64 AnimationTime = 1000;

constexpr qint32 HorizontalLabelsPerDay         = 7;
constexpr qint32 HorizontalLabelsPerWeek        = 7;
constexpr qint32 HorizontalLabelsPerMonth       = 5;
constexpr qint32 HorizontalLabelsPerThreeMonths = 4;
constexpr qint32 HorizontalLabelsPerSixMonths   = 6;
constexpr qint32 HorizontalLabelsPerYear        = 13;

constexpr qint32 MaximumVericalLabelsCount = 8;

LineChartViewModel::LineChartViewModel(QObject* parent)
    : QObject(parent)
    , m_horizontalLabelsModel(new Models::AxesModel)
    , m_verticalLabelsModel(new Models::AxesModel)
{
    const auto& quotesByPeriod = [](const Data::State& state, Data::Asset::Type type, Period period) -> QList<Data::Quote> {
        if (type == Data::Asset::Type::Unknown || !state.wallet().assets().contains(type))
        {
            return {};
        }

        switch (period)
        {
            case Period::DayPeriod:
                return state.wallet().assets().value(type).quotes(Data::Asset::Resolution::Max);
            case Period::WeekPeriod:
            case Period::MonthPeriod:
            case Period::ThreeMonthsPeriod:
                return state.wallet().assets().value(type).quotes(Data::Asset::Resolution::Mid);
            case Period::SixMonthsPeriod:
            case Period::YearPeriod:
                return state.wallet().assets().value(type).quotes(Data::Asset::Resolution::Min);
            default:
                return {};
        }
    };

    const auto& requestQuotes = [](Data::Asset::Type type, Period period) -> void {
        Data::Asset::Resolution resolution;

        switch (period)
        {
            case Period::DayPeriod:
                resolution = Data::Asset::Resolution::Max;
                break;
            case Period::WeekPeriod:
            case Period::MonthPeriod:
            case Period::ThreeMonthsPeriod:
                resolution = Data::Asset::Resolution::Mid;
                break;
            case Period::SixMonthsPeriod:
            case Period::YearPeriod:
                resolution = Data::Asset::Resolution::Min;
                break;
            default:
                resolution = Data::Asset::Resolution::Unknown;
                break;
        }

        Store::instance().dispatch(DatabaseServiceActions::LoadQuotes { type, resolution });
        Store::instance().dispatch(NetworkServiceActions::RequestQuote(type, resolution, NetworkServiceActions::Service::CoinGecko));
    };

    const auto& limitQuotes = [](const QList<Data::Quote>& quotes, Period period) -> QList<Data::Quote> {
        return LineChartViewModel::limitQuotesList(quotes, QDateTime::currentDateTime().addDays(-static_cast<qint64>(period)));
    };

    connect(&Store::instance(), &Store::stateChanged, this, [this, quotesByPeriod, limitQuotes](const Data::State& state) {
        const auto& quotes = quotesByPeriod(state, m_assetType, m_period);

        m_quotes = limitQuotes(quotes, m_period);

        if (!m_quotes.isEmpty())
        {
            setState(State::CompletedState);
            updateLineChart();
        }
    });

    connect(this, &LineChartViewModel::assetTypeChanged, this, [this, quotesByPeriod, limitQuotes, requestQuotes]() {
        const auto& state = Store::instance().state();

        const auto& quotes = quotesByPeriod(state, m_assetType, m_period);

        m_quotes = limitQuotes(quotes, m_period);

        if (m_quotes.isEmpty())
        {
            setState(State::LoadingState);
            requestQuotes(m_assetType, m_period);
        }

        updateLineChart();
    });

    connect(this, &LineChartViewModel::periodChanged, this, [this, quotesByPeriod, limitQuotes, requestQuotes]() {
        const auto& state = Store::instance().state();

        const auto& quotes = quotesByPeriod(state, m_assetType, m_period);

        m_quotes = limitQuotes(quotes, m_period);

        if (m_quotes.isEmpty())
        {
            setState(State::LoadingState);
            requestQuotes(m_assetType, m_period);
        }

        updateLineChart();
    });

    connect(this, &LineChartViewModel::chartSizeChanged, this, [this]() {
        updateLineChart();
    });

    connect(this, &LineChartViewModel::hoverPointChanged, this, [this]() {
        if (m_quotes.isEmpty() || m_hoverPoint.isNull())
        {
            setQuote(Data::Quote());
            setLinePointer(QPointF());

            return;
        }

        const auto& nearestPoint = std::min_element(m_points.constBegin(), m_points.constEnd(), [point = m_hoverPoint](QPointF lhs, QPointF rhs) {
            return std::abs(lhs.x() - point.x()) < std::abs(rhs.x() - point.x());
        });

        if (nearestPoint == m_points.constEnd())
        {
            return;
        }

        const auto& nearestQuote = m_quotes.at(nearestPoint - m_points.constBegin());

        setLinePointer(*nearestPoint);
        setQuote(nearestQuote);
    });
}

LineChartViewModel::~LineChartViewModel() = default;

void LineChartViewModel::updateLineChart()
{
    if (m_quotes.isEmpty() || m_chartSize.isEmpty())
    {
        return;
    }

    QtConcurrent::run([quotes = m_quotes, chartSize = m_chartSize]() {
        return LineChartViewModel::normalizeQuoteList(quotes, chartSize);
    }).then(this, [this](const QList<QPointF>& points) {
        animateLineChart(points);

        const auto& horizontalLabelsList = updateHorizontalLabels(m_quotes, m_chartSize, m_period);
        const auto& verticalLabelsList   = updateVerticalLabels(m_quotes, m_chartSize);

        m_horizontalLabelsModel->setList(horizontalLabelsList);
        m_verticalLabelsModel->setList(verticalLabelsList);
    });
}

void LineChartViewModel::animateLineChart(const QList<QPointF>& newPoints)
{
    if (newPoints.isEmpty())
    {
        return;
    }

    const auto& generateSvgLine = [this](const QList<QPointF>& points, QSizeF chartSize) -> QString {
        QString linePath = QStringLiteral("M -30 %1").arg(points.first().y());

        for (const auto& point : points)
        {
            linePath.append(QStringLiteral("L %1 %2").arg(point.x()).arg(point.y()));
        }

        linePath.append(QStringLiteral("L %1 %2").arg(points.last().x() + 10).arg(points.last().y()));
        linePath.append(QStringLiteral("L %1 %2").arg(points.last().x() + 10).arg(chartSize.height() + 30));
        linePath.append(QStringLiteral("L -30 %1").arg(chartSize.height() + 30));
        linePath.append(QStringLiteral("Z"));

        return linePath;
    };

    if (m_animationConnection)
    {
        disconnect(m_animationConnection);
    }

    const auto& oldPoints = resizePointsList(m_points, newPoints.count());

    if (oldPoints.isEmpty())
    {
        int i = 9;
    }

    QElapsedTimer timer;
    timer.start();

    QEasingCurve easingCurve(QEasingCurve::InOutQuad);

    QQuickWindow* window = qobject_cast<QQuickWindow*>(QGuiApplication::allWindows().at(0));

    m_animationConnection = connect(window, &QQuickWindow::frameSwapped, this, [this, oldPoints, newPoints, timer, easingCurve, generateSvgLine]() {
        qint64 elapsed = timer.elapsed();

        if (elapsed < AnimationTime)
        {
            qreal progress = easingCurve.valueForProgress(static_cast<qreal>(elapsed) / static_cast<qreal>(AnimationTime));

            QList<QPointF> points;
            points.reserve(newPoints.count());

            if (oldPoints.isEmpty())
            {
                int i = 9;
            }

            for (int i = 0; i < oldPoints.count(); ++i)
            {
                const auto& oldPoint = oldPoints.at(i);
                const auto& newPoint = newPoints.at(i);

                points.append({ newPoint.x(),
                                oldPoint.y() + (newPoint.y() - oldPoint.y()) * progress });
            }

            if (points.isEmpty())
            {
                return;
            }

            setLinePath(generateSvgLine(points, m_chartSize));
        }
        else
        {
            disconnect(m_animationConnection);

            setLinePath(generateSvgLine(newPoints, m_chartSize));
        }
    });

    m_points = newPoints;
}

QList<Data::Quote> LineChartViewModel::limitQuotesList(const QList<Data::Quote>& quotes, const QDateTime& fromDate)
{
    QList<Data::Quote> result;
    result.reserve(quotes.count());

    std::copy_if(quotes.constBegin(), quotes.constEnd(), std::back_inserter(result), [fromDate](const Data::Quote& quote) {
        return quote.timestamp() >= fromDate;
    });

    return result;
}

QList<QPointF> LineChartViewModel::normalizeQuoteList(const QList<Data::Quote>& quotes, QSizeF chartSize)
{
    if (quotes.isEmpty())
    {
        return {};
    }

    qreal maximum = std::max_element(quotes.constBegin(), quotes.constEnd(), [](const Data::Quote& lhs, const Data::Quote& rhs) {
        return lhs.price() < rhs.price();
    })->price();

    qreal minimum = std::min_element(quotes.constBegin(), quotes.constEnd(), [](const Data::Quote& lhs, const Data::Quote& rhs) {
        return lhs.price() < rhs.price();
    })->price();

    qreal range = makeNiceNumber(maximum - minimum, true); // range with ceiling
    qreal step  = makeNiceNumber(range / (MaximumVericalLabelsCount - 1), false);

    qreal roundedMinimum = std::floor(minimum / step) * step;
    qreal roundedMaximum = std::ceil(maximum / step) * step;

    QList<QPointF> points;
    points.reserve(quotes.count());

    qreal part = chartSize.width() / static_cast<qreal>(quotes.count());

    for (const auto& quote : quotes)
    {
        qreal x = part * points.count();
        points.append({ x, chartSize.height() - (quote.price() - roundedMinimum) / (roundedMaximum - roundedMinimum) * chartSize.height() });
    }

    return points;
}

QList<QPointF> LineChartViewModel::resizePointsList(const QList<QPointF>& points, qsizetype size)
{
    QList<QPointF> resizedPoints;
    resizedPoints.reserve(size);

    if (points.isEmpty())
    {
        return resizedPoints.fill(QPointF(), size);
    }

    if (points.count() < size)
    {
        const qsizetype insertationPerCell = size / points.count();
        const qsizetype lastInsert         = size - insertationPerCell * points.count();

        if (insertationPerCell > 1)
        {
            for (qsizetype i = 0; i < points.count(); ++i)
            {
                resizedPoints.append({ insertationPerCell, points.at(i) });
            }
        }
        else
        {
            resizedPoints = points;
        }

        if (lastInsert > 0)
        {
            resizedPoints.append(points.last(lastInsert));
        }
    }
    else if (points.count() > size)
    {
        const qsizetype removingsPerCell = points.count() / size;

        if (removingsPerCell > 1)
        {
            for (qsizetype i = 0; i < points.count() && resizedPoints.count() != size; i += removingsPerCell)
            {
                resizedPoints.append(points.at(i));
            }
        }
        else
        {
            resizedPoints = points.last(size);
        }
    }
    else
    {
        resizedPoints = points;
    }

    return resizedPoints;
}

QList<Models::Item::Label> LineChartViewModel::updateHorizontalLabels(const QList<Data::Quote>& quotes, QSizeF chartSize, Period period)
{
    if (quotes.isEmpty() || chartSize.isEmpty())
    {
        return {};
    }

    std::function<bool(const Data::Quote&)>    condition = nullptr;
    std::function<QString(const Data::Quote&)> text      = nullptr;

    switch (period)
    {
        case Period::DayPeriod:
        {
            condition = [](const Data::Quote& quote) -> bool {
                qint32 hour   = quote.timestamp().time().hour();
                qint32 minute = quote.timestamp().time().minute();

                // Some times minutes is drifting by 1-3 minutes
                return (hour % 3 == 0) && minute == 0;
            };

            text = [](const Data::Quote& quote) -> QString {
                return quote.timestamp().time().toString(QStringLiteral("h:00 A"));
            };

            break;
        }
        case Period::WeekPeriod:
        {
            condition = [](const Data::Quote& quote) -> bool {
                return quote.timestamp().time().hour() == 0;
            };

            text = [](const Data::Quote& quote) -> QString {
                return quote.timestamp().date().toString(QStringLiteral("MMM, d"));
            };

            break;
        }
        case Period::MonthPeriod:
        {
            condition = [](const Data::Quote& quote) -> bool {
                const auto& currentDateTime = QDateTime::currentDateTime();

                qint32 daysPerPeriod = static_cast<qint32>(Period::MonthPeriod) / HorizontalLabelsPerMonth;

                for (qint32 i = 0; i < HorizontalLabelsPerMonth; ++i)
                {
                    if (quote.timestamp().date().day() == currentDateTime.addDays(-daysPerPeriod * i).date().day() &&
                        quote.timestamp().time().hour() == 0)
                    {
                        return true;
                    }
                }

                return false;
            };

            text = [](const Data::Quote& quote) -> QString {
                return quote.timestamp().date().toString(QStringLiteral("MMM, d"));
            };

            break;
        }
        case Period::ThreeMonthsPeriod:
        {
            condition = [](const Data::Quote& quote) -> bool {
                return quote.timestamp().date().day() == 1 && quote.timestamp().time().hour() == 0;
            };

            text = [](const Data::Quote& quote) -> QString {
                return quote.timestamp().date().toString(QStringLiteral("MMM"));
            };

            break;
        }
        case Period::SixMonthsPeriod:
        {
            condition = [](const Data::Quote& quote) -> bool {
                return quote.timestamp().date().day() == 1;
            };

            text = [](const Data::Quote& quote) -> QString {
                return quote.timestamp().date().toString(QStringLiteral("MMM"));
            };

            break;
        }
        case Period::YearPeriod:
        {
            condition = [](const Data::Quote& quote) -> bool {
                return quote.timestamp().date().day() == 1;
            };

            text = [](const Data::Quote& quote) -> QString {
                return quote.timestamp().date().toString(QStringLiteral("MMM"));
            };

            break;
        }
        default:
            condition = nullptr;
            text      = nullptr;
    }

    if (condition == nullptr || text == nullptr)
    {
        return {};
    }

    qreal part = chartSize.width() / static_cast<qreal>(quotes.count());

    QList<Models::Item::Label> horizontalLabels;

    horizontalLabels.append(Models::Item::Label(QString(), QPointF(0, 0)));

    for (qsizetype i = 0; i < quotes.count(); ++i)
    {
        const auto& quote = quotes.at(i);

        if (condition(quote))
        {
            horizontalLabels.append(Models::Item::Label(text(quote), QPointF(part * i, 0)));
        }
    }

    horizontalLabels.append(Models::Item::Label(QString(), QPointF(part * quotes.count() - 1, 0)));

    return horizontalLabels;
}

QList<Models::Item::Label> LineChartViewModel::updateVerticalLabels(const QList<Data::Quote>& quotes, QSizeF chartSize)
{
    if (quotes.isEmpty())
    {
        return {};
    }

    const auto& text = [](qreal value) -> QString {
        return QLocale().toString(value, 'f', 2);
    };

    qreal maximum = std::max_element(quotes.constBegin(), quotes.constEnd(), [](const Data::Quote& lhs, const Data::Quote& rhs) {
        return lhs.price() < rhs.price();
    })->price();

    qreal minimum = std::min_element(quotes.constBegin(), quotes.constEnd(), [](const Data::Quote& lhs, const Data::Quote& rhs) {
        return lhs.price() < rhs.price();
    })->price();

    int ticksCount = 8;

    qreal range = makeNiceNumber(maximum - minimum, true); // range with ceiling
    qreal step  = makeNiceNumber(range / (ticksCount - 1), false);

    qreal roundedMinimum = std::floor(minimum / step);
    qreal roundedMaximum = std::ceil(maximum / step);

    ticksCount = int(roundedMaximum - roundedMinimum);

    roundedMinimum *= step;
    roundedMaximum *= step;

    QList<Models::Item::Label> verticalLabels;

    for (int i = 0; i <= ticksCount; ++i)
    {
        qreal value = roundedMinimum + i * step;

        qreal y = chartSize.height() - ((value - roundedMinimum) / (roundedMaximum - roundedMinimum) * chartSize.height());

        verticalLabels.append(Models::Item::Label(text(value), QPointF(0, y)));
    }

    return verticalLabels;
}

qreal LineChartViewModel::makeNiceNumber(qreal value, bool ceiling)
{
    qreal z = qPow(10, qFloor(std::log10(value))); // find corresponding number of the form of 10^n than is smaller than x
    qreal q = value / z;                           // q<10 && q>=1;

    if (ceiling)
    {
        if (q <= 1.0)
        {
            q = 1;
        }
        else if (q <= 2.0)
        {
            q = 2;
        }
        else if (q <= 5.0)
        {
            q = 5;
        }
        else
        {
            q = 10;
        }
    }
    else
    {
        if (q < 1.5)
        {
            q = 1;
        }
        else if (q < 3.0)
        {
            q = 2;
        }
        else if (q < 7.0)
        {
            q = 5;
        }
        else
        {
            q = 10;
        }
    }

    return q * z;
}

void LineChartViewModel::setState(State state)
{
    if (m_state == state)
    {
        return;
    }
    m_state = state;
    emit stateChanged();
}

void LineChartViewModel::setPeriod(Period period)
{
    if (m_period == period)
    {
        return;
    }
    m_period = period;
    emit periodChanged();
}

void LineChartViewModel::setLinePath(const QString& linePath)
{
    if (m_linePath == linePath)
    {
        return;
    }
    m_linePath = linePath;
    emit linePathChanged();
}

void LineChartViewModel::setLinePointer(QPointF linePointer)
{
    if (m_linePointer == linePointer)
    {
        return;
    }
    m_linePointer = linePointer;
    emit linePointerChanged();
}

void LineChartViewModel::setQuote(const Data::Quote& quote)
{
    if (m_quote == quote)
    {
        return;
    }
    m_quote = quote;
    emit quoteChanged();
}

void LineChartViewModel::setAxis(const QList<QPointF>& axis)
{
    if (m_axis == axis)
    {
        return;
    }
    m_axis = axis;
    emit axisChanged();
}

void LineChartViewModel::setChartSize(QSizeF chartSize)
{
    if (m_chartSize == chartSize)
    {
        return;
    }
    m_chartSize = chartSize;
    emit chartSizeChanged();
}

void LineChartViewModel::setAssetType(Data::Asset::Type assetType)
{
    if (m_assetType == assetType)
    {
        return;
    }
    m_assetType = assetType;
    emit assetTypeChanged();
}

void LineChartViewModel::setHoverPoint(QPointF hoverPoint)
{
    if (m_hoverPoint.x() == hoverPoint.x())
    {
        return;
    }
    m_hoverPoint = hoverPoint;
    emit hoverPointChanged();
}

} // namespace XWallet::ViewModels
