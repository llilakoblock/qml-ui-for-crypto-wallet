import QtQuick
import QtQuick.Controls.impl
import QtQuick.Layouts

import XWallet.Data
import XWallet.Effects

import XWallet.UI.Components

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Fonts

BaseScreen {
    id: idRoot

    property int maxInactivityDuration: 12

    contentItem: ColumnLayout {
        spacing: 0

        VerticalSpacer {}

        ColumnLayout {
            spacing: Dimensions.Spacing12

            Label {
                text: qsTr("INHERITANCE")

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
                text: qsTr("Inherit your assets to a close one.")

                font: Fonts.promo_36_wide

                wrapMode: Text.WordWrap
            }
        }

        Item {
            Layout.preferredHeight: Dimensions.Spacing32
        }

        Pane {
            Layout.maximumHeight: 460
            Layout.fillHeight: true

            Layout.fillWidth: true
            Layout.maximumWidth: Dimensions.columnsWidth(8, idRoot.width)

            contentItem: ColumnLayout {
                spacing: Dimensions.Spacing24

                RowLayout {
                    Layout.fillWidth: true

                    spacing: Dimensions.Spacing24

                    Clause {
                        Layout.alignment: Qt.AlignTop

                        index: 1
                    }

                    ColumnLayout {
                        spacing: Dimensions.Spacing12

                        Label {
                            Layout.fillWidth: true

                            text: qsTr("Inheritors Key")
                            font: Fonts.h2_wide

                            wrapMode: Text.Wrap
                        }

                        Label {
                            Layout.fillWidth: true

                            opacity: 0.7

                            text: qsTr("Give this key to your inheritor. Make sure they keep it in a safe place. With the below key, access to your wallet will be granted after you stop using the wallet for an extended period of time.")
                            font: Fonts.p3_medium_comp

                            wrapMode: Text.Wrap
                        }

                        TextField {
                            id: idInheritanceKeyTextField

                            Layout.fillWidth: true

                            readOnly: true

                            text: idRoot.viewModel.step.inheritanceKey
                            placeholderText: qsTr("inheritance key goes here...")

                            icon: TextField.Icon.Copy

                            onIconClicked: {
                                selectAll()
                                copy()
                                deselect()
                            }
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true

                    spacing: Dimensions.Spacing24

                    Clause {
                        Layout.alignment: Qt.AlignTop

                        index: 2
                    }

                    ColumnLayout {
                        spacing: Dimensions.Spacing12

                        Label {
                            Layout.fillWidth: true

                            text: qsTr("Inactivity Timespan")
                            font: Fonts.h2_wide

                            wrapMode: Text.Wrap
                        }

                        Label {
                            Layout.fillWidth: true

                            opacity: 0.7

                            text: qsTr("Choose the timespan of your inactivity after which access to your assets will be granted to your inheritors.")
                            font: Fonts.p3_medium_comp

                            wrapMode: Text.Wrap
                        }

                        TabBar {
                            id: idTabBar

                            Layout.fillWidth: true
                            Layout.preferredHeight: 56

                            currentIndex: 1

                            Repeater {
                                model: [6, 12, 18]

                                delegate: TabButton {
                                    property int value: modelData

                                    text: qsTr("%1 months").arg(value)
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

        RowLayout {
            Layout.fillWidth: true
            Layout.maximumWidth: Dimensions.columnsWidth(8, idRoot.width)

            spacing: 0

            Button {
                Layout.preferredWidth: Dimensions.columnsWidth(4, idRoot.width)

                appearance: Button.Appearance.Primary

                text: qsTr("Finish")

                BaseScreen.StepErrorsToolTip {
                    visible: parent.activeFocus &&
                             errors > 0
                }

                onClicked: {
                    forceActiveFocus()

                    idRoot.viewModel.finishInheritanceScreen(Step.Inheritance, Step.Unknown, idTabBar.currentItem.value);
                }
            }

            HorizontalSpacer {}

            Button {
                Layout.preferredWidth: Dimensions.columnsWidth(4, idRoot.width)

                appearance: Button.Appearance.Tertiary

                text: qsTr("Add another inheritance")

                BaseScreen.StepErrorsToolTip {
                    visible: parent.activeFocus &&
                             errors > 0
                }

                onClicked: {
                    forceActiveFocus()

                    idRoot.viewModel.finishInheritanceScreen(Step.Inheritance, Step.Inheritance, idTabBar.currentItem.value);
                }
            }
        }

        VerticalSpacer {}
    }

    component Clause : Control {
        id: idClauseControl

        required property int index

        contentItem: Label {
            text: idClauseControl.index
            font: Fonts.h2_wide

            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
        }

        background: Rectangle {
            implicitWidth: 56
            implicitHeight: 56

            color: Color.transparent(Colors.grayscale000, 0.2)
            radius: Math.min(width, height) / 2
        }
    }
}
