import QtQuick 2.9
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Controls.Styles 1.4


MenuBar {
    id: menuBar;
    property var controller: Controllers.menu;


    ApplicationMenuItem {
        title: qsTr("File");
        Action {
            text: qsTr("Open file");
            onTriggered: controller.onFileOpen();
            shortcut: "Ctrl+Shift+O";
        }

        Action {
            text: qsTr("Save file");
            onTriggered: controller.onFileSave();
            shortcut: "Ctrl+S";
        }

        Action {
            text: qsTr("Save file as");
            onTriggered: controller.onFileSaveAs();
            shortcut: "Ctrl+Shift+S";
        }

        Action {
            text: qsTr("New file");
            onTriggered: controller.onFileNew();
            shortcut: "Ctrl+Shift+N";
        }
    }

    ApplicationMenuItem {
        title: qsTr("View");

        Action {
            text: qsTr("Find");
            onTriggered: controller.onFindFind();
        }

        Action {
            text: qsTr("Increase font");
            onTriggered: controller.onViewIncreaseFontSize();
            shortcut: "Ctrl+Ctrl+Shift+=";

        }

        Action {
            text: qsTr("Decrease font");
            onTriggered: controller.onViewDecreaseFontSize();
            shortcut: "Ctrl+Shift+-";

        }

        Action {
            text: qsTr("Reset font");
            onTriggered: controller.onViewResetFontSize();
        }
    }

    ApplicationMenuItem {
        Loader {
            id: gamepadPopupLoader;
            active: false;
            source: "qrc:/qml/Ui/GamepadSettings.qml";
        }
        title: qsTr("Settings");
        Action {
            //enabled: Controllers.image.Gamepad.Gamepad.connected;
            text: qsTr("Gamepad");
            onTriggered: {
                if(gamepadPopupLoader.active) {
                    gamepadPopupLoader.item.open();
                }
                else {
                    gamepadPopupLoader.active = true;
                    }
                }
            }
    }

    ApplicationMenuItem {
        Loader {
            id: aboutPopupLoader;
            active: false;
            source: "qrc:/qml/Ui/AboutPopup.qml";
        }

        Loader {
            id: checkUpdateLoader;
            active: false;
            source: "qrc:/qml/Ui/UpdatePopup.qml";
        }

        title: qsTr("Help");
        Action {
            text: qsTr("About");
            onTriggered: {
                if(aboutPopupLoader.active) {
                    aboutPopupLoader.item.open();
                }
                else {
                    aboutPopupLoader.active = true;
                }
            }
        }

        Action {
            text: qsTr("Update");
            onTriggered: {
                if(checkUpdateLoader.active) {
                    checkUpdateLoader.item.open();
                }
                else {
                    checkUpdateLoader.active = true;
                }
            }
        }

        ApplicationMenuItem {
            title: qsTr("Examples");
            id: itemExamples;

            Instantiator  {
                model: controller.examples;
                onObjectAdded: itemExamples.insertItem(index, object);
                onObjectRemoved: itemExamples.removeItem(object);

                delegate: MenuItem  {
                    id: exampleMenuItem;
                    text: modelData;
                    implicitHeight: 30;
                    onTriggered: controller.onHelpExample(text);

                    contentItem: Text {
                        text: exampleMenuItem.text;
                        font.family: "Segoe WPC";
                        font.pointSize: 10;
                        color: exampleMenuItem.highlighted ? "#9DA5B4" : "#6E7582";
                        horizontalAlignment: Text.AlignLeft;
                        verticalAlignment: Text.AlignVCenter;
                        elide: Text.ElideRight;
                        renderType: TextEdit.NativeRendering;
                    }

                    background: Rectangle {
                        implicitWidth: 40;
                        implicitHeight: 10;
                        opacity: enabled ? 1 : 0.3;
                        color: exampleMenuItem.highlighted ? "#181A1F" : "transparent";
                    }
                }
            }
        }
    }

    delegate: MenuBarItem {
        id: menuBarItem;

        contentItem: Text {
            text: menuBarItem.text;
            font.family: "Segoe WPC";
            font.pointSize: 10;
            color: menuBarItem.highlighted ? "#9DA5B4" : "#6E7582";
            horizontalAlignment: Text.AlignLeft;
            verticalAlignment: Text.AlignVCenter;
            elide: Text.ElideRight;
            renderType: TextEdit.NativeRendering;
        }

        background: Rectangle {
            implicitWidth: 40;
            implicitHeight: 8;
            opacity: enabled ? 1 : 0.3;
            color: menuBarItem.highlighted ? "#181A1F" : "transparent";
        }
    }

    background: Rectangle {
        implicitWidth: 40;
        implicitHeight: 8;
        color: "#21252B";

        Rectangle {
            color: "#181A1F";
            width: parent.width;
            height: 1;
            anchors.bottom: parent.bottom;
        }
    }


}
