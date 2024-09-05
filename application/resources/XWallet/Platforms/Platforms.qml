pragma Singleton

import QtQuick 

QtObject {
    readonly property bool isIos: Qt.platform.os === "ios"
    readonly property bool isAndroid: Qt.platform.os === "android"
    readonly property bool isDesktop: Qt.platform.os === "windows" ||
                                      Qt.platform.os === "linux" ||
                                      Qt.platform.os === "osx"
}
