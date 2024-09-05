#include "ApplicationSettings.h"

#include <QQmlEngine>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickWindow>

namespace XWallet::Singletones {

ApplicationSettings& ApplicationSettings::instance()
{
    static ApplicationSettings instance;

    return instance;
}

ApplicationSettings* ApplicationSettings::create(QQmlEngine* engine, QJSEngine* scriptEngine)
{
    Q_UNUSED(scriptEngine)

    ApplicationSettings* instance = &ApplicationSettings::instance();
    engine->setObjectOwnership(instance, QQmlEngine::CppOwnership);

    return instance;
}

void ApplicationSettings::setGraphics(Graphics graphics)
{
    if (m_graphics == graphics)
    {
        return;
    }
    m_graphics = graphics;
    emit graphicsChanged();
}

QString ApplicationSettings::password() const
{
    return m_password;
}
void ApplicationSettings::setPassword(QString password)
{
    if (m_password == password)
    {
        return;
    }
    emit passwordChanged();
    m_password = password;
}

ApplicationSettings::ApplicationSettings(QObject* parent)
    : QObject(parent)
{
}

} // namespace XWallet::Singletones
