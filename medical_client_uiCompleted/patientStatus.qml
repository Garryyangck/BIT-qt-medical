import QtQuick 2.12
import QtQuick.Window 2.1
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

import "mycomponent"

//查看患者信息窗口
Item {
    width: 850
    height: 680

    property alias patientname: patientName.text
    property alias heightvalue: heightValue.text
    property alias weightvalue: weightValue.text
    property alias heartratevalue: heartRateValue.text
    property alias vitalcapacityvalue: vitalCapacityValue.text
    property alias systolicbloodpressurevalue: systolicBloodPressureValue.text
    property alias diastolicbloodpressurevalue: diastolicBloodPressureValue.text

    Rectangle{
        anchors.fill: parent
        color: "transparent"

        MyButton {
            height: 80
            width: 200
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            hoveredcolor: "#DBDBDB"
            color: "#F8F8F8"
            onClicked: stackView.pop()
            text:"返回患者列表"
        }

        Column{
            width: parent.width
            height: parent.height
            anchors.top: parent.top
            anchors.topMargin: 80
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10 // 子项之间的垂直间距

            Rectangle{
                width: parent.width-20
                height: parent.height*0.1
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 5
                color: "lightgrey"
                Text {
                    id: patientName
                    font.pixelSize: 20
                    font.family: "楷体"
                    anchors.centerIn: parent
                }
            }
            Rectangle{
                width: parent.width-20
                height: parent.height*0.1
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 5
                color: "lightgrey"
                Text {
                    id: heightValue
                    font.pixelSize: 20
                    font.family: "楷体"
                    anchors.centerIn: parent
                }
            }
            Rectangle{
                width: parent.width-20
                height: parent.height*0.1
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 5
                color: "lightgrey"
                Text {
                    id: weightValue
                    font.pixelSize: 20
                    font.family: "楷体"
                    anchors.centerIn: parent
                }
            }
            Rectangle{
                width: parent.width-20
                height: parent.height*0.1
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 5
                color: "lightgrey"
                Text {
                    id: heartRateValue
                    font.pixelSize: 20
                    font.family: "楷体"
                    anchors.centerIn: parent
                }
            }
            Rectangle{
                width: parent.width-20
                height: parent.height*0.1
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 5
                color: "lightgrey"
                Text {
                    id: vitalCapacityValue
                    font.pixelSize: 20
                    font.family: "楷体"
                    anchors.centerIn: parent
                }
            }
            Rectangle{
                width: parent.width-20
                height: parent.height*0.1
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 5
                color: "lightgrey"
                Text {
                    id: systolicBloodPressureValue
                    font.pixelSize: 20
                    font.family: "楷体"
                    anchors.centerIn: parent
                }
            }
            Rectangle{
                width: parent.width-20
                height: parent.height*0.1
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 5
                color: "lightgrey"
                Text {
                    id: diastolicBloodPressureValue
                    font.pixelSize: 20
                    font.family: "楷体"
                    anchors.centerIn: parent
                }
            }
        }
    }
}
