import QtQuick
import QtQuick.Controls.impl
import QtQuick.Layouts

import XWallet.Data
import XWallet.Natives

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

    required property asset asset
    required property historyEntry historyEntry

    required property bool expanded

    readonly property FontMetrics fontMetrics: FontMetrics { font: Fonts.h4_wide }
    property date currentTime: new Date()

    signal noteChanged(note : string)

    Behavior on implicitHeight {
        NumberAnimation { duration: Animations.shortDuration }
    }

    function timeSince(fromDate : date, toDate : date) : string {
        const intervals = [
            { label: 'year', seconds: 31536000 },
            { label: 'month', seconds: 2592000 },
            { label: 'day', seconds: 86400 },
            { label: 'hour', seconds: 3600 },
            { label: 'minute', seconds: 60 },
            { label: 'second', seconds: 1 }
        ]

        const seconds = Math.floor((fromDate.getTime() - toDate.getTime()) / 1000) + 1
        const interval = intervals.find(i => i.seconds < seconds)
        const count = Math.floor(seconds / interval.seconds)

        return ("%1 %2%3 ago").arg(count).arg(interval.label).arg(count !== 1 ? "s" : "")
    }

    AssetPresenter {
        id: idAssetPresenter

        asset: control.asset
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
            spacing: Dimensions.Spacing32

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
                spacing: Dimensions.Spacing8

                ColorImage {
                    Layout.preferredWidth: 24
                    Layout.preferredHeight: 24

                    source: {
                        switch(control.historyEntry.direction) {
                        case HistoryEntry.Income:
                            return Images.arrowDown
                        case HistoryEntry.Outgo:
                            return Images.arrowUp
                         default:
                            return ""
                        }
                    }

                    color: Colors.grayscale000
                }

                Label {
                    Layout.fillWidth: true
                    Layout.maximumWidth: control.fontMetrics.advanceWidth(qsTr("Received"))

                    text: {
                        switch(control.historyEntry.direction) {
                        case HistoryEntry.Income:
                            return qsTr("Received")
                        case HistoryEntry.Outgo:
                            return qsTr("Sent")
                         default:
                            return ""
                        }
                    }

                    font: Fonts.h4_wide

                    horizontalAlignment: Text.AlignLeft
                }
            }

            Rectangle {
                Layout.preferredHeight: 30
                Layout.preferredWidth: 1

                color: Colors.backgroundStroke
            }

            RowLayout {
                spacing: Dimensions.Spacing8

                Label {
                    text: ("%1 %2").arg(control.historyEntry.amount).arg(control.asset.currency.symbol)

                    font: Fonts.p3_medium_comp
                }

                Label {
                    text: ("$%1").arg(control.asset.fiatAmountFromCoin(control.historyEntry.amount))

                    font: Fonts.p4_medium_comp

                    color: Colors.textSecondary
                }
            }

            HorizontalSpacer {}

            Label {
                text: control.historyEntry.success ?
                          control.timeSince(control.currentTime, control.historyEntry.timestamp) :
                          qsTr("Pending")
            }

            ToolButton {
                icon.source: control.expanded ? Images.chevronUp : Images.chevronDown

                onClicked: control.model.expanded = !control.model.expanded
            }
        }

        VerticalSpacer {
            Layout.minimumHeight: control.expanded ? Dimensions.Spacing32 : -1
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.leftMargin: control.fontMetrics.advanceWidth(qsTr("Received")) + 24 + 32 + 8

            visible: control.expanded

            spacing: Dimensions.Spacing32

            RowLayout {
                Layout.fillWidth: true

                spacing: Dimensions.Spacing32

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.horizontalStretchFactor: 1

                    spacing: Dimensions.Spacing24

                    ContentLabel {
                        description: qsTr("Date:")
                        value: control.historyEntry.timestamp.toLocaleString(Qt.locale(), Locale.ShortFormat)
                    }

                    ContentLabel {
                        description: qsTr("Now:")
                        value: ("$%1").arg(control.asset.fiatAmountFromCoin(control.historyEntry.amount))
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.maximumWidth: Dimensions.columnsWidth(3, control.width)
                    Layout.horizontalStretchFactor: 2

                    spacing: Dimensions.Spacing24

                    ContentLabelLink {
                        description: qsTr("Transaction ID:")
                        value: control.historyEntry.hash
                    }

                    ContentLabel {
                        description: ("%1:").arg(Qt.formatDate(control.historyEntry.timestamp, "MMM dd, yyyy"))
                        value: ("$%1").arg(control.asset.fiatAmountFromCoin(control.historyEntry.amount))
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.maximumWidth: Dimensions.columnsWidth(3, control.width)
                    Layout.horizontalStretchFactor: 2

                    spacing: Dimensions.Spacing24

                    ContentLabelLink {
                        description: {
                            switch(control.historyEntry.direction) {
                                case HistoryEntry.Income:
                                    return qsTr("From:")
                                case HistoryEntry.Outgo:
                                    return qsTr("To:")
                            }
                        }
                        value: {
                            switch(control.historyEntry.direction) {
                                case HistoryEntry.Income:
                                    return control.historyEntry.from[0]
                                case HistoryEntry.Outgo:
                                    return control.historyEntry.to[0]
                            }
                        }
                    }

                    VerticalSpacer {
                        visible: control.historyEntry.direction === HistoryEntry.Income
                    }

                    ContentLabel {
                        visible: control.historyEntry.direction === HistoryEntry.Outgo
                        description: qsTr("Fee amount:")
                        value: ("%1 %2").arg(control.historyEntry.fee).arg(idAssetPresenter.parentAssetExists ?
                                                                               idAssetPresenter.parentAsset.currency.symbol :
                                                                               idAssetPresenter.asset.currency.symbol)
                    }
                }
            }

            TextField {
                Layout.fillWidth: true

                text: control.historyEntry.note
                placeholderText: qsTr("Personal note")

                onTextEdited: {
                    control.noteChanged(text)
                }
            }
        }
    }

    background: Rectangle {
        color: Color.transparent(Colors.grayscale000, 0.05)
        radius: 18
    }

    component ContentLabel : ColumnLayout {
        id: idContentLabel

        spacing: Dimensions.Spacing2

        required property string description
        required property string value

        Label {
            Layout.fillWidth: true

            text: idContentLabel.description

            font: Fonts.c1_medium_comp
            color: Colors.textSecondary

            horizontalAlignment: Text.AlignLeft
        }

        Label {
            Layout.fillWidth: true

            text: idContentLabel.value

            font: Fonts.p3_medium_text
            elide: Text.ElideRight

            horizontalAlignment: Text.AlignLeft
        }
    }

    component ContentLabelLink : ColumnLayout {
        id: idContentLabelLink

        spacing: Dimensions.Spacing2

        required property string description
        required property string value

        Label {
            Layout.fillWidth: true

            text: idContentLabelLink.description

            font: Fonts.c1_medium_comp
            color: Colors.textSecondary

            horizontalAlignment: Text.AlignLeft
        }

        RowLayout {
            spacing: Dimensions.Spacing8

            Label {
                Layout.fillWidth: true

                text: idContentLabelLink.value

                font: Fonts.p3_medium_text
                color: Colors.textVisitedLink
                elide: Text.ElideRight

                horizontalAlignment: Text.AlignLeft
            }

            ToolButton {
                icon.source: Images.copy
                padding: 0

                TextEdit {
                    id: idHelperTextEdit

                    visible: false
                    text: idContentLabelLink.value
                }

                background: Item {
                    implicitWidth: 24
                    implicitHeight: 24
                }

                onClicked: {
                    idHelperTextEdit.selectAll()
                    idHelperTextEdit.copy()
                    idHelperTextEdit.deselect()
                }
            }
        }
    }
}
