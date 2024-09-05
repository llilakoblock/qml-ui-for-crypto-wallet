import QtQuick
import QtQuick.Effects
import QtQuick.Controls.impl
import QtQuick.Templates as T

import XWallet.Effects
import XWallet.Style

T.Pane {
    id: control

    focusPolicy: Qt.StrongFocus

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    horizontalPadding: Dimensions.Spacing56
    verticalPadding: Dimensions.Spacing40

    property Item backgroundSource: ApplicationWindow.window?.background ?? control
    property rect backgroundSourceRect: background.mapToItem(backgroundSource, Qt.rect(1, 1, background.width - 1, background.height - 1))

    Behavior on width {
        NumberAnimation { easing.type: Animations.easingType; duration: Animations.mediumDuration }
    }

    function updateBackground() {
        backgroundSourceRect = background.mapToItem(backgroundSource, Qt.rect(1, 1, background.width - 1, background.height - 1))
    }

    background: Item {
        Rectangle {
            width: control.background.width
            height: control.background.height

            radius: Dimensions.Spacing16

            layer.enabled: true
            layer.effect: LinearGradientEffect {
                angle: 90
                gradient: Colors.cardBorderGradient
            }
        }

        GlassBlurEffect {
            x: 1
            y: 1
            width: control.background.width - 2
            height: control.background.height - 2

            backgroundSource: control.backgroundSource
            backgroundSourceRect: control.backgroundSourceRect

            hovered: control.hovered
            radius: Dimensions.Spacing16 - 0.5
        }
    }
}
