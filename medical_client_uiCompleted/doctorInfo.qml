import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "mycomponent"

Item {
    width: 850
    height: 680

    property string doctorName
    property string doctorGender
    property string doctorAge
    property string doctorPhone
    property string doctorDepartment

    Connections {
        target: initPage
        onReturnDoctorProfile: {
            doctorName = doctorProfile[0]
            doctorGender = doctorProfile[1]
            doctorAge = doctorProfile[2]
            doctorPhone = doctorProfile[3]
            doctorDepartment = doctorProfile[4]
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 20

        // 返回按钮
        Button {
            text: "返回"
            Layout.alignment: Qt.AlignLeft
            onClicked: {
                stackView.pop()
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            height: 500
            color: "white"
            radius: 15

            // 头像和医生基本信息
            RowLayout {
                spacing: 20
                anchors.centerIn: parent

                // 圆形头像
                CircularAvatar {
                    identity: false
                    width: 100
                    height: 100
                }

                ColumnLayout {
                    spacing: 10

                    Text {
                        text: doctorName
                        font.pixelSize: 24
                        font.bold: true
                    }

                    Text {
                        text: "性别: " + doctorGender
                        font.pixelSize: 18
                    }

                    Text {
                        text: "电话: " + doctorPhone
                        font.pixelSize: 18
                    }

                    Text {
                        text: "科室: " + doctorDepartment
                        font.pixelSize: 18
                    }
                }
            }
        }
    }
    Component.onCompleted: {
        initPage.getDoctorProfile(doctorName)
    }
}
