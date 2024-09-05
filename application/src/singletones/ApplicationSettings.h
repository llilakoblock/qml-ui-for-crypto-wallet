#pragma once

#include <QObject>
#include <QtQmlIntegration>

class QQmlEngine;
class QJSEngine;

namespace XWallet::Singletones {

class ApplicationSettings final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_DISABLE_COPY_MOVE(ApplicationSettings)

    Q_PROPERTY(Graphics graphics READ graphics WRITE setGraphics NOTIFY graphicsChanged FINAL)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged FINAL)

public:
    enum class Graphics
    {
        FullEffects,
        SimplifiedEffects
    };
    Q_ENUM(Graphics)

public:
    static ApplicationSettings& instance();
    static ApplicationSettings* create(QQmlEngine* engine, QJSEngine* scriptEngine);

    Graphics graphics() const { return m_graphics; }
    void     setGraphics(Graphics graphics);

    QString password() const;
    void    setPassword(QString password);

private:
    ApplicationSettings(QObject* parent = nullptr);

signals:
    void graphicsChanged();
    void passwordChanged();

private:
    Graphics m_graphics { Graphics::FullEffects };
    QString  m_password {};
};

} // namespace XWallet::Singletones
