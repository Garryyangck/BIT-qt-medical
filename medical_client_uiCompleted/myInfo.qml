import QtQuick 2.12
import QtQuick.Window 2.1
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

import "mycomponent"

Rectangle{
    width: parent.width
    height: parent.width
    color: "transparent"

    property string age:input1.userInput
    property alias gender: genderComboBox.model
    property string phnoenumber: input3.userInput

    Connections {
        target: initPage
        onReturnPatientInfo: {
            if(info[1] === "男"){
                genderComboBox.currentIndex = 1
            }
            else if(info[1] === "女"){
                genderComboBox.currentIndex = 2
            }
            input1.text = info[2]
            input3.text = info[3]
        }
        onUpdatePatientInfoResult: {

        }
    }

    Component.onCompleted: {
        if(initPage.getIdentity()){
            initPage.getPatientProfile()
        }
        else {

        }
    }

    //个人信息编辑页面
    Column{
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        anchors.topMargin: 90
        spacing: 60 // 子项之间的垂直间距
        //年龄
        Row{
            id: row1
            width: parent.width*0.6
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                id: ageText
                font.pixelSize: 20
                color: "black"
                width: parent.width*0.4
                height: parent.height
                anchors.top: parent.top
                anchors.topMargin: 10
                text: "年龄："
            }
            TextField {
                id: input1
                font.pixelSize: 20
            }
        }
        //性别
        Row{
            id: row2
            width: parent.width*0.6
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                font.pixelSize: 20
                color: "black"
                width: parent.width*0.4
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.top: parent.top
                anchors.topMargin: 10
                text: "性别："
            }
            ComboBox{
                id :genderComboBox
                width: parent.width*0.6
                model:["请选择", "男","女"]
                font.pixelSize: 24
            }
        }
        //电话
        Row{
            id: row3
            width: parent.width*0.6
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                id: telText
                font.pixelSize: 20
                color: "black"
                width: parent.width*0.4
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.top: parent.top
                anchors.topMargin: 10
                text: "电话："
            }
            TextField {
                id: input3
                font.pixelSize: 20
            }
        }
        //科室
        Row{
            id: row4
            width: parent.width*0.6
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter
            visible: !initPage.getIdentity()
            Text {
                font.pixelSize: 20
                color: "black"
                width: parent.width*0.4
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.top: parent.top
                anchors.topMargin: 10
                text: "科室："
            }
            ComboBox{
                id :departmentComboBox
                width: parent.width*0.6
                model:["内科","外科","儿科","妇科","皮肤科","眼科","耳鼻喉科","骨科"]
                font.pixelSize: 24
            }
        }
        MyButton{
            anchors.horizontalCenter: parent.horizontalCenter
            text: "确认修改信息"
            hoveredcolor: "#DBDBDB"
            color: "#F8F8F8"
            onClicked: {
                editResult.text="修改个人信息成功！"
                editResult.open()
            }
        }

        MessageDialog{
            id: editResult
            title: "提示"
            visible: false
            standardButtons: MessageDialog.Ok
        }
    }

}
