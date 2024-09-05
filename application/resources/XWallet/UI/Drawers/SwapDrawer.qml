import QtQuick
import QtQuick.Layouts

import XWallet.Natives
import XWallet.Data
import XWallet.Effects

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Images
import XWallet.Style.Fonts

import XWallet.UI.Components
import XWallet.UI.Delegates

Drawer {
    id: idRoot

    width: Overlay.overlay.width
    height: Overlay.overlay.height

    interactive: false
    closePolicy: Popup.NoAutoClose

    property alias bidAsset: idSwapViewModel.bidAsset
    property alias askAsset: idSwapViewModel.askAsset

    onYChanged: {
        idSplitInputsPane.updateBackground()
        idSearchPane.updateBackground()
        idSlippagePane.updateBackground()
        idHistoryPane.updateBackground()
    }

    AssetPresenter {
        id: idBidAssetPresenter

        asset: idRoot.bidAsset
    }

    AssetPresenter {
        id: idAskAssetPresenter

        asset: idRoot.askAsset
    }

    SwapViewModel {
        id: idSwapViewModel

        property int previousStatus: SwapViewModel.IdleStatus

        onStatusChanged: {
            if(status === SwapViewModel.IdleStatus && previousStatus === SwapViewModel.CreatingOrderStatus) {
                idContentSwipeView.setCurrentIndex(1)
            }

            previousStatus = status
        }
    }

    SwapHistoryViewModel {
        id: idSwapHistoryViewModel
    }

    Timer {
        id: idBlinkingTimer

        property bool blink: true

        interval: 750
        running: true
        repeat: true

        onTriggered: blink = !blink
    }

    PageIndicator {
        id: idPageIndicator

        parent: idRoot.background

        x: idRoot.width - Dimensions.Spacing24 - width / 2
        y: idRoot.contentItem.y + idRoot.contentItem.height / 2 - height / 2

        interactive: true

        count: idContentSwipeView?.count
        currentIndex: idContentSwipeView?.currentIndex
    }

    contentItem: Control {
        contentItem: ColumnLayout {
            spacing: 0

            RowLayout {
                Layout.fillWidth: true

                spacing: Dimensions.Spacing16

                HorizontalSpacer {}

                RoundButton {
                    icon.source: Images.close

                    onClicked: idRoot.close()
                }
            }

            SwipeView {
                id: idContentSwipeView

                Layout.fillWidth: true
                Layout.fillHeight: true

                spacing: Dimensions.Spacing64
                orientation: Qt.Vertical

                currentIndex: idPageIndicator.currentIndex

                onContentPositionChanged: {
                    idSplitInputsPane.updateBackground()
                    idSearchPane.updateBackground()
                    idSlippagePane.updateBackground()
                    idHistoryPane.updateBackground()
                }

                Control {
                    contentItem: ColumnLayout {
                        spacing: 0

                        VerticalSpacer {
                            Layout.minimumHeight: Dimensions.Spacing16
                        }

                        Item {
                            id: idFlipable

                            Layout.fillWidth: true
                            Layout.maximumWidth: Dimensions.columnsWidth(8, idRoot.width)
                            Layout.maximumHeight: 450
                            Layout.minimumHeight: 450
                            Layout.alignment: Qt.AlignHCenter

                            property bool flipped: false

                            transform: Rotation {
                                id: rotation

                                origin {
                                    x: idFlipable.width / 2
                                    y: idFlipable.height / 2
                                }

                                axis {
                                    x: 0
                                    y: 1
                                    z: 0
                                }

                                angle: idFlipable.flipped ? 180 : 0

                                Behavior on angle {
                                    NumberAnimation {
                                        id: idFlipableAnimation

                                        duration: Animations.longDuration
                                        easing.type: Animations.easingType
                                    }
                                }
                            }

                            Shortcut {
                                enabled: idFlipable.flipped

                                autoRepeat: false
                                sequence: "Escape"

                                onActivated: {
                                    idFlipable.flipped = !idFlipable.flipped
                                }
                            }

                            SplitPane {
                                id: idSplitInputsPane

                                width: idFlipable.width
                                height: idFlipable.height

                                visible: rotation.angle < 90

                                contentItem: ColumnLayout {
                                    spacing: 0

                                    Label {
                                        Layout.bottomMargin: Dimensions.Spacing32

                                        text: qsTr("I have %1 %2").arg(idBidAssetPresenter.balance).arg(idRoot.bidAsset.currency.name)

                                        font: Fonts.p2_regular_comp

                                        color: Colors.textSecondary
                                    }

                                    AssetRow {
                                        id: idBidInput

                                        Layout.fillWidth: true
                                        Layout.alignment: Qt.AlignVCenter

                                        asset: idRoot.bidAsset

                                        text: idSwapViewModel.bidValue
                                        descr: idSwapViewModel.fiatValue

                                        tooltipVisible: idSwapViewModel.errors & SwapViewModel.InsufficientAmountError &&
                                                        idContentSwipeView.contentY <= 100 &&
                                                        !idFlipable.flipped &&
                                                        !idFlipableAnimation.running

                                        onTextEdited: {
                                            idSwapViewModel.bidValue = text
                                        }

                                        onClicked: {
                                            idSwapViewModel.filterAsset = SwapViewModel.AskAsset
                                            idFlipable.flipped = !idFlipable.flipped
                                        }

                                        onFocusedChanged: (activeFocus) => {
                                            if(activeFocus) {
                                                idSwapViewModel.setFocusedField(SwapViewModel.BidField)
                                            }
                                        }

                                        Component.onCompleted: {
                                            Qt.callLater(idBidInput.valueTextField.forceActiveFocus)
                                        }
                                    }

                                    VerticalSpacer {}

                                    Item {
                                        Layout.preferredHeight: Dimensions.Spacing48
                                    }

                                    Label {
                                        Layout.bottomMargin: Dimensions.Spacing32

                                        text: qsTr("I want %1").arg(idRoot.askAsset.currency.name)

                                        font: Fonts.p2_regular_comp

                                        color: Colors.textSecondary
                                    }

                                    AssetRow {
                                        id: idAskInput

                                        Layout.fillWidth: true
                                        Layout.alignment: Qt.AlignVCenter

                                        asset: idRoot.askAsset

                                        text: idSwapViewModel.askValue
                                        descr: idSwapViewModel.fiatValue

                                        tooltipVisible: false

                                        onTextEdited: {
                                            idSwapViewModel.askValue = text
                                        }

                                        onClicked: {
                                            idSwapViewModel.filterAsset = SwapViewModel.BidAsset
                                            idFlipable.flipped = !idFlipable.flipped
                                        }

                                        onFocusedChanged: (activeFocus) => {
                                            if(activeFocus) {
                                                idSwapViewModel.setFocusedField(SwapViewModel.AskField)
                                            }
                                        }
                                    }

                                    VerticalSpacer {}
                                }
                            }

                            Pane {
                                id: idSearchPane

                                width: idFlipable.width
                                height: idFlipable.height

                                visible: rotation.angle > 90

                                transform: Rotation {
                                    origin {
                                        x: idSearchPane.width / 2
                                        y: idSearchPane.height / 2
                                    }

                                    axis {
                                        x: 0
                                        y: 1
                                        z: 0
                                    }

                                    angle: 180
                                }

                                contentItem: ColumnLayout {
                                    spacing: Dimensions.Spacing16

                                    Label {
                                        Layout.fillWidth: true

                                        text: qsTr("Select Asset to Swap")

                                        font: Fonts.p2_regular_comp
                                    }

                                    TextField {
                                        Layout.fillWidth: true

                                        text: idSwapViewModel.searchQuery
                                        placeholderText: qsTr("Search")

                                        icon: TextField.Icon.Search

                                        onTextEdited: {
                                            idSwapViewModel.searchQuery = text
                                        }
                                    }

                                    ListView {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true

                                        visible: idSwapViewModel.assetsModel.length > 0

                                        boundsBehavior: Flickable.StopAtBounds
                                        clip: true
                                        spacing: Dimensions.Spacing8

                                        model: idSwapViewModel.assetsModel

                                        delegate: SwapAssetDelegate {
                                            id: idAssetDelegate

                                            width: ListView.view.width

                                            highlighted: idSwapViewModel.filterAsset === SwapViewModel.AskAsset?
                                                             idAssetDelegate.asset.type === idRoot.bidAsset.type :
                                                             idAssetDelegate.asset.type === idRoot.askAsset.type

                                            onClicked: {
                                                if(idSwapViewModel.filterAsset === SwapViewModel.AskAsset) {
                                                    idRoot.bidAsset = idAssetDelegate.asset
                                                }
                                                else {
                                                    idRoot.askAsset = idAssetDelegate.asset
                                                }

                                                idFlipable.flipped = !idFlipable.flipped

                                                idSwapViewModel.searchQuery = ""
                                            }
                                        }
                                    }

                                    Label {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true

                                        visible: idSwapViewModel.assetsModel.length === 0

                                        text: qsTr("Asset not found.")

                                        font: Fonts.p2_regular_text

                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }

                        VerticalSpacer {
                            Layout.minimumHeight: Dimensions.Spacing32
                        }

                        Pane {
                            id: idSlippagePane

                            Layout.fillWidth: true
                            Layout.maximumWidth: Dimensions.columnsWidth(8, idRoot.width)
                            Layout.alignment: Qt.AlignHCenter

                            visible: idSwapViewModel.status === SwapViewModel.IdleStatus

                            verticalPadding: 16
                            horizontalPadding: 20

                            contentItem: RowLayout {
                                spacing: Dimensions.Spacing16

                                Label {
                                    Layout.fillWidth: true

                                    text: qsTr("Slippage:")

                                    font: Fonts.h4_wide
                                }

                                TabBar {
                                    currentIndex: 0

                                    TabButton {
                                        width: implicitWidth

                                        text: qsTr("0.1%")
                                    }

                                    TabButton {
                                        width: implicitWidth

                                        text: qsTr("0.5%")
                                    }

                                    TabButton {
                                        width: implicitWidth

                                        text: qsTr("1.0%")
                                    }
                                }
                            }
                        }

                        VerticalSpacer {
                            Layout.minimumHeight: Dimensions.Spacing32
                        }

                        Button {
                            Layout.fillWidth: true
                            Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)
                            Layout.alignment: Qt.AlignHCenter

                            visible: idSwapViewModel.status === SwapViewModel.IdleStatus

                            text: qsTr("Swap")

                            icon.source: Images.swap

                            onClicked: {
                                if(idSwapViewModel.errors !== 0) {
                                    idBidInput.forceActiveFocus()
                                    return
                                }

                                if(parseFloat(idSwapViewModel.bidValue) <= 0 || parseFloat(idSwapViewModel.askValue) <= 0) {
                                    return
                                }

                                if(idSwapViewModel.bidValue.length === 0 || idSwapViewModel.askValue.length === 0) {
                                    return
                                }

                                idSwapViewModel.makeSwap()
                            }
                        }

                        Label {
                            Layout.alignment: Qt.AlignHCenter
                            Layout.bottomMargin: Dimensions.Spacing16

                            visible: idSwapViewModel.status !== SwapViewModel.IdleStatus

                            text: qsTr("Creating swap...")
                        }

                        BusyIndicator {
                            Layout.alignment: Qt.AlignHCenter

                            visible: idSwapViewModel.status !== SwapViewModel.IdleStatus
                        }
                    }
                }

                Control {
                    id: idHistoryControl

                    readonly property bool isMoving: ListView.view?.moving || false

                    contentItem: ColumnLayout {
                        spacing: Dimensions.Spacing32

                        Label {
                            text: qsTr("HISTORY")

                            font: Fonts.p2_regular_comp
                        }

                        Pane {
                            id: idHistoryPane

                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            ScrollBar {
                                id: idHistoryListScrollBar

                                parent: idHistoryPane

                                x: idHistoryPane.width - (idHistoryPane.horizontalPadding - width) / 2
                                y: idHistoryPane.verticalPadding + idListControls.height + 12

                                height: idHistoryPane.height - idHistoryPane.verticalPadding * 2 - idListControls.height - 12
                            }

                            contentItem: ColumnLayout {
                                spacing: Dimensions.Spacing12

                                RowLayout {
                                    id: idListControls

                                    spacing: Dimensions.Spacing8

                                    TabBar {
                                        currentIndex: idSwapHistoryViewModel.filteringKind

                                        SwapTabButton {
                                            text: qsTr("All")

                                            onClicked: {
                                                idSwapHistoryViewModel.filteringKind = SwapHistoryViewModel.AllKind
                                            }
                                        }

                                        SwapTabButton {
                                            text: qsTr("Processing")

                                            onClicked: {
                                                idSwapHistoryViewModel.filteringKind = SwapHistoryViewModel.ProcessingKind
                                            }
                                        }

                                        SwapTabButton {
                                            text: qsTr("Completed")

                                            onClicked: {
                                                idSwapHistoryViewModel.filteringKind = SwapHistoryViewModel.CompletedKind
                                            }
                                        }
                                    }

                                    HorizontalSpacer {}

                                    TabBar {
                                        currentIndex: idSwapHistoryViewModel.filteringDate

                                        SwapTabButton {
                                            text: qsTr("All")

                                            onClicked: {
                                                idSwapHistoryViewModel.filteringDate = SwapHistoryViewModel.AllDate
                                            }
                                        }

                                        SwapTabButton {
                                            text: qsTr("Today")

                                            onClicked: {
                                                idSwapHistoryViewModel.filteringDate = SwapHistoryViewModel.TodayDate
                                            }
                                        }
                                    }
                                }

                                ListView {
                                    id: idHistoryList

                                    Layout.fillWidth: true
                                    Layout.fillHeight: true

                                    visible: count > 0

                                    interactive: !idHistoryControl.isMoving
                                    clip: true
                                    boundsBehavior: Flickable.StopAtBounds

                                    spacing: Dimensions.Spacing16

                                    model: idSwapHistoryViewModel.swapsModel

                                    delegate: SwapHistoryDelegate {
                                        width: ListView.view.width

                                        blink: idBlinkingTimer.blink

                                        onCancelOrder: (orderId) => {
                                            idSwapViewModel.cancelOrder(orderId)
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

                                    visible: idHistoryList.count === 0

                                    text: qsTr("There is no history yet...")

                                    font: Fonts.p1_regular_text

                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    component AssetRow : RowLayout {
        id: idAssetRow

        spacing: Dimensions.Spacing16

        required property asset asset

        required property bool tooltipVisible

        property alias valueTextField: idValueTextField
        property alias text: idValueTextField.text
        property alias descr: idValueTextField.descr

        signal clicked()
        signal textEdited()
        signal focusedChanged(focused : bool)

        AbstractButton {
            HoverHandler {
                cursorShape: Qt.PointingHandCursor
            }

            contentItem: RowLayout {
                spacing: Dimensions.Spacing24

                Image {
                    Layout.preferredWidth: 88
                    Layout.preferredHeight: 88

                    source: Images.iconForAssetType(idAssetRow.asset.type)

                    fillMode: Image.PreserveAspectFit
                }

                Label {
                    text: idAssetRow.asset.currency.symbol
                    font: Fonts.p1_regular_text
                }

                ColorImage {
                    Layout.preferredWidth: 24
                    Layout.preferredHeight: 24

                    source: Images.chevronDown
                    color: Colors.grayscale000
                }
            }

            onClicked: {
                idAssetRow.clicked()
            }
        }

        HorizontalSpacer {}

        CustomTextField {
            id: idValueTextField

            Layout.fillWidth: true

            asset: idAssetRow.asset

            onTextEdited: {
                idAssetRow.textEdited()
            }

            onActiveFocusChanged: {
                idAssetRow.focusedChanged(activeFocus)
            }

            ToolTip {
                visible: idAssetRow.tooltipVisible

                y: (parent.height - implicitHeight - idValueTextField.bottomPadding) / 2

                contentItem: Label {
                    text: qsTr("You don't have" + "\n" +
                               "enough funds" + "\n" +
                               "to swap")

                    font: Fonts.p1_regular_text
                }
            }
        }
    }

    component SplitPane : Pane {
        id: idSplitPane

        horizontalPadding: 32
        verticalPadding: 16

        AbstractButton {
            id: idSwitchButton

            parent: idSplitPane

            x: (idSplitPane.width - width) / 2
            y: (idSplitPane.height - height)  / 2

            horizontalPadding: 18
            verticalPadding: 6

            HoverHandler {
                cursorShape: Qt.PointingHandCursor
            }

            contentItem: ColorImage {
                width: 24
                height: 24

                source: Images.arrowsSwitch
                color: idSwitchButton.hovered ? Colors.iconPrimary : Colors.iconSecondary
            }

            background: Rectangle {
                color: Color.transparent(Colors.grayscale500, 0.9)
                radius: 18
            }

            onClicked: {
                idSwapViewModel.interchangeAssets()
            }
        }

        background: Item {
            Rectangle {
                id: idMask

                visible: false

                width: idSplitPane.background.width
                height: idSplitPane.background.height

                radius: Dimensions.Spacing24
            }

            GlassBlurEffect {
                y: 0

                width: idSplitPane.background.width
                height: idSplitPane.background.height / 2

                brightness: hovered ? 0.25 : 0.2

                backgroundSource: idSplitPane.backgroundSource
                backgroundSourceRect: Qt.rect(idSplitPane.backgroundSourceRect.x,
                                              idSplitPane.backgroundSourceRect.y,
                                              idSplitPane.backgroundSourceRect.width,
                                              idSplitPane.backgroundSourceRect.height / 2)

                hovered: idSplitPane.hovered
                radius: Dimensions.Spacing24

                maskSource: ShaderEffectSource {
                    sourceItem: idMask
                    sourceRect: Qt.rect(idMask.x,
                                        idMask.y,
                                        idMask.width,
                                        idMask.height / 2)
                }
            }

            GlassBlurEffect {
                y: idSplitPane.background.height / 2

                width: idSplitPane.background.width
                height: idSplitPane.background.height / 2

                brightness: hovered ? 0.15 : 0.1

                backgroundSource: idSplitPane.backgroundSource
                backgroundSourceRect: Qt.rect(idSplitPane.backgroundSourceRect.x,
                                              idSplitPane.backgroundSourceRect.y + idSplitPane.backgroundSourceRect.height / 2,
                                              idSplitPane.backgroundSourceRect.width,
                                              idSplitPane.backgroundSourceRect.height / 2)

                hovered: idSplitPane.hovered
                radius: Dimensions.Spacing24

                maskSource: ShaderEffectSource {
                    sourceItem: idMask
                    sourceRect: Qt.rect(idMask.x,
                                        idMask.y + idMask.height / 2,
                                        idMask.width,
                                        idMask.height / 2)
                }
            }

            Rectangle {
                y: idSplitPane.background.height / 2

                width: idSplitPane.background.width
                height: 1

                color: Colors.backgroundStroke
            }
        }
    }

    component CustomTextField : TextField {
        id: idCustomTextField

        topPadding: 0
        bottomPadding: descFontMetrics.height + Dimensions.Spacing8
        leftPadding: 8
        rightPadding: 0

        placeholderText: "0.00"

        inputMethodHints: Qt.ImhDigitsOnly
        horizontalAlignment: Qt.AlignRight

        font: Fonts.promo_36_wide

        validator: DoubleValidator {
            bottom: 0.00
            decimals: idCustomTextField.asset.currency.decimal
            notation: DoubleValidator.StandardNotation
            locale: Qt.locale("US").name
        }

        readonly property font fiatFont: Fonts.h3_comp

        readonly property FontMetrics mainFontMetrics: FontMetrics { font: idCustomTextField.font }
        readonly property FontMetrics descFontMetrics: FontMetrics { font: idCustomTextField.fiatFont }

        required property asset asset

        required property string descr

        background: Item {
            Label {
                x: idCustomTextField.width - idCustomTextField.rightPadding - width
                y: idCustomTextField.mainFontMetrics.height + Dimensions.Spacing8

                text: ("$%1").arg(idCustomTextField.descr)

                font: Fonts.p2_regular_text
                color: Colors.textSecondary
            }
        }
    }

    component SwapTabButton : TabButton {
        horizontalPadding: 12
        verticalPadding: 6

        width: implicitWidth

        font: Fonts.p4_medium_comp

        color: TabBar.tabBar.currentIndex === TabBar.index ?
                   Colors.textPrimary :
                   Colors.textSecondary
    }
}
