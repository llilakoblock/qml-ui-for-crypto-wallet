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

    component CheckingField : QtObject {
        property int index
        property bool valid
    }

    readonly property list<CheckingField> checkingFields: [
        CheckingField { index: Math.floor(Math.random() * idRoot.viewModel.step.mnemonicPassphrase.length); valid: false },
        CheckingField { index: Math.floor(Math.random() * idRoot.viewModel.step.mnemonicPassphrase.length); valid: false },
        CheckingField { index: Math.floor(Math.random() * idRoot.viewModel.step.mnemonicPassphrase.length); valid: false }
    ]

    function contains(array : list<CheckingField>, index : int) : bool {
        for(const item of array) {
            if(item.index === index) {
                return true
            }
        }

        return false
    }

    function lowestIndex(array : list<CheckingField>) : int {
        var index = idRoot.viewModel.step.mnemonicPassphrase.length - 1

        for(const item of array) {
            if(item.index < index) {
                index = item.index
            }
        }

        return index
    }

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
                text: qsTr("Let's check your mnemonic phrase.")

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
                        text: qsTr("Let's check")

                        font: Fonts.h2_wide
                    }

                    Label {
                        text: qsTr("Write the missing words in the empty fields")

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

                        model: idRoot.viewModel.step.mnemonicPassphrase

                        delegate: RowLayout {
                            id: idFieldDelegate

                            spacing: Dimensions.Spacing8

                            Layout.fillWidth: true

                            readonly property bool checking: idRoot.contains(idRoot.checkingFields, index)

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

                                readOnly: idFieldDelegate.checking ? false : true

                                text: idFieldDelegate.checking ? "" : modelData
                                placeholderText: idFieldDelegate.checking ? qsTr("enter word") : ""

                                echoMode: idFieldDelegate.checking ? TextInput.Normal : TextInput.Password

                                Component.onCompleted: {
                                    if(index === idRoot.lowestIndex(idRoot.checkingFields)) {
                                        Qt.callLater(idDelegateTextField.forceActiveFocus)
                                    }
                                }

                                ToolTip {
                                    visible: idFieldDelegate.checking &&
                                             idFinishButton.activeFocus &&
                                             idDelegateTextField.text !== modelData

                                    contentItem: Label {
                                        text: qsTr("Incorrect word")

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

            text: qsTr("Next")

            BaseScreen.StepErrorsToolTip {
                visible: parent.activeFocus &&
                         errors > 0
            }

            onClicked: {
                forceActiveFocus()

                for(const field of idRoot.checkingFields) {
                    let index = field.index

                    if(idRepeater.itemAt(index).text !== idRoot.viewModel.step.mnemonicPassphrase[index]) {
                        console.log("Wrong word at index:", field.index + 1, "expected:", idRoot.viewModel.step.mnemonicPassphrase[field.index])

                        return
                    }
                }

                switch(idRoot.viewModel.step.restorationMethod) {
                case Step.FromBiometric:
                    idRoot.viewModel.finishScreen(Step.CheckMnemonicPassphrase, Step.SuggestInheritance)
                    break
                default:
                    idRoot.viewModel.finishScreen(Step.CheckMnemonicPassphrase, Step.SuggestBiometricCreation)
                    break;
                }
            }
        }

        VerticalSpacer {}
    }
}
