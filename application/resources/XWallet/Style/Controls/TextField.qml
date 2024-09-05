import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

import XWallet.Style
import XWallet.Style.Fonts
import XWallet.Style.Images

T.TextField {
    id: control

    enum Icon {
        None,
        Password,
        Search,
        Copy,
        Pencil,
        Check
    }

    implicitWidth: implicitBackgroundWidth + leftInset + rightInset ||
                   Math.max(contentWidth, placeholder.implicitWidth) + leftPadding + rightPadding

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding,
                             placeholder.implicitHeight + topPadding + bottomPadding)

    leftPadding: 12
    rightPadding: icon === TextField.Icon.None ? 12 : 48
    topPadding: 14
    bottomPadding: 14

    color: Colors.textPrimary

    selectionColor: Color.transparent(Colors.acent, 0.3)
    selectedTextColor: Colors.textPrimary

    placeholderTextColor: Colors.backgroundSecondary

    verticalAlignment: TextInput.AlignVCenter

    font: Fonts.p1_regular_text

    property int icon: TextField.Icon.None

    signal iconClicked()

    PlaceholderText {
        id: placeholder
        x: control.leftPadding
        y: control.topPadding
        width: control.width - (control.leftPadding + control.rightPadding)
        height: control.height - (control.topPadding + control.bottomPadding)

        text: control.placeholderText
        font: control.font
        color: Colors.textSecondary
        verticalAlignment: control.verticalAlignment
        visible: !control.length && !control.preeditText && (!control.activeFocus || control.horizontalAlignment !== Qt.AlignHCenter)
        elide: Text.ElideRight
        renderType: control.renderType
    }

    ToolButton {
        visible: control.icon !== TextField.Icon.None
        enabled: control.icon === TextField.Icon.Password ||
                 control.icon === TextField.Icon.Copy ||
                 control.icon === TextField.Icon.Check

        x: control.width - width - 8
        y: (control.height - height) / 2

        width: 32
        height: 32

        icon.source: {
            switch (control.icon) {
                case TextField.Icon.Password:
                    return control.echoMode === TextInput.Password ?
                                Images.eyeOpen :
                                Images.eyeClosed
                case TextField.Icon.Search:
                    return Images.search
                case TextField.Icon.Copy:
                    return Images.copy
                case TextField.Icon.Pencil:
                    return Images.pencil
                case TextField.Icon.Check:
                    return Images.check
                default:
                    return ""
            }
        }

        onClicked: {
            control.iconClicked()
        }
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 52

        color: "#33F2F3F7"

        radius: 8

        border.width: 1
        border.color: {
            if(control.activeFocus) {
                if(control.acceptableInput) {
                    return Colors.backgroundSecondary
                }
                else {
                    return Colors.backgroundNegative
                }
            }
            else {
                return Colors.transparent
            }
        }

        Behavior on border.color {
            ColorAnimation {
                easing.type: Animations.easingType
                duration: Animations.mediumDuration
            }
        }
    }
}
