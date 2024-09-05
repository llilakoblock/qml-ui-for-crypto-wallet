set_source_files_properties(
    Fonts.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(StyleFonts
    STATIC
    VERSION 1.0
    NO_PLUGIN
    URI
        XWallet.Style.Fonts
    QML_FILES
        Fonts.qml
)

qt_add_resources(StyleFonts "xwallet_style_fonts"
    PREFIX
        "XWallet/Style/Fonts/"
    FILES
        Resources/SF-Compact-Display-Bold.otf
        Resources/SF-Compact-Display-Medium.otf
        Resources/SF-Compact-Display-Regular.otf
        Resources/SF-Compact-Text-Bold.otf
        Resources/SF-Compact-Text-Medium.otf
        Resources/SF-Compact-Text-Regular.otf
        Resources/SF-Pro-Display-Bold.otf
        Resources/SF-Pro-Display-Medium.otf
        Resources/SF-Pro-Display-Regular.otf
        Resources/SF-Pro-Text-Bold.otf
        Resources/SF-Pro-Text-Medium.otf
        Resources/SF-Pro-Text-Regular.otf
        Resources/Aldrich-Regular.ttf
)
