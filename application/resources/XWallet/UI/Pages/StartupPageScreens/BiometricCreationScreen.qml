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

                    text: qsTr("BIOMETRIC BACKUP")

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
                    Layout.fillWidth: true

                    text: qsTr("\nA scan of your face will be taken and used later to recover your assets, even if your mnemonic, wallet, or computer are lost.")

                    font: Fonts.promo_36_wide

                    wrapMode: Text.Wrap
                }

                Label {
                    Layout.fillWidth: true

                    text: {
                        switch(idViewModel.state) {
                            case BiometricViewModel.UnknownState:
                                //return qsTr("\nPress Start scan to begin and follow the onscreen instructions.")
                                return qsTr("")
                            case BiometricViewModel.CapturingState:
                                return qsTr("\nPlease wait while we capture your biometric data.")
                            case BiometricViewModel.ErrorState:
                                return qsTr("\nAn error occurred while capturing your biometric data. Please try again.")
                            case BiometricViewModel.SuccessState:
                                return qsTr("\nBiometric data captured successfully. Continue to the next step.")
                        }
                    }

                    font: Fonts.p2_regular_text

                    wrapMode: Text.Wrap
                }
            }

            VerticalSpacer {
                Layout.minimumHeight: Dimensions.Spacing8
            }

            Button {
                Layout.fillWidth: true
                Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)

                enabled: idViewModel.state !== BiometricViewModel.CapturingState

                size: Button.Size.Large
                appearance: Button.Appearance.Primary

                text: {
                    switch(idViewModel.state) {
                    case BiometricViewModel.UnknownState:
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

                    switch(idViewModel.state) {
                    case BiometricViewModel.UnknownState:
                    case BiometricViewModel.ErrorState:
                        idViewModel.showEnrollView()
                        break
                    case BiometricViewModel.CapturingState:
                        break
                    case BiometricViewModel.SuccessState:
                        switch(idRoot.viewModel.step.restorationMethod) {
                        case Step.FromInheritance:
                            idRoot.viewModel.finishScreen(Step.BiometricCreation, Step.Unknown)
                            break
                        default:
                            idRoot.viewModel.finishScreen(Step.BiometricCreation, Step.SuggestInheritance)
                            break
                        }
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
}
