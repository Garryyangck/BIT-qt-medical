import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "mycomponent"

Item {
    id: chatPage
    anchors.fill: parent //650*680
    property string receiver
    property string myname: initPage.getUsername()
    property bool identity: initPage.getIdentity()

    Connections {
        target: initPage
        onReturnHistories: {
            chatModel.clear()
            // 每一个history样式: content + "_" + chatDate + "_" + isRead + "_" + sender
            for (var i = 0; i < histories.length; i++) {
                var parts = histories[i].split("_");
                chatModel.append({
                    messageText: parts[0],
                    messageTime: parts[1],
                    isread: parts[2],
                    identity: parts[3] === myname ? identity : !identity,
                    isRightAligned: parts[3] === myname
                })
            }
            chatListView.positionViewAtIndex(chatModel.count - 1, ListView.End)
        }

//        if (msg.messageType == MessageType::RECEIVE_MESSAGE) { // 收到消息
//            // content样式: msg.content + "_" + now.toString(DBUtils::DATE_FORMAT) + "_" + QString::number(1) + "_" + msg.sender->username
//            QString content = msg.content;
//            emit receiveNewMessage(content); // 展示对方发来的信息

//        } else if (msg.messageType == MessageType::SEND_SUCCESS)  { // 发送成功，对方在线
//            // content样式: msg.content + "_" + now.toString(DBUtils::DATE_FORMAT) + "_" + QString::number(1) + "_" + msg.sender->username
//            QString content = msg.content;
//            emit sendSuccess(content); // 展示自己发给对方的消息

//        } else if (msg.messageType == MessageType::RECEIVER_OFFLINE) { // 对方不在线
//            // content样式: msg.content + "_" + now.toString(DBUtils::DATE_FORMAT) + "_" + QString::number(1) + "_" + msg.sender->username
//            QString content = msg.content;
//            emit receiverOffline(content); // 展示自己发给对方的消息
//        }
        onReceiveNewMessage: {
            for (var i = 0; i < content.length; i++) {
                var parts = content[i].split("_");
                chatModel.append({
                    messageText: parts[0],
                    messageTime: parts[1],
                    isread: parts[2],
                    identity: parts[3] === myname ? identity : !identity,
                    isRightAligned: parts[3] === myname
                })
            }
        }

        onSendSuccess: {
            for (var i = 0; i < content.length; i++) {
                var parts = content[i].split("_");
                chatModel.append({
                    messageText: parts[0],
                    messageTime: parts[1],
                    isread: parts[2],
                    identity: parts[3] === myname ? identity : !identity,
                    isRightAligned: parts[3] === myname
                })
            }
        }

        onReceiverOffline: {
            for (var i = 0; i < content.length; i++) {
                var parts = content[i].split("_");
                chatModel.append({
                    messageText: parts[0],
                    messageTime: parts[1],
                    isread: parts[2],
                    identity: parts[3] === myname ? identity : !identity,
                    isRightAligned: parts[3] === myname
                })
            }
        }

    }

    Timer {
        id: timer
        interval: 100
        repeat: false
        onTriggered: {
            initPage.beginChat(receiver)
        }
    }

    Component.onCompleted: {
        timer.start()
    }

    Rectangle {
        width: 650
        height: 80
        anchors.top: parent.top
        color: "#EDEDED"
        Text {
            text: receiver
            font.pixelSize: 30
            anchors.centerIn: parent
        }
    }

    Rectangle {
        width: 650
        height: 380
        anchors.top: parent.top
        anchors.topMargin: 80
        color: "white"

        ListModel {
            id: chatModel
        }

        ListView {
            id: chatListView
            anchors.fill: parent
            spacing: 10
            model: chatModel
            clip: true
            delegate: ChatBox {
                messageText: model.messageText
                messageTime: model.messageTime
                identity: model.identity
                isRightAligned: model.isRightAligned
                isread: model.isRead === 1 ? false : true
            }
        }
    }

    TextArea {
        id: messageInput
        width: 650
        height: 220
        anchors.bottom: parent.bottom
        placeholderText: "输入消息..."
        font.pixelSize: 20
        Keys.onReturnPressed: {
            if (event.modifiers & Qt.ShiftModifier) messageInput.insert(messageInput.cursorPosition, "\n")
            else sendMessage()
        }

        Rectangle {
            id: sendButton
            width: 70
            height: 40
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.bottomMargin: 20
            anchors.rightMargin: 20
            radius: 5
            color: {
                if(!pressed && !hovered) return("#F0F0F0");
                else if(!pressed && hovered) return("#ECECEC");
                else return("#D1D1D1");
            }

            property bool hovered: false
            property bool pressed: false

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                   initPage.sendMessage(messageInput.text, receiver)
                }
                onExited: sendButton.hovered = false
                onEntered: sendButton.hovered = true
                onPressed: sendButton.pressed = true
                onReleased: sendButton.pressed = false
            }

            Text {
                text: "发送"
                font.pixelSize: 20
                anchors.centerIn: parent
                color: "green"
            }
        }
    }
}
