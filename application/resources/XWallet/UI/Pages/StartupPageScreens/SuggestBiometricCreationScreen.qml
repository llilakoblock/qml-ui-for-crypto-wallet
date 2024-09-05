import QtQuick
import QtQuick.Layouts
import QtQuick.Effects

import XWallet.Data
import XWallet.Effects

import XWallet.UI.Components

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Fonts
import XWallet.Style.Images

BaseScreen {
    id: idRoot

    contentItem: RowLayout {
        uniformCellSizes: true
        spacing: Dimensions.Spacing32

        ColumnLayout {
            Layout.fillWidth: true
            Layout.maximumHeight: idRoot.availableHeight

            spacing: 0

            ColumnLayout {
                spacing: Dimensions.Spacing12

                Label {
                    Layout.fillWidth: true

                    text: qsTr("SETUP\nBIOMETRIC BACKUP")

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
                    //Layout.fillWidth: true
                    text: qsTr("\nSafeguard yourself from\nlosing your assets forever,\nuse your face-scan\nand password to recover\nyour wallet anytime.")
                    //text: qsTr("\nSafeguard yourself from\nlosing your assets forever, use your face-scan\nand password to recover your wallet anytime.")

                    font: Fonts.promo_36_wide

                    wrapMode: Text.WordWrap
                }
            }

            VerticalSpacer {
                Layout.minimumHeight: Dimensions.Spacing8
            }

            RowLayout {
                Layout.fillWidth: true

                uniformCellSizes: true
                spacing: Dimensions.Spacing32

                Button {
                    Layout.fillWidth: true

                    appearance: Button.Appearance.Primary

                    text: qsTr("Setup")

                    BaseScreen.StepErrorsToolTip {
                        visible: parent.activeFocus &&
                                 errors > 0
                    }

                    onClicked: {
                        forceActiveFocus()

                        idRoot.viewModel.finishScreen(Step.SuggestBiometricCreation, Step.BiometricCreation)
                    }
                }

                Button {
                    Layout.fillWidth: true

                    appearance: Button.Appearance.Tertiary

                    text: qsTr("Skip")

                    BaseScreen.StepErrorsToolTip {
                        visible: parent.activeFocus &&
                                 errors > 0
                    }

                    onClicked: {
                        forceActiveFocus()

                        idRoot.viewModel.finishScreen(Step.SuggestBiometricCreation, Step.SuggestInheritance)
                    }
                }
            }
        }

        Pane {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: idRoot.availableHeight

            horizontalPadding: 24
            verticalPadding: 24

            contentItem: Image {
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

                Rectangle {
                    id: idMask

                    anchors.fill: parent

                    visible: false

                    radius: 12

                    layer.enabled: true
                }
            }
        }
    }
}
