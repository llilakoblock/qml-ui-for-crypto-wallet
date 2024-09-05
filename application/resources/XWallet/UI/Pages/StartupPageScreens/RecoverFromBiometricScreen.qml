import QtQuick
import QtQuick.Layouts
import QtQuick.Effects

import QtWebEngine

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

    BiometricViewModel {
        id: idViewModel
    }

    contentItem: StackedView {
        id: idStackedView

        spacing: Dimensions.Spacing32

        appearance: {
            switch(idViewModel.state) {
            case BiometricViewModel.CapturingState:
                return StackedView.Appearance.Right
            case BiometricViewModel.UnknownState:
            case BiometricViewModel.ErrorState:
            case BiometricViewModel.SuccessState:
            case BiometricViewModel.NoneExistingUserState:
            case BiometricViewModel.EmptyPassword:
            default:
                return StackedView.Appearance.Both
            }
        }

        leftItem: ColumnLayout {
            spacing: 0

            ColumnLayout {
                spacing: Dimensions.Spacing12

                Label {
                    Layout.fillWidth: true

                    text: qsTr("BIOMETRIC RECOVERY")

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

                    text: qsTr("Recover your wallet using biometric data")

                    font: Fonts.promo_36_wide

                    wrapMode: Text.Wrap
                }

                Label {
                    Layout.fillWidth: true

                    text: switch(idViewModel.state) {
                        case BiometricViewModel.UnknownState:
                            return qsTr("Please enter your password and scan your face to recover your account.")
                        case BiometricViewModel.CapturingState:
                            return qsTr("Please wait while we capture your biometric data.")
                        case BiometricViewModel.ErrorState:
                            return qsTr("An error occurred while capturing your biometric data. Please try again.")
                        case BiometricViewModel.SuccessState:
                            return qsTr("Biometric data captured successfully. Continue to the next step.")
                        case BiometricViewModel.NoneExistingUserState:
                            return qsTr("User not found.")
                        case BiometricViewModel.EmptyPassword:
                            return qsTr("Please enter your password.")
                    }

                    font: Fonts.p2_regular_text

                    wrapMode: Text.Wrap
                }
            }

            VerticalSpacer {
                Layout.minimumHeight: Dimensions.Spacing8
            }

            Pane {
                Layout.maximumWidth: idStackedView.itemWidth
                Layout.minimumWidth: idStackedView.itemWidth
                Layout.minimumHeight: 200
                Layout.maximumHeight: 200

                horizontalPadding: 24
                verticalPadding: 24

                contentItem: ColumnLayout {
                    spacing: Dimensions.Spacing8

                    RowLayout {
                        Layout.fillWidth: true

                        spacing: Dimensions.Spacing12

                        Clause {
                            index: 1
                        }

                        Label {
                            Layout.fillWidth: true

                            text: qsTr("Enter your password")
                            font: Fonts.h3_comp

                            horizontalAlignment: Qt.AlignLeft
                        }
                    }

                    VerticalSpacer {}

                    TextField {
                        id: idPasswordTextField

                        Layout.fillWidth: true

                        placeholderText: qsTr("password")

                        echoMode: idRoot.isPasswordHidden ? TextInput.Password : TextInput.Normal
                        icon: TextField.Icon.Password

                        Component.onCompleted: {
                            Qt.callLater(idPasswordTextField.forceActiveFocus)
                        }

                        ToolTip {
                            visible: idViewModel.state === BiometricViewModel.EmptyPassword ||
                                     idViewModel.state === BiometricViewModel.NoneExistingUserState

                            contentItem: Label {
                                text: {
                                    switch (idViewModel.state) {
                                        case BiometricViewModel.EmptyPassword:
                                            return qsTr("Password is required")
                                        case BiometricViewModel.NoneExistingUserState:
                                            return qsTr("Password not found")
                                        default:
                                            return ""
                                    }
                                }

                                font: Fonts.p1_regular_text
                            }
                        }

                        onIconClicked: {
                            idRoot.isPasswordHidden = !idRoot.isPasswordHidden
                        }
                    }
                }
            }

            VerticalSpacer {
                Layout.minimumHeight: Dimensions.Spacing8
            }

            Button {
                id: idFinishButton

                Layout.fillWidth: true
                Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)

                size: Button.Size.Large
                appearance: Button.Appearance.Primary

                text: {
                    switch(idViewModel.state) {
                    case BiometricViewModel.UnknownState:
                    case BiometricViewModel.NoneExistingUserState:
                    case BiometricViewModel.EmptyPassword:
                        return qsTr("Start scan")
                    case BiometricViewModel.CapturingState:
                        return qsTr("Capturing")
                    case BiometricViewModel.ErrorState:
                        return qsTr("Retry")
                    case BiometricViewModel.SuccessState:
                        return qsTr("Next")
                    }
                }

                BaseScreen.StepErrorsToolTip {
                    visible: parent.activeFocus &&
                             errors > 0
                }

                onClicked: {
                    forceActiveFocus()

                    switch(idViewModel.state) {
                    case BiometricViewModel.UnknownState:
                    case BiometricViewModel.NoneExistingUserState:
                    case BiometricViewModel.EmptyPassword:
                    case BiometricViewModel.ErrorState:
                        idViewModel.showAuthenticateView(idPasswordTextField.text)
                        break
                    case BiometricViewModel.CapturingState:
                        break
                    case BiometricViewModel.SuccessState:
                        idRoot.viewModel.finishScreen(Step.RecoverFromBiometric, Step.ShowMnemonicPassphrase, idPasswordTextField.text)
                    }
                }
            }
        }

        rightItem: Pane {
            id: idWebViewPane

            horizontalPadding: 24
            verticalPadding: 24

            Rectangle {
                id: idMask

                width: idWebViewPane.availableWidth
                height: idWebViewPane.availableHeight

                visible: false

                layer.enabled: true

                radius: 12
            }

            contentItem: ColumnLayout {
                spacing: Dimensions.Spacing20

                RowLayout {
                    Layout.fillWidth: true

                    visible: idViewModel.state !== BiometricViewModel.CapturingState

                    spacing: Dimensions.Spacing16

                    Clause {
                        index: 2
                    }

                    Label {
                        Layout.fillWidth: true

                        text: qsTr("Scan your face")
                        font: Fonts.h3_comp

                        horizontalAlignment: Qt.AlignLeft
                    }
                }

                WebEngineView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    visible: idViewModel.state === BiometricViewModel.CapturingState &&
                             !loading

                    url: idViewModel.serverUrl

                    backgroundColor: Colors.transparent

                    settings.pluginsEnabled: true
                    settings.localContentCanAccessRemoteUrls: true

                    onFeaturePermissionRequested: (securityOrigin, feature) => {
                        grantFeaturePermission(securityOrigin, feature, true);
                    }

                    layer.enabled: true
                    layer.effect: MultiEffect {
                        autoPaddingEnabled: false

                        maskEnabled: true
                        maskThresholdMin: 0.5
                        maskSpreadAtMin: 1.0

                        maskSource: idMask
                    }
                }

                Image {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    visible: idViewModel.state !== BiometricViewModel.CapturingState

                    source: Images.biometricCoverArt

                    fillMode: Image.PreserveAspectCrop

                    layer.enabled: true
                    layer.effect: MultiEffect {
                        autoPaddingEnabled: false

                        maskEnabled: true
                        maskThresholdMin: 0.5
                        maskSpreadAtMin: 1.0

                        maskSource: idMask
                    }
                }
            }
        }
    }

    component Clause : Control {
        id: idClauseControl

        required property int index

        contentItem: Label {
            text: idClauseControl.index
            font: Fonts.h2_wide

            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
        }

        background: Rectangle {
            implicitWidth: 56
            implicitHeight: 56

            color: Color.transparent(Colors.grayscale000, 0.2)
            radius: Math.min(width, height) / 2
        }
    }
}
