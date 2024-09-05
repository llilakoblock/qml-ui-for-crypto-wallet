import QtQuick
import QtQuick.Layouts
import QtQuick.Effects

import XWallet.Effects

import XWallet.Natives
import XWallet.Data

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Fonts
import XWallet.Style.Images

import XWallet.UI.Components
import XWallet.UI.Pages
import XWallet.UI.Drawers

ApplicationWindow {
    id: idRoot

    visible: true

    minimumWidth: 1280
    minimumHeight: 900

    width: 1280
    height: 900

    title: `${Application.displayName}`

    color: "#000000"

    //flags: Qt.Window | Qt.CustomizeWindowHint | Qt.WindowMinMaxButtonsHint | Qt.WindowCloseButtonHint

    Behavior on contentItem.opacity {
        NumberAnimation {
            duration: Animations.longDuration
            easing.type: Animations.easingType
        }
    }

    ApplicationViewModel {
        id: idViewModel

        onApplicationChanged: {
            switch (idViewModel.application.state) {
            case WalletApplication.Start:
                idStackView.replace(idStartupPageComponent)
                break
            case WalletApplication.Ready:
                idStackView.replace(idWalletPageComponent)
                break
            }
        }
    }

    WalletPresenter {
        id: idWalletPresenter

        wallet: idViewModel.wallet
    }

    StackView {
        id: idStackView

        anchors.fill: parent

        initialItem: idSplashPageComponent

        replaceEnter: Transition {
            OpacityAnimator {
                from: 0
                to: 1
                duration: 500
                easing.type: Easing.InCubic
            }
        }

        replaceExit: Transition {
            OpacityAnimator {
                from: 1
                to: 0
                duration: 500
                easing.type: Easing.OutCubic
            }

            ScaleAnimator {
                from: 1
                to: 1.1
                duration: 500
                easing.type: Easing.OutCubic
            }
        }
    }

    header: Control {
        contentItem: ColumnLayout {
            spacing: Dimensions.Spacing4

            ProgressLine {
                Layout.fillWidth: true
                Layout.preferredHeight: 6

                visible: idViewModel.application.state === WalletApplication.Start &&
                         ((idStackView.currentItem as StartupPage)?.progress >= 0 || false)

                progress: (idStackView.currentItem as StartupPage)?.progress || 0.0
            }

            GridLayout {
                id: idHeaderRow

                Layout.fillWidth: true
                Layout.preferredHeight: 78
                Layout.leftMargin: Dimensions.Spacing40
                Layout.rightMargin: Dimensions.Spacing40

                rows: 1
                columns: 3

                uniformCellWidths: true

                RowLayout {
                    Layout.fillWidth: true

                    spacing: Dimensions.Spacing16

                    Image {
                        Layout.preferredHeight: 50
                        Layout.alignment: Qt.AlignVCenter

                        source: Images.logo

                        fillMode: Image.PreserveAspectFit
                    }

                    Label {
                        Layout.topMargin: 2
                        Layout.alignment: Qt.AlignVCenter

                        visible: idViewModel.application.state === WalletApplication.Ready

                        text: qsTr("$%1").arg(idWalletPresenter.fiatBalance)

                        font: Fonts.p2_medium_comp

                        color: Colors.textPrimary
                    }

                    HorizontalSpacer {}
                }

                RowLayout {
                    Layout.fillWidth: true

                    HorizontalSpacer {}

                    SearchField {
                        id: idSearchField

                        Layout.fillWidth: true
                        Layout.maximumWidth: Dimensions.columnsWidth(4, idRoot.width)

                        visible: idViewModel.application.state === WalletApplication.Ready

                        placeholderText: qsTr("Search")
                    }

                    HorizontalSpacer {}
                }

                RowLayout {
                    Layout.fillWidth: true

                    spacing: Dimensions.Spacing16

                    HorizontalSpacer {}

                    ToolButton {
                        visible: idViewModel.application.state === WalletApplication.Ready

                        icon.source: Images.settings

                        onClicked: {
                            var obj = idSettingsDrawerComponent.createObject(idRoot)
                            obj.closed.connect(obj.destroy)
                            obj.open()
                        }
                    }
                }
            }
        }
    }

    footer: Item {}

    background: {
        switch (ApplicationSettings.graphics) {
            case ApplicationSettings.FullEffects:
                return idFullEffectsBackgroundComponent.createObject(idRoot)
            case ApplicationSettings.SimplifiedEffects:
                return idSimplifiedEffectsBackgroundComponent.createObject(idRoot)
        }
    }

    component ProgressLine : Control {
        id: idProgressLine

        property real progress: 0.0

        Rectangle {
            width: idProgressLine.width * idProgressLine.progress
            height: idProgressLine.height

            gradient: Colors.progressBarGradient

            Behavior on width {
                NumberAnimation { easing.type: Animations.easingType; duration: Animations.mediumDuration }
            }
        }

        background: Rectangle {
            opacity: 0.4

            gradient: Colors.progressBarGradient
        }
    }

    Component {
        id: idFullEffectsBackgroundComponent

        Item {
            layer.enabled: true

            LiquidGradientEffect {
                y: parent.height * 0.3

                width: parent.width
                height: parent.height - y
            }

            Rectangle {
                anchors.fill: parent

                gradient: Gradient {
                    GradientStop { position: 0.3; color: "#FF000000" }
                    GradientStop { position: 1.0; color: "#00000000" }
                }
            }
        }
    }

    Component {
        id: idSimplifiedEffectsBackgroundComponent

        Rectangle {
            layer.enabled: true
            color: Colors.grayscale600
        }
    }

    Component {
        id: idSplashPageComponent

        SplashPage {}
    }

    Component {
        id: idStartupPageComponent

        StartupPage {}
    }

    Component {
        id: idWalletPageComponent

        WalletPage {
            searchQuery: idSearchField.text
        }
    }

    Component {
        id: idSettingsDrawerComponent

        SettingsDrawer {}
    }
}
