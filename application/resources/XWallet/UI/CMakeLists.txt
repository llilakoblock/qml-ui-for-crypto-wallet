find_package(Qt6
    COMPONENTS
        Qml
        Quick
        QuickControls2
        QuickTemplates2
    REQUIRED
)

add_subdirectory(Pages)
add_subdirectory(Components)
add_subdirectory(Drawers)
add_subdirectory(Delegates)

qt_add_qml_module(UI
    STATIC
    VERSION
        1.0
    NO_PLUGIN
    URI
        XWallet.UI
    RESOURCE_PREFIX
        /
    QML_FILES
        Main.qml
)

target_link_libraries(UI
    PRIVATE
        Pages
        Components
        Drawers
        Delegates
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::QuickTemplates2
)
