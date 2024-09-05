import QtQuick
import QtQuick.Layouts

import XWallet.Data

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Fonts

RowLayout {
    id: idRoot

    spacing: Dimensions.Spacing8

    required property string title

    required property int state
    required property int prevState
    required property int nextState

    required property bool blink

    enum State {
        Unknown,
        NotStarted,
        InProgress,
        Done,
        Failed
    }

    function colorForState(state : int) : color {
        switch (state) {
            case SwapStepDescriber.State.Unknown:
                return Colors.transparent
            case SwapStepDescriber.State.NotStarted:
            case SwapStepDescriber.State.InProgress:
                return Colors.textTertiary
            case SwapStepDescriber.State.Done:
                return Colors.textPositive
            case SwapStepDescriber.State.Failed:
                return Colors.textNegative
        }
    }

    ColumnLayout {
        Layout.fillHeight: true
        Layout.alignment: Qt.AlignVCenter

        spacing: 0

        Rectangle {
            Layout.preferredWidth: 1
            Layout.preferredHeight: 10

            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: idRoot.colorForState(idRoot.prevState)
                }

                GradientStop {
                    position: 1.0
                    color: {
                        switch(idRoot.prevState) {
                            case SwapStepDescriber.State.Unknown:
                                return Colors.transparent
                            default:
                                return idRoot.colorForState(idRoot.state)
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.preferredWidth: 10
            Layout.preferredHeight: 10
            Layout.leftMargin: -4.5

            color: idRoot.colorForState(idRoot.state)
            radius: 5
        }

        Rectangle {
            Layout.preferredWidth: 1
            Layout.preferredHeight: 10

            color: {
                switch(idRoot.nextState) {
                    case SwapStepDescriber.State.Unknown:
                        return Colors.transparent
                    default:
                        return idRoot.colorForState(idRoot.state)
                }
            }
        }
    }

    Label {
        text: idRoot.title

        font: Fonts.p4_medium_comp

        color: Colors.textSecondary

        opacity: idRoot.blink &&
                 idRoot.state === SwapStepDescriber.State.InProgress ?
                     0.2 :
                     1.0

        Behavior on opacity {
            NumberAnimation { duration: 750 }
        }
    }

    Label {
        text: {
            switch (idRoot.state) {
                case SwapStepDescriber.State.NotStarted:
                    return ""
                case SwapStepDescriber.State.InProgress:
                    return qsTr("in progress")
                case SwapStepDescriber.State.Done:
                    return qsTr("done")
                case SwapStepDescriber.State.Failed:
                    return qsTr("failed")
            }
        }

        color: idRoot.colorForState(idRoot.state)

        font: Fonts.p4_regular_comp
    }
}
