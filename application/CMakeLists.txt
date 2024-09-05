cmake_minimum_required(VERSION 3.16)

set(DATABASE_VERSION 1.0.0)

set(QML_IMPORT_PATH
    ${QML_IMPORT_PATH}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/resources
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}/qml
    CACHE STRING "" FORCE
)

find_package(Qt6
    COMPONENTS
        Qml
        Quick
        QuickControls2
        Network
        Multimedia
        ShaderTools
        Svg
        Concurrent
        Core
        LinguistTools
        WebEngineQuick
    REQUIRED
)

find_package(libqrencode REQUIRED)
find_package(PostgreSQL REQUIRED)

add_subdirectory(resources)
add_subdirectory(modules/data)
add_subdirectory(modules/biometric)

set(SOURCES_LIST
    src/main.cpp

    src/misc/Initializer.h
    src/misc/Visitor.h
    src/misc/GadgetsListModel.h
    src/misc/Streamer.h
    src/misc/VariantCast.h
    src/misc/SortFilterProxyModelByCondition.h src/misc/SortFilterProxyModelByCondition.cpp

    src/actions/RootActions.h
    src/actions/ApplicationActions.h
    src/actions/AssetActions.h
    src/actions/StateActions.h
    src/actions/WalletActions.h
    src/actions/NetworkServiceActions.h
    src/actions/StepActions.h
    src/actions/OrderActions.h
    src/actions/SwapActions.h
    src/actions/WalletServiceActions.h
    src/actions/DatabaseServiceActions.h

    src/middleware/LoggingMiddleware.h src/middleware/LoggingMiddleware.cpp
    src/middleware/DatabaseMiddleware.h src/middleware/DatabaseMiddleware.cpp
    src/middleware/WalletMiddleware.h src/middleware/WalletMiddleware.cpp
    src/middleware/TaskMiddleware.h src/middleware/TaskMiddleware.cpp
    src/middleware/NetworkMiddleware.h src/middleware/NetworkMiddleware.cpp

    src/models/AssetsModel.h src/models/AssetsModel.cpp
    src/models/HistoryModel.h src/models/HistoryModel.cpp
    src/models/SwapsModel.h src/models/SwapsModel.cpp
    src/models/OrdersModel.h src/models/OrdersModel.cpp
    src/models/AxesModel.h src/models/AxesModel.cpp
    src/models/ConcatenatedSwapsModel.h src/models/ConcatenatedSwapsModel.cpp

    src/presenters/AssetPresenter.h src/presenters/AssetPresenter.cpp
    src/presenters/WalletPresenter.h src/presenters/WalletPresenter.cpp

    src/reducers/StateReducer.h src/reducers/StateReducer.cpp
    src/reducers/ApplicationReducer.h src/reducers/ApplicationReducer.cpp
    src/reducers/AssetsReducer.h src/reducers/AssetsReducer.cpp
    src/reducers/WalletReducer.h src/reducers/WalletReducer.cpp
    src/reducers/StepReducer.h src/reducers/StepReducer.cpp
    src/reducers/SwapsReducer.h src/reducers/SwapsReducer.cpp
    src/reducers/OrdersReducer.h src/reducers/OrdersReducer.cpp

    src/services/NetworkService.h src/services/NetworkService.cpp
    src/services/DatabaseService.h src/services/DatabaseService.cpp

    src/singletones/Store.h src/singletones/Store.cpp
    src/singletones/ApplicationSettings.h src/singletones/ApplicationSettings.cpp

    src/viewmodels/ApplicationViewModel.h src/viewmodels/ApplicationViewModel.cpp
    src/viewmodels/StartupViewModel.h src/viewmodels/StartupViewModel.cpp
    src/viewmodels/WalletViewModel.h src/viewmodels/WalletViewModel.cpp
    src/viewmodels/LineChartViewModel.h src/viewmodels/LineChartViewModel.cpp
    src/viewmodels/SendViewModel.h src/viewmodels/SendViewModel.cpp
    src/viewmodels/SwapViewModel.h src/viewmodels/SwapViewModel.cpp
    src/viewmodels/BiometricViewModel.h src/viewmodels/BiometricViewModel.cpp
    src/viewmodels/HistoryViewModel.h src/viewmodels/HistoryViewModel.cpp
    src/viewmodels/SwapHistoryViewModel.h src/viewmodels/SwapHistoryViewModel.cpp

    src/services/WalletService.h src/services/WalletService.cpp
    src/services/TasksService.h src/services/TasksService.cpp

    src/providers/QrCodeImageProvider.h src/providers/QrCodeImageProvider.cpp
)

qt_add_executable(Application ${SOURCES_LIST})

qt_add_shaders(Application "xwallet_shaders"
    BATCHABLE
    ZORDER_LOC
    PRECOMPILE
    OPTIMIZED
    BASE
        shaders
    PREFIX
        "/shaders"
    FILES
        shaders/liquid_gradient.frag
        shaders/liquid_gradient.vert
        shaders/linear_gradient.frag
)

qt_add_resources(Application "xwallet_resources"
    PREFIX
        "/"
    FILES
        qtquickcontrols2.conf
)

set_source_files_properties(resources/Database/${DATABASE_VERSION}/database.sql
    PROPERTIES
    QT_RESOURCE_ALIAS initial/database.sql
)

qt_add_resources(Application "database_resources"
    PREFIX
        "/database"
    BASE
        "resources/Database"
    FILES
        resources/Database/${DATABASE_VERSION}/database.sql
)

qt_add_resources(Application "webapi"
    PREFIX
        "/webapi"
    BASE
        "../src/core/private/webapi"
    FILES
        ../src/core/private/webapi/index.html
        ../src/core/private/webapi/gear-js-v2.js
        ../src/core/private/webapi/qwebchannel.js
        ../src/core/private/webapi/htlc.meta.txt
        ../src/core/private/webapi/order_book.meta.txt
)

qt_add_qml_module(Application
    VERSION
        1.0
    URI
        XWallet.Natives
    NO_PLUGIN
)

# qt_add_translations(Application
#     TS_FILES
#         translations/wallet.en.ts
#         translations/wallet.ru.ts
#         translations/wallet.vi.ts
#         translations/wallet.km.ts
#     SOURCES
#         ${TRANSLATION_SOURCES}
#     INCLUDE_DIRECTORIES
#         ${CMAKE_SOURCE_DIR}
#     RESOURCE_PREFIX
#         translations
# )

# add_dependencies(Application update_translations release_translations)

target_include_directories(Application
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/viewmodels
        ${CMAKE_CURRENT_SOURCE_DIR}/src/models
        ${CMAKE_CURRENT_SOURCE_DIR}/src/presenters
        ${CMAKE_CURRENT_SOURCE_DIR}/src/singletones
)

target_link_libraries(Application
    PRIVATE
        Qt6::Qml
        Qt6::Quick
        Qt6::Gui
        Qt6::QuickControls2
        Qt6::Network
        Qt6::Multimedia
        Qt6::Svg
        Qt6::Concurrent
        Qt6::WebEngineQuick
        core
        Biometric
        Resources
        Data
        DataPlugin
        StylePlugin
        libqrencode::libqrencode
        OpenSSL::SSL
        OpenSSL::Crypto
        PostgreSQL::PostgreSQL
)

if(IOS)
    # Asset catalog root
    target_sources(Application PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/ios/Assets.xcassets")
    set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/ios/Assets.xcassets"
        PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources
    )

    # Asset catalog app icon set
    list(APPEND app_icon_set "${CMAKE_CURRENT_SOURCE_DIR}/ios/Assets.xcassets/AppIcon.appiconset")
    set_source_files_properties(${app_icon_set}
        PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources/Assets.xcassets
    )

    # Asset catalog icon files
    file(GLOB app_icon_files CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ios/Assets.xcassets/AppIcon.appiconset/*.png")
    list(APPEND app_icon_files "${CMAKE_CURRENT_SOURCE_DIR}/ios/Assets.xcassets/AppIcon.appiconset/Contents.json")
    set_source_files_properties(${app_icon_set}
        PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources/Assets.xcassets/AppIcon.appiconset
    )
endif()

if(APPLE AND NOT IOS)
    set(MACOSX_BUNDLE_ICON_FILE AppIcon)
    set(APP_ICON_MACOS "${CMAKE_CURRENT_SOURCE_DIR}/deployment/macos/AppIcon.icns")

    set_source_files_properties(${APP_ICON_MACOS}
        PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources"
    )

    target_sources(Application PUBLIC "${APP_ICON_MACOS}")
endif()

if(GENERATE_DEBUG_SYMBOLS)
    set(XCODE_GENERATE_DEBUGGING_SYMBOLS "YES")
else()
    set(XCODE_GENERATE_DEBUGGING_SYMBOLS "NO")
endif()

set_target_properties(Application
    PROPERTIES
        QT_QML_IMPORT_PATH "${CMAKE_BINARY_DIR}/qml"
        QT_QML_ROOT_PATH "${CMAKE_BINARY_DIR}/qml"
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/deployment/ios/Info.plist.in"
        MACOSX_BUNDLE_INFO_STRING "Created by XWallet team"
        MACOSX_BUNDLE_BUNDLE_NAME "Freedom"
        MACOSX_BUNDLE_LAUNCHSCREEN_STORYBOARD "LaunchScreen"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.app"
        MACOSX_BUNDLE_BUNDLE_VERSION "${CMAKE_PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${CMAKE_PROJECT_VERSION}"
        MACOSX_BUNDLE_LONG_VERSION_STRING "${CMAKE_PROJECT_VERSION}"
        MACOSX_BUNDLE TRUE
        #WIN32_EXECUTABLE TRUE
)

install(TARGETS Application
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(IMPORTED_RUNTIME_ARTIFACTS Application
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_qml_app_script(TARGET Application
    OUTPUT_SCRIPT
        QT_DEPLOYMENT_SCRIPT
    MACOS_BUNDLE_POST_BUILD
    NO_UNSUPPORTED_PLATFORM_ERROR
    DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
)

install(SCRIPT ${QT_DEPLOYMENT_SCRIPT})

if(WIN32)
    install(CODE [[
        file(RENAME "${CMAKE_INSTALL_PREFIX}/QtWebEngineProcess.exe" "${CMAKE_INSTALL_PREFIX}/bin/QtWebEngineProcess.exe")
    ]])
endif()

set(CPACK_PACKAGE_VENDOR "Freedom Team")
set(CPACK_PACKAGE_NAME "Freedom Wallet")
set(CPACK_PACKAGE_CONTACT "Freedom Team <no-reply@freedom.it>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Freedom Wallet Application for Desktop")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}")

set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "Freedom")
set(CPACK_CREATE_DESKTOP_LINKS "${PROJECT_NAME}")
set(CPACK_STRIP_FILES TRUE)
set(CPACK_VERBATIM_VARIABLES TRUE)

set(CPACK_OUTPUT_CONFIG_FILE "${CMAKE_BINARY_DIR}/BundleConfig.cmake")

add_custom_target(bundle
                  COMMAND ${CMAKE_CPACK_COMMAND} "--config" "${CMAKE_BINARY_DIR}/BundleConfig.cmake"
                  COMMENT "Running CPACK. Please wait..."
                  DEPENDS Application)

unset(CPACK_GENERATOR)

if(WIN32)
    set(CPACK_GENERATOR "WIX")
    set(CPACK_WIX_VERSION 3)
    set(CPACK_WIX_UPGRADE_GUID "D8E9D5B3-3A6A-4A3E-8D7E-3F9E5A0A4E9A")
    set(CPACK_WIX_PRODUCT_GUID "D8E9D5B3-3A6A-4A3E-8D7E-3F9E5A0A4E9A")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
    set(CPACK_DMG_FORMAT "UDBZ")
    set(CPACK_DMG_FILESYSTEM "APFS")
else()
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_SECTION "net")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS libc6 libstdc++6 libgcc-s1)
    set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION TRUE)
endif()

include(CPack)

