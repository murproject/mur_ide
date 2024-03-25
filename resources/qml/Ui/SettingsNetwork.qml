import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Layouts 1.12

GridLayout {
    property string tabTitle: "Network";
    property bool restartRequired: true;
    anchors.top: parent.top;
    anchors.left: parent.left;
    anchors.right: parent.right;
    anchors.margins: 8;
    Layout.fillWidth: true;
    Layout.fillHeight: false;
    columns: 2;

    function save() {
        Controllers.network.setConnectionAddress(addressEdit.text);
        Controllers.network.setReconnectTime(reconnectTimeEdit.text * 1);
        Controllers.network.setPingTime(pingTimeEdit.text * 1);
        Controllers.network.setPongTime(pongTimeEdit.text * 1);

        Controllers.network.saveSettings();
    }

    UiLabel {
        text: "Address and port:"
    }

    UiTextInput {
        id: addressEdit;
        text: Controllers.network.getConnectionAddress();
    }

    UiLabel {
        text: "Timers (msec):"
    }

    UiLabel {}

    UiLabel {
        text: "Reconnection:"
    }

    UiTextInput {
        id: reconnectTimeEdit;

        validator: IntValidator {
            bottom: 500;
            top: 10000;
        }

        text: Controllers.network.getReconnectTime();
    }

    UiLabel {
        text: "Ping (check):"
    }

    UiTextInput {
        id: pingTimeEdit;

        validator: IntValidator {
            bottom: 500;
            top: 10000;
        }

        text: Controllers.network.getPingTime();
    }

    UiLabel {
        text: "Pong (timeout):"
    }

    UiTextInput {
        id: pongTimeEdit;

        validator: IntValidator {
            bottom: 500;
            top: 10000;
        }

        text: Controllers.network.getPongTime();
    }
}
