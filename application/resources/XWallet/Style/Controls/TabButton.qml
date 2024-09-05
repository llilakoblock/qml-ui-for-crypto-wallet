import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

import XWallet.Style
import XWallet.Style.Fonts

T.TabButton {
    id: control

    focusPolicy: Qt.StrongFocus

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    padding: 12
    spacing: Dimensions.Spacing4

    icon.width: 24
    icon.height: 24
    icon.color: control.color

    font: Fonts.h4_wide

    property color color: Colors.textPrimary

    HoverHandler {
        cursorShape: Qt.PointingHandCursor
    }

    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font
        color: control.color
    }

    background: Item {}
}
