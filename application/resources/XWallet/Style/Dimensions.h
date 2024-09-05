#pragma once

#include <QObject>
#include <QtQmlIntegration>

class QQmlEngine;
class QJSEngine;

namespace XWallet::Style {

class Dimensions final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_DISABLE_COPY_MOVE(Dimensions)

    Q_PROPERTY(Layout layout READ layout NOTIFY layoutChanged FINAL)
    Q_PROPERTY(qint32 windowWidth READ windowWidth NOTIFY windowWidthChanged FINAL)
    Q_PROPERTY(qreal horizontalPadding READ horizontalPadding NOTIFY horizontalPaddingChanged FINAL)

    Q_PROPERTY(qint32 columns READ columns CONSTANT FINAL)

public:
    enum class Layout
    {
        Unknown,
        DesktopXS,
        DesktopS,
        DesktopM,
        DesktopL,
        DesktopXL
    };
    Q_ENUM(Layout)

    enum Spacing
    {
        Spacing2  = 2,
        Spacing4  = 4,
        Spacing8  = 8,
        Spacing12 = 12,
        Spacing16 = 16,
        Spacing20 = 20,
        Spacing24 = 24,
        Spacing32 = 32,
        Spacing40 = 40,
        Spacing48 = 48,
        Spacing56 = 56,
        Spacing64 = 64,
        Spacing72 = 72,
        Spacing80 = 80,
    };
    Q_ENUM(Spacing)

public:
    static Dimensions& instance();
    static Dimensions* create(QQmlEngine* engine, QJSEngine* scriptEngine);

    Q_INVOKABLE qreal columnsWidth(qint32 columns, qreal width) const;

private:
    Dimensions(QObject* parent = nullptr);

    Layout layout() const { return m_layout; }
    void   setLayout(Layout layout);

    qint32 windowWidth() const { return m_windowWidth; }
    void   setWindowWidth(qint32 windowWidth);

    qreal horizontalPadding() const { return m_horizontalPadding; }
    void  setHorizontalPadding(qreal horizontalPadding);

    static qint32 columns();

signals:
    void layoutChanged();
    void windowWidthChanged();
    void horizontalPaddingChanged();

private:
    Layout m_layout { Layout::Unknown };
    qint32 m_windowWidth { 0 };
    qreal  m_horizontalPadding { 0.0 };
};

} // namespace XWallet::Style
