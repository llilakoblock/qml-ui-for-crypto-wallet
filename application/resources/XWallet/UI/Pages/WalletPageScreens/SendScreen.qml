import QtQuick
import QtQuick.Layouts

import XWallet.Natives
import XWallet.Data

import XWallet.Style
import XWallet.Style.Images
import XWallet.Style.Fonts
import XWallet.Style.Controls

import XWallet.UI.Components

BaseScreen {
    id: idRoot

    Connections {
        target: idRoot.ListView.view

        function onContentYChanged(y) {
            idContentPane.updateBackground()
            idSuccessContentPane.updateBackground()
        }
    }

    SendViewModel {
        id: idSendViewModel

        assetType: idRoot.viewModel.currentAsset.type
    }

    contentItem: SwipeView {
        id: idContentSwipeView

        interactive: false
        orientation: Qt.Vertical

        verticalPadding: 0

        clip: true
        smoothCutting: false

        ColumnLayout {
            spacing: Dimensions.Spacing32

            Connections {
                target: idSuccessLayout.ListView.view

                function onContentYChanged(y) {
                    idContentPane.updateBackground()
                }
            }

            Pane {
                id: idContentPane

                Layout.fillWidth: true
                Layout.maximumWidth: Dimensions.columnsWidth(8, idRoot.width)
                Layout.alignment: Qt.AlignHCenter

                horizontalPadding: 32
                verticalPadding: 32

                contentItem: ColumnLayout {
                    spacing: Dimensions.Spacing16

                    TextField {
                        id: idAddressTextField

                        Layout.fillWidth: true

                        placeholderText: qsTr("Send to %1 address").arg(idRoot.viewModel.currentAsset.currency.name)

                        icon: TextField.Icon.Copy

                        horizontalAlignment: Qt.AlignHCenter

                        validator: RegularExpressionValidator {
                            regularExpression: {
                                switch(idRoot.viewModel.currentAsset.type) {
                                case Asset.Bitcoin:
                                    return /(^([13]|n|m)[a-zA-HJ-NP-Z0-9]{25,39}$|^(bc1|tb1)[a-zA-HJ-NP-Z0-9]{39}$|^(bc1|tb1)[a-zA-HJ-NP-Z0-9]{59}$)/
                                case Asset.Ethereum:
                                case Asset.Erc20:
                                    return /^0x[a-fA-F0-9]{40}$/
                                case Asset.VaraNetwork:
                                    return /.*/
                                default:
                                    return /./
                                }
                            }
                        }
                    }

                    DoubleTextFields {
                        id: idAmountTextFields

                        Layout.fillWidth: true

                        asset: idRoot.viewModel.currentAsset

                        firstText: idSendViewModel.coinsToSend
                        secondText: idSendViewModel.fiatsToSend

                        onFirstTextEdited: idSendViewModel.coinsToSend = firstText
                        onSecondTextEdited: idSendViewModel.fiatsToSend = secondText
                    }

                    Slider {
                        id: idFeeSlider

                        Layout.fillWidth: true

                        from: 10
                        to: 100
                        value: 38
                    }

                    ColumnLayout {
                        Layout.fillWidth: true

                        spacing: Dimensions.Spacing4

                        RowLayout {
                            spacing: Dimensions.Spacing16

                            Label {
                                text: qsTr("Recommended Fee")

                                font: Fonts.p3_regular_text

                                color: Colors.textSecondary
                            }

                            HorizontalSpacer {}

                            Label {
                                text: qsTr("gwei/gas 38")

                                font: Fonts.p3_regular_text

                                color: Colors.textSecondary
                            }
                        }

                        RowLayout {
                            spacing: Dimensions.Spacing16

                            Label {
                                text: qsTr("Confirmation speed: Normal")

                                font: Fonts.p3_regular_text

                                color: Colors.textSecondary
                            }

                            HorizontalSpacer {}

                            Label {
                                text: qsTr("$%1").arg((idFeeSlider.value * 0.2).toLocaleString(Qt.locale(), "f", 2))

                                font: Fonts.p3_regular_text

                                color: Colors.textSecondary
                            }
                        }
                    }
                }
            }

            Button {
                Layout.fillWidth: true
                Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)
                Layout.alignment: Qt.AlignHCenter

                text: qsTr("Send")

                icon.source: Images.arrowUp

                onClicked: {
                    if(idAddressTextField.text.length === 0 || !idAddressTextField.acceptableInput) {
                        idAddressTextField.forceActiveFocus()
                        return
                    }

                    if(idAmountTextFields.firstText.length === 0 && idAmountTextFields.secondText.length === 0) {
                        return
                    }

                    idSendViewModel.makeTransaction(idAddressTextField.text, idAmountTextFields.firstText, idFeeSlider.value)

                    idContentSwipeView.setCurrentIndex(1)
                }
            }
        }

        ColumnLayout {
            id: idSuccessLayout

            spacing: Dimensions.Spacing32

            Connections {
                target: idSuccessLayout.ListView.view

                function onContentYChanged(y) {
                    idSuccessContentPane.updateBackground()
                }
            }

            Pane {
                id: idSuccessContentPane

                Layout.fillWidth: true
                Layout.maximumWidth: Dimensions.columnsWidth(8, idRoot.width)
                Layout.alignment: Qt.AlignHCenter

                horizontalPadding: 32
                verticalPadding: 32

                contentItem: ColumnLayout {
                    spacing: Dimensions.Spacing16

                    Image {
                        Layout.preferredWidth: 72
                        Layout.preferredHeight: 72
                        Layout.alignment: Qt.AlignHCenter

                        source: Images.successCircle
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter

                        spacing: Dimensions.Spacing8

                        Label {
                            Layout.alignment: Qt.AlignHCenter

                            text: qsTr("Success")

                            font: Fonts.h4_text
                        }

                        Label {
                            Layout.alignment: Qt.AlignHCenter

                            text: qsTr("Your transaction has been sent.")

                            font: Fonts.p4_medium_comp

                            color: Colors.textSecondary
                        }
                    }

                    Label {
                        Layout.alignment: Qt.AlignHCenter

                        text: qsTr("$%1").arg(idSendViewModel.fiatsToSend)

                        font: Fonts.promo_36_wide
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter

                        spacing: Dimensions.Spacing8

                        Label {
                            Layout.alignment: Qt.AlignHCenter

                            text: qsTr("Recipient's address:")
                            /*text: qsTr("To address on %1 mainnet (%2) network").
                                        arg(idRoot.viewModel.currentAsset.currency.name).
                                        arg(idRoot.viewModel.currentAsset.currency.symbol)*/

                            font: Fonts.p3_regular_text

                            color: Colors.textSecondary
                        }

                        Label {
                            Layout.alignment: Qt.AlignHCenter

                            text: idAddressTextField.text

                            font: Fonts.p3_regular_text
                        }
                    }

                    TextField {
                        Layout.fillWidth: true

                        placeholderText: qsTr("Personal note")

                        icon: TextField.Icon.Pencil
                    }
                }
            }

            Button {
                Layout.fillWidth: true
                Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)
                Layout.alignment: Qt.AlignHCenter

                text: qsTr("Done")

                onClicked: {
                    idContentSwipeView.setCurrentIndex(0)
                }
            }
        }
    }
}
