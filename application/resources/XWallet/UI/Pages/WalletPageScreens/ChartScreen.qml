import QtQuick
import QtQuick.Layouts

import XWallet.Natives
import XWallet.Data

import XWallet.Style
import XWallet.Style.Images
import XWallet.Style.Fonts
import XWallet.Style.Controls

import XWallet.UI.Components

BaseScreen {
    id: idRoot

    signal sendClicked()
    signal receiveClicked()
    signal swapClicked()

    AssetPresenter {
        id: idAssetPresenter

        asset: idRoot.viewModel.currentAsset
    }

    Connections {
        target: idRoot.ListView.view

        function onContentYChanged(y) {
            idContentPane.updateBackground()
        }
    }

    contentItem: ColumnLayout {
        spacing: Dimensions.Spacing32

        ColumnLayout {
            Layout.alignment: Qt.AlignHCenter

            spacing: Dimensions.Spacing16

            Image {
                Layout.preferredWidth: 56
                Layout.preferredHeight: 56
                Layout.alignment: Qt.AlignHCenter

                source: Images.iconForAssetType(idRoot.viewModel.currentAsset.type)
            }

            ColumnLayout {
                Layout.alignment: Qt.AlignHCenter

                spacing: Dimensions.Spacing4

                Label {
                    Layout.alignment: Qt.AlignHCenter

                    text: ("%1 %2").arg(idAssetPresenter.balance).arg(idRoot.viewModel.currentAsset.currency.symbol)

                    font: Fonts.h3_wide
                    color: Colors.colorForAssetType(idRoot.viewModel.currentAsset.type)
                }

                Label {
                    Layout.alignment: Qt.AlignHCenter

                    text: ("$%1").arg(idAssetPresenter.fiatBalance)

                    font: Fonts.p1_regular_comp
                    color: Colors.textSecondary
                }
            }
        }

        RowLayout {
            Layout.maximumWidth: Dimensions.columnsWidth(8, idRoot.width)
            Layout.alignment: Qt.AlignCenter

            spacing: 16

            Button {
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1

                text: qsTr("Send")

                icon.source: Images.arrowUp

                appearance: Button.Appearance.Tertiary
                size: Button.Size.Medium

                onClicked: {
                    idRoot.sendClicked()
                }
            }

            Button {
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1

                text: qsTr("Receive")

                icon.source: Images.arrowDown

                appearance: Button.Appearance.Tertiary
                size: Button.Size.Medium

                onClicked: {
                    idRoot.receiveClicked()
                }
            }

            Button {
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1

                text: qsTr("Swap")

                icon.source: Images.swap

                appearance: Button.Appearance.Tertiary
                size: Button.Size.Medium

                onClicked: {
                    idRoot.swapClicked()
                }
            }
        }

        Label {
            text: qsTr("PRICE CHART")

            font: Fonts.p2_regular_comp
        }

        Pane {
            id: idContentPane

            Layout.fillWidth: true
            Layout.fillHeight: true

            contentItem: ColumnLayout {
                spacing: Dimensions.Spacing24

                RowLayout {
                    spacing: Dimensions.Spacing12

                    Label {
                        text: qsTr("$%1").arg(idAssetPresenter.fiatPrice)

                        font: Fonts.p1_regular_text
                    }

                    Label {
                        Layout.alignment: Qt.AlignBaseline

                        text: qsTr("1 %1").arg(idRoot.viewModel.currentAsset.currency.symbol)

                        font: Fonts.p3_regular_text

                        color: Colors.textSecondary
                    }

                    HorizontalSpacer {}

                    TabBar {
                        Layout.alignment: Qt.AlignRight

                        currentIndex: 0

                        ChartTabButton {
                            text: qsTr("1D")

                            onClicked: idLineChart.period = LineChartViewModel.DayPeriod
                        }

                        ChartTabButton {
                            text: qsTr("1W")

                            onClicked: idLineChart.period = LineChartViewModel.WeekPeriod
                        }

                        ChartTabButton {
                            text: qsTr("1M")

                            onClicked: idLineChart.period = LineChartViewModel.MonthPeriod
                        }

                        ChartTabButton {
                            text: qsTr("3M")

                            onClicked: idLineChart.period = LineChartViewModel.ThreeMonthsPeriod
                        }

                        ChartTabButton {
                            text: qsTr("6M")

                            onClicked: idLineChart.period = LineChartViewModel.SixMonthsPeriod
                        }

                        ChartTabButton {
                            text: qsTr("1Y")

                            onClicked: idLineChart.period = LineChartViewModel.YearPeriod
                        }
                    }
                }

                LineChart {
                    id: idLineChart

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    visible: state === LineChartViewModel.CompletedState

                    period: LineChartViewModel.DayPeriod
                    assetType: idRoot.viewModel.currentAsset.type
                }

                Label {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    visible: idLineChart.state === LineChartViewModel.LoadingState

                    text: qsTr("Loading chart data...")

                    font: Fonts.p1_regular_text

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

    component ChartTabButton : TabButton {
        horizontalPadding: 12
        verticalPadding: 6

        width: implicitWidth

        font: Fonts.p4_medium_comp

        color: TabBar.tabBar.currentIndex === TabBar.index ?
                   Colors.textPrimary :
                   Colors.textSecondary
    }
}
