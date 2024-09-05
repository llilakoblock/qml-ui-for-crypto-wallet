import QtQuick
import QtQuick.Layouts

import XWallet.Data
import XWallet.Natives
import XWallet.Effects

import XWallet.UI.Components

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Fonts
import XWallet.Style.Images

BaseScreen {
    id: idRoot

    property bool isPasswordHidden: true

    contentItem: ColumnLayout {
        spacing: 0

        VerticalSpacer {}

        ColumnLayout {
            spacing: Dimensions.Spacing12

            Label {
                text: qsTr("CREATE PASSWORD")

                font: Fonts.promo1_wide
                lineHeight: 0.8

                wrapMode: Text.WordWrap

                layer.enabled: true
                layer.effect: LinearGradientEffect {
                    gradient: Colors.promoGradient
                    angle: 90
                }
            }

            Label {
                text: qsTr("Create a password to secure your wallet.")

                font: Fonts.promo_36_wide

                wrapMode: Text.WordWrap
            }
        }

        Item {
            Layout.preferredHeight: Dimensions.Spacing32
        }

        Pane {
            Layout.maximumHeight: 350
            Layout.fillHeight: true

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

                    text: qsTr("\nYour password must be at least 8 characters long and contain a mix of letters, numbers and symbols.")
                    font: Fonts.p3_medium_comp
                    opacity: 0.7

                    horizontalAlignment: Qt.AlignLeft
                    wrapMode: Label.Wrap
                }

                VerticalSpacer {}

                TextField {
                    id: idPasswordTextField

                    Layout.fillWidth: true

                    text: idRoot.viewModel.step.password

                    placeholderText: qsTr("password")

                    echoMode: idRoot.isPasswordHidden ? TextInput.Password : TextInput.Normal
                    icon: TextField.Icon.Password

                    onTextEdited: {
                        idPasswordToolTip.errors = idRoot.viewModel.validatePassword(text)
                    }

                    onActiveFocusChanged: {
                        idPasswordToolTip.errors = idRoot.viewModel.validatePassword(text)
                    }

                    onIconClicked: {
                        idRoot.isPasswordHidden = !idRoot.isPasswordHidden
                    }

                    Component.onCompleted: {
                        Qt.callLater(idPasswordTextField.forceActiveFocus)
                    }

                    ErrorToolTip {
                        id: idPasswordToolTip

                        visible: parent.activeFocus || errors > 0

                        property int errors: 0

                        model: ObjectModel {
                            ErrorToolTip.CheckRow {
                                trigger: idPasswordToolTip.errors & StartupViewModel.MinimumLengthError
                                text: qsTr("Minimum 8 characters")
                            }

                            ErrorToolTip.CheckRow {
                                trigger: idPasswordToolTip.errors & StartupViewModel.WithoutCapitalLetterError
                                text: qsTr("Capital letters")
                            }

                            ErrorToolTip.CheckRow {
                                trigger: idPasswordToolTip.errors & StartupViewModel.WithoutLowercaseLetterError
                                text: qsTr("Lowercase letters")
                            }

                            ErrorToolTip.CheckRow {
                                trigger: idPasswordToolTip.errors & StartupViewModel.WithoutNumbersError
                                text: qsTr("Numbers")
                            }
                        }
                    }
                }

                TextField {
                    id: idPasswordRepeatTextField

                    Layout.fillWidth: true

                    text: idRoot.viewModel.step.password

                    placeholderText: qsTr("repeat password")

                    echoMode: idRoot.isPasswordHidden ? TextInput.Password : TextInput.Normal
                    icon: TextField.Icon.Password

                    onIconClicked: {
                        idRoot.isPasswordHidden = !idRoot.isPasswordHidden
                    }

                    ErrorToolTip {
                        id: idPasswordRepeatToolTip

                        visible: !parent.activeFocus &&
                                 !idPasswordToolTip.visible &&
                                 idPasswordTextField.text !== idPasswordRepeatTextField.text

                        model: ObjectModel {
                            ErrorToolTip.ErrorRow {
                                trigger: idPasswordTextField.text !== idPasswordRepeatTextField.text
                                text: qsTr("Passwords do not match")
                            }
                        }
                    }
                }
            }
        }

        Item {
            Layout.preferredHeight: Dimensions.Spacing32
        }

        Button {
            Layout.fillWidth: true
            Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)

            size: Button.Size.Large
            appearance: Button.Appearance.Primary

            text: qsTr("Create new wallet")

            BaseScreen.StepErrorsToolTip {
                visible: !idPasswordToolTip.visible &&
                         !idPasswordRepeatToolTip.visible &&
                         parent.activeFocus &&
                         errors > 0
            }

            onClicked: {
                forceActiveFocus()

                if(idPasswordToolTip.errors === 0 && idPasswordTextField.text === idPasswordRepeatTextField.text) {

                    switch(idRoot.viewModel.step.restorationMethod) {
                    case Step.FromMnemonicPassphrase:
                        idRoot.viewModel.finishScreen(Step.CreatePassword, Step.SuggestBiometricCreation, idPasswordTextField.text)
                        break
                    default:
                        idRoot.viewModel.finishScreen(Step.CreatePassword, Step.ShowMnemonicPassphrase, idPasswordTextField.text)
                        break
                    }
                }
            }
        }

        VerticalSpacer {}
    }
}
