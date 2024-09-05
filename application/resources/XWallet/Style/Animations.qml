pragma Singleton

import QtQuick

QtObject {
    readonly property int easingType: Easing.InOutQuad

    readonly property int shortDuration: 100
    readonly property int mediumDuration: 200
    readonly property int longDuration: 400
}
