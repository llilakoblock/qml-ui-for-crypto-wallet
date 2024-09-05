import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

import QtQuick.Layouts

import XWallet.Style

T.PageIndicator {
    id: control

    focusPolicy: Qt.StrongFocus

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    padding: 6
    spacing: 6

    HoverHandler {
        cursorShape: Qt.PointingHandCursor
    }

    delegate: Rectangle {
        Layout.preferredWidth: 8
        Layout.preferredHeight: isCurrent ? 72 : 24

        radius: width / 2
        color: Colors.grayscale000

        opacity: isCurrent ?
                     0.8 :
                     pressed ?
                         0.6 :
                         0.2

        required property int index

        property bool isCurrent: index === control.currentIndex

        Behavior on opacity {
            OpacityAnimator {
                easing.type: Animations.easingType
                duration: Animations.mediumDuration
            }
        }

        Behavior on height {
            NumberAnimation {
                easing.type: Animations.easingType
                duration: Animations.mediumDuration
            }
        }
    }

    contentItem: ColumnLayout {
        spacing: control.spacing

        Repeater {
            model: control.count
            delegate: control.delegate
        }
    }
}
