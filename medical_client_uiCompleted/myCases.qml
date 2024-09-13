import QtQuick 2.12
import QtQuick.Window 2.1
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

import "mycomponent"

Rectangle{
    width: 850
    height: 680
    color: "transparent"

    property alias patientname: patientName.text
    property alias doctorname: doctorName.text
    property alias symptomsname: symptomsName.text
    property alias diagnosisname:diagnosisName.text
    property alias medicine: medicineName.text
    property alias datename:dateName.text

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
        text:"返回病历列表"
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
            height: parent.height*0.05
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 5
            color: "lightgrey"
            Text {
                id: patientName
                font.pixelSize: 20
                font.family: "楷体"
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
            }
        }
        Rectangle{
            width: parent.width-20
            height: parent.height*0.05
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 5
            color: "lightgrey"
            Text {
                id: doctorName
                font.pixelSize: 20
                font.family: "楷体"
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
            }
        }
        Rectangle{
            width: parent.width-20
            height: parent.height*0.2
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 5
            color: "lightgrey"
            Text {
                id: symptomsName
                font.pixelSize: 20
                font.family: "楷体"
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
            }
        }
        Rectangle{
            width: parent.width-20
            height: parent.height*0.2
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 5
            color: "lightgrey"
            Text {
                id: diagnosisName
                font.pixelSize: 20
                font.family: "楷体"
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
            }
        }
        Rectangle{
            width: parent.width-20
            height: parent.height*0.2
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 5
            color: "lightgrey"
            Text {
                id: medicineName
                font.pixelSize: 20
                font.family: "楷体"
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
            }
        }
        Rectangle{
            width: parent.width-20
            height: parent.height*0.05
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 5
            color: "lightgrey"
            Text {
                id: dateName
                font.pixelSize: 20
                font.family: "楷体"
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
            }
        }
    }

}
