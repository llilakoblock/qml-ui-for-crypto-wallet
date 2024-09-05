pragma Singleton

import QtQuick

import XWallet.Data

QtObject {
    // Unknown,
    // Bitcoin,
    // Ethereum,
    // // MOCK
    // Tether,
    // Solana,
    // Usdc,
    // Xrp,
    // Dogecoin,
    // Cardano,
    // Polkadot,
    // VaraNetwork,
    // // MOCK
    // Usdt,

    function iconForAssetType(assetType : int) : string {
        switch (assetType) {
            case Asset.Bitcoin:
                return bitcoinIcon
            case Asset.Ethereum:
                return ethereumIcon
            case Asset.Tether:
                return tetherIcon
            case Asset.Solana:
                return solanaIcon
            case Asset.Usdc:
                return usdcIcon
            case Asset.Dogecoin:
                return dogecoinIcon
            case Asset.Cardano:
                return cardanoIcon
            case Asset.Polkadot:
                return polkadotIcon
            case Asset.VaraNetwork:
                return varanetworkIcon
            case Asset.Xrp:
                return xrpIcon

            default:
                return ""
        }
    }
    readonly property url logo: Qt.resolvedUrl("Resources/Logo.svg")

    readonly property url tooltipArrow: Qt.resolvedUrl("Resources/ToolTipArrow.svg")

    readonly property url close: Qt.resolvedUrl("Resources/Close.svg")

    readonly property url check: Qt.resolvedUrl("Resources/Check.svg")
    readonly property url minus: Qt.resolvedUrl("Resources/Minus.svg")

    readonly property url search: Qt.resolvedUrl("Resources/Search.svg")
    readonly property url copy: Qt.resolvedUrl("Resources/Copy.svg")
    readonly property url pencil: Qt.resolvedUrl("Resources/Pencil.svg")

    readonly property url eyeOpen: Qt.resolvedUrl("Resources/EyeOpen.svg")
    readonly property url eyeClosed: Qt.resolvedUrl("Resources/EyeClosed.svg")

    readonly property url chevronUp: Qt.resolvedUrl("Resources/ChevronUp.svg")
    readonly property url chevronDown: Qt.resolvedUrl("Resources/ChevronDown.svg")

    readonly property url arrowUp: Qt.resolvedUrl("Resources/ArrowUp.svg")
    readonly property url arrowDown: Qt.resolvedUrl("Resources/ArrowDown.svg")
    readonly property url arrowLeft: Qt.resolvedUrl("Resources/ArrowLeft.svg")
    readonly property url arrowRight: Qt.resolvedUrl("Resources/ArrowRight.svg")
    readonly property url arrowRightUp: Qt.resolvedUrl("Resources/ArrowRightUp.svg")
    readonly property url arrowRightDown: Qt.resolvedUrl("Resources/ArrowRightDown.svg")
    readonly property url arrowsSwitch: Qt.resolvedUrl("Resources/ArrowsSwitch.svg")

    readonly property url swap: Qt.resolvedUrl("Resources/Swap.svg")

    readonly property url settings: Qt.resolvedUrl("Resources/Settings.svg")

    readonly property url bitcoinIcon: Qt.resolvedUrl("Resources/Coins/Bitcoin.svg")
    readonly property url ethereumIcon: Qt.resolvedUrl("Resources/Coins/Ethereum.svg")
    readonly property url tetherIcon: Qt.resolvedUrl("Resources/Coins/Tether.svg")
    readonly property url cardanoIcon: Qt.resolvedUrl("Resources/Coins/Cardano.svg")
    readonly property url dogecoinIcon: Qt.resolvedUrl("Resources/Coins/Dogecoin.svg")
    readonly property url polkadotIcon: Qt.resolvedUrl("Resources/Coins/Polkadot.svg")
    readonly property url solanaIcon: Qt.resolvedUrl("Resources/Coins/Solana.svg")
    readonly property url usdcIcon: Qt.resolvedUrl("Resources/Coins/USDC.svg")
    readonly property url varanetworkIcon: Qt.resolvedUrl("Resources/Coins/VaraNetwork.svg")
    readonly property url xrpIcon: Qt.resolvedUrl("Resources/Coins/XRP.svg")

    readonly property url successCircle: Qt.resolvedUrl("Resources/SuccessCircle.svg")
    readonly property url errorCircle: Qt.resolvedUrl("Resources/ErrorCircle.svg")

    readonly property url biometricCoverArt: Qt.resolvedUrl("Resources/Art/BiometricCoverMan.png")
    readonly property url inheritanceCoverArt: Qt.resolvedUrl("Resources/Art/InheritanceCover.png")
}
