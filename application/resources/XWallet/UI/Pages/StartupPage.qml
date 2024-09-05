import QtCore
import QtQuick
import QtQuick.Layouts

import Qt.labs.animation

import XWallet.Natives
import XWallet.Data

import XWallet.Effects

import XWallet.UI.Components
import XWallet.UI.Pages.StartupPageScreens

import XWallet.Style
import XWallet.Style.Controls
import XWallet.Style.Fonts
import XWallet.Style.Images

Page {
    id: idRoot

    topPadding: Dimensions.Spacing16
    bottomPadding: Dimensions.Spacing64

    horizontalPadding: Dimensions.horizontalPadding + Dimensions.columnsWidth(1, width)

    property real progress: 0.0

    readonly property list<ScreenInfo> screensList: [
        ScreenInfo {
            screen: Step.Unlock
            name: "UnlockScreen"
            progress: -1.0
        },
        ScreenInfo {
            screen: Step.Welcome
            name: "WelcomeScreen"
            progress: 0.0
        },
        ScreenInfo {
            screen: Step.CreatePassword
            name: "CreatePasswordScreen"
            progress: 0.2
        },
        ScreenInfo {
            screen: Step.ShowMnemonicPassphrase
            name: "ShowMnemonicPassphraseScreen"
            progress: 0.3
        },
        ScreenInfo {
            screen: Step.CheckMnemonicPassphrase
            name: "CheckMnemonicPassphraseScreen"
            progress: 0.4
        },
        ScreenInfo {
            screen: Step.SuggestBiometricCreation
            name: "SuggestBiometricCreationScreen"
            progress: 0.5
        },
        ScreenInfo {
            screen: Step.BiometricCreation
            name: "BiometricCreationScreen"
            progress: 0.5
        },
        ScreenInfo {
            screen: Step.Inheritance
            name: "InheritanceScreen"
            progress: 0.6
        },
        ScreenInfo {
            screen: Step.SuggestInheritance
            name: "SuggestInheritanceScreen"
            progress: 0.6
        },
        ScreenInfo {
            screen: Step.RecoverFromInheritance
            name: "RecoverFromInheritanceScreen"
            progress: 0.6
        },
        ScreenInfo {
            screen: Step.SuggestRecoveringVariants
            name: "SuggestRecoveringVariantsScreen"
            progress: 0.3
        },
        ScreenInfo {
            screen: Step.RecoverFromMnemonicPassphrase
            name: "RecoverFromMnemonicPassphraseScreen"
            progress: 0.6
        },
        ScreenInfo {
            screen: Step.RecoverFromBiometric
            name: "RecoverFromBiometricScreen"
            progress: 0.6
        }
    ]

    function screenComponent(screen : int) : Component {
        // return Qt.createComponent("XWallet.UI.Pages.StartupPageScreens", "RecoverFromBiometricScreen")

        for(const screenInfo of screensList) {
            if(screenInfo.screen === screen) {
                idRoot.progress = screenInfo.progress

                return Qt.createComponent("XWallet.UI.Pages.StartupPageScreens", screenInfo.name)
            }
        }

        console.warn("Screen with id", screen, "not found")

        return null
    }

    StartupViewModel {
        id: idViewModel

        property bool canMoveForward: idViewModel.step.screen !== idViewModel.step.screensStack[idViewModel.step.screensStack.length - 1]
        property bool canMoveBackward: idViewModel.step.screen !== idViewModel.step.screensStack[0]

        onStepChanged: {
            var changedStep = idViewModel.step

            if(changedStep.direction === Step.UnknownDirection) {
                return
            }

            var pageComponent = idRoot.screenComponent(changedStep.screen)

            if(pageComponent === null) {
                console.warn("Page component is null")
                return
            }

            var transition = (changedStep.direction === Step.ForwardDirection ?
                        StackView.PushTransition :
                        StackView.PopTransition)

            idStackView.replace(pageComponent, { viewModel: idViewModel }, transition)
        }
    }

    ColumnLayout {
        x: idRoot.width - idRoot.rightPadding - width - Dimensions.Spacing8
        y: (idRoot.height - height) / 2

        spacing: 0

        opacity: idViewModel.step.screensStack.length > 1 ? 1 : 0

        Behavior on opacity {
            NumberAnimation { easing.type: Animations.easingType; duration: Animations.mediumDuration }
        }

        ArrowButton {
            enabled: idViewModel.canMoveBackward

            icon.source: Images.chevronUp

            onClicked: {
                idViewModel.previousScreen()
            }

            Shortcut {
                enabled: idViewModel.canMoveBackward

                autoRepeat: false
                sequence: "Up"

                onActivated: {
                    idViewModel.previousScreen()
                }
            }
        }

        ArrowButton {
            enabled: idViewModel.canMoveForward

            icon.source: Images.chevronDown

            onClicked: {
                idViewModel.nextScreen()
            }

            Shortcut {
                enabled: idViewModel.canMoveForward

                autoRepeat: false
                sequence: "Down"

                onActivated: {
                    idViewModel.nextScreen()
                }
            }
        }
    }

    DragHandler {
        id: idDragHandler

        parent: idRoot
        target: idRoot.contentItem

        xAxis.enabled: false

        onActiveChanged: {
            if (!active) {
                idBoudaryRule.returnToBounds();
            }
        }
    }

    header: Item {

    }

    background: Item {

    }

    contentItem: StackView {
        id: idStackView

        popEnter: Transition {
            NumberAnimation { property: "y"; from: 0.5 * -idStackView.height; to: 0; duration: 200; easing.type: Easing.OutCubic }
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 200; easing.type: Easing.OutCubic }
        }

        popExit: Transition {
            NumberAnimation { property: "y"; from: 0; to: 0.5 * idStackView.height; duration: 200; easing.type: Easing.OutCubic }
            NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 200; easing.type: Easing.OutCubic }
        }

        pushEnter: Transition {
            NumberAnimation { property: "y"; from: 0.5 * idStackView.height; to: 0; duration: 200; easing.type: Easing.OutCubic }
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 200; easing.type: Easing.OutCubic }
        }

        pushExit: Transition {
            NumberAnimation { property: "y"; from: 0; to: 0.5 * -idStackView.height; duration: 200; easing.type: Easing.OutCubic }
            NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 200; easing.type: Easing.OutCubic }
        }

        Component.onCompleted: {
            idStackView.push(idRoot.screenComponent(idViewModel.step.screen), { viewModel: idViewModel })
        }

        BoundaryRule on y {
            id: idBoudaryRule

            minimum: 0
            maximum: 0
            minimumOvershoot: 50
            maximumOvershoot: 50

            onCurrentOvershootChanged: {
                if(idViewModel.canMoveBackward && currentOvershoot >= maximumOvershoot) {
                    idViewModel.previousScreen()
                    idDragHandler.enabled = false
                }
                else if(idViewModel.canMoveForward && currentOvershoot <= -minimumOvershoot) {
                    idViewModel.nextScreen()
                    idDragHandler.enabled = false
                }
            }

            onReturnedToBounds: {
                idDragHandler.enabled = true
            }
        }
    }

    component ScreenInfo : QtObject {
        required property int screen
        required property string name
        required property real progress
    }

    component ArrowButton : ToolButton {
        color: {
            if(!enabled || down) {
                return Colors.iconSecondary
            }

            return Colors.iconPrimary
        }
    }
}
