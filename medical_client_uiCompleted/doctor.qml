import QtQuick 2.12
import QtQuick.Window 2.1
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "mycomponent"

Window {
    id:rootwindow
    visible: true
    width: 1100
    height: 700
    color: "transparent"
    title: "medicalClient_doctor"

    Rectangle {
        id: rootRectangle
        anchors.fill: parent
        color: "#F7F7F7"
        border.color: "gray"
        border.width: 0.5
        radius: 5

        property string username
        property var pages: ["appointmentList.qml", "editCases.qml", "myInfo", "patientStatus.qml", "communication.qml"]

        Connections {
            //target: 用于存储用户名
            //username = ...
        }

        Image {
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: 125
            width: 200
            height: 200
            source: "qrc:/images/redCross.png"
            opacity: 0.5
        }

        CircularAvatar {
            width: 60
            height: 60
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.leftMargin: 10
            identity: true
        }

        Text {
            text: rootRectangle.username
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 90
            anchors.topMargin: 25
            font.pixelSize: 30
        }

        ListView {
            id:listView
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 100
            width: 250
            height: 600
            model: ["首页", "挂号信息", "编辑病例/处方", "个人信息编辑", "患者状况", "医患沟通"]
            delegate: sidebarButtonDelegate
            highlight: Rectangle { color: "transparent" }  // 防止默认高亮效果
            interactive: true  // 允许交互
        }

        Component {
            id: sidebarButtonDelegate

            Item {
                width: parent.width
                height: 80

                Rectangle {
                    id: button
                    width: parent.width
                    height: parent.height
                    color: "transparent"

                    property bool hovered: false

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            listView.currentIndex = index
                            pageLoader.source = rootRectangle.pages[index-1]
                        }
                        onExited: button.hovered = false
                        onEntered: button.hovered = true
                    }

                    Rectangle {
                        anchors.centerIn: parent
                        height: 70
                        width: 240
                        radius: 5
                        color: button.hovered || listView.currentIndex === index ? "#EBEBEB" : "transparent"
                    }

                    Rectangle {
                        width: 5
                        radius: 2.5  // 圆角半径
                        color: "green"
                        anchors.top: parent.top
                        anchors.topMargin: 9
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        visible: listView.currentIndex === index
                        Behavior on height {
                            SequentialAnimation {
                                NumberAnimation { to: 0; duration: 0 }  // 初始为0
                                NumberAnimation { to: 62; duration: 300 }  // 300ms动画从中间向上下伸展
                            }
                        }

                        height: listView.currentIndex === index ? parent.height : 0  // 动画目标值
                    }

                    Text {
                        text: modelData  // 使用 modelData 绑定到模型数据
                        font.pixelSize: 30
                        anchors.centerIn: parent
                        color: "black"
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 1
                    color: "#cccccc"  // 分隔线颜色
                    anchors.bottom: parent.bottom
                    visible: index === 5 ? false : true
                }
            }
        }

        Loader {
            id: pageLoader
            anchors.fill: parent
            anchors.leftMargin: 250
            anchors.topMargin: 20
            //850*680
        }

    }
}
