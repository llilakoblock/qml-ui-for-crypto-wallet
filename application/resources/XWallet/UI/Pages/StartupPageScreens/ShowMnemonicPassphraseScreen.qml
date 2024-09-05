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

    property int currentlyShownFieldIndex: -1

    contentItem: ColumnLayout {
        spacing: 0

        VerticalSpacer {}

        ColumnLayout {
            spacing: Dimensions.Spacing12

            Label {
                text: qsTr("MNEMONIC PHRASE")

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
                text: qsTr("Remember to keep your phrase safe and secure.")

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
                        text: qsTr("Save the words listed below")

                        font: Fonts.h2_wide
                    }

                    Label {
                        text: qsTr("The following words will be used to recover your wallet")

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
                        model: idRoot.viewModel.step.mnemonicPassphrase

                        delegate: RowLayout {
                            spacing: Dimensions.Spacing8

                            Layout.fillWidth: true

                            Label {
                                Layout.minimumWidth: 34
                                Layout.alignment: Qt.AlignLeft

                                text: index + 1

                                font: Fonts.p2_regular_text

                                horizontalAlignment: Text.AlignRight
                            }

                            TextField {
                                Layout.fillWidth: true

                                readOnly: true

                                text: modelData

                                /*echoMode: idRoot.currentlyShownFieldIndex === index ?
                                              TextInput.Normal :
                                              TextInput.Password*/
                                icon: TextField.Icon.Password

                                onIconClicked: {
                                    forceActiveFocus()

                                    if(idRoot.currentlyShownFieldIndex === index) {
                                        idRoot.currentlyShownFieldIndex = -1
                                    }
                                    else {
                                        idRoot.currentlyShownFieldIndex = index
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
            Layout.fillWidth: true
            Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)

            size: Button.Size.Large
            appearance: Button.Appearance.Primary

            text: qsTr("Next")

            BaseScreen.StepErrorsToolTip {
                visible: parent.activeFocus &&
                         errors > 0
            }

            onClicked: {
                forceActiveFocus()

                console.log("Mnemonic phrase:")
                console.log(idRoot.viewModel.step.mnemonicPassphrase)

                idRoot.viewModel.finishScreen(Step.ShowMnemonicPassphrase, Step.CheckMnemonicPassphrase)
            }
        }

        VerticalSpacer {}
    }
}
