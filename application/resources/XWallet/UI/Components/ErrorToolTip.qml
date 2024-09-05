import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.impl

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Fonts
import XWallet.Style.Images

ToolTip {
    id: idRoot

    required property ObjectModel model

    contentItem: ColumnLayout {
        spacing: 0

        Repeater {
            model: idRoot.model
        }
    }

    component CheckRow : RowLayout {
        id: idCheckRow

        spacing: Dimensions.Spacing12

        required property bool trigger
        required property string text

        ColorImage {
            Layout.preferredWidth: 24
            Layout.preferredHeight: 24

            source: idCheckRow.trigger ? Images.minus : Images.check
            color: idCheckRow.trigger ? Colors.grayscale500 : Colors.grayscale000

            fillMode: Image.PreserveAspectFit
        }

        Label {
            text: idCheckRow.text
            font: Fonts.p1_regular_text

            horizontalAlignment: Qt.AlignLeft
        }
    }

    component ErrorRow : RowLayout {
        id: idErrorkRow

        visible: trigger

        spacing: Dimensions.Spacing12

        required property bool trigger
        required property string text

        ColorImage {
            Layout.preferredWidth: 24
            Layout.preferredHeight: 24

            source: Images.minus
            color: Colors.textNegative

            fillMode: Image.PreserveAspectFit
        }

        Label {
            text: idErrorkRow.text
            font: Fonts.p1_regular_text

            horizontalAlignment: Qt.AlignLeft
        }
    }
}
