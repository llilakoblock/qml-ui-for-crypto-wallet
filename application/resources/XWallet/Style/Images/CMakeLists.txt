set_source_files_properties(
    Images.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(StyleImages
    STATIC
    VERSION 1.0
    NO_PLUGIN
    URI
        XWallet.Style.Images
    RESOURCE_PREFIX /
    RESOURCES
        Resources/Logo.svg

        Resources/ArrowUp.svg
        Resources/ArrowDown.svg
        Resources/ArrowLeft.svg
        Resources/ArrowRight.svg
        Resources/ArrowRightUp.svg
        Resources/ArrowRightDown.svg
        Resources/ArrowsSwitch.svg
        Resources/Swap.svg
        Resources/ToolTipArrow.svg
        Resources/Check.svg
        Resources/Copy.svg
        Resources/Close.svg
        Resources/Minus.svg
        Resources/EyeOpen.svg
        Resources/EyeClosed.svg
        Resources/ChevronUp.svg
        Resources/ChevronDown.svg
        Resources/Search.svg
        Resources/Settings.svg
        Resources/Pencil.svg

        Resources/SuccessCircle.svg
        Resources/ErrorCircle.svg

        Resources/Coins/Bitcoin.svg
        Resources/Coins/Cardano.svg
        Resources/Coins/Dogecoin.svg
        Resources/Coins/Ethereum.svg
        Resources/Coins/Polkadot.svg
        Resources/Coins/Solana.svg
        Resources/Coins/Tether.svg
        Resources/Coins/USDC.svg
        Resources/Coins/VaraNetwork.svg
        Resources/Coins/XRP.svg

        Resources/Art/BiometricCoverMan.png
        Resources/Art/InheritanceCover.png
    QML_FILES
        Images.qml
)
