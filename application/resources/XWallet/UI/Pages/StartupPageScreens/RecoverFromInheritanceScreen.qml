import QtQuick
import QtQuick.Layouts

import XWallet.Data
import XWallet.Effects

import XWallet.UI.Components

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Fonts

import XWallet.Natives

BaseScreen {
    id: idRoot

    contentItem: ColumnLayout {
        spacing: 0

        VerticalSpacer {}

        ColumnLayout {
            spacing: Dimensions.Spacing12

            Label {
                text: qsTr("CLAIM YOUR INHERITANCE")

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
                text: qsTr("Enter inheritance key to inherit assets")

                font: Fonts.promo_36_wide

                wrapMode: Text.WordWrap
            }
        }

        Item {
            Layout.preferredHeight: Dimensions.Spacing32
        }

        Pane {
            Layout.maximumHeight: 250
            Layout.fillHeight: true

            Layout.fillWidth: true
            Layout.maximumWidth: Dimensions.columnsWidth(8, idRoot.width)

            contentItem: ColumnLayout {
                spacing: Dimensions.Spacing8

                Label {
                    Layout.fillWidth: true

                    text: qsTr("Enter your inheritance key")
                    font: Fonts.h2_comp

                    horizontalAlignment: Qt.AlignLeft
                    wrapMode: Text.Wrap
                }

                Label {
                    Layout.fillWidth: true

                    text: qsTr("Your secret inheritance key will be validated for authenticity and maturity")
                    font: Fonts.p3_medium_comp
                    opacity: 0.7

                    horizontalAlignment: Qt.AlignLeft
                    wrapMode: Text.Wrap
                }

                VerticalSpacer {}

                TextField {
                    id: idInheritanceKeyTextField

                    Layout.fillWidth: true

                    placeholderText: qsTr("inheritance key")

                    Component.onCompleted: {
                        Qt.callLater(idInheritanceKeyTextField.forceActiveFocus)
                    }

                    ErrorToolTip {
                        id: idInheritanceKeyToolTip

                        visible: errors > 0 &&
                                 idFinishButton.activeFocus

                        property int errors: idRoot.viewModel.step.inheritanceErrors

                        model: ObjectModel {
                            ErrorToolTip.ErrorRow {
                                trigger: idInheritanceKeyToolTip.errors & Step.InheritanceNotSetup
                                text: qsTr("Inheritance not setup")
                            }

                            ErrorToolTip.ErrorRow {
                                trigger: idInheritanceKeyToolTip.errors & Step.KeyNotMatureError
                                text: qsTr("Key not mature")
                            }

                            ErrorToolTip.ErrorRow {
                                trigger: idInheritanceKeyToolTip.errors & Step.KeyEmptyError
                                text: qsTr("Key cannot be empty")
                            }

                            ErrorToolTip.ErrorRow {
                                trigger: idInheritanceKeyToolTip.errors & Step.KeyFormatError
                                text: qsTr("Wrong key format")
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

            text: qsTr("Inherit assets")

            BaseScreen.StepErrorsToolTip {
                visible: parent.activeFocus &&
                         errors > 0
            }

            onClicked: {
                forceActiveFocus()

                idRoot.viewModel.finishInheritanceScreen(Step.RecoverFromInheritance, Step.CreatePassword, idInheritanceKeyTextField.text)
            }
        }

        VerticalSpacer {}
    }
}
