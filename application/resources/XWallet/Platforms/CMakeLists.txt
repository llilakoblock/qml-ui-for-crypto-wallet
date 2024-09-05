cmake_minimum_required(VERSION 3.24)

find_package(Qt6
    COMPONENTS
        Qml
    REQUIRED
)

set_source_files_properties(Platforms.qml
    PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(Platforms
    STATIC
    VERSION
        1.0
    NO_PLUGIN
    URI
        XWallet.Platforms
    RESOURCE_PREFIX
        /
    QML_FILES
        Platforms.qml
)
