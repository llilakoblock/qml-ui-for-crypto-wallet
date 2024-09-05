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
                text: qsTr("START YOUR JORNEY")

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
                text: qsTr("Welcome to Freedom Wallet. Let's get started.")

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
                Layout.maximumWidth: Dimensions.columnsWidth(6, idRoot.width)

                contentItem: ColumnLayout {
                    spacing: Dimensions.Spacing8

                    Label {
                        Layout.fillWidth: true

                        text: qsTr("Create new wallet")
                        font: Fonts.h2_comp

                        //horizontalAlignment: Qt.AlignHCenter
                        horizontalAlignment: Qt.AlignLeft
                        wrapMode: Label.WrapAtWordBoundaryOrAnywhere

                        maximumLineCount: 2
                        elide: Label.ElideRight
                    }

                    Label {
                        Layout.fillWidth: true

                        text: qsTr("\nGet started with a new multicurrency crypto wallet. Store, send, receive, and trade your crypto.")
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

                        text: qsTr("Create new wallet")

                        onClicked: {
                            idRoot.viewModel.finishScreen(Step.Welcome, Step.CreatePassword)
                        }
                    }
                }
            }

            Pane {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumWidth: Dimensions.columnsWidth(6, idRoot.width)

                contentItem: ColumnLayout {
                    spacing: Dimensions.Spacing8

                    Label {
                        Layout.fillWidth: true

                        text: qsTr("Restore existing wallet")
                        font: Fonts.h2_comp

                        //horizontalAlignment: Qt.AlignHCenter
                        horizontalAlignment: Qt.AlignLeft
                        wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                    }

                    Label {
                        Layout.fillWidth: true

                        text: qsTr("\nRecover your wallet with your 12-word mnemonic phrase, your non-custodial biometric face scan, or your inheritance key.")
                        font: Fonts.p3_medium_comp
                        opacity: 0.7

                        //horizontalAlignment: Qt.AlignHCenter
                        horizontalAlignment: Qt.AlignLeft
                        wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                    }

                    VerticalSpacer {}

                    Button {
                        Layout.fillWidth: true

                        text: qsTr("Recover existing wallet")

                        onClicked: {
                            idRoot.viewModel.finishScreen(Step.Welcome, Step.SuggestRecoveringVariants)
                        }
                    }
                }
            }
        }

        VerticalSpacer {}
    }
}
