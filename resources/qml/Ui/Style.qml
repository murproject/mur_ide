pragma Singleton
import QtQuick 2.11

QtObject {
    property color transparent: "#00000000";
    property color shadowColor: "#C0000000";
    property color outlineColor:"#80000000";

    property color lighterGray: "#D6DBDF";
    property color lightGray:   "#9DA5B4";
    property color gray:        "#6E7582";
    property color darkGray:    "#5E636E";
    property color darkerGray:  "#4C515A";

    property color green:       "#148F77";
    property color greenDark:   "#044F47";
    property color yellow:      "#FFEF38";
    property color yellowDark:  "#DAB709";
    property color orange:      "#F39C12";
    property color orangeDark:  "#A35C02";
    property color red:         "#E74C3C";

    property color lightestBlue:"#6DAFF2";
    property color lightBlue:   "#1D9FF2";
    property color blue:        "#2B68A4";

    property color bgDarker:    "#181A1F";
    property color bgDark:      "#21252B";
    property color bgBlue:      "#282C34";
    property color semiDarkest: "#333943";
    property color semiDarker:  "#363C46";
    property color semiDark:    "#3F4552";
    property color white:       "#FFFFFF";

    property color overlayBack: "#80202020";

    property color batteryColor: {
        if (!Controllers.network.connected) {
            batteryColor: darkGray;
        } else if (Controllers.network.battery >= 50) {
            batteryColor: green;
        } else if (Controllers.network.battery >= 20) {
            batteryColor: yellowDark;
        } else {
            batteryColor: red;
        }
    }

    property string fontMono: "Noto Mono";
    property string fontSans: "Noto Sans";

    property real headerFontSize: 16;

    property double animDuration: 500;
    property double animFast: 250;
    property double animFastest: 150;
    property int animEasing: Easing.OutQuart;
}
