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

    required property bool isCurrentItem
    required property real imageSize
    required property real nameSize
    required property real balanceSize
    required property real currencySize
    required property real fiatOpacity

    AssetPresenter {
        id: idAssetPresenter

        asset: idRoot.asset
    }

    contentItem: ColumnLayout {
        spacing: Dimensions.Spacing4

        Image {
            Layout.preferredWidth: idRoot.imageSize
            Layout.preferredHeight: idRoot.imageSize
            Layout.alignment: Qt.AlignHCenter

            source: Images.iconForAssetType(idRoot.asset.type)

            fillMode: Image.PreserveAspectFit
        }

        Label {
            Layout.alignment: Qt.AlignHCenter

            text: idRoot.asset.currency.name

            font.family: Fonts.sfCompactDisplayMedium.font.family
            font.pointSize: idRoot.nameSize

            wrapMode: Text.WordWrap

            color: Colors.textPrimary
        }

        Label {
            Layout.alignment: Qt.AlignHCenter

            text: ("%1 %2").arg(idAssetPresenter.balance).arg(idRoot.asset.currency.symbol)

            font.family: Fonts.sfCompactDisplayMedium.font.family
            font.pointSize: idRoot.balanceSize

            color: idRoot.isCurrentItem ? Colors.textPrimary : Colors.textSecondary
        }

        // RowLayout {
        //     Layout.alignment: Qt.AlignHCenter

        //     spacing: Dimensions.Spacing4

        //     Label {
        //         Layout.alignment: Qt.AlignHCenter

        //         text: idRoot.asset.balanceString

        //         font.family: Fonts.sfCompactDisplayMedium.font.family
        //         font.pointSize: idRoot.balanceSize

        //         color: idRoot.isCurrentItem ? Colors.textPrimary : Colors.textSecondary
        //     }

        //     Label {
        //         Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

        //         text: idRoot.asset.currency.symbol

        //         font.family: Fonts.sfCompactDisplayRegular.font.family
        //         font.pointSize: idRoot.currencySize

        //         color: idRoot.isCurrentItem ? Colors.textPrimary : Colors.textSecondary
        //     }
        // }

        Label {
            Layout.alignment: Qt.AlignHCenter

            opacity: idRoot.fiatOpacity

            text: qsTr("$ %1").arg(idAssetPresenter.fiatBalance)

            font: Fonts.h3_comp

            color: Colors.textSecondary
        }

        VerticalSpacer {}
    }

    background: Item {
        implicitWidth: 250
        implicitHeight: 250
    }
}
