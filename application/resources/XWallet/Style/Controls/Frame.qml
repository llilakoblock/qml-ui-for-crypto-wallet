import QtQuick
import QtQuick.Effects
import QtQuick.Controls.impl
import QtQuick.Templates as T

import XWallet.Effects

import XWallet.Style

T.Frame {
    id: control

    focusPolicy: Qt.StrongFocus

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    padding: 32

    background: Item {
        Rectangle {
            id: idSource

            visible: false

            width: control.background.width
            height: control.background.height

            radius: Dimensions.Spacing16

            layer.enabled: true
            layer.effect: LinearGradientEffect {
                angle: 90
                gradient: Colors.cardBorderGradient
            }
        }

        Rectangle {
            id: idMask

            visible: false

            width: control.background.width
            height: control.background.height

            color: Colors.grayscale000

            radius: Dimensions.Spacing16

            border.width: 1
            border.color: Colors.grayscale000

            layer.enabled: true
        }

        MultiEffect {
            width: control.background.width
            height: control.background.height

            maskEnabled: true
            maskThresholdMin: 0.5
            maskSpreadAtMin: 1.0

            maskSource: idMask

            source: idSource
        }
    }
}
