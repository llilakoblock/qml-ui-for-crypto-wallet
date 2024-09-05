import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.impl

import XWallet.Data
import XWallet.Natives

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Fonts
import XWallet.Style.Images

import XWallet.UI.Components

ItemDelegate {
    id: idRoot

    required property int index

    required property asset asset

    required property bool isCurrentItem
    required property bool isLastItem

    padding: 16

    AssetPresenter {
        id: idAssetPresenter

        asset: idRoot.asset
    }

    HoverHandler {
        cursorShape: Qt.PointingHandCursor
    }

    contentItem: RowLayout {
        spacing: 0

        ColumnLayout {
            spacing: 0

            VerticalSpacer {}

            RowLayout {
                spacing: Dimensions.Spacing16

                Image {
                    Layout.preferredWidth: 24
                    Layout.preferredHeight: 24

                    source: Images.iconForAssetType(idRoot.asset.type)

                    fillMode: Image.PreserveAspectFit
                }

                Label {
                    text: idRoot.asset.currency.name

                    font: Fonts.p2_regular_text
                    color: {
                        if(idRoot.isCurrentItem) {
                            return Colors.colorForAssetType(idRoot.asset.type)
                        }
                        else {
                            return Colors.textPrimary
                        }
                    }
                }
            }

            Item {
                Layout.minimumHeight: Dimensions.Spacing12
            }

            RowLayout {
                spacing: Dimensions.Spacing8

                Label {
                    text: idAssetPresenter.balance

                    font: Fonts.h4_text
                    color: Colors.textSecondary
                }

                Label {
                    text: idRoot.asset.currency.symbol

                    font: Fonts.h4_text
                    color: Colors.textTertiary
                }
            }

            VerticalSpacer {}
        }

        HorizontalSpacer {
            Layout.minimumWidth: Dimensions.Spacing24
        }

        ColumnLayout {
            spacing: Dimensions.Spacing12

            RowLayout {
                spacing: 0

                ColorImage {
                    source: idRoot.asset.difference > 0 ? Images.arrowRightUp : Images.arrowRightDown
                    color: idRoot.asset.difference > 0 ? Colors.iconPositive : Colors.iconNegative
                }

                Label {
                    text: ("%1%").arg(idRoot.asset.difference.toLocaleString(Qt.locale(), "f", 2))

                    font: Fonts.h4_text
                    color: idRoot.asset.difference > 0 ? Colors.textPositive : Colors.textNegative
                }
            }

            RowLayout {
                spacing: Dimensions.Spacing2
            }
        }
    }

    background: Item {
        implicitWidth: 270

        Rectangle {
            visible: !idRoot.isLastItem

            x: parent.width
            y: (parent.height - height) / 2

            width: 1
            height: 35

            color: Colors.grayscale400
        }
    }
}
