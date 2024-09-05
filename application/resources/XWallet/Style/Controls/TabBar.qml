import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

import XWallet.Style

T.TabBar {
    id: control

    focusPolicy: Qt.StrongFocus

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    spacing: Dimensions.Spacing8

    horizontalPadding: Dimensions.Spacing4
    verticalPadding: Dimensions.Spacing4

    contentItem: ListView {
        model: control.contentModel
        currentIndex: control.currentIndex

        spacing: control.spacing
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.AutoFlickIfNeeded
        snapMode: ListView.SnapToItem

        highlightMoveDuration: Animations.shortDuration
        highlightRangeMode: ListView.ApplyRange
        preferredHighlightBegin: 40
        preferredHighlightEnd: width - 40

        highlight: Rectangle {
            color: Color.transparent(Colors.grayscale000, 0.17)
            radius: Dimensions.Spacing16
        }
    }

    background: Rectangle {
        //implicitHeight: 56

        color: Color.transparent(Colors.grayscale000, 0.05)
        radius: 18
    }
}
