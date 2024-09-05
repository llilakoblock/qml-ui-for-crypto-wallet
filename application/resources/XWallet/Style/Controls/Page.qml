import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

import XWallet.Style

T.Page {
    id: control

    focusPolicy: Qt.StrongFocus

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding,
                            implicitHeaderWidth,
                            implicitFooterWidth)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding
                             + (implicitHeaderHeight > 0 ? implicitHeaderHeight + spacing : 0)
                             + (implicitFooterHeight > 0 ? implicitFooterHeight + spacing : 0))

    horizontalPadding: Dimensions.horizontalPadding

    Behavior on horizontalPadding {
        NumberAnimation { easing.type: Animations.easingType; duration: Animations.mediumDuration }
    }

    background: Rectangle {
        color: control.palette.window
    }
}
