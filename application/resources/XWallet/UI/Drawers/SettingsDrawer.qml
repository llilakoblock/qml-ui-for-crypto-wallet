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

    onYChanged: {
        idSettingsPane.updateBackground()
    }

    contentItem: Control {
        contentItem: ColumnLayout {
            spacing: Dimensions.Spacing32

            RowLayout {
                Layout.fillWidth: true

                spacing: Dimensions.Spacing16

                HorizontalSpacer {}

                RoundButton {
                    icon.source: Images.close

                    onClicked: idRoot.close()
                }
            }

            Label {
                text: qsTr("SETTINGS")

                font: Fonts.p2_regular_comp
            }

            Pane {
                id: idSettingsPane

                Layout.fillWidth: true
                Layout.fillHeight: true

                contentItem: ColumnLayout {
                    spacing: Dimensions.Spacing16

                    RowLayout {
                        spacing: Dimensions.Spacing16

                        Label {
                            text: qsTr("Language")

                            font: Fonts.p1_regular_text
                        }

                        HorizontalSpacer {}

                        TabBar {
                            currentIndex: 0

                            TabButton {
                                width: implicitWidth

                                text: qsTr("EN")
                            }

                            TabButton {
                                width: implicitWidth

                                text: qsTr("IT")
                            }
                        }
                    }

                    Divider {}

                    RowLayout {
                        spacing: Dimensions.Spacing16

                        Label {
                            text: qsTr("Currency")

                            font: Fonts.p1_regular_text
                        }

                        HorizontalSpacer {}

                        TabBar {
                            currentIndex: 0

                            TabButton {
                                width: implicitWidth

                                text: qsTr("USD")
                            }

                            TabButton {
                                width: implicitWidth

                                text: qsTr("EUR")
                            }
                        }
                    }

                    Divider {}

                    RowLayout {
                        spacing: Dimensions.Spacing16

                        Label {
                            text: qsTr("Assets view")

                            font: Fonts.p1_regular_text
                        }

                        HorizontalSpacer {}

                        TabBar {
                            currentIndex: 0

                            TabButton {
                                width: implicitWidth

                                text: qsTr("Tape")
                            }

                            TabButton {
                                width: implicitWidth

                                text: qsTr("Carousel")
                            }
                        }
                    }

                    Divider {}

                    RowLayout {
                        spacing: Dimensions.Spacing16

                        Label {
                            text: qsTr("Assets sort by")

                            font: Fonts.p1_regular_text
                        }

                        HorizontalSpacer {}

                        TabBar {
                            currentIndex: 0

                            TabButton {
                                width: implicitWidth

                                text: qsTr("Balance")
                            }

                            TabButton {
                                width: implicitWidth

                                text: qsTr("Market cap")
                            }

                            TabButton {
                                width: implicitWidth

                                text: qsTr("Alphabetical")
                            }
                        }
                    }

                    Divider {}

                    RowLayout {
                        spacing: Dimensions.Spacing16

                        Label {
                            text: qsTr("Change wallet password")

                            font: Fonts.p1_regular_text
                        }

                        HorizontalSpacer {}

                        ToolButton {
                            icon.source: Images.arrowRight
                        }
                    }

                    Divider {}

                    RowLayout {
                        spacing: Dimensions.Spacing16

                        Label {
                            text: qsTr("Mnemonic & private key")

                            font: Fonts.p1_regular_text
                        }

                        HorizontalSpacer {}

                        ToolButton {
                            icon.source: Images.arrowRight
                        }
                    }

                    Divider {}

                    RowLayout {
                        spacing: Dimensions.Spacing16

                        Label {
                            text: qsTr("Manage inheritance")

                            font: Fonts.p1_regular_text
                        }

                        HorizontalSpacer {}

                        ToolButton {
                            icon.source: Images.arrowRight
                        }
                    }
                }
            }
        }
    }

    component Divider : Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 2

        color: Colors.backgroundStroke
        radius: 1
    }
}
