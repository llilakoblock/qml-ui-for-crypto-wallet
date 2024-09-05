import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

import XWallet.Effects

import XWallet.Style
import XWallet.Style.Fonts

T.Button {
    id: control

    enum Size {
        Small,
        Medium,
        Large
    }

    enum Appearance {
        Primary,
        Secondary,
        Tertiary
    }

    property int size: Button.Size.Large
    property int appearance: Button.Appearance.Primary

    focusPolicy: Qt.StrongFocus

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    leftPadding: Dimensions.Spacing24
    rightPadding: Dimensions.Spacing24

    verticalPadding: {
        switch (control.size) {
            case Button.Size.Small:
                return Dimensions.Spacing12
            case Button.Size.Medium:
                return Dimensions.Spacing16
            case Button.Size.Large:
                return Dimensions.Spacing24
        }
    }

    spacing: Dimensions.Spacing8

    icon.width: 24
    icon.height: 24
    icon.color: Colors.textPrimary

    font: {
        switch (control.size) {
            case Button.Size.Small:
            case Button.Size.Medium:
                return Fonts.p3_regular_comp
            case Button.Size.Large:
                return Fonts.p2_regular_text
        }
    }

    HoverHandler {
        cursorShape: Qt.PointingHandCursor
    }

    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font

        color: Colors.textPrimary
    }

    background: Rectangle {
        radius: 12

        border.width: control.appearance === Button.Appearance.Tertiary ? 1 : 0
        border.color: Colors.backgroundStroke

        color: {
            if(control.hovered) {
                switch(control.appearance) {
                    case Button.Appearance.Primary:
                        return Colors.buttonPrimaryHover
                    case Button.Appearance.Secondary:
                        return Colors.buttonSecondaryHover
                    case Button.Appearance.Tertiary:
                        return Colors.buttonTertiaryHover
                }
            }
            else {
                switch(control.appearance) {
                    case Button.Appearance.Primary:
                        return Colors.buttonPrimary
                    case Button.Appearance.Secondary:
                        return Colors.buttonSecondary
                    case Button.Appearance.Tertiary:
                        return Colors.buttonTertiary
                }
            }
        }

        Behavior on color {
            ColorAnimation { easing.type: Animations.easingType; duration: Animations.shortDuration }
        }
    }
}
