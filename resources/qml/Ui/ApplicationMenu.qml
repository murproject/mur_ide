import QtQuick 2.9
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQml.Models 2.12

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
        title: qsTr("Settings");

        Loader {
            id: settingsPopupLoader;
            active: false;
            source: "qrc:/qml/Ui/SettingsPanel.qml";
        }

        Action {
            text: qsTr("IDE settings");
            onTriggered: {
                if (settingsPopupLoader.active) {
                    settingsPopupLoader.item.open();
                } else {
                    settingsPopupLoader.active = true;
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
                onObjectAdded: function(index, object) {
                    itemExamples.insertItem(index, object);
                }

                onObjectRemoved: function(object) {
                    itemExamples.removeItem(object);
                }


                delegate: MenuItem  {
                    id: exampleMenuItem;
                    text: modelData;
                    implicitHeight: 30;
                    onTriggered: controller.onHelpExample(text);

                    contentItem: Text {
                        text: exampleMenuItem.text;
                        font.family: Style.fontSans;
                        font.pointSize: 10;
                        color: exampleMenuItem.highlighted ? Style.lightGray : Style.gray;
                        horizontalAlignment: Text.AlignLeft;
                        verticalAlignment: Text.AlignVCenter;
                        elide: Text.ElideRight;
                        renderType: TextEdit.NativeRendering;
                    }

                    background: Rectangle {
                        implicitWidth: 40;
                        implicitHeight: 10;
                        opacity: enabled ? 1 : 0.3;
                        color: exampleMenuItem.highlighted ? Style.bgDarker : "transparent";
                    }
                }
            }
        }
    }

    delegate: MenuBarItem {
        id: menuBarItem;

        contentItem: Text {
            text: menuBarItem.text;
            font.family: Style.fontSans;
            font.pointSize: 10;
            color: menuBarItem.highlighted ? "#9DA5B4" : Style.gray;
            horizontalAlignment: Text.AlignLeft;
            verticalAlignment: Text.AlignVCenter;
            elide: Text.ElideRight;
            renderType: TextEdit.NativeRendering;
        }

        background: Rectangle {
            implicitWidth: 40;
            implicitHeight: 8;
            opacity: enabled ? 1 : 0.3;
            color: menuBarItem.highlighted ? Style.bgDarker : "transparent";
        }
    }

    background: Rectangle {
        implicitWidth: 40;
        implicitHeight: 8;
        color: Style.bgDark;

        Rectangle {
            color: Style.bgDarker;
            width: parent.width;
            height: 1;
            anchors.bottom: parent.bottom;
        }
    }


}
