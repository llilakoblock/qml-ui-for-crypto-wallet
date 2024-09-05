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

    onYChanged: {
        idQrCodePane.updateBackground()
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

            VerticalSpacer {}

            ColumnLayout {
                Layout.fillWidth: true
                Layout.maximumWidth: Dimensions.columnsWidth(9, idRoot.width)
                Layout.alignment: Qt.AlignHCenter

                spacing: Dimensions.Spacing32

                ColumnLayout {
                    Layout.alignment: Qt.AlignHCenter

                    spacing: Dimensions.Spacing16

                    Image {
                        Layout.preferredWidth: 88
                        Layout.preferredHeight: 88
                        Layout.alignment: Qt.AlignHCenter

                        source: Images.iconForAssetType(idRoot.asset.type)
                    }

                    Label {
                        Layout.alignment: Qt.AlignHCenter

                        text: idRoot.asset.currency.symbol

                        font: Fonts.h3_wide
                        color: Colors.colorForAssetType(idRoot.asset.type)
                    }
                }

                Label {
                    Layout.alignment: Qt.AlignHCenter

                    text: qsTr("Your %1 address on %2 network").
                            arg(idRoot.asset.currency.name).
                            arg(idRoot.asset.currency.symbol)

                    font: Fonts.h3_text
                }

                TextField {
                    Layout.fillWidth: true

                    text: idRoot.asset.address
                    icon: TextField.Icon.Copy

                    readOnly: true

                    horizontalAlignment: Qt.AlignHCenter

                    onIconClicked: {
                        selectAll()
                        copy()
                        deselect()
                    }
                }

                Pane {
                    id: idQrCodePane

                    Layout.alignment: Qt.AlignHCenter

                    horizontalPadding: 32
                    verticalPadding: 32

                    contentItem: Image {
                        source: "image://qrcode/" + idRoot.asset.address
                        sourceSize: Qt.size(200, 200)

                        fillMode: Image.PreserveAspectFit
                    }
                }
            }

            VerticalSpacer {}
        }
    }
}
