add_subdirectory(StartupPageScreens)
add_subdirectory(WalletPageScreens)

qt_add_qml_module(Pages
    STATIC
    VERSION
        1.0
    NO_PLUGIN
    URI
        XWallet.UI.Pages
    RESOURCE_PREFIX
        /
    QML_FILES
        StartupPage.qml
        SplashPage.qml
        WalletPage.qml
)

target_link_libraries(Pages
    PRIVATE
        StartupPageScreens
        WalletPageScreens
)
