import QtQuick

import XWallet.Style

ShaderEffect {
    id: shader

    property real angle: 0.0
    property size sourceSize: Qt.size(width, height)

    property Gradient gradient: Colors.cardBorderGradient

    property var gradientSource: gradientRect;

    Rectangle {
        id: gradientRect;

        visible: false;

        width: 255
        height: 1
        gradient: shader.gradient

        layer.enabled: true;
        layer.smooth: true
    }

    fragmentShader: "qrc:/shaders/linear_gradient.frag.qsb"
}
