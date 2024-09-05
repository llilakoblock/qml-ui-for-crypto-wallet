pragma Singleton

import QtQuick

QtObject {
    readonly property FontLoader sfProDisplayRegular: FontLoader {
        source: Qt.resolvedUrl("Resources/SF-Pro-Display-Regular.otf")
    }

    readonly property FontLoader sfProDisplayMedium: FontLoader {
        source: Qt.resolvedUrl("Resources/SF-Pro-Display-Medium.otf")
    }

    readonly property FontLoader sfProDisplayBold: FontLoader {
        source: Qt.resolvedUrl("Resources/SF-Pro-Display-Bold.otf")
    }

    readonly property FontLoader sfProTextRegular: FontLoader {
        source: Qt.resolvedUrl("Resources/SF-Pro-Text-Regular.otf")
    }

    readonly property FontLoader sfProTextMedium: FontLoader {
        source: Qt.resolvedUrl("Resources/SF-Pro-Text-Medium.otf")
    }

    readonly property FontLoader sfProTextBold: FontLoader {
        source: Qt.resolvedUrl("Resources/SF-Pro-Text-Bold.otf")
    }

    readonly property FontLoader sfCompactDisplayRegular: FontLoader {
        source: Qt.resolvedUrl("Resources/SF-Compact-Display-Regular.otf")
    }

    readonly property FontLoader sfCompactDisplayMedium: FontLoader {
        source: Qt.resolvedUrl("Resources/SF-Compact-Display-Medium.otf")
    }

    readonly property FontLoader sfCompactDisplayBold: FontLoader {
        source: Qt.resolvedUrl("Resources/SF-Compact-Display-Bold.otf")
    }

    readonly property FontLoader sfCompactTextRegular: FontLoader {
        source: Qt.resolvedUrl("Resources/SF-Compact-Text-Regular.otf")
    }

    readonly property FontLoader sfCompactTextMedium: FontLoader {
        source: Qt.resolvedUrl("Resources/SF-Compact-Text-Medium.otf")
    }

    readonly property FontLoader sfCompactTextBold: FontLoader {
        source: Qt.resolvedUrl("Resources/SF-Compact-Text-Bold.otf")
    }

    readonly property FontLoader aldrichRegular: FontLoader {
        source: Qt.resolvedUrl("Resources/Aldrich-Regular.ttf")
    }

    //HEADER
    readonly property font header: Qt.font({
        family: aldrichRegular.font.family,
        weight: aldrichRegular.font.weight,
        pixelSize: 20,
    })

    //PROMO
    readonly property font promo1_wide: Qt.font({
        family: sfProDisplayMedium.font.family,
        weight: sfProDisplayMedium.font.weight,
        pixelSize: 72,
    })

    readonly property font promo_56_wide: Qt.font({
        family: sfProDisplayMedium.font.family,
        weight: sfProDisplayMedium.font.weight,
        pixelSize: 56,
    })

    readonly property font promo2_wide: Qt.font({
        family: sfProDisplayMedium.font.family,
        weight: sfProDisplayMedium.font.weight,
        pixelSize: 44,
    })

    readonly property font promo_36_wide: Qt.font({
        family: sfProDisplayMedium.font.family,
        weight: sfProDisplayMedium.font.weight,
        pixelSize: 36,
    })

    //HEADER
    readonly property font h1_wide: Qt.font({
        family: sfProDisplayMedium.font.family,
        weight: sfProDisplayMedium.font.weight,
        pixelSize: 36,
    })

    readonly property font h1_comp: Qt.font({
        family: sfCompactDisplayMedium.font.family,
        weight: sfCompactDisplayMedium.font.weight,
        pixelSize: 36,
    })

    readonly property font h2_wide: Qt.font({
        family: sfProDisplayMedium.font.family,
        weight: sfProDisplayMedium.font.weight,
        pixelSize: 32,
    })

    readonly property font h2_comp: Qt.font({
        family: sfCompactDisplayMedium.font.family,
        weight: sfCompactDisplayMedium.font.weight,
        pixelSize: 32,
    })

    readonly property font h3_wide: Qt.font({
        family: sfProDisplayMedium.font.family,
        weight: sfProDisplayMedium.font.weight,
        pixelSize: 24,
    })

    readonly property font h3_comp: Qt.font({
        family: sfCompactDisplayMedium.font.family,
        weight: sfCompactDisplayMedium.font.weight,
        pixelSize: 24,
    })

    readonly property font h3_text: Qt.font({
        family: sfProTextMedium.font.family,
        weight: sfProTextMedium.font.weight,
        pixelSize: 24,
    })

    readonly property font h4_wide: Qt.font({
        family: sfProDisplayMedium.font.family,
        weight: sfProDisplayMedium.font.weight,
        pixelSize: 20,
    })

    readonly property font h4_wide_underline: Qt.font({
        family: sfProDisplayMedium.font.family,
        weight: sfProDisplayMedium.font.weight,
        pixelSize: 20,
        underline: true,
    })

    readonly property font h4_comp: Qt.font({
        family: sfCompactDisplayMedium.font.family,
        weight: sfCompactDisplayMedium.font.weight,
        pixelSize: 20,
    })

    readonly property font h4_text: Qt.font({
        family: sfProTextMedium.font.family,
        weight: sfProTextMedium.font.weight,
        pixelSize: 20,
    })

    //PARAGRAPH
    readonly property font p1_regular_comp: Qt.font({
        family: sfCompactTextRegular.font.family,
        weight: sfCompactTextRegular.font.weight,
        pixelSize: 24,
    })

    readonly property font p1_regular_text: Qt.font({
        family: sfProTextRegular.font.family,
        weight: sfProTextRegular.font.weight,
        pixelSize: 24,
    })

    readonly property font p2_medium_comp: Qt.font({
        family: sfCompactTextMedium.font.family,
        weight: sfCompactTextMedium.font.weight,
        pixelSize: 20,
    })

    readonly property font p2_regular_comp: Qt.font({
        family: sfCompactTextRegular.font.family,
        weight: sfCompactTextRegular.font.weight,
        pixelSize: 20,
    })

    readonly property font p2_regular_text: Qt.font({
        family: sfProTextRegular.font.family,
        weight: sfProTextRegular.font.weight,
        pixelSize: 20,
    })

    readonly property font p3_bold_comp: Qt.font({
        family: sfCompactTextBold.font.family,
        weight: sfCompactTextBold.font.weight,
        pixelSize: 17,
    })

    readonly property font p3_medium_comp: Qt.font({
        family: sfCompactTextMedium.font.family,
        weight: sfCompactTextMedium.font.weight,
        pixelSize: 17,
    })

    readonly property font p3_medium_text: Qt.font({
        family: sfProTextMedium.font.family,
        weight: sfProTextMedium.font.weight,
        pixelSize: 17,
    })

    readonly property font p3_regular_comp: Qt.font({
        family: sfCompactTextRegular.font.family,
        weight: sfCompactTextRegular.font.weight,
        pixelSize: 17,
    })

    readonly property font p3_regular_text: Qt.font({
        family: sfProTextRegular.font.family,
        weight: sfProTextRegular.font.weight,
        pixelSize: 17,
    })

    readonly property font p4_bold_wide: Qt.font({
        family: sfProDisplayBold.font.family,
        weight: sfProDisplayBold.font.weight,
        pixelSize: 14,
    })

    readonly property font p4_bold_comp: Qt.font({
        family: sfCompactTextBold.font.family,
        weight: sfCompactTextBold.font.weight,
        pixelSize: 14,
    })

    readonly property font p4_medium_comp: Qt.font({
        family: sfCompactTextMedium.font.family,
        weight: sfCompactTextMedium.font.weight,
        pixelSize: 14,
    })

    readonly property font p4_medium_text: Qt.font({
        family: sfProTextMedium.font.family,
        weight: sfProTextMedium.font.weight,
        pixelSize: 14,
    })

    readonly property font p4_regular_comp: Qt.font({
        family: sfCompactTextRegular.font.family,
        weight: sfCompactTextRegular.font.weight,
        pixelSize: 14,
    })

    readonly property font p4_regular_text: Qt.font({
        family: sfProTextRegular.font.family,
        weight: sfProTextRegular.font.weight,
        pixelSize: 14,
    })

    //CAPTION
    readonly property font c1_bold_wide: Qt.font({
        family: sfProDisplayBold.font.family,
        weight: sfProDisplayBold.font.weight,
        pixelSize: 12,
    })

    readonly property font c1_bold_comp: Qt.font({
        family: sfCompactTextBold.font.family,
        weight: sfCompactTextBold.font.weight,
        pixelSize: 12,
    })

    readonly property font c1_medium_comp: Qt.font({
        family: sfCompactTextMedium.font.family,
        weight: sfCompactTextMedium.font.weight,
        pixelSize: 12,
    })

    readonly property font c1_regular_comp: Qt.font({
        family: sfCompactTextRegular.font.family,
        weight: sfCompactTextRegular.font.weight,
        pixelSize: 12,
    })

    readonly property font c2_bold_wide: Qt.font({
        family: sfProDisplayBold.font.family,
        weight: sfProDisplayBold.font.weight,
        pixelSize: 10,
    })
}
