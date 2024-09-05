import QtQuick

import XWallet.Style
import XWallet.Style.Controls

Control {
    id: control

    enum Appearance {
        Both,
        Left,
        Right
    }

    required property Component leftItem
    required property Component rightItem

    property int appearance: StackedView.Appearance.Both

    readonly property real itemWidth: (availableWidth - spacing) / 2

    contentItem: Item {
        ItemLoader {
            x: control.appearance === StackedView.Appearance.Right ?
                   -control.itemWidth - control.spacing :
                   0
            y: 0

            width: control.appearance === StackedView.Appearance.Left ? control.availableWidth : control.itemWidth
            height: control.availableHeight

            opacity: (control.appearance === StackedView.Appearance.Both || control.appearance === StackedView.Appearance.Left) ? 1.0 : 0.0

            sourceComponent: control.leftItem
        }

        ItemLoader {
            x: control.appearance === StackedView.Appearance.Right ?
                   0 :
                   control.itemWidth + control.spacing
            y: 0

            width: control.appearance === StackedView.Appearance.Right ? control.availableWidth : control.itemWidth
            height: control.availableHeight

            opacity: (control.appearance === StackedView.Appearance.Both || control.appearance === StackedView.Appearance.Right) ? 1.0 : 0.0

            sourceComponent: control.rightItem
        }
    }

    component ItemLoader : Loader {
        Behavior on opacity {
            OpacityAnimator {
                duration: Animations.longDuration
                easing.type: Animations.easingType
            }
        }

        Behavior on x {
            NumberAnimation {
                duration: Animations.longDuration
                easing.type: Animations.easingType
            }
        }

        Behavior on width {
            NumberAnimation {
                duration: Animations.longDuration
                easing.type: Animations.easingType
            }
        }
    }
}
