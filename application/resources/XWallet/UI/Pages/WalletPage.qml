import QtQuick
import QtQuick.Layouts
import QtQuick.Effects

import XWallet.Natives
import XWallet.Data

import XWallet.Style
import XWallet.Style.Fonts
import XWallet.Style.Controls

import XWallet.UI.Components
import XWallet.UI.Drawers
import XWallet.UI.Delegates
import XWallet.UI.Pages.WalletPageScreens

Page {
    id: idRoot

    horizontalPadding: 0

    required property string searchQuery

    WalletViewModel {
        id: idViewModel

        searchQuery: idRoot.searchQuery
    }

    PageIndicator {
        id: idPageIndicator

        parent: idRoot

        x: idRoot.width - Dimensions.Spacing24 - width / 2
        y: idRoot.contentItem.y + idRoot.contentItem.height / 2 - height / 2

        interactive: true

        count: (idRoot.contentItem as SwipeView)?.count
        currentIndex: (idRoot.contentItem as SwipeView)?.currentIndex
    }

    header: ColumnLayout {
        spacing: Dimensions.Spacing8

        Control {
            Layout.fillWidth: true
            Layout.preferredHeight: 90

            contentItem: ColumnLayout{
                spacing: 0

                AssetsList {
                    id: idAssetsList

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    visible: count > 0

                    model: idViewModel.assetsModel
                    currentIndex: idViewModel.currentIndex

                    onDelegateClicked: (index) => {
                        idViewModel.selectIndex(index)
                    }
                }

                Label {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    visible: idAssetsList.count === 0

                    text: qsTr("Asset not found.")

                    font: Fonts.p2_regular_text

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            background: Rectangle {
                color: Colors.grayscale800
            }
        }
    }

    background: Item {}

    contentItem: SwipeView {
        id: idContentSwipeView

        spacing: Dimensions.Spacing64
        orientation: Qt.Vertical

        currentIndex: idPageIndicator.currentIndex

        ChartScreen {
            horizontalPadding: Dimensions.horizontalPadding

            viewModel: idViewModel

            onSendClicked: {
                var obj = idSendDrawerComponent.createObject(idRoot)
                obj.closed.connect(obj.destroy)
                obj.open()
            }

            onReceiveClicked: {
                var obj = idReceiveDrawerComponent.createObject(idRoot)
                obj.closed.connect(obj.destroy)
                obj.open()
            }

            onSwapClicked: {
                var obj = idSwapDrawerComponent.createObject(idRoot)
                obj.closed.connect(obj.destroy)
                obj.open()
            }
        }

        HistoryScreen {
            horizontalPadding: Dimensions.horizontalPadding

            viewModel: idViewModel
        }

        // SendScreen {
        //     viewModel: idViewModel
        // }

        // ReceiveScreen {
        //     viewModel: idViewModel
        // }
    }

    Component {
        id: idReceiveDrawerComponent

        ReceiveDrawer {
            asset: idViewModel.currentAsset
        }
    }

    Component {
        id: idSendDrawerComponent

        SendDrawer {
            asset: idViewModel.currentAsset
        }
    }

    Component {
        id: idSwapDrawerComponent

        SwapDrawer {
            bidAsset: idViewModel.currentAsset
        }
    }
}
