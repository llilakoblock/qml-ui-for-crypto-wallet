import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

import XWallet.Style
import XWallet.Style.Fonts
import XWallet.Style.Images

T.TextField {
    id: control

    implicitWidth: implicitBackgroundWidth + leftInset + rightInset ||
                   Math.max(contentWidth, placeholder.implicitWidth) + leftPadding + rightPadding

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding,
                             placeholder.implicitHeight + topPadding + bottomPadding)

    leftPadding: 48
    rightPadding: 48
    topPadding: 12
    bottomPadding: 12

    color: Colors.textPrimary

    selectionColor: control.palette.highlight
    selectedTextColor: control.palette.highlightedText

    placeholderTextColor: Colors.backgroundSecondary

    verticalAlignment: TextInput.AlignVCenter

    font: Fonts.p3_regular_comp

    Label {
        id: placeholder

        x: control.leftPadding
        y: control.topPadding
        width: control.width - (control.leftPadding + control.rightPadding)
        height: control.height - (control.topPadding + control.bottomPadding)

        text: control.placeholderText
        font: control.font
        color: "#697683"
        verticalAlignment: control.verticalAlignment
        horizontalAlignment: Qt.AlignHCenter
        visible: !control.length && !control.preeditText && !control.activeFocus
        elide: Text.ElideRight
        renderType: control.renderType
    }

    ColorImage {
        x: 8
        y: (control.height - height) / 2

        width: 24
        height: 24

        source: Images.search
        color: Colors.iconTertiary
    }

    ToolButton {
        visible: control.text.length > 0

        x: control.width - width - 8
        y: (control.height - height) / 2

        width: 32
        height: 32

        icon.source: Images.chevronUp
        rotation: -90

        onClicked: {
            control.clear()
            forceActiveFocus()
        }
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 52

        color: "#99262745"

        radius: 8

        border.width: 1
        border.color: control.activeFocus ?
                          Colors.grayscale500 :
                          "transparent"

        Behavior on border.color {
            ColorAnimation {
                easing.type: Animations.easingType
                duration: Animations.mediumDuration
            }
        }
    }
}
