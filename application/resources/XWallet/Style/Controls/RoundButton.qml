import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

import XWallet.Style

T.RoundButton {
    id: control

    focusPolicy: Qt.StrongFocus

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    padding: 10
    spacing: 4

    icon.width: 24
    icon.height: 24
    icon.color: control.color

    radius: Math.min(background.width, background.height) / 2

    property color color: {
        if(!enabled || !hovered || down) {
            return Colors.iconSecondary
        }

        if(hovered) {
            Colors.iconPrimary
        }
    }

    HoverHandler {
        cursorShape: Qt.PointingHandCursor
    }

    Behavior on icon.color {
        ColorAnimation {
            easing.type: Animations.easingType
            duration: Animations.shortDuration
        }
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

    background: Rectangle {
        implicitWidth: 20
        implicitHeight: 20

        radius: control.radius

        color: {
            if(control.hovered) {
                return Colors.buttonPrimaryHover
            }
            else {
                return Colors.buttonPrimary
            }
        }

        Behavior on color {
            ColorAnimation { easing.type: Animations.easingType; duration: Animations.shortDuration }
        }
    }
}
