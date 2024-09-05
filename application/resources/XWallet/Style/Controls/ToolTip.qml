import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

import XWallet.Style
import XWallet.Style.Images

T.ToolTip {
    id: control

    enum Position { Top, Right }

    property int position: ToolTip.Position.Top

    x: parent ? parent.width + 8 + Dimensions.Spacing4 : 0
    y: parent ? (parent.height - implicitHeight) / 2 : 0

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    margins: 8

    horizontalPadding: 12
    verticalPadding: 10

    closePolicy: T.Popup.NoAutoClose

    opacity: visible ? 1 : 0

    Behavior on opacity {
        NumberAnimation {
            easing.type: Animations.easingType
            duration: Animations.mediumDuration
        }
    }

    contentItem: Text {
        text: control.text
        font: control.font
        wrapMode: Text.Wrap
        color: control.palette.toolTipText
    }

    background: Rectangle {
        implicitWidth: 60
        implicitHeight: 20

        color: Color.transparent(Colors.grayscale800, 0.9)
        radius: 8

        ColorImage {
            x: -width
            y: (parent.height - height) / 2

            width: 8
            height: 20

            source: Images.tooltipArrow
            color: Color.transparent(Colors.grayscale800, 0.9)

            fillMode: Image.PreserveAspectFit
        }
    }
}
