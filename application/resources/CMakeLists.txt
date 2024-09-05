add_subdirectory(XWallet/UI)
add_subdirectory(XWallet/Effects)
add_subdirectory(XWallet/Platforms)
add_subdirectory(XWallet/Style)

qt_add_library(Resources INTERFACE)

target_link_libraries(Resources
    INTERFACE
        UI
        Effects
        Platforms
        Style
)
