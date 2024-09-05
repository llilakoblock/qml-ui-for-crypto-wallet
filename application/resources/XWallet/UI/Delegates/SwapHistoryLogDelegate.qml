import QtQuick
import QtQuick.Layouts

import XWallet.Style
import XWallet.Style.Fonts
import XWallet.Style.Controls

import XWallet.Data
import XWallet.UI.Components

ItemDelegate {
    id: control

    verticalPadding: 4
    horizontalPadding: 8

    required property swapLog swapLog

    contentItem: RowLayout {
        spacing: control.spacing

        Label {
            Layout.alignment: Qt.AlignTop

            text: control.swapLog.time.toLocaleString(Qt.locale(), Locale.ShortFormat)

            font: Fonts.p4_regular_comp
        }

        TextArea {
            Layout.fillWidth: true

            padding: 0
            readOnly: true

            text: control.swapLog.text

            font: Fonts.p4_medium_comp
            wrapMode: Text.Wrap

            color: {
                switch(control.swapLog.state) {
                case SwapLog.InProgressState:
                    return Colors.textPrimary
                case SwapLog.DoneState:
                    return Colors.textPositive
                case SwapLog.FailedState:
                    return Colors.textNegative
                default:
                    return Colors.textSecondary
                }
            }
        }
    }

    background: Item {}
}
