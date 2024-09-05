import QtQuick
import QtQuick.Controls.impl
import QtQuick.Layouts

import XWallet.Data

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Images
import XWallet.Style.Fonts

import XWallet.UI.Components

ItemDelegate {
    id: control

    clip: true
    horizontalPadding: 24
    verticalPadding: 20

    required property var model

    required property swap swap

    required property asset bidAsset
    required property asset askAsset

    required property string bidCoinAmount
    required property string bidFiatAmount

    required property string askCoinAmount
    required property string askFiatAmount

    required property bool expanded
    required property bool showLogs

    required property bool blink

    readonly property FontMetrics fontMetrics: FontMetrics { font: Fonts.h4_wide }

    readonly property real largestTitleTextWidth: fontMetrics.advanceWidth(qsTr("Processing..."))
    readonly property real largestTimeTextWidth: fontMetrics.advanceWidth(qsTr("12 months ago"))

    readonly property real leftEdgeCellWidth: largestTitleTextWidth + idLeftCellLayout.spacing + 24
    readonly property real rightEdgeCellWidth: largestTimeTextWidth + idLeftCellLayout.spacing + 24

    property date currentTime: new Date()

    signal cancelOrder(orderId : string)

    Behavior on implicitHeight {
        NumberAnimation { duration: Animations.shortDuration }
    }

    function timeSince(fromDate : date, toDate : date) : string {
        const intervals = [
            { label: 'year', seconds: 31536000 },
            { label: 'month', seconds: 2592000 },
            { label: 'day', seconds: 86400 },
            { label: 'hour', seconds: 3600 },
            { label: 'min', seconds: 60 },
            { label: 'sec', seconds: 1 }
        ]

        const seconds = Math.floor((fromDate.getTime() - toDate.getTime()) / 1000) + 1
        const interval = intervals.find(i => i.seconds < seconds)
        const count = Math.floor(seconds / interval.seconds)

        return ("%1 %2%3 ago").arg(count).arg(interval.label).arg(count !== 1 ? "s" : "")
    }

    Timer {
        running: true
        repeat: true
        interval: 1000

        onTriggered: {
            control.currentTime = new Date()
        }
    }

    contentItem: ColumnLayout {
        spacing: 0

        RowLayout {
            id: idHeaderLayout

            Layout.fillWidth: true

            spacing: Dimensions.Spacing24

            HoverHandler {
                cursorShape: Qt.PointingHandCursor

                margin: 20
            }

            TapHandler {
                cursorShape: Qt.PointingHandCursor

                margin: 20

                onTapped: {
                    control.model.expanded = !control.model.expanded
                }
            }

            RowLayout {
                id: idLeftCellLayout

                Layout.minimumWidth: control.leftEdgeCellWidth
                Layout.maximumWidth: control.leftEdgeCellWidth

                spacing: Dimensions.Spacing8

                opacity: !control.expanded &&
                         control.blink &&
                         control.swap.state !== Swap.ErrorState &&
                         control.swap.state !== Swap.FinishedState &&
                         control.swap.state !== Swap.DeclinedState ?
                             0.2 :
                             1

                Behavior on opacity {
                    NumberAnimation { duration: 750 }
                }

                ColorImage {
                    Layout.preferredWidth: 24
                    Layout.preferredHeight: 24

                    source: Images.swap

                    color: Colors.grayscale000
                }

                Label {
                    Layout.fillWidth: true

                    text: {
                        switch(control.swap.state) {
                        case Swap.ErrorState:
                            return qsTr("Error")
                        case Swap.FinishedState:
                            return qsTr("Swapped")
                        case Swap.DeclinedState:
                            return qsTr("Declined")
                        default:
                            return qsTr("Processing...")
                        }
                    }

                    font: Fonts.h4_wide

                    horizontalAlignment: Text.AlignLeft
                }
            }

            Rectangle {
                Layout.preferredWidth: 1
                Layout.preferredHeight: 30

                color: Colors.backgroundStroke
            }

            RowLayout {
                id: idValuesLayout

                Layout.fillWidth: true

                spacing: 0

                readonly property real cellWidth: (width - idArrowImage.width - Dimensions.Spacing24 * 2) / 2

                OrderValues {
                    Layout.minimumWidth: idValuesLayout.cellWidth
                    Layout.maximumWidth: idValuesLayout.cellWidth

                    order: control.swap.myOrder
                    asset: control.bidAsset

                    coinAmount: control.bidCoinAmount
                    fiatAmount: control.bidFiatAmount

                    alignment: Text.AlignRight
                }

                HorizontalSpacer {
                    Layout.minimumWidth: Dimensions.Spacing24
                }

                ColorImage {
                    id: idArrowImage

                    Layout.preferredWidth: 24
                    Layout.preferredHeight: 24

                    source: Images.arrowRight
                    color: Colors.grayscale000
                }

                HorizontalSpacer {
                    Layout.minimumWidth: Dimensions.Spacing24
                }

                OrderValues {
                    Layout.minimumWidth: idValuesLayout.cellWidth
                    Layout.maximumWidth: idValuesLayout.cellWidth

                    order: control.swap.otherOrder
                    asset: control.askAsset

                    coinAmount: control.askCoinAmount
                    fiatAmount: control.askFiatAmount

                    alignment: Text.AlignLeft
                }
            }

            Rectangle {
                Layout.preferredWidth: 1
                Layout.preferredHeight: 30

                color: Colors.backgroundStroke
            }

            RowLayout {
                Layout.minimumWidth: control.rightEdgeCellWidth
                Layout.maximumWidth: control.rightEdgeCellWidth

                spacing: 0

                HorizontalSpacer {}

                ToolButton {
                    visible: control.swap.state === Swap.EmptyState

                    text: qsTr("Cancel")
                    font: Fonts.h4_wide_underline

                    onClicked: {
                        control.cancelOrder(control.swap.myOrder.id)
                    }
                }

                Label {
                    visible: control.swap.state !== Swap.EmptyState

                    text: control.swap.state !== Swap.EmptyState ?
                              control.timeSince(control.currentTime, control.swap.timestamp) :
                              ""
                    font: Fonts.h4_wide
                }

                HorizontalSpacer {}

                ToolButton {
                    Layout.leftMargin: Dimensions.Spacing8

                    icon.source: control.expanded ? Images.chevronUp : Images.chevronDown

                    onClicked: control.model.expanded = !control.model.expanded
                }
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.topMargin: Dimensions.Spacing32
            Layout.leftMargin: control.leftEdgeCellWidth + idHeaderLayout.spacing

            visible: control.expanded

            spacing: Dimensions.Spacing32

            ColumnLayout {
                spacing: 0

                SwapStepDescriber {
                    id: idMatchingOrdersDescriber

                    title: qsTr("Matching orders")
                    blink: control.blink

                    state: {
                        if(control.swap.state === Swap.EmptyState) {
                            return SwapStepDescriber.State.InProgress
                        }
                        else {
                            return SwapStepDescriber.State.Done
                        }
                    }

                    prevState: SwapStepDescriber.State.Unknown
                    nextState: idFundingDescriber.state
                }

                SwapStepDescriber {
                    id: idFundingDescriber

                    title: qsTr("Funding")
                    blink: control.blink

                    state: {
                        if(control.swap.state >= Swap.FundSucceedState) {
                            return SwapStepDescriber.State.Done
                        }
                        else if(control.swap.state >= Swap.ReadyForFundState && control.swap.state <= Swap.FundingState) {
                            return SwapStepDescriber.State.InProgress
                        }
                        else if(control.swap.state === Swap.FundFailedState) {
                            return SwapStepDescriber.State.Failed
                        }
                        else {
                            return SwapStepDescriber.State.NotStarted
                        }
                    }

                    prevState: idMatchingOrdersDescriber.state
                    nextState: idRedeemingDescriber.state
                }

                SwapStepDescriber {
                    id: idRedeemingDescriber

                    title: qsTr("Redeeming")
                    blink: control.blink

                    state: {
                        if(control.swap.state >= Swap.RedeemSucceedState) {
                            return SwapStepDescriber.State.Done
                        }
                        else if(control.swap.state >= Swap.ReadyForRedeemState && control.swap.state <= Swap.RedeemingState) {
                            return SwapStepDescriber.State.InProgress
                        }
                        else if(control.swap.state === Swap.RedeemFailedState) {
                            return SwapStepDescriber.State.Failed
                        }
                        else {
                            return SwapStepDescriber.State.NotStarted
                        }
                    }

                    prevState: idFundingDescriber.state
                    nextState: idRefundingDescriber.visible ? idRefundingDescriber.state : idFinalizingDescriber.state
                }

                SwapStepDescriber {
                    id: idRefundingDescriber

                    visible: (control.swap.state >= Swap.ReadyForRefundState &&
                              control.swap.state <= Swap.RefundSucceedState) ||
                             control.swap.state === Swap.DeclinedState

                    title: qsTr("Refunding")
                    blink: control.blink

                    state: {
                        if(control.swap.state >= Swap.RefundSucceedState) {
                            return SwapStepDescriber.State.Done
                        }
                        else if(control.swap.state >= Swap.ReadyForRefundState && control.swap.state <= Swap.RefundingState) {
                            return SwapStepDescriber.State.InProgress
                        }
                        else if(control.swap.state === Swap.RefundFailedState) {
                            return SwapStepDescriber.State.Failed
                        }
                        else {
                            return SwapStepDescriber.State.NotStarted
                        }
                    }

                    prevState: idRedeemingDescriber.state
                    nextState: idFinalizingDescriber.state
                }

                SwapStepDescriber {
                    id: idFinalizingDescriber

                    title: control.swap.state === Swap.DeclinedState ?
                               qsTr("Declining") :
                               qsTr("Finalazing")
                    blink: control.blink

                    state: {
                        if(control.swap.state === Swap.FinishedState || control.swap.state === Swap.DeclinedState) {
                            return SwapStepDescriber.State.Done
                        }
                        else {
                            return SwapStepDescriber.State.NotStarted
                        }
                    }

                    prevState: idRefundingDescriber.visible ? idRefundingDescriber.state : idRedeemingDescriber.state
                    nextState: SwapStepDescriber.State.Unknown
                }
            }
        }

        ColumnLayout {
            visible: control.expanded && control.swap.logs.length > 0

            spacing: Dimensions.Spacing16

            Behavior on implicitHeight {
                NumberAnimation { duration: 100 }
            }

            ToolButton {
                text: control.showLogs ? qsTr("Hide logs") : qsTr("Show logs")

                icon.source: control.showLogs ? Images.chevronUp : Images.chevronDown

                onClicked: control.model.showLogs = !control.model.showLogs
            }

            LogsView {
                Layout.fillWidth: true

                visible: control.showLogs

                model: control.swap.logs
            }
        }

        VerticalSpacer {}
    }

    background: Rectangle {
        color: Color.transparent(Colors.grayscale000, 0.05)
        radius: 18
    }

    component OrderValues : ColumnLayout {
        id: idOrderValues

        spacing: Dimensions.Spacing4

        required property order order
        required property asset asset

        required property string coinAmount
        required property string fiatAmount

        required property int alignment

        // Label {
        //     text: idOrderValues.asset.currency.name

        //     font: Fonts.p4_medium_comp

        //     color: Colors.textSecondary
        // }

        RowLayout {
            Layout.fillWidth: true

            spacing: Dimensions.Spacing4

            HorizontalSpacer {
                visible: idOrderValues.alignment === Text.AlignRight
            }

            Label {
                Layout.fillWidth: true
                Layout.maximumWidth: implicitWidth + 1

                text: idOrderValues.coinAmount

                font: Fonts.p3_medium_comp

                horizontalAlignment: idOrderValues.alignment
                elide: Text.ElideRight
            }

            Label {
                text: idOrderValues.asset.currency.symbol

                font: Fonts.p3_medium_comp
                color: Colors.textSecondary

                horizontalAlignment: idOrderValues.alignment
            }

            HorizontalSpacer {
                visible: idOrderValues.alignment === Text.AlignLeft
            }
        }

        Label {
            Layout.fillWidth: true

            text: ("$%1").arg(idOrderValues.fiatAmount)

            font: Fonts.p4_medium_comp
            color: Colors.textSecondary

            horizontalAlignment: idOrderValues.alignment
        }
    }

    component LogsView : Control {
        property alias model: idLogsViewRepeater.model

        padding: 12

        contentItem: ColumnLayout {
            spacing: Dimensions.Spacing4

            Repeater {
                id: idLogsViewRepeater

                delegate: SwapHistoryLogDelegate {
                    Layout.fillWidth: true

                    spacing: Dimensions.Spacing16

                    swapLog: modelData

                    required property swapLog modelData
                }
            }
        }

        background: Rectangle {
            color: Colors.backgroundOverlay
            radius: 8
        }
    }
}
