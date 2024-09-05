import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

import XWallet.Style

T.Drawer {
    id: control

    parent: T.Overlay.overlay

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    horizontalPadding: Dimensions.horizontalPadding
    verticalPadding: Dimensions.Spacing48

    edge: Qt.TopEdge

    enter: Transition { SmoothedAnimation { velocity: 3 } }
    exit: Transition { SmoothedAnimation { velocity: 3 } }

    background: Item {}

    T.Overlay.modal: Item {}

    T.Overlay.modeless: Item {}

    onAboutToShow: {
        ApplicationWindow.contentItem.opacity = 0.0
    }

    onAboutToHide: {
        ApplicationWindow.contentItem.opacity = 1.0
    }
}
