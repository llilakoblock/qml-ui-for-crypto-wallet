import QtQuick
import QtQuick.Layouts
import QtQuick.Templates as T

import XWallet.Data

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Fonts

Control {
    id: control

    required property asset asset

    property alias firstField: idFirstTextField
    property alias secondField: idSecondTextField

    property alias firstText: idFirstTextField.text
    property alias secondText: idSecondTextField.text

    property bool inActiveFocus: idFirstTextField.activeFocus || idSecondTextField.activeFocus

    signal firstTextEdited(string text)
    signal secondTextEdited(string text)

    contentItem: ColumnLayout {
        spacing: 0

        CustomTextField {
            id: idFirstTextField

            Layout.fillWidth: true

            text: control.firstText
            currency: control.asset.currency.symbol

            validator: DoubleValidator {
                bottom: 0.00
                decimals: control.asset.currency.decimal
                notation: DoubleValidator.StandardNotation
                locale: Qt.locale("US").name
            }

            onTextEdited: {
                control.firstTextEdited(text)
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1

            color: Colors.backgroundStroke
        }

        CustomTextField {
            id: idSecondTextField

            Layout.fillWidth: true

            text: control.secondText
            currency: qsTr("USD")

            validator: DoubleValidator {
                bottom: 0.00
                decimals: 2
                notation: DoubleValidator.StandardNotation
                locale: Qt.locale("US").name
            }

            onTextEdited: {
                control.secondTextEdited(text)
            }
        }
    }

    background: Item {}

    component CustomTextField : TextField {
        id: idCustomTextField

        leftPadding: 12
        rightPadding: 60

        placeholderText: "0.00"

        inputMethodHints: Qt.ImhDigitsOnly

        required property string currency

        Label {
            x: idCustomTextField.width - width - Dimensions.Spacing12
            y: (idCustomTextField.height - height) / 2

            text: idCustomTextField.currency

            font: Fonts.p2_regular_text
        }

        background: Item {}
    }
}
