import QtQuick
import QtQuick.Layouts

import XWallet.Natives
import XWallet.Data

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Images
import XWallet.Style.Fonts

import XWallet.UI.Components

Drawer {
    id: idRoot

    width: Overlay.overlay.width
    height: Overlay.overlay.height

    interactive: false
    closePolicy: Popup.NoAutoClose

    required property asset asset

    AssetPresenter {
        id: idAssetPresenter

        asset: idRoot.asset
    }

    SendViewModel {
        id: idSendViewModel

        asset: idRoot.asset

        onStatusChanged: {
            switch (status) {
                case SendViewModel.UnknownStatus:
                    idStackView.replace(idInputsComponent)
                    break
                case SendViewModel.BusyStatus:
                    idStackView.replace(idBusyComponent)
                    break
                case SendViewModel.SuccessStatus:
                    idStackView.replace(idSuccessComponent)
                    break
                case SendViewModel.ErrorStatus:
                    idStackView.replace(idErrorComponent)
                    break
                default:
                    console.warn("Wrong status:", status)
                    break
            }
        }
    }

    contentItem: Control {
        contentItem: ColumnLayout {
            spacing: 0

            RowLayout {
                Layout.fillWidth: true

                spacing: Dimensions.Spacing16

                HorizontalSpacer {}

                RoundButton {
                    icon.source: Images.close

                    onClicked: idRoot.close()
                }
            }

            StackView {
                id: idStackView

                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumWidth: Dimensions.columnsWidth(9, idRoot.width)
                Layout.alignment: Qt.AlignHCenter

                initialItem: idInputsComponent

                replaceEnter: Transition {
                    OpacityAnimator {
                        from: 0
                        to: 1
                        duration: 500
                        easing.type: Easing.InCubic
                    }
                }

                replaceExit: Transition {
                    OpacityAnimator {
                        from: 1
                        to: 0
                        duration: 500
                        easing.type: Easing.OutCubic
                    }

                    ScaleAnimator {
                        from: 1
                        to: 1.1
                        duration: 500
                        easing.type: Easing.OutCubic
                    }
                }
            }
        }

        Component {
            id: idInputsComponent

            ColumnLayout {
                spacing: Dimensions.Spacing32

                VerticalSpacer {}

                ColumnLayout {
                    Layout.alignment: Qt.AlignHCenter

                    spacing: Dimensions.Spacing16

                    Image {
                        Layout.preferredWidth: 88
                        Layout.preferredHeight: 88
                        Layout.alignment: Qt.AlignHCenter

                        source: Images.iconForAssetType(idRoot.asset.type)
                    }

                    ColumnLayout {
                        Layout.alignment: Qt.AlignHCenter

                        spacing: Dimensions.Spacing4

                        Label {
                            Layout.alignment: Qt.AlignHCenter

                            text: ("%1 %2").arg(idAssetPresenter.balance).arg(idRoot.asset.currency.symbol)

                            font: Fonts.h3_wide
                            color: Colors.colorForAssetType(idRoot.asset.type)
                        }

                        Label {
                            Layout.alignment: Qt.AlignHCenter

                            text: ("$%1").arg(idAssetPresenter.fiatBalance)

                            font: Fonts.p1_regular_comp
                            color: Colors.textSecondary
                        }
                    }
                }

                ColumnLayout {
                    spacing: Dimensions.Spacing32

                    TextField {
                        id: idAddressTextField

                        Layout.fillWidth: true

                        text: idSendViewModel.address
                        placeholderText: qsTr("Send to %1 address").arg(idRoot.asset.currency.name)

                        validator: RegularExpressionValidator {
                            regularExpression: {
                                switch(idRoot.asset.type) {
                                case Asset.Bitcoin:
                                    return /(^([13]|n|m)[a-zA-HJ-NP-Z0-9]{25,39}$|^(bc1|tb1|bcrt1)[a-zA-HJ-NP-Z0-9]{39}$|^(bc1|tb1|bcrt1)[a-zA-HJ-NP-Z0-9]{59}$)/
                                case Asset.Ethereum:
                                case Asset.Tether:
                                    return /^0x[a-fA-F0-9]{40}$/
                                case Asset.VaraNetwork:
                                    return /^kG[a-zA-Z0-9]{47}$/
                                default:
                                    return /./
                                }
                            }
                        }

                        onTextEdited: {
                            idSendViewModel.address = text
                        }

                        Component.onCompleted: {
                            Qt.callLater(idAddressTextField.forceActiveFocus)
                        }

                        ToolTip {
                            visible: idAddressTextField.length > 0 &&
                                     !idAddressTextField.acceptableInput

                            contentItem: Label {
                                text: qsTr("Wrong address format")

                                font: Fonts.p1_regular_text
                            }
                        }
                    }

                    DoubleTextFields {
                        id: idAmountTextFields

                        Layout.fillWidth: true

                        asset: idRoot.asset

                        firstText: idSendViewModel.coinsToSend
                        secondText: idSendViewModel.fiatsToSend

                        onFirstTextEdited: idSendViewModel.coinsToSend = firstText
                        onSecondTextEdited: idSendViewModel.fiatsToSend = secondText

                        ToolTip {
                            visible: idSendViewModel.errors & SendViewModel.InsufficientAmountError ||
                                     idSendViewModel.errors & SendViewModel.ZeroAmountError

                            contentItem: Label {
                                text: {
                                    if(idSendViewModel.errors & SendViewModel.InsufficientAmountError) {
                                        return qsTr("You don't have" + "\n" +
                                                    "enough funds" + "\n" +
                                                    "to send")
                                    }
                                    else if(idSendViewModel.errors & SendViewModel.ZeroAmountError) {
                                        return qsTr("Please enter" + "\n" +
                                                    "a valid amount")
                                    }
                                    else {
                                        return ""
                                    }
                                }

                                font: Fonts.p1_regular_text
                            }
                        }
                    }

                    // Slider {
                    //     id: idFeeSlider

                    //     Layout.fillWidth: true

                    //     from: 10
                    //     to: 100
                    //     value: 38
                    // }

                    ColumnLayout {
                        Layout.fillWidth: true

                        spacing: Dimensions.Spacing4

                        RowLayout {
                            spacing: Dimensions.Spacing16

                            Label {
                                text: qsTr("%1 network fee").arg(idAssetPresenter.parentAssetExists ?
                                                                     idAssetPresenter.parentAsset.currency.name :
                                                                     idAssetPresenter.asset.currency.name)

                                font: Fonts.p3_regular_text

                                color: Colors.textSecondary
                            }

                            HorizontalSpacer {}

                            Label {
                                text: qsTr("%1 %2").arg(idAssetPresenter.fee).arg(idAssetPresenter.parentAssetExists ?
                                                                                      idAssetPresenter.parentAsset.currency.symbol :
                                                                                      idAssetPresenter.asset.currency.symbol)

                                font: Fonts.p3_regular_text

                                color: Colors.textSecondary
                            }
                        }

                        RowLayout {
                            spacing: Dimensions.Spacing16

                            Label {
                                text: qsTr("Fee price")

                                font: Fonts.p3_regular_text

                                color: Colors.textSecondary
                            }

                            HorizontalSpacer {}

                            Label {
                                text: qsTr("$%1").arg(idAssetPresenter.fiatFee)

                                font: Fonts.p3_regular_text

                                color: Colors.textSecondary
                            }
                        }

                        RowLayout {
                            spacing: Dimensions.Spacing16

                            Label {
                                text: qsTr("Remaining balance")

                                font: Fonts.p3_regular_text

                                color: Colors.textSecondary
                            }

                            HorizontalSpacer {}

                            Label {
                                text: qsTr("%1 %2").arg(idSendViewModel.remainingBalance).arg(idRoot.asset.currency.symbol)

                                font: Fonts.p3_regular_text

                                color: Colors.textSecondary
                            }
                        }
                    }
                }

                VerticalSpacer {}

                Button {
                    Layout.fillWidth: true
                    Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)
                    Layout.alignment: Qt.AlignHCenter

                    text: qsTr("Send")

                    icon.source: Images.arrowUp

                    onClicked: {
                        if(idAddressTextField.length === 0 || !idAddressTextField.acceptableInput) {
                            idAddressTextField.forceActiveFocus()
                            return
                        }

                        var errors = idSendViewModel.makeTransaction()

                        switch(errors) {
                            case SendViewModel.ZeroAmountError:
                                idAmountTextFields.firstField.forceActiveFocus()
                                break
                            case SendViewModel.InsufficientAmountError:
                                idAmountTextFields.firstField.forceActiveFocus()
                                break
                            default:
                                break
                        }

                        if(idAmountTextFields.firstText.length === 0) {
                            idAmountTextFields.firstField.forceActiveFocus()
                            return
                        }

                        if(idAmountTextFields.secondText.length === 0) {
                            idAmountTextFields.secondField.forceActiveFocus()
                            return
                        }
                    }
                }
            }
        }
    }

    Component {
        id: idBusyComponent

        ColumnLayout {
            spacing: Dimensions.Spacing32

            VerticalSpacer {}

            Label {
                Layout.alignment: Qt.AlignHCenter

                text: qsTr("Processing transaction")

                font: Fonts.h3_wide
            }

            BusyIndicator {
                Layout.alignment: Qt.AlignHCenter
            }

            VerticalSpacer {}
        }

    }

    Component {
        id: idSuccessComponent

        ColumnLayout {
            spacing: Dimensions.Spacing32

            VerticalSpacer {}

            Pane {
                Layout.fillWidth: true
                Layout.maximumWidth: Dimensions.columnsWidth(8, idRoot.width)
                Layout.alignment: Qt.AlignHCenter

                horizontalPadding: 32
                verticalPadding: 32

                contentItem: ColumnLayout {
                    spacing: Dimensions.Spacing24

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter

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
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter

                        spacing: Dimensions.Spacing4

                        Label {
                            Layout.alignment: Qt.AlignHCenter

                            text: qsTr("%1 %2").arg(idSendViewModel.coinsToSend).arg(idRoot.asset.currency.symbol)

                            font: Fonts.h1_comp
                        }

                        Label {
                            Layout.alignment: Qt.AlignHCenter

                            text: qsTr("$%1").arg(idSendViewModel.fiatsToSend)

                            font: Fonts.h2_comp
                            color: Colors.textSecondary
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter

                        spacing: Dimensions.Spacing20

                        ContentLabelLink {
                            description: qsTr("Recipient's address:")
                            /*description: qsTr("To address on %1 mainnet (%2) network:").
                                            arg(idAssetPresenter.parentAssetExists ?
                                                    idAssetPresenter.parentAsset.currency.name :
                                                    idAssetPresenter.asset.currency.name).
                                            arg(idAssetPresenter.parentAssetExists ?
                                                    idAssetPresenter.parentAsset.currency.symbol :
                                                    idAssetPresenter.asset.currency.symbol)*/

                            value: idSendViewModel.transaction.to[0]
                        }

                        ContentLabelLink {
                            description: qsTr("Transaction ID:")

                            value: idSendViewModel.transaction.hash
                        }
                    }

                    TextField {
                        id: idNoteTextField

                        Layout.fillWidth: true

                        placeholderText: qsTr("Personal note")

                        icon: TextField.Icon.Pencil

                        Component.onCompleted: {
                            Qt.callLater(idNoteTextField.forceActiveFocus)
                        }
                    }
                }
            }

            VerticalSpacer {}

            Button {
                Layout.fillWidth: true
                Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)
                Layout.alignment: Qt.AlignHCenter

                text: qsTr("Done")

                onClicked: {
                    idSendViewModel.leaveNote(idNoteTextField.text)
                    idRoot.close()
                }
            }
        }
    }

    Component {
        id: idErrorComponent

        ColumnLayout {
            spacing: Dimensions.Spacing32

            VerticalSpacer {}

            Pane {
                Layout.fillWidth: true
                Layout.maximumWidth: Dimensions.columnsWidth(8, idRoot.width)
                Layout.alignment: Qt.AlignHCenter

                horizontalPadding: 32
                verticalPadding: 32

                contentItem: ColumnLayout {
                    spacing: Dimensions.Spacing24

                    Image {
                        Layout.preferredWidth: 72
                        Layout.preferredHeight: 72
                        Layout.alignment: Qt.AlignHCenter

                        source: Images.errorCircle
                    }

                    Label {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter

                        text: idSendViewModel.errorMessage

                        font: Fonts.h4_text

                        wrapMode: Text.Wrap
                    }
                }
            }

            VerticalSpacer {}

            Button {
                Layout.fillWidth: true
                Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)
                Layout.alignment: Qt.AlignHCenter

                text: qsTr("Retry")

                onClicked: {
                    idSendViewModel.reset()
                }
            }
        }
    }

    component ContentLabelLink : ColumnLayout {
        id: idContentLabelLink

        spacing: Dimensions.Spacing8

        required property string description
        required property string value

        Label {
            Layout.fillWidth: true

            text: idContentLabelLink.description

            font: Fonts.p3_regular_text
            color: Colors.textSecondary

            horizontalAlignment: Text.AlignLeft
        }

        RowLayout {
            spacing: Dimensions.Spacing8

            Label {
                Layout.fillWidth: true

                text: idContentLabelLink.value

                font: Fonts.p3_regular_text
                color: Colors.textVisitedLink
                elide: Text.ElideRight

                horizontalAlignment: Text.AlignLeft
            }

            ToolButton {
                icon.source: Images.copy
                padding: 0

                TextEdit {
                    id: idHelperTextEdit

                    visible: false
                    text: idContentLabelLink.value
                }

                background: Item {
                    implicitWidth: 24
                    implicitHeight: 24
                }

                onClicked: {
                    idHelperTextEdit.selectAll()
                    idHelperTextEdit.copy()
                    idHelperTextEdit.deselect()
                }
            }
        }
    }
}
