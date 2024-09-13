import QtQuick 2.12
import QtQuick.Layouts 1.12

Rectangle {
    id: root
    height: chatBubble.height + 40
    width: parent.width
    property string messageText: "hello"
    property string messageTime: "2024.8.27 21:33"
    property bool identity: false // false for doctor, true for patient
    property bool isRightAligned: false // true for user's message on the right, false for other's message on the left
    property bool isread: false


    CircularAvatar {
        id: avatar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 5
        anchors.topMargin: 5
        width: 50
        height: 50
        identity: root.identity
    }

    Text {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 15
        anchors.leftMargin: 72
        text: isread ? "已读" : "未读"
        color: isread ? "grey" : "red"
        visible: !isRightAligned
    }

    Rectangle {
        id: chatBubble
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 60
        anchors.topMargin: 25

        property int maxCharsPerLine: 40
        property int lineHeight: 30

        width: maxCharsPerLine * 7 + 20
        height: (Math.ceil(messageText.length / maxCharsPerLine)) * lineHeight + 25
        color: "#7BE060"
        radius: 12

        Text {
            text: messageText
            anchors.centerIn: parent
            font.pixelSize: 22
            color: "black"
            wrapMode: Text.Wrap
            width: chatBubble.width - 20  // 确保Text的宽度与Rectangle一致
        }
    }

    Text {
        text: messageTime
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 72
        anchors.topMargin: 28 + chatBubble.height
        color: "grey"
    }

    // Adjusting alignment
    LayoutMirroring.enabled: isRightAligned
    LayoutMirroring.childrenInherit: true
}

