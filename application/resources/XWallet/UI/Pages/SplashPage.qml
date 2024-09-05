import QtQuick
import QtQuick.Layouts

import XWallet.Effects

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Fonts

Page {
    id: idRoot

    background: Item {}

    contentItem: ColumnLayout {
        spacing: 8

        Label {
            Layout.fillWidth: true
            Layout.fillHeight: true

            text: qsTr("XWallet")
            font: Fonts.promo1_wide

            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter

            layer.enabled: true
            layer.effect: LinearGradientEffect {
                gradient: Colors.promoGradient
                angle: 90
            }
        }
    }
}
