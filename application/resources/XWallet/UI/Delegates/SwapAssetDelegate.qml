import QtQuick
import QtQuick.Layouts

import XWallet.Data
import XWallet.Natives

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Fonts
import XWallet.Style.Images

import XWallet.UI.Components

ItemDelegate {
    id: idRoot

    required property int index
    required property asset asset

    AssetPresenter {
        id: idAssetPresenter

        asset: idRoot.asset
    }

    contentItem: RowLayout {
        spacing: Dimensions.Spacing24

        opacity: idRoot.enabled ? 1.0 : 0.5

        HoverHandler {
            cursorShape: Qt.PointingHandCursor

            margin: 50
        }

        Image {
            Layout.preferredWidth: 64
            Layout.preferredHeight: 64

            source: Images.iconForAssetType(idRoot.asset.type)
            fillMode: Image.PreserveAspectFit
        }

        ColumnLayout {
            spacing: Dimensions.Spacing2

            Label {
                text: idRoot.asset.currency.symbol
                font: Fonts.p1_regular_text
            }

            Label {
                text: idRoot.asset.currency.name
                font: Fonts.c1_medium_comp
                color: Colors.textSecondary
            }
        }

        HorizontalSpacer {}

        ColumnLayout {
            spacing: Dimensions.Spacing2

            Label {
                Layout.alignment: Qt.AlignRight

                text: ("%1 %2").arg(idAssetPresenter.balance).arg(idRoot.asset.currency.symbol)
                font: Fonts.p3_medium_text
            }

            Label {
                Layout.alignment: Qt.AlignRight

                text: ("$%1").arg(idAssetPresenter.fiatBalance)
                font: Fonts.c1_medium_comp
                color: Colors.textSecondary
            }
        }
    }

    background: Rectangle {
        radius: 24

        color: idRoot.highlighted ?
                   Color.transparent(Colors.grayscale000, 0.11) :
                   idRoot.hovered ?
                       Color.transparent(Colors.grayscale000, 0.05) :
                       Colors.transparent
    }
}
