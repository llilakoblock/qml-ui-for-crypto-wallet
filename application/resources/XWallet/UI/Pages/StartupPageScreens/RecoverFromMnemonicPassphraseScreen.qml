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
                text: qsTr("IMPORT FROM MNEMONIC")

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
                text: qsTr("Enter your 12-word mnemonic phrase.")

                font: Fonts.promo_36_wide

                wrapMode: Text.WordWrap
            }
        }

        Item {
            Layout.preferredHeight: Dimensions.Spacing32
        }

        Pane {
            Layout.fillWidth: true

            contentItem: ColumnLayout {
                spacing: Dimensions.Spacing40

                ColumnLayout {
                    spacing: Dimensions.Spacing8

                    Label {
                        text: qsTr("Let's recover your wallet")

                        font: Fonts.h2_wide
                    }

                    Label {
                        text: qsTr("Enter your 12-word mnemonic phrase.")

                        font: Fonts.p3_medium_comp

                        opacity: 0.7
                    }
                }

                GridLayout {
                    Layout.fillWidth: true

                    columns: 3
                    rows: 4

                    columnSpacing: Dimensions.Spacing32
                    rowSpacing: Dimensions.Spacing16

                    Repeater {
                        id: idRepeater

                        model: 12

                        delegate: RowLayout {
                            id: idFieldDelegate

                            spacing: Dimensions.Spacing8

                            Layout.fillWidth: true

                            required property int index

                            readonly property string text: idDelegateTextField.text

                            Label {
                                Layout.minimumWidth: 34
                                Layout.alignment: Qt.AlignLeft

                                text: index + 1

                                font: Fonts.p2_regular_text

                                horizontalAlignment: Text.AlignRight
                            }

                            TextField {
                                id: idDelegateTextField

                                Layout.fillWidth: true

                                placeholderText: qsTr("enter word")

                                echoMode: activeFocus ? TextInput.Normal : TextInput.Password

                                Component.onCompleted: {
                                    if(index === 0) {
                                        Qt.callLater(idDelegateTextField.forceActiveFocus)
                                    }
                                }

                                ToolTip {
                                    visible: idFinishButton.activeFocus &&
                                             idDelegateTextField.text.length === 0

                                    contentItem: Label {
                                        text: qsTr("Word is required")

                                        font: Fonts.p1_regular_text
                                    }
                                }
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
            id: idFinishButton

            Layout.fillWidth: true
            Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)

            size: Button.Size.Large
            appearance: Button.Appearance.Primary

            text: qsTr("Import")

            BaseScreen.StepErrorsToolTip {
                visible: parent.activeFocus &&
                         errors > 0
            }

            onClicked: {
                forceActiveFocus()

                var mnemonicPassphrase = []

                for(var i = 0; i < idRepeater.count; i++) {
                    if(idRepeater.itemAt(i).text.length === 0) {
                        return
                    }

                    mnemonicPassphrase.push(idRepeater.itemAt(i).text)
                }

                idRoot.viewModel.finishScreen(Step.RecoverFromMnemonicPassphrase, Step.CreatePassword, mnemonicPassphrase)
            }
        }

        VerticalSpacer {}
    }
}
