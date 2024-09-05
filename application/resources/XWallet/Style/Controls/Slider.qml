import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

import XWallet.Style
import XWallet.Style.Fonts

T.Slider {
    id: control

    focusPolicy: Qt.StrongFocus

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitHandleWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitHandleHeight + topPadding + bottomPadding)

    padding: 6

    Label {
        x: control.handle.x + (control.handle.width - width) / 2
        y: control.handle.y + control.handle.height + 8

        text: control.value.toFixed(0)

        font: Fonts.p3_regular_text
    }

    HoverHandler {
        cursorShape: Qt.PointingHandCursor
    }

    handle: Rectangle {
        x: control.leftPadding + (control.horizontal ? control.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : control.visualPosition * (control.availableHeight - height))

        implicitWidth: 20
        implicitHeight: 20

        radius: width / 2

        color: control.pressed ? Colors.backgroundPrimaryElevated : Colors.backgroundPrimary

        border.width: 4
        border.color: Color.transparent(Colors.grayscale000, 0.18)
    }

    background: Rectangle {
        x: control.leftPadding + (control.horizontal ? 0 : (control.availableWidth - width) / 2)
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : 0)

        implicitWidth: control.horizontal ? 200 : 4
        implicitHeight: control.horizontal ? 4 : 200

        width: control.horizontal ? control.availableWidth : implicitWidth
        height: control.horizontal ? implicitHeight : control.availableHeight

        radius: 2

        color: Colors.backgroundStrokeDisabled

        scale: control.horizontal && control.mirrored ? -1 : 1

        Rectangle {
            y: control.horizontal ? 0 : control.visualPosition * parent.height

            width: control.horizontal ? control.position * parent.width : 4
            height: control.horizontal ? 4 : control.position * parent.height

            radius: 2

            color: Colors.grayscale400
        }
    }
}
