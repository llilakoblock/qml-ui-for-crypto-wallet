#include "Application.h"
#include "providers/QrCodeImageProvider.h"
#include "singletones/ApplicationSettings.h"
#include "singletones/Store.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngineQuick>

using namespace Qt::Literals::StringLiterals;

int main(int argc, char* argv[])
{
    // qputenv("QT_DEBUG_PLUGINS", QByteArray("1"));
    QtWebEngineQuick::initialize();

    XWallet::Application app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Freedom Desktop"_L1);
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption dataDirectoryOption({ "d", "datadir" },
                                           QCoreApplication::translate("main", "Relative path for storing wallet data"),
                                           QCoreApplication::translate("main", "directory"));
    parser.addOption(dataDirectoryOption);

    QCommandLineOption graphicsOption({ "g", "graphics" },
                                      QCoreApplication::translate("main", "Graphics mode (full or simplified)"),
                                      QCoreApplication::translate("main", "graphics"));

    parser.addOption(graphicsOption);

    QCommandLineOption passwordOption({ "p", "password" },
                                      QCoreApplication::translate("main", "Auto password mode"),
                                      QCoreApplication::translate("main", "password"));

    parser.addOption(passwordOption);

    parser.parse(app.arguments());

    app.setApplicationName("Freedom"_L1);
    app.setInstanceName(parser.value(dataDirectoryOption));
    app.initSubstrateObserver();

    auto& settings = XWallet::Singletones::ApplicationSettings::instance();

    settings.setGraphics(parser.value(graphicsOption) == "simplified"_L1 ?
                             XWallet::Singletones::ApplicationSettings::Graphics::SimplifiedEffects :
                             XWallet::Singletones::ApplicationSettings::Graphics::FullEffects);

    settings.setPassword(parser.value(passwordOption));

    XWallet::Store::instance();

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/"_L1);

    engine.addImageProvider("qrcode"_L1, new XWallet::Providers::QrCodeImageProvider);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() {
        XWallet::Application::exit(-1);
    }, Qt::QueuedConnection);

    engine.loadFromModule("XWallet.UI", "Main");

    return XWallet::Application::exec();
}
