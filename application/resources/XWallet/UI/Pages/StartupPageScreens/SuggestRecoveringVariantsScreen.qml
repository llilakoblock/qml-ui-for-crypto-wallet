import QtQuick
import QtQuick.Layouts

import XWallet.Data
import XWallet.Effects

import XWallet.UI.Components

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Fonts

BaseScreen {
    id: idRoot

    contentItem: ColumnLayout {
        spacing: 0

        VerticalSpacer {}

        ColumnLayout {
            spacing: Dimensions.Spacing12

            Label {
                text: qsTr("WALLET RESTORE")

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
                text: qsTr("Let's restore your wallet.")

                font: Fonts.promo2_wide

                wrapMode: Text.WordWrap
            }
        }

        Item {
            Layout.preferredHeight: Dimensions.Spacing32
        }

        RowLayout {
            Layout.maximumHeight: 350
            spacing: Dimensions.Spacing32

            Pane {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)

                contentItem: ColumnLayout {
                    spacing: Dimensions.Spacing8

                    Label {
                        Layout.fillWidth: true

                        text: qsTr("Mnemonic")
                        font: Fonts.h2_comp

                        //horizontalAlignment: Qt.AlignHCenter
                        horizontalAlignment: Qt.AlignLeft
                        wrapMode: Label.WrapAtWordBoundaryOrAnywhere

                        maximumLineCount: 2
                        elide: Label.ElideRight
                    }

                    Label {
                        Layout.fillWidth: true

                        text: qsTr("\nUse your 12 word secret mnemonic phrase.")
                        font: Fonts.p3_medium_comp
                        opacity: 0.7

                        //horizontalAlignment: Qt.AlignHCenter
                        horizontalAlignment: Qt.AlignLeft
                        wrapMode: Label.WrapAtWordBoundaryOrAnywhere

                        //maximumLineCount: 2
                        elide: Label.ElideRight
                    }

                    VerticalSpacer {}

                    Button {
                        Layout.fillWidth: true

                        text: qsTr("Import")

                        BaseScreen.StepErrorsToolTip {
                            visible: parent.activeFocus &&
                                     errors > 0
                        }

                        onClicked: {
                            forceActiveFocus()

                            idRoot.viewModel.finishScreen(Step.SuggestRecoveringVariants, Step.RecoverFromMnemonicPassphrase)
                        }
                    }
                }
            }

            Pane {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)

                contentItem: ColumnLayout {
                    spacing: Dimensions.Spacing8

                    Label {
                        Layout.fillWidth: true

                        text: qsTr("Face-scan")
                        font: Fonts.h2_comp

                        //horizontalAlignment: Qt.AlignHCenter
                        horizontalAlignment: Qt.AlignLeft
                        wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                    }

                    Label {
                        Layout.fillWidth: true

                        text: qsTr("\nUse your passphrase and face-scan.")
                        font: Fonts.p3_medium_comp
                        opacity: 0.7

                        //horizontalAlignment: Qt.AlignHCenter
                        horizontalAlignment: Qt.AlignLeft
                        wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                    }

                    VerticalSpacer {}

                    Button {
                        Layout.fillWidth: true

                        text: qsTr("Recover")

                        BaseScreen.StepErrorsToolTip {
                            visible: parent.activeFocus &&
                                     errors > 0
                        }

                        onClicked: {
                            forceActiveFocus()

                            idRoot.viewModel.finishScreen(Step.SuggestRecoveringVariants, Step.RecoverFromBiometric)
                        }
                    }
                }
            }

            Pane {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)

                contentItem: ColumnLayout {
                    spacing: Dimensions.Spacing8

                    Label {
                        Layout.fillWidth: true

                        text: qsTr("Inheritance")
                        font: Fonts.h2_comp

                        //horizontalAlignment: Qt.AlignHCenter
                        horizontalAlignment: Qt.AlignLeft
                        wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                    }

                    Label {
                        Layout.fillWidth: true

                        text: qsTr("\nUse your secret key to claim your inheritance.")
                        font: Fonts.p3_medium_comp
                        opacity: 0.7

                        //horizontalAlignment: Qt.AlignHCenter
                        horizontalAlignment: Qt.AlignLeft
                        wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                    }

                    VerticalSpacer {}

                    Button {
                        Layout.fillWidth: true

                        text: qsTr("Inherit")

                        BaseScreen.StepErrorsToolTip {
                            visible: parent.activeFocus &&
                                     errors > 0
                        }

                        onClicked: {
                            forceActiveFocus()

                            idRoot.viewModel.finishScreen(Step.SuggestRecoveringVariants, Step.RecoverFromInheritance)
                        }
                    }
                }
            }
        }

        VerticalSpacer {}
    }
}
