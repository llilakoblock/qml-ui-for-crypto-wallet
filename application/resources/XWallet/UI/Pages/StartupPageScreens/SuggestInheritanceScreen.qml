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
        spacing: 0

        ColumnLayout {
            Layout.fillWidth: true
            Layout.maximumWidth: Dimensions.columnsWidth(6, idRoot.width)
            Layout.maximumHeight: 650

            spacing: 0

            ColumnLayout {
                spacing: Dimensions.Spacing12

                Label {
                    Layout.fillWidth: true

                    text: qsTr("SETUP \nINHERITANCE")

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
                    //text: qsTr("Optionaly inherit your assets.")
                    text: qsTr("\nMake sure your close \nones will have access\nto your assets after\nyou stop using the\nwallet for an extended\nperiod of time.")

                    font: Fonts.promo_36_wide

                    wrapMode: Text.Wrap
                }

                Label {
                    Layout.fillWidth: true

                    //text: qsTr("Make sure your close ones will have access to your assets after you stop using the wallet for an extended period of time.")

                    //font: Fonts.h3_text
                    font: Fonts.h4_text

                    wrapMode: Text.Wrap

                }
            }

            VerticalSpacer {
                Layout.minimumHeight: Dimensions.Spacing8
            }

            RowLayout {
                Layout.fillWidth: true

                spacing: 0

                Button {
                    Layout.preferredWidth: Dimensions.columnsWidth(3, idRoot.width)

                    appearance: Button.Appearance.Primary

                    text: qsTr("Setup")

                    BaseScreen.StepErrorsToolTip {
                        visible: parent.activeFocus &&
                                 errors > 0
                    }

                    onClicked: {
                        forceActiveFocus()

                        idRoot.viewModel.finishScreen(Step.SuggestInheritance, Step.Inheritance)
                    }
                }

                HorizontalSpacer {}

                Button {
                    Layout.preferredWidth: Dimensions.columnsWidth(3, idRoot.width)

                    appearance: Button.Appearance.Tertiary

                    text: qsTr("Skip")

                    BaseScreen.StepErrorsToolTip {
                        visible: parent.activeFocus &&
                                 errors > 0
                    }

                    onClicked: {
                        forceActiveFocus()

                        idRoot.viewModel.finishScreen(Step.SuggestInheritance, Step.Unknown)
                    }
                }
            }
        }

        HorizontalSpacer {}

        Pane {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)
            Layout.maximumHeight: 650

            horizontalPadding: 24
            verticalPadding: 24

            contentItem: Image {
                source: Images.inheritanceCoverArt

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
