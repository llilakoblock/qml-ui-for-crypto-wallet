import QtQuick
import QtQuick.Effects

import XWallet.Style

MultiEffect {
    id: effect

    required property Item backgroundSource
    required property rect backgroundSourceRect

    required property bool hovered
    required property real radius

    autoPaddingEnabled: false
    blurEnabled: true
    blurMax: 80
    blur: 1.0
    saturation: -0.1
    brightness: effect.hovered ? 0.15 : 0.1

    maskEnabled: true
    maskThresholdMin: 0.5
    maskSpreadAtMin: 1.0

    maskSource: ShaderEffectSource {
        sourceItem: Rectangle {
            width: effect.width
            height: effect.height

            radius: effect.radius
        }
    }

    source: ShaderEffectSource {
        sourceItem: effect.backgroundSource
        sourceRect: effect.backgroundSourceRect
    }

    Behavior on brightness {
        NumberAnimation { easing.type: Animations.easingType; duration: Animations.mediumDuration }
    }
}
