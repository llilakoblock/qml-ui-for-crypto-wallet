pragma Singleton

import QtQuick

import XWallet.Data

QtObject {
    function colorForAssetType(assetType : int) : color {
        switch (assetType) {
            case Asset.Bitcoin:
                return bitcoin
            case Asset.Ethereum:
                return ethereum
            case Asset.Tether:
                return tether
            case Asset.Solana:
                return solana
            case Asset.Usdc:
                return usdc
            case Asset.Dogecoin:
                return dogecoin
            case Asset.Cardano:
                return cardano
            case Asset.Polkadot:
                return polkadot
            case Asset.VaraNetwork:
                return varanetwork
            case Asset.Xrp:
                return xrp
            default:
                return grayscale000
        }
    }

    readonly property Gradient cardBorderGradient: Gradient {
        orientation: Gradient.Horizontal

        GradientStop { position: 0.0; color: "#36FFFFFF" }
        GradientStop { position: 1.0; color: "#B2FFFFFF" }
    }

    readonly property Gradient promoGradient: Gradient {
        orientation: Gradient.Horizontal

        GradientStop { position: 0.0; color: "#ADA2FE" }
        GradientStop { position: 0.8; color: "#FFFFFF" }
        GradientStop { position: 1.0; color: "#FFFFFF" }
    }

    readonly property Gradient progressBarGradient: Gradient {
        orientation: Gradient.Horizontal

        GradientStop { position: 0.0; color: "#9BA5FF" }
        GradientStop { position: 0.3; color: "#7B72EF" }
        GradientStop { position: 0.6; color: "#6654F6" }
        GradientStop { position: 1.0; color: "#7828DD" }
    }

    readonly property color acent: "#3AA6FB"

    readonly property color buttonPrimary: "#0C0C23"
    readonly property color buttonPrimaryHover: "#111131"

    readonly property color buttonSecondary: "#B26A6C8D"
    readonly property color buttonSecondaryHover: "#B28284A1"

    readonly property color buttonTertiary: "#00000000"
    readonly property color buttonTertiaryHover: "#1A8284A1"

    readonly property color textPrimary: "#FAFAFA"
    readonly property color textSecondary: "#969FA8"
    readonly property color textTertiary: "#626C77"
    readonly property color textInverted: "#1D2023"
    readonly property color textPositive: "#74DF8B"
    readonly property color textNegative: "#FFA080"
    readonly property color textPrimaryLink: "#45B6FC"
    readonly property color textVisitedLink: "#B767B7"
    readonly property color textPrimaryLinkInverted: "#0070E5"
    readonly property color textSecondaryLink: "#969FA8"
    readonly property color textInactiveTabBar: "#75FFFFFF"

    readonly property color backgroundPrimary: "#FFFFFF"
    readonly property color backgroundPrimaryElevated: "#FFFFFF"
    readonly property color backgroundModal: "#FFFFFF"
    readonly property color backgroundStroke: "#80BCC3D0"
    readonly property color backgroundSecondary: "#F2F3F7"
    readonly property color backgroundSecondaryElevated: "#FFFFFF"
    readonly property color backgroundOverlay: "#661D2023"
    readonly property color backgroundInverted: "#1D2023"
    readonly property color backgroundHover: "#40BCC3D0"
    readonly property color backgroundBar: "#E0FFFFFF"
    readonly property color backgroundDisabled: "#F8F8FB"
    readonly property color backgroundStrokeDisabled: "#40BCC3D0"
    readonly property color backgroundLower: "#F2F3F7"
    readonly property color backgroundPositive: "#74DF8B"
    readonly property color backgroundNegative: "#FFA080"

    readonly property color iconPrimary: "#FAFAFA"
    readonly property color iconSecondary: "#969FA8"
    readonly property color iconTertiary: "#626C77"
    readonly property color iconPositive: "#74DF8B"
    readonly property color iconNegative: "#FFA080"

    readonly property color grayscale900: "#000000"
    readonly property color grayscale800: "#1D2023"
    readonly property color grayscale700: "#2C3135"
    readonly property color grayscale600: "#434A51"
    readonly property color grayscale500: "#626C77"
    readonly property color grayscale400: "#969FA8"
    readonly property color grayscale300: "#BBC1C7"
    readonly property color grayscale200: "#E2E5EB"
    readonly property color grayscale100: "#F2F3F7"
    readonly property color grayscale000: "#FFFFFF"

    readonly property color transparent: "#00000000"

    // Crypto assets colors
    readonly property color bitcoin: "#F7931A"
    readonly property color ethereum: "#CFCFCF"
    readonly property color usdt: "#4991DF"
    readonly property color usdc: "#4991DF"
    readonly property color tether: "#26BF96"
    readonly property color solana: "#9945FF"
    readonly property color dogecoin: "#F2BA02"
    readonly property color cardano: "#3468D1"
    readonly property color polkadot: "#E6007A"
    readonly property color varanetwork: "#26FCC4"
    readonly property color xrp: "#FFFFFF"
}
