import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12


ApplicationWindow {
    id:rootwindow
    visible: true
    width: 450
    height: 600
    flags: Qt.FramelessWindowHint
    color: "transparent"

    Rectangle {
        id: rootRectangle
        anchors.fill: parent
        color: "white"
        border.color: "gray"
        border.width: 0.5
        radius: 5

        property bool isRegistering: false // 标志是否处于注册模式
        property bool identity: false //标志身份，0医生，1患者

        Connections {
            target: initPage

            onErrorOccurred: {
                serverMessage.color = "red"
                serverMessage.text = message
            }

            onRegisterSuccess: {
                serverMessage.color = "green"
                serverMessage.text = "注册成功!"
            }

            onLoginSuccess: {
                serverMessage.color = "green"
                serverMessage.text = "登录成功!"

            }
        }

        // 顶部工具栏
        RowLayout {
            id: topBar
            anchors.top: parent.top
            width: parent.width
            height: 40
            spacing: 5

            Image {
                id: identitySwap
                source: "qrc:/images/identity_swap.png"
                Layout.leftMargin: 10
                Layout.topMargin: 7
                sourceSize: Qt.size(30, 30)
                fillMode: Image.PreserveAspectFit
                Layout.alignment: Qt.AlignVCenter
                MouseArea {
                    anchors.fill: parent
                    onEntered: {
                        identitySwap.source = "qrc:/images/identity_swap_grey.png"
                    }

                    onExited: {
                        identitySwap.source = "qrc:/images/identity_swap.png"
                    }

                    onClicked: {
                        rootRectangle.identity = !rootRectangle.identity
                    }
                }
            }

            Text {
                text: qsTr("点击切换身份")
                Layout.preferredWidth: implicitWidth
                Layout.preferredHeight: implicitHeight
                Layout.topMargin: 7
                color: "grey"
            }

            Item {
                Layout.fillWidth: true  // 推动右边元素到右侧
            }

            Text {
                id: exit
                text: qsTr("×")
                font.pixelSize: 20
                Layout.preferredWidth: implicitWidth
                Layout.preferredHeight: parent.height
                Layout.rightMargin: 10  // 设置与右侧边界的距离
                Layout.topMargin: 8
                MouseArea {
                    anchors.fill: parent
                    onEntered: {
                        exit.color="grey"
                    }

                    onExited: {
                        exit.color="black"
                    }

                    onClicked: {
                        Qt.exit(0)
                    }
                }
            }
        }
        Text {
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 50
            text: "当前身份：" + (rootRectangle.identity ? "患者" : "医生")
            font.pixelSize: 20
            color: "blue"
        }

        ColumnLayout {
            anchors.centerIn: parent
            width: 300
            spacing: 20

            Text {
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("Login")
                font.pixelSize: 70
                font.weight: Font.Bold
                font.family: "Segoe UI"
                color: "green"
            }


            Rectangle {
                color: "transparent"
                border.width: 0
                height: 10
            }

            TextField {
                id: username
                background: Rectangle{
                    width: 300
                    height: 40
                    color: "#E8E8E8"
                    radius: 5
                }
                placeholderText: "用户名："
                font.pixelSize: 20
                padding: 10
                color: "black"
            }

            TextField {
                id: password
                background: Rectangle{
                    width: 300
                    height: 40
                    color: "#E8E8E8"
                    radius: 5

                }
                placeholderText: "密码："
                echoMode: TextInput.Password
                font.pixelSize: 20
                padding: 10
                color: "black"
            }

            TextField {
                id: confirmPassword
                background: Rectangle{
                    width: 300
                    height: 40
                    color: "#E8E8E8"
                    radius: 5

                }
                placeholderText: "确认密码："
                echoMode: TextInput.Password
                font.pixelSize: 20
                padding: 10
                color: "black"
                visible: rootRectangle.isRegistering
            }

            Rectangle {
                id: button1
                width: 300
                height: 40
                radius: 5
                visible: !rootRectangle.isRegistering
                color: button1mousearea.enabled ? (hovered ? "#70ADFF" : "#4E99FF" ) : "#99CFFD"
                property bool hovered: false

                MouseArea {
                    id: button1mousearea
                    anchors.fill: parent
                    enabled: username.text != "" && password.text != ""
                    hoverEnabled: true
                    onClicked: {
                        initPage.doLogin(username.text, password.text, rootRectangle.identity)
                    }
                    onExited: button1.hovered = false
                    onEntered: button1.hovered = true
                }

                Text {
                    text: "登录"
                    font.pixelSize: 25
                    anchors.centerIn: parent
                    color: !button1mousearea.enabled || button1.hovered ? "#EBEBEB" : "white"
                }
            }



            RowLayout {
                Layout.fillWidth: true
                height: 40
                visible: !rootRectangle.isRegistering
                Item {
                    Layout.fillWidth: true  // 推动右边元素到右侧
                }
                Text {
                    text: qsTr("注册")
                    color: "grey"
                    Layout.rightMargin: 5
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            rootRectangle.isRegistering = true
                        }
                    }
                }
            }

            Rectangle {
                id: button2
                width: 300
                height: 40
                radius: 5
                visible: rootRectangle.isRegistering
                color: button2mousearea.enabled ? (hovered ? "#70ADFF" : "#4E99FF" ) : "#99CFFD"

                property bool hovered: false

                MouseArea {
                    id: button2mousearea
                    anchors.fill: parent
                    enabled: username.text != "" && password.text != "" && confirmPassword.text != ""
                    hoverEnabled: true
                    onClicked: {
                        if(password.text != confirmPassword.text){
                            errorMessage.text = "确认密码有误"
                        }
                        else initPage.doRegister(username.text, password.text, rootRectangle.identity)
                    }
                    onExited: button2.hovered = false
                    onEntered: button2.hovered = true
                }

                Text {
                    text: "注册"
                    font.pixelSize: 25
                    anchors.centerIn: parent
                    color: !button2mousearea.enabled || button2.hovered ? "#EBEBEB" : "white"
                }
            }


            RowLayout {
                Layout.fillWidth: true
                height: 40
                visible: rootRectangle.isRegistering
                Item {
                    Layout.fillWidth: true  // 推动右边元素到右侧
                }
                Text {
                    text: qsTr(" < 返回")
                    color: "grey"
                    Layout.rightMargin: 5
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            rootRectangle.isRegistering = false
                        }
                    }
                }
            }
        }

        Text {
            id: serverMessage
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            text: ""

            onTextChanged: {
                resetTimer.restart();
            }

            Timer {
                id: resetTimer
                interval: 1500
                repeat: false
                onTriggered: {
                    serverMessage.text = "";
                }
            }
        }
    }
}
