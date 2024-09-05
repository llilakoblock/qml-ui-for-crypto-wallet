find_package(Qt6 COMPONENTS Core REQUIRED)

set(SOURCES
    src/Application.cpp
    src/Asset.cpp
    src/Balance.cpp
    src/Currency.cpp
    src/HistoryEntry.cpp
    src/State.cpp
    src/Wallet.cpp
    src/Quote.cpp
    src/Step.cpp
)

set(HEADERS
    src/ApplicationData.h
    src/AssetData.h
    src/BalanceData.h
    src/CurrencyData.h
    src/HistoryEntryData.h
    src/StateData.h
    src/WalletData.h
    src/QuoteData.h
    src/StepData.h
)

set(PUBLIC_HEADERS
    include/data/Application.h
    include/data/Asset.h
    include/data/Balance.h
    include/data/Currency.h
    include/data/HistoryEntry.h
    include/data/State.h
    include/data/Wallet.h
    include/data/Quote.h
    include/data/Step.h
    include/data/NumericTypes.h
)

add_library(Data STATIC ${HEADERS} ${SOURCES} ${PUBLIC_HEADERS})

qt_add_qml_module(Data
    VERSION
        1.0
    URI
        XWallet.Data
    PLUGIN_TARGET
        DataPlugin
    NO_PLUGIN_OPTIONAL
    SOURCES src/SwapData.h
    SOURCES include/data/Swap.h
    SOURCES src/SwapLogData.h
    SOURCES include/data/SwapLog.h
    SOURCES src/Swap.cpp
    SOURCES src/SwapLog.cpp
    SOURCES src/OrderData.h
    SOURCES include/data/Order.h
    SOURCES src/Order.cpp
    SOURCES include/data/Fee.h
    SOURCES src/FeeData.h
    SOURCES src/Fee.cpp
)

target_include_directories(Data
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src
        include/data
)

target_link_libraries(Data
    PRIVATE
    Qt6::Core
    Boost::boost
)
