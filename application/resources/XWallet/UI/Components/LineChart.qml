import QtQuick
import QtQuick.Controls.impl
import QtQuick.Layouts
import QtQuick.Shapes
import QtQml.Models

import XWallet.Natives
import XWallet.Data

import XWallet.Style
import XWallet.Style.Fonts
import XWallet.Style.Controls

Control {
    id: control

    required property int assetType
    required property int period

    // topPadding: idPointerDescription.implicitHeight + 4
    // topInset: topPadding
    rightPadding: 70
    bottomPadding: 20

    readonly property alias state: idViewModel.state

    LineChartViewModel {
        id: idViewModel

        period: control.period

        chartSize: Qt.size(idChart.width, idChart.height)
        hoverPoint: idHoverHandler.point.position
        assetType: control.assetType
    }

    Control {
        id: idPointerDescription

        visible: idHoverHandler.hovered

        x: idViewModel.linePointer.x + Dimensions.Spacing12
        y: idViewModel.linePointer.y - height / 2

        horizontalPadding: 8
        verticalPadding: 8

        Behavior on x { NumberAnimation { duration: 32 } }
        Behavior on y { NumberAnimation { duration: 32 } }

        contentItem: ColumnLayout {
            spacing: 4

            Label {
                Layout.fillWidth: true

                text: idViewModel.quote.timestamp.toLocaleString(Locale.ShortFormat)

                font: Fonts.c1_medium_comp
                color: Colors.textInverted

                horizontalAlignment: Qt.AlignHCenter
            }

            Label {
                Layout.fillWidth: true

                text: qsTr("$%1").arg(idViewModel.quote.price.toLocaleString(Qt.locale(), "f", 2))

                font: Fonts.p3_regular_comp
                color: Colors.textInverted

                horizontalAlignment: Qt.AlignHCenter
            }
        }

        background: Rectangle {
            color: Colors.backgroundPrimary

            radius: 8
        }
    }

    contentItem: Shape {
        id: idChart

        Layout.fillWidth: true
        Layout.fillHeight: true

        asynchronous: true

        layer.enabled: true
        layer.samples: 4

        ShapePath {
            strokeWidth: 1
            strokeColor: Colors.grayscale000

            fillGradient: LinearGradient {
                y1: 0
                y2: control.height

                GradientStop { position: 0; color: Color.transparent(Colors.grayscale200, 0.2) }
                GradientStop { position: 1; color: Colors.transparent }
            }

            startX: 0
            startY: 0

            PathSvg {
                path: idViewModel.linePath
            }
        }

        ShapePath {
            fillColor: idHoverHandler.hovered ? Colors.grayscale000 : Colors.transparent

            strokeWidth: 2
            strokeColor: idHoverHandler.hovered ? Colors.grayscale700 : Colors.transparent

            capStyle: ShapePath.FlatCap

            PathAngleArc {
                centerX: idViewModel.linePointer.x
                centerY: idViewModel.linePointer.y
                radiusX: 6
                radiusY: 6
                startAngle: 0
                sweepAngle: 360

                Behavior on centerX { NumberAnimation { duration: 32 } }
                Behavior on centerY { NumberAnimation { duration: 32 } }
            }
        }

        HoverHandler {
            id: idHoverHandler

            cursorShape: Qt.PointingHandCursor
        }
    }

    background: Item {
        Repeater {
            id: idHorizontalRepeater

            model: idViewModel.horizontalLabelsModel

            delegate: Item {
                id: idHorizontalDelegate

                x: idHorizontalDelegate.label.point.x

                required property int index
                required property label label

                Rectangle {
                    width: 1
                    height: control.background.height - control.bottomPadding

                    color: Colors.backgroundStroke
                }

                Label {
                    x: -width / 2
                    y: control.background.height - height

                    text: idHorizontalDelegate.label.text

                    font: Fonts.c1_regular_comp
                }
            }
        }

        Repeater {
            id: idVerticalRepeater

            model: idViewModel.verticalLabelsModel

            delegate: Item {
                id: idVerticalDelegate

                y: label.point.y

                required property int index
                required property label label

                Rectangle {
                    width: control.background.width - control.rightPadding
                    height: 1

                    color: Colors.backgroundStroke
                }

                Label {
                    x: control.background.width - width
                    y: -height / 2

                    text: idVerticalDelegate.label.text

                    font: Fonts.c1_regular_comp
                }
            }
        }
    }

    component AxisPath : ShapePath {
        id: idAxisPath
        required property int index

        property real axis: idViewModel.axis.length > index ? idViewModel.axis[index].y : -100
        property real label: idViewModel.axis.length > index ? idViewModel.axis[index].x : 0

        strokeWidth: 1
        strokeColor: axis > 0 ? "#AA666666" : "transparent"

        startX: control.leftPadding
        startY: axis

        PathLine {
            x: control.width
            y: idAxisPath.axis
        }

        PathText {
            x: 0
            y: idAxisPath.axis - font.pixelSize / 2

            text: qsTr("$%1").arg(idAxisPath.label.toLocaleString(Qt.locale("US"), "f", 2))

            font.family: control.font.family
            font.pixelSize: 12
            font.weight: Font.Thin
        }
    }
}
