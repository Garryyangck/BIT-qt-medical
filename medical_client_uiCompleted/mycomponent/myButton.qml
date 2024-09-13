import QtQuick 2.12

Item {
    id: root
    width: 200
    height: 80
    property int radius: 5
    property bool hovered: false
    property bool pressed: false
    property string text: ""
    property string textcolor: "black"
    property string color: "blue"
    property string hoveredcolor: "blue"
    property string pressedcolor: "transparent"

    signal clicked()

    Rectangle {
        anchors.fill: parent
        radius: root.radius
        color: {
            if(!root.pressed && !root.hovered) return(root.color);
            else if(!root.pressed && root.hovered) return(root.hoveredcolor);
            else return(root.pressedcolor);
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                root.clicked()
            }
            onExited: root.hovered = false
            onEntered: root.hovered = true
            onPressed: root.pressed = true
            onReleased: root.pressed = false
        }

        Text {
            text: root.text
            font.pixelSize: 20
            anchors.centerIn: parent
            color: root.textcolor
        }
    }
}
