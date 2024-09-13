import QtQuick 2.12
import QtQuick.Window 2.1
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

//查看患者信息列表，展示患者姓名

Item {
    height: parent.height
    width: parent.width

    StackView{
        id: stackView
        anchors.fill: parent
        initialItem: Rectangle {
            width: parent.width
            height: parent.height
            color: "transparent"

            ListView{
                id: recordListView
                anchors.fill: parent
                spacing: 20

                //到时候从后端调用接口输入数据，这里的表示每个病历的具体数据
                model: ListModel {
                    ListElement { patientName:"abc";heightValue:173;weightValue:70;heartRateValue:85;vitalCapacityValue:3500;systolicBloodPressureValue:130;diastolicBloodPressureValue:90 }
                    ListElement { patientName:"bdc";heightValue:174;weightValue:70;heartRateValue:85;vitalCapacityValue:3500;systolicBloodPressureValue:130;diastolicBloodPressureValue:90 }
                    ListElement { patientName:"wbc";heightValue:175;weightValue:70;heartRateValue:85;vitalCapacityValue:3500;systolicBloodPressureValue:130;diastolicBloodPressureValue:90 }
                }

                delegate: Item {
                    width: parent.width*0.5
                    height: 60
                    Rectangle{
                        width: parent.width
                        height: parent.height
                        color: "lightgrey"
                        radius: 10

                        MouseArea{
                            id: mouseArea
                            anchors.fill: parent

                            onClicked: {
                                stackView.push("patientStatus.qml",{
                                patientname:"患者姓名："+patientName,
                                heightvalue:"身高："+heightValue+"厘米",
                                weightvalue:"体重："+weightValue+"千克",
                                heartratevalue:"心率："+heartRateValue+"次/分钟",
                                vitalcapacityvalue:"肺活量："+vitalCapacityValue+"毫升",
                                systolicbloodpressurevalue:"收缩压："+systolicBloodPressureValue+"mmHg",
                                diastolicbloodpressurevalue:"舒张压："+diastolicBloodPressureValue+"mmHg"})
                                //把从后端获得的数据，使用push方法传给页面
                            }
                        }
                        Text{
                            text: patientName
                            font.pixelSize: 20
                            anchors.centerIn: parent
                        }
                    }
                }
            }
        }
    }
}

