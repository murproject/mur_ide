import QtQuick 2.11
import QtQuick.Controls 2.4

import QtQml 2.2
import QtWebView 1.1
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: root;
    visible: true;
    width: 640;
    height: 640;
    color: Style.bgBlue;
    minimumWidth: 640;
    minimumHeight: 640;

    menuBar: ApplicationMenu {
        id: menu;
        visible: !fullWindow.visible;
    }

    Loader {
        id: startupCheckUpdateLoader;
        property bool shown: false;
        active: Controllers.updates.checkUpdate && Controllers.updates.updateAvailable && !checkUpdateLoader.shown;
        source: "qrc:/qml/Ui/UpdatePopup.qml";
        onLoaded: {
            startupCheckUpdateLoader.active = true;
        }
    }

    ApplicationHeader {
        id: header;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.top: parent.top;
    }

    SplitView {
        id: spliter;
        anchors.top: header.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: footer.top;

        leftItem: CodeEditor {
             anchors.fill: parent;
        }

        rightItem: TabView {
            id: tabView;
            anchors.fill: parent;

            ApplicationLogger {
                anchors.fill: parent
            }

            WebView {
                property string tabTitle: "Help";
                anchors.fill: parent
                url: "qrc:/help/help.html";
            }

            RemoteView {
                anchors.fill: parent
            }
        }
    }

    ApplicationStatusBar {
        id: footer;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
    }

    Notifications {
        id: notifications;
    }

    Item {
        id: fullWindow;
        visible: false;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;

        ColumnLayout {
            id: fullWindowContainer;
            anchors.fill: parent;
        }

        Item {
            id: fullWindowTop;
            anchors.fill: parent;

            Compass {
                id: compass;
                visible: Controllers.network.rov;
            }

            Altimeter {
                id: altimeter;
                visible: Controllers.network.rov;
            }
        }
    }
}
