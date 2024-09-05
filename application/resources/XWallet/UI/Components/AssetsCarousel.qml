import QtQuick
import QtQuick.Layouts

import XWallet.UI.Delegates

PathView {
    id: idPathView

    pathItemCount: 7
    preferredHighlightBegin: 0.5
    preferredHighlightEnd: 0.5
    highlightRangeMode: PathView.StrictlyEnforceRange
    highlightMoveDuration: 400
    maximumFlickVelocity: 500

    property int currentAssetType: 0

    // Rectangle {
    //     anchors.fill: parent
    //     color: "#10FF0000"
    // }

    onCurrentItemChanged: {
        currentAssetType = (currentItem as CurrencyDelegate)?.asset.type || 0
    }

    delegate: CurrencyDelegate {
        opacity: PathView.opacity

        isCurrentItem: PathView.isCurrentItem

        imageSize: PathView.imageSize
        nameSize: PathView.nameSize
        balanceSize: PathView.balanceSize
        currencySize: PathView.currencySize
        fiatOpacity: PathView.fiatOpacity

        onClicked: {
            idPathView.currentIndex = index
        }
    }

    path: Path {
        startX: 0
        startY: idPathView.height / 2 + 20

        PathAttribute {
            name: "opacity"
            value: 0.0
        }

        PathAttribute {
            name: "imageSize"
            value: 50.0
        }

        PathAttribute {
            name: "nameSize"
            value: 17.0
        }

        PathAttribute {
            name: "balanceSize"
            value: 17.0
        }

        PathAttribute {
            name: "currencySize"
            value: 17.0
        }

        PathAttribute {
            name: "fiatOpacity"
            value: 0.0
        }

        PathLine {
            x: idPathView.width / 2 - 100
            y: idPathView.height / 2 + 20
        }

        PathAttribute {
            name: "imageSize"
            value: 50.0
        }

        PathAttribute {
            name: "nameSize"
            value: 17.0
        }

        PathAttribute {
            name: "balanceSize"
            value: 17.0
        }

        PathAttribute {
            name: "currencySize"
            value: 17.0
        }

        PathAttribute {
            name: "fiatOpacity"
            value: 0.0
        }

        PathAttribute {
            name: "opacity"
            value: 1.0
        }

        PathCurve {
            x: idPathView.width / 2
            y: idPathView.height / 2
        }

        PathAttribute {
            name: "imageSize"
            value: 90.0
        }

        PathAttribute {
            name: "nameSize"
            value: 32.0
        }

        PathAttribute {
            name: "balanceSize"
            value: 36.0
        }

        PathAttribute {
            name: "currencySize"
            value: 24.0
        }

        PathAttribute {
            name: "fiatOpacity"
            value: 1.0
        }

        PathCurve {
            x: idPathView.width / 2 + 100
            y: idPathView.height / 2 + 20
        }

        PathAttribute {
            name: "imageSize"
            value: 50.0
        }

        PathAttribute {
            name: "nameSize"
            value: 17.0
        }

        PathAttribute {
            name: "balanceSize"
            value: 17.0
        }

        PathAttribute {
            name: "currencySize"
            value: 17.0
        }

        PathAttribute {
            name: "fiatOpacity"
            value: 0.0
        }

        PathAttribute {
            name: "opacity"
            value: 1.0
        }

        PathLine {
            x: idPathView.width
            y: idPathView.height / 2 + 20
        }

        PathAttribute {
            name: "opacity"
            value: 0.0
        }
    }
}
