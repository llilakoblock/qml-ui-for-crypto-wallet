import QtQuick
import QtQuick.Templates as T

T.Container {
    id: control

    focusPolicy: Qt.StrongFocus

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)
}
