import QtQuick

ShaderEffect {
    id: idShader

    mesh: GridMesh {
        resolution: Qt.size(width / 10, height / 10)
    }

    property size resolution: Qt.size(width, height)

    property real deltaTime: 4e-5
    property real time: 0.0
    property real counter: 0.0

    property color color1: "#2E1BA9"
    property color color2: "#4879B5"
    property color color3: "#0E0F2D"
    property color color4: "#14245B"
    property color color5: "#556CD5"

    property real angle: 0.0
    property real amp: 360.0
    property real seed: Math.random() * 10.0
    property real freqX: 14e-5
    property real freqY: 37e-5
    property real noiseSpeed: 3e-6

    Behavior on color1 { ColorAnimation { duration: 500 } }
    Behavior on color2 { ColorAnimation { duration: 500 } }
    Behavior on color3 { ColorAnimation { duration: 500 } }
    Behavior on color4 { ColorAnimation { duration: 500 } }
    Behavior on color5 { ColorAnimation { duration: 500 } }

    onCounterChanged: time += deltaTime

    NumberAnimation {
        target: idShader
        property: "counter"
        duration: 1000
        from: 0.0
        to: 1.0
        loops: Animation.Infinite
        running: true
    }

    vertexShader: "qrc:/shaders/liquid_gradient.vert.qsb"
    fragmentShader: "qrc:/shaders/liquid_gradient.frag.qsb"
}
