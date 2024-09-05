#include "Dimensions.h"

#include <QQmlEngine>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickWindow>

namespace XWallet::Style {
constexpr qint32 ColumnsCount = 12;

constexpr qint32 GutterWidth = Dimensions::Spacing32;

constexpr qint32 WindowWidthDesktopXS = 960;
constexpr qint32 WindowWidthDesktopS  = 1280;
constexpr qint32 WindowWidthDesktopM  = 1440;
constexpr qint32 WindowWidthDesktopL  = 1920;
constexpr qint32 WindowWidthDesktopXL = 2560;

constexpr qreal HorizontalPaddingDesktopXS = 40;
constexpr qreal HorizontalPaddingDesktopS  = 56;
constexpr qreal HorizontalPaddingDesktopM  = 88;
constexpr qreal HorizontalPaddingDesktopL  = 160;
constexpr qreal HorizontalPaddingDesktopXL = 184;

Dimensions& Dimensions::instance()
{
    static Dimensions instance;

    return instance;
}

Dimensions* Dimensions::create(QQmlEngine* engine, QJSEngine* scriptEngine)
{
    Q_UNUSED(scriptEngine)

    Dimensions* instance = &Dimensions::instance();
    engine->setObjectOwnership(instance, QQmlEngine::CppOwnership);

    return instance;
}

qreal Dimensions::columnsWidth(qint32 columns, qreal width) const
{
    Q_UNUSED(width) // we need width for proper binding in QML

    if (columns <= 0)
    {
        return 0;
    }

    qreal columnWidth = (m_windowWidth - m_horizontalPadding * 2 - GutterWidth * (ColumnsCount - 1)) / ColumnsCount;

    qreal columnsWidth = columnWidth * columns + GutterWidth * (columns - 1);

    return columnsWidth;
}

Dimensions::Dimensions(QObject* parent)
    : QObject(parent)
{
    const auto& calculateLayout = [](qint32 windowWidth) -> Layout {
        if (windowWidth < WindowWidthDesktopXS)
        {
            return Layout::DesktopXS;
        }
        else if (windowWidth < WindowWidthDesktopS)
        {
            return Layout::DesktopS;
        }
        else if (windowWidth < WindowWidthDesktopM)
        {
            return Layout::DesktopM;
        }
        else if (windowWidth < WindowWidthDesktopL)
        {
            return Layout::DesktopL;
        }
        else
        {
            return Layout::DesktopXL;
        }
    };

    const auto& calculateHorizontalPadding = [](Layout layout) -> qreal {
        switch (layout)
        {
            case Layout::DesktopXS:
                return HorizontalPaddingDesktopXS;
            case Layout::DesktopS:
                return HorizontalPaddingDesktopS;
            case Layout::DesktopM:
                return HorizontalPaddingDesktopM;
            case Layout::DesktopL:
                return HorizontalPaddingDesktopL;
            case Layout::DesktopXL:
                return HorizontalPaddingDesktopXL;
            default:
                return 0;
        }
    };

    QWindow* window = QGuiApplication::allWindows().at(0);

    connect(window, &QWindow::widthChanged, this, [this, calculateLayout, calculateHorizontalPadding](int width) {
        setWindowWidth(width);

        setLayout(calculateLayout(width));
        setHorizontalPadding(calculateHorizontalPadding(layout()));
    });

    setWindowWidth(window->width());

    setLayout(calculateLayout(window->width()));
    setHorizontalPadding(calculateHorizontalPadding(layout()));
}

void Dimensions::setLayout(Layout layout)
{
    if (m_layout == layout)
    {
        return;
    }

    m_layout = layout;
    emit layoutChanged();
}

void Dimensions::setWindowWidth(qint32 windowWidth)
{
    if (m_windowWidth == windowWidth)
    {
        return;
    }

    m_windowWidth = windowWidth;
    emit windowWidthChanged();
}

void Dimensions::setHorizontalPadding(qreal horizontalPadding)
{
    if (m_horizontalPadding == horizontalPadding)
    {
        return;
    }

    m_horizontalPadding = horizontalPadding;
    emit horizontalPaddingChanged();
}

qint32 Dimensions::columns()
{
    return ColumnsCount;
}

} // namespace XWallet::Style
