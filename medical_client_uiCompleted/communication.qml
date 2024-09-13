import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "mycomponent"

Item {
    id: root
    width: 850
    height: 680

    property int selectedIndex: -1  // 用于跟踪选中项的索引
    property string receiver

    Connections {
        target: initPage
        onReturnChats: {
            contactListModel.clear()
            for (var i = 0; i < chats.length; i++) {
                var parts = chats[i].split("_");
                contactListModel.append({
                    name: parts[0],
                    unreadCount: parts[1]
                })
            }
        }
    }

    Component.onCompleted: {
        initPage.getChats()
    }

    ListModel {
        id: contactListModel
    }

    ListView {
        width: 200
        height: parent.height
        anchors.top: parent.top
        anchors.left: parent.left
        model: contactListModel

        delegate: Item {
            width: 200
            height: 80

            Rectangle {
                id: rootContiner
                anchors.fill: parent
                color: root.selectedIndex === index ? "#D3D3D3" : (hovered ? "#DBDBDB" : "#F8F8F8")
                property bool hovered: false

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: rootContiner.hovered = true
                    onExited: rootContiner.hovered = false
                    onClicked: {
                        root.selectedIndex = index;  // 更新选中项索引
                        receiver = name
                        pageLoader.source = "chatPage.qml"
                    }
                }
                RowLayout {
                    anchors.fill: parent
                    spacing: 13

                    // 使用 CircularAvatar 作为头像
                    CircularAvatar {
                        width: 50
                        height: 50
                        Layout.leftMargin: 15
                        Layout.alignment: Qt.AlignHCenter
                        identity: !initPage.getIdentity()
                    }

                    Text {
                        text: name
                        Layout.alignment: Qt.AlignHCenter
                        font.pixelSize: 20
                        font.bold: true
                    }
                    Item {
                        Layout.fillWidth: true  // 推动右边元素到右侧
                    }
                    Rectangle {
                        width: 20
                        height: 20
                        Layout.rightMargin: 15
                        Layout.alignment: Qt.AlignHCenter
                        radius: width / 2
                        color: "#E14E4E"
                        clip: true
                        visible: unreadCount > 0

                        Text {
                            id: numberText
                            text: unreadCount
                            color: "white"
                            anchors.centerIn: parent
                            font.family: "Consolas"
                            font.weight: Font.Bold
                            font.pixelSize: 14
                            font.bold: true
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        height: parent.height
        width: 2
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 200
        color: "lightgrey"
    }

    Loader {
        id: pageLoader
        anchors.fill: parent
        anchors.leftMargin: 200
        onLoaded: {
            pageLoader.item.receiver = receiver
        }

        //650*680
    }
}
