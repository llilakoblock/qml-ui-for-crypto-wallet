add_subdirectory(Fonts)
add_subdirectory(Images)
add_subdirectory(Controls)

set_source_files_properties(
    Animations.qml
    Colors.qml
    Dimensions.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(Style
    STATIC
    VERSION 1.0
    PLUGIN_TARGET
        StylePlugin
    NO_PLUGIN_OPTIONAL
    URI
        XWallet.Style
    RESOURCE_PREFIX /
    DEFAULT_IMPORTS
        XWallet.Style.Fonts
        XWallet.Style.Images
        XWallet.Style.Controls
    QML_FILES
        Animations.qml
        Colors.qml
    SOURCES
        Dimensions.h Dimensions.cpp
)

target_link_libraries(Style
    PRIVATE
        StyleFonts
        StyleImages
        StyleControls
)
