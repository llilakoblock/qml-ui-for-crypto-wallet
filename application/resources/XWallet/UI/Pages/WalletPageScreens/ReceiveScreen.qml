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
        }
    }

    contentItem: ColumnLayout {
        spacing: 0

        Pane {
            id: idContentPane

            Layout.fillWidth: true
            Layout.maximumWidth: Dimensions.columnsWidth(8, idRoot.width)
            Layout.alignment: Qt.AlignHCenter

            horizontalPadding: 32
            verticalPadding: 32

            contentItem: ColumnLayout {
                spacing: Dimensions.Spacing16

                Label {
                    Layout.alignment: Qt.AlignHCenter

                    text: qsTr("Your %1 address on %2 network").
                            arg(idRoot.viewModel.currentAsset.currency.name).
                            arg(idRoot.viewModel.currentAsset.currency.symbol)

                    font: Fonts.h3_text
                }

                TextField {
                    Layout.fillWidth: true

                    text: idRoot.viewModel.currentAsset.address
                    icon: TextField.Icon.Copy

                    readOnly: true

                    horizontalAlignment: Qt.AlignHCenter

                    onIconClicked: {
                        selectAll()
                        copy()
                        deselect()
                    }
                }

                Frame {
                    Layout.alignment: Qt.AlignHCenter

                    contentItem: Image {
                        source: "image://qrcode/" + idRoot.viewModel.currentAsset.address
                        sourceSize: Qt.size(200, 200)

                        fillMode: Image.PreserveAspectFit
                    }
                }
            }
        }
    }
}
