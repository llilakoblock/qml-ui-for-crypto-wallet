import QtQuick
import QtQuick.Effects
import QtQuick.Templates as T

T.SwipeView {
    id: control

    focusPolicy: Qt.StrongFocus

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    verticalPadding: 32

    readonly property real contentY: (contentItem as ListView).contentY

    property bool smoothCutting: true

    signal contentPositionChanged()

    contentItem: ListView {
        model: control.contentModel
        interactive: control.interactive
        currentIndex: control.currentIndex
        focus: control.focus

        spacing: control.spacing
        orientation: control.orientation
        snapMode: ListView.SnapOneItem
        boundsBehavior: Flickable.StopAtBounds

        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0
        highlightMoveDuration: 250
        maximumFlickVelocity: 4 * (control.orientation === Qt.Horizontal ? width : height)

        onContentYChanged: control.contentPositionChanged()
    }

    layer.enabled: smoothCutting
    layer.effect: MultiEffect {
        maskEnabled: true
        maskSpreadAtMin: 0.2
        maskThresholdMin: 0.5

        maskSource: ShaderEffectSource {
            sourceItem: Rectangle {
                width: control.availableWidth
                height: control.height

                gradient: Gradient {
                    orientation: Gradient.Vertical

                    GradientStop {
                        color: "transparent"
                        position: 0.0
                    }

                    GradientStop {
                        color: "white"
                        position: control.verticalPadding / control.height
                    }

                    GradientStop {
                        color: "white"
                        position: 1.0 - control.verticalPadding / control.height
                    }

                    GradientStop {
                        color: "transparent"
                        position: 1.0
                    }
                }
            }
        }
    }
}
