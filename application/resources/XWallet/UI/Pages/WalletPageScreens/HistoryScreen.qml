import QtQuick
import QtQuick.Layouts

import XWallet.Natives
import XWallet.Data

import XWallet.Style
import XWallet.Style.Images
import XWallet.Style.Fonts
import XWallet.Style.Controls

import XWallet.UI.Components
import XWallet.UI.Delegates

BaseScreen {
    id: idRoot

    readonly property bool isMoving: ListView.view?.moving || false

    HistoryViewModel {
        id: idHistoryViewModel

        selectedAssetType: idRoot.viewModel.currentAsset.type
    }

    Connections {
        target: idRoot.ListView.view

        function onContentYChanged(y) {
            idContentPane.updateBackground()
        }
    }

    contentItem: ColumnLayout {
        spacing: Dimensions.Spacing32

        Label {
            text: qsTr("HISTORY")

            font: Fonts.p2_regular_comp
        }

        Pane {
            id: idContentPane

            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollBar {
                id: idHistoryListScrollBar

                parent: idContentPane

                x: idContentPane.width - (idContentPane.horizontalPadding - width) / 2
                y: idContentPane.verticalPadding + idListControls.height + 12

                height: idContentPane.height - idContentPane.verticalPadding * 2 - idListControls.height - 12
            }

            contentItem: ColumnLayout {
                spacing: Dimensions.Spacing12

                RowLayout {
                    id: idListControls

                    spacing: Dimensions.Spacing8

                    TabBar {
                        currentIndex: idHistoryViewModel.filteringKind

                        HistoryTabButton {
                            text: qsTr("All")

                            onClicked: {
                                idHistoryViewModel.filteringKind = HistoryViewModel.AllKind
                            }
                        }

                        HistoryTabButton {
                            text: qsTr("Sent")

                            onClicked: {
                                idHistoryViewModel.filteringKind = HistoryViewModel.SentKind
                            }
                        }

                        HistoryTabButton {
                            text: qsTr("Received")

                            onClicked: {
                                idHistoryViewModel.filteringKind = HistoryViewModel.ReceivedKind
                            }
                        }
                    }

                    HorizontalSpacer {}

                    TabBar {
                        currentIndex: idHistoryViewModel.filteringDate

                        HistoryTabButton {
                            text: qsTr("All")

                            onClicked: {
                                idHistoryViewModel.filteringDate = HistoryViewModel.AllDate
                            }
                        }

                        HistoryTabButton {
                            text: qsTr("Today")

                            onClicked: {
                                idHistoryViewModel.filteringDate = HistoryViewModel.TodayDate
                            }
                        }
                    }
                }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    visible: idHistoryViewModel.historyModel.length > 0

                    interactive: !idRoot.isMoving
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds

                    spacing: Dimensions.Spacing16

                    model: idHistoryViewModel.historyModel

                    delegate: HistoryDelegate {
                        id: idHistoryDelegate

                        width: ListView.view.width

                        asset: idRoot.viewModel.currentAsset

                        onNoteChanged: (note) => {
                            idHistoryViewModel.leaveNote(idHistoryDelegate.historyEntry.hash, note)
                        }
                    }

                    section.property: "date"
                    section.criteria: ViewSection.FullString
                    section.delegate: ColumnLayout {
                        spacing: 0

                        required property string section

                        Label {
                            leftPadding: 24
                            rightPadding: 24
                            topPadding: 24
                            bottomPadding: 8

                            text: parent.section
                            font: Fonts.p2_regular_text
                            color: Colors.textSecondary
                        }
                    }

                    ScrollBar.vertical: idHistoryListScrollBar
                }

                Label {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    visible: idHistoryViewModel.historyModel.length === 0

                    text: qsTr("There is no history yet...")

                    font: Fonts.p1_regular_text

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

    component HistoryTabButton : TabButton {
        horizontalPadding: 12
        verticalPadding: 6

        width: implicitWidth

        font: Fonts.p4_medium_comp

        color: TabBar.tabBar.currentIndex === TabBar.index ?
                   Colors.textPrimary :
                   Colors.textSecondary
    }
}
