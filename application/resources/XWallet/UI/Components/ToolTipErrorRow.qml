import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.impl

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Fonts
import XWallet.Style.Images

RowLayout {
    id: idRoot

    spacing: Dimensions.Spacing12

    required property bool error
    required property string text

    ColorImage {
        Layout.preferredWidth: 24
        Layout.preferredHeight: 24

        source: idRoot.error ? Images.minus : Images.check
        color: idRoot.error ? Colors.grayscale500 : Colors.grayscale000

        fillMode: Image.PreserveAspectFit
    }

    Label {
        text: idRoot.text
        font: Fonts.p1_regular_text

        horizontalAlignment: Qt.AlignLeft
    }
}
