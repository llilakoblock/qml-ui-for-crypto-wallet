import QtQuick
import QtQuick.Controls.Basic.impl
import QtQuick.Templates as T

import XWallet.Style

T.BusyIndicator {
    id: control

    focusPolicy: Qt.StrongFocus

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    padding: 0
    spacing: 10

    property real duration: 2500

    property real size: 30
    readonly property real elementSize: Math.SQRT2 * size

    readonly property point topPoint: Qt.point(control.availableWidth / 2, control.elementSize / 2)
    readonly property point rightPoint: Qt.point(control.availableWidth - control.elementSize / 2, control.availableHeight / 2)
    readonly property point bottomPoint: Qt.point(control.availableWidth / 2, control.availableHeight - control.elementSize / 2)
    readonly property point leftPoint: Qt.point(control.elementSize / 2, control.availableHeight / 2)

    PathAnimation {
        running: true
        loops: Animation.Infinite
        duration: control.duration

        anchorPoint: Qt.point(control.size / 2, control.size / 2)

        path: Path {
            startX: control.topPoint.x
            startY: control.topPoint.y

            PathLine {
                x: control.rightPoint.x
                y: control.rightPoint.y
            }

            PathLine {
                x: control.bottomPoint.x
                y: control.bottomPoint.y
            }

            PathLine {
                x: control.leftPoint.x
                y: control.leftPoint.y
            }

            PathLine {
                x: control.topPoint.x
                y: control.topPoint.y
            }
        }

        target: idFirstRect
    }

    PathAnimation {
        running: true
        loops: Animation.Infinite
        duration: control.duration

        anchorPoint: Qt.point(control.size / 2, control.size / 2)

        path: Path {
            startX: control.rightPoint.x
            startY: control.rightPoint.y

            PathLine {
                x: control.bottomPoint.x
                y: control.bottomPoint.y
            }

            PathLine {
                x: control.leftPoint.x
                y: control.leftPoint.y
            }

            PathLine {
                x: control.topPoint.x
                y: control.topPoint.y
            }

            PathLine {
                x: control.rightPoint.x
                y: control.rightPoint.y
            }
        }

        target: idSecondRect
    }

    PathAnimation {
        running: true
        loops: Animation.Infinite
        duration: control.duration

        anchorPoint: Qt.point(control.size / 2, control.size / 2)

        path: Path {
            startX: control.bottomPoint.x
            startY: control.bottomPoint.y

            PathLine {
                x: control.leftPoint.x
                y: control.leftPoint.y
            }

            PathLine {
                x: control.topPoint.x
                y: control.topPoint.y
            }

            PathLine {
                x: control.rightPoint.x
                y: control.rightPoint.y
            }

            PathLine {
                x: control.bottomPoint.x
                y: control.bottomPoint.y
            }
        }

        target: idThirdRect
    }

    PathAnimation {
        running: true
        loops: Animation.Infinite
        duration: control.duration

        anchorPoint: Qt.point(control.size / 2, control.size / 2)

        path: Path {
            startX: control.leftPoint.x
            startY: control.leftPoint.y

            PathLine {
                x: control.topPoint.x
                y: control.topPoint.y
            }

            PathLine {
                x: control.rightPoint.x
                y: control.rightPoint.y
            }

            PathLine {
                x: control.bottomPoint.x
                y: control.bottomPoint.y
            }

            PathLine {
                x: control.leftPoint.x
                y: control.leftPoint.y
            }
        }

        target: idFourthRect
    }

    contentItem: Item {
        implicitHeight: control.elementSize * 2 + control.spacing
        implicitWidth: control.elementSize * 2 + control.spacing

        Rectangle {
            id: idFirstRect

            width: control.size
            height: control.size

            rotation: 45

            color: "#6654F6"
        }

        Rectangle {
            id: idSecondRect

            width: control.size
            height: control.size

            rotation: 45

            color: "#9BA5FF"
        }

        Rectangle {
            id: idThirdRect

            width: control.size
            height: control.size

            rotation: 45

            color: "#6654F6"
        }

        Rectangle {
            id: idFourthRect

            width: control.size
            height: control.size

            rotation: 45

            color: "#9BA5FF"
        }
    }
}
