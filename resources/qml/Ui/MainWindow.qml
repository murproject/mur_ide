import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQml 2.2
import QtWebView 1.0


ApplicationWindow {
    id: root;
    visible: true;
    width: 640;
    height: 480;
    color: "#282C34";
    minimumWidth: 640;
    minimumHeight: 640;

    menuBar: ApplicationMenu {
        id: menu;
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
    /*
    CodeEditor {
        anchors.top: header.bottom;
        anchors.left: parent.left;
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
    } */

    SplitView {
        id: spliter;
        anchors.top: header.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: footer.top;
        leftItem: CodeEditor {
             anchors.fill: parent;
        }


        rightItem :   TabView {
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
                property string tabTitle: "Remote";
            }
        }
    }
    ApplicationStatusBar {
        id: footer;

        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
    }
}
