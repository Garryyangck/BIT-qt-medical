import QtQuick 2.12
import QtQuick.Window 2.1
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

import "mycomponent"

Rectangle {
    width: 850
    height: 680
    color: "transparent"

    property alias patientname: patientName.text
    property alias doctorname: doctorName.text
    property alias symptomsname: symptomsName.text
    property alias diagnosisname: diagnosisName.text
    property alias medicinename: medicineName.text
    property alias datename: dateName.text

    MyButton {
        height: 80
        width: 200
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        hoveredcolor: "#DBDBDB"
        color: "#F8F8F8"
        text: "返回病历列表"
        onClicked: {
            stackView.pop()
        }
    }
    MyButton{
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        hoveredcolor: "#DBDBDB"
        color: "#F8F8F8"
        text: "确认修改病历"
        onClicked: {
            editResult.text="修改病历成功！"
            editResult.open()
        }

        //将数据保存到数据库中
    }
    MessageDialog{
        id: editResult
        title: "提示"
        visible: false
        standardButtons: MessageDialog.Ok
    }
    Column {
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        Rectangle {
            width: parent.width - 20
            height: parent.height * 0.05
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 5
            color: "lightgrey"
            Text {
                id: patientName
                text: "病人姓名："
                font.pixelSize: 20
                font.family: "楷体"
                anchors.fill: parent
                padding: 10
            }
        }
        Rectangle {
            width: parent.width - 20
            height: parent.height * 0.05
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 5
            color: "lightgrey"
            Text {
                id: doctorName
                text: "医生姓名："
                font.pixelSize: 20
                font.family: "楷体"
                anchors.fill: parent
                padding: 10
            }
        }
        Rectangle {
            width: parent.width - 20
            height: parent.height * 0.2
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 5
            color: "lightgrey"
            TextArea {
                id: symptomsName
                text: "病症："
                font.pixelSize: 20
                font.family: "楷体"
                anchors.fill: parent
                padding: 10
                wrapMode: TextArea.Wrap
            }
        }
        Rectangle {
            width: parent.width - 20
            height: parent.height * 0.2
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 5
            color: "lightgrey"
            TextArea {
                id: diagnosisName
                text: "医生诊断："
                font.pixelSize: 20
                font.family: "楷体"
                anchors.fill: parent
                padding: 10
                wrapMode: TextArea.Wrap
            }
        }
        Rectangle {
            width: parent.width - 20
            height: parent.height * 0.2
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 5
            color: "lightgrey"
            TextArea {
                id: medicineName
                text: "处方："
                font.pixelSize: 20
                font.family: "楷体"
                anchors.fill: parent
                padding: 10
                wrapMode: TextArea.Wrap
            }
        }
        Rectangle {
            width: parent.width - 20
            height: parent.height * 0.05
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 5
            color: "lightgrey"
            Text {
                id: dateName
                text: "日期："
                font.pixelSize: 20
                font.family: "楷体"
                anchors.fill: parent
                padding: 10
            }
        }
    }
}
