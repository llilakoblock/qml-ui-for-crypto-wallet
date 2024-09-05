import QtQuick
import QtQuick.Layouts

import XWallet.Data
import XWallet.Effects
import XWallet.Natives

import XWallet.UI.Components

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Fonts

BaseScreen {
    id: idRoot

    property bool isPasswordHidden: true

    contentItem: ColumnLayout {
        spacing: 0

        VerticalSpacer {}

        ColumnLayout {
            spacing: Dimensions.Spacing12

            Label {
                Layout.fillWidth: true

                text: qsTr("WELCOME BACK")

                font: Fonts.promo1_wide
                lineHeight: 0.8

                wrapMode: Text.Wrap

                layer.enabled: true
                layer.effect: LinearGradientEffect {
                    gradient: Colors.promoGradient
                    angle: 90
                }
            }

            Label {
                Layout.fillWidth: true

                text: qsTr("Enter your password to unlock")
                /*text: qsTr("Enter your password to unlock your wallet" +
                           "\n" +
                           "and access your funds")*/

                font: Fonts.promo_36_wide

                wrapMode: Text.Wrap
            }
        }

        Item {
            Layout.preferredHeight: Dimensions.Spacing32
        }

        Pane {
            Layout.maximumHeight: 300
            Layout.fillWidth: true
            Layout.maximumWidth: Dimensions.columnsWidth(6, idRoot.width)

            contentItem: ColumnLayout {
                spacing: Dimensions.Spacing8

                Label {
                    Layout.fillWidth: true

                    text: qsTr("Enter your password")
                    font: Fonts.h2_comp

                    horizontalAlignment: Qt.AlignLeft
                    wrapMode: Label.WrapAtWordBoundaryOrAnywhere

                }

                Label {
                    Layout.fillWidth: true

                    text: qsTr("Your password must be at least 8 characters long and contain a mix of letters, numbers and symbols")
                    font: Fonts.p3_medium_comp
                    opacity: 0.7

                    horizontalAlignment: Qt.AlignLeft
                    wrapMode: Label.Wrap
                }

                VerticalSpacer {}

                TextField {
                    id: idPasswordTextField

                    Layout.fillWidth: true

                    // text: "123"

                    placeholderText: qsTr("password")

                    echoMode: idRoot.isPasswordHidden ? TextInput.Password : TextInput.Normal
                    icon: TextField.Icon.Password

                    onIconClicked: {
                        idRoot.isPasswordHidden = !idRoot.isPasswordHidden
                    }

                    Component.onCompleted: {
                        Qt.callLater(idPasswordTextField.forceActiveFocus)
                    }

                    ToolTip {
                        visible: idPasswordTextField.text.length === 0 &&
                                 idFinishButton.activeFocus

                        contentItem: Label {
                            text: qsTr("Password is required")

                            font: Fonts.p1_regular_text
                        }
                    }
                }
            }
        }

        Item {
            Layout.preferredHeight: Dimensions.Spacing32
        }

        Button {
            id: idFinishButton

            Layout.fillWidth: true
            Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)

            size: Button.Size.Large
            appearance: Button.Appearance.Primary

            text: qsTr("Unlock")

            BaseScreen.StepErrorsToolTip {
                visible: parent.activeFocus &&
                         errors > 0
            }

            onClicked: {
                forceActiveFocus()

                if(idPasswordTextField.text.length === 0) {
                    return
                }

                idRoot.viewModel.finishScreen(Step.Unlock, Step.Unknown, idPasswordTextField.text)
            }
        }

        VerticalSpacer {}
    }
}
