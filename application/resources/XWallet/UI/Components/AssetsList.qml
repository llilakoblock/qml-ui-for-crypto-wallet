import QtQuick

import XWallet.Data

import XWallet.Style

import XWallet.UI.Delegates

ListView {
    id: idRoot

    orientation: ListView.Horizontal

    highlightMoveDuration: Animations.mediumDuration
    highlightFollowsCurrentItem: false

    cacheBuffer: 1000

    signal delegateClicked(int index)

    delegate: AssetDelegate {
        height: ListView.view.height

        isCurrentItem: ListView.isCurrentItem
        isLastItem: index === idRoot.count - 1

        onClicked: {
            idRoot.positionViewAtIndex(index, ListView.Contain)
            idRoot.delegateClicked(index)
        }
    }

    highlight: Item {
        Rectangle {
            visible: idRoot.currentItem

            y: (idRoot.currentItem?.y || idRoot.height) - height
            x: idRoot.currentItem?.x || 0

            width: idRoot.currentItem?.width || 0
            height: 4

            color: Colors.colorForAssetType(idRoot.currentItem?.asset.type || Asset.Unknown)

            Behavior on color { ColorAnimation { duration: Animations.shortDuration } }

            Behavior on x { NumberAnimation { duration: Animations.mediumDuration } }
        }
    }
}
