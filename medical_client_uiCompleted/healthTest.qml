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
    //健康自测页面
    Column{
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        anchors.topMargin: 50
        spacing: 45 // 子项之间的垂直间距

        Row{
            id: row1
            width: parent.width*0.6
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                font.pixelSize: 20
                color: "black"
                width: parent.width*0.4
                height: parent.height
                anchors.top: parent.top
                anchors.topMargin: 10
                text: "请输入身高(厘米)"
            }
            Input {
                id: input1
            }
        }
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
                text: "请输入体重(千克)"
            }
            Input {
                id: input2
            }
        }
        Row{
            id: row3
            width: parent.width*0.6
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                id: tex
                font.pixelSize: 20
                color: "black"
                width: parent.width*0.4
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.top: parent.top
                anchors.topMargin: 10
                text: "请输入心率(次/分钟)"
            }
            Input {
                id: input3
            }
        }
        Row{
            id: row4
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
                text: "请输入肺活量(毫升)"
            }
            Input {
                id: input4
            }
        }
        Row{
            id: row5
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
                text: "请输入收缩压(mmHg)"
            }
            Input {
                id: input5
            }
        }
        Row{
            id: row6
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
                text: "请输入舒张压(mmHg)"
            }
            Input {
                id: input6
            }
        }

        MyButton{
            anchors.horizontalCenter: parent.horizontalCenter
            text: "测试"
            hoveredcolor: "#DBDBDB"
            color: "#F8F8F8"
            onClicked: {
                var value1=parseFloat(input1.userInput)
                var value2=parseFloat(input2.userInput)
                var value3=parseFloat(input3.userInput)
                var value4=parseFloat(input4.userInput)
                var value5=parseFloat(input5.userInput)
                var value6=parseFloat(input6.userInput)

                //判断BMI
                var result1=value2/(value1*value1)*10000
                var string1
                if(result1<=16.4){
                    string1="您属于极瘦体重"
                }
                else if(result1<=18.4){
                    string1="您体重偏瘦"
                }
                else if(result1<=24.9){
                    string1="您属于正常体重"
                }
                else if(result1<=29.9){
                    string1="您体重过重"
                }
                else if(result1<=34.9){
                    string1="您属于1类肥胖"
                }
                else if(result1<=39.0){
                    string1="您属于2类肥胖"
                }
                else if(result1>39.0){
                    string1="您属于3类肥胖"
                }
                else{
                    string1=""
                }

                //判断心率
                var string2
                if(value3<60){
                    string2="您心率过缓"
                }
                else if(value3>100){
                    string2="您心率过快"
                }
                else if(value3>=60 && value3<=100){
                    string2="您心率正常"
                }
                else{
                    string2=""
                }

                //判断肺活量
                var string3
                if(value4<2800){
                    string3="您肺活量较弱"
                }
                else if(value4>3500){
                    string3="您肺活量较强"
                }
                else if(value4>=2800 && value4<=3500){
                    string3="您肺活量正常"
                }
                else{
                    string3=""
                }

                //判断血压
                var string4
                if(value5<=90 && value6<=60){
                    string4="您血压较低"
                }
                else if(value5<=120 && value6<=80){
                    string4="您血压正常"
                }
                else if(value5<=140 && value6<=90){
                    string4="您处于血压正常的高限"
                }
                else if(value5<=160 && value6<=100){
                    string4="您有中度高血压"
                }
                else if(value5>160 || value6>100){
                    string4="您有重度高血压"
                }
                else{
                    string4=""
                }


                testResult.text="身高: "+value1+" 厘米, 体重: "+value2+" 千克, "+string1+"\n"+
                                "心率: "+value3+" 次/分钟, "+string2+"\n"+
                                "肺活量: "+value4+" 毫升, "+string3+"\n"+
                                "收缩压: "+value5+" mmHg, 舒张压: "+value6+" mmHg,"+string4+"\n"


                testResult.open()
            }
        }

        MessageDialog{
            id: testResult
            title: "您的健康自测结果"
            visible: false
            standardButtons: MessageDialog.Ok
        }
    }

}
