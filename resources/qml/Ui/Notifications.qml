import QtQuick 2.11
import QtQml 2.12

Item {

    Loader {
        id: notificationLoader;
        active: false;
        source: "qrc:/qml/Ui/NotificationPopup.qml";
        onLoaded: {
            notificationLoader.active = true;
        }
    }

    function notify(type, headerText, messageText = "", icon = "", disappear = false, delay = 5000, mini = false) {
        notificationLoader.setSource("qrc:/qml/Ui/NotificationPopup.qml",
                                     {"type": type,
                                      "headerText": headerText,
                                      "messageText": messageText,
                                      "customIcon": icon,
                                      "disappear": disappear,
                                      "delay": delay,
                                      "mini": mini
                                     })
        notificationLoader.active = true;
        notificationLoader.item.open();
    }

    function notifyImageSaved() {
        notify("info", "Image saved", "", fa.icons.fa_picture_o, true, 1500);
    }

    function notifyEmptyImage() {
        notify("info", "No camera image", "", "", true, 1500);
    }

    Icon {
        id: fa;
        visible: false;
    }

    Item {
        id: last;
        property bool connected: false;
        property bool is_charging: false;
        property bool running: false;
        property bool remote: false;
    }

    Connections {
        target: Controllers.network;

        function onConnectedChanged() {
            if (last.connected === Controllers.network.connected || Controllers.network.connected === true) return;

            if (!Controllers.network.connected) {
                notify("warn", "Disconnected", "Vehicle is unavailable", "", true);
            }

            last.connected = Controllers.network.connected;
        }

        onTelemetryUpdated: function() {
            if (last.connected === Controllers.network.connected || Controllers.network.connected === false) return;

            if (Controllers.network.connected) {
                notify("ok", "Connected", Controllers.network.vehicle_name, fa.icons.fa_rocket, true);
            }

            if (Controllers.network.rov) {
                spliter.collapseLeft(); // hide code editor
                tabView.selectTab(2); // show remote mode
            }

            last.connected = Controllers.network.connected;
        }

        function onRunningChanged() {
            if (last.running === Controllers.network.running) return;

            if (!Controllers.network.running) {
            }

            last.running = Controllers.network.running;
        }

        function onIs_chargingChanged() {
            if (last.is_charging === Controllers.network.is_charging) return;

            if (Controllers.network.is_charging) {
                notify("ok", "Charging", "", fa.icons.fa_bolt, true);
            } else {
                notify("info", "Not charging", "", fa.icons.fa_plug, true);
            }

            last.is_charging = Controllers.network.is_charging;
        }

        function onNotificationReceived(status, msg) {
            if (msg === "run_when_charging") {
                notify(status, "Motors disabled!", "Please, unplug charger before run");
            }
            if (msg === "low_battery") {
                notify(status, "Low battery level!", "", "", true, 2500);
            }
            if (msg === "already_running") {
                notify(status, "Vehicle is busy", "Something is already running");
            }
        }

        function onRemoteChanged() {
            if (last.remote === Controllers.network.remote) return;

            if (Controllers.network.remote) {
                notify("info", "Remote mode activated", "", fa.icons.fa_gamepad, true, 1500);
            } else if (Controllers.network.connected) {
                notify("info", "Remote mode stopped", "", "", true, 1500);
            }

            last.remote = Controllers.network.remote;
        }
    }

    Connections {
        target: Controllers.image.Gamepad;
        function onGamepadConnectedChanged(connected) {
            if (connected) {
                notify("ok", "Gamepad connected", "", fa.icons.fa_gamepad, true);
            } else {
                notify("warn", "Gamepad disconnected", "", fa.icons.fa_gamepad, true);
            }
        }
    }

    Connections {
        target: Controllers.image;
        function onRecordingVideoChanged() {
            if (Controllers.image.recordingVideo) {
                notify("info", "Recording video", "", fa.icons.fa_circle, true, 1500);
            } else {
                notify("info", "Recording stopped", "", fa.icons.fa_stop, true, 1500);
            }
        }
    }

}
