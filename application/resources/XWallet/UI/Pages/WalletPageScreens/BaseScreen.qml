import QtQuick

import XWallet.Style.Controls
import XWallet.Natives
import XWallet.Data

import XWallet.UI.Components

Control {
    id: idRoot

    required property WalletViewModel viewModel

    signal contentYChanged()

    background: Item {}
}
