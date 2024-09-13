import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    width: parent.width
    height: parent.height

    Connections {
        target: initPage

        //                QString patient = parts[0];
        //                QString doctor = parts[1];
        //                QString date = parts[2];
        //                QString department = parts[3];
        //                QString symp = parts[4]; // 症状
        //                QString diag = parts[5]; // 医嘱
        //                QString medic = parts[6]; // 处方和缴费

        onReturnAllCase: {
            recordListModel.clear()
            for (var i = 0; i < cases.length; i++) {
                var parts = cases[i].split("_")
                recordListModel.append({
                      patientName: parts[0],
                      doctorName: parts[1],
                      dateName: parts[2],
                      departmentName: parts[3],
                      symptomsName: parts[4],
                      diagnosisName: parts[5],
                      medicine: parts[6]
                })
            }
        }

    }

    Component.onCompleted: {
        initPage.getAllCase()
    }

    StackView{
        id: stackView
        anchors.fill: parent

        initialItem: Rectangle{
            width: parent.width
            height: parent.height
            color: "transparent"

            Connections {
                target: initPage
            }

            ListView{
                id: recordListView
                anchors.fill: parent
                spacing: 20

                //到时候从后端调用接口输入数据，这里的表示每个病历的具体数据
                model: ListModel {
                    id:recordListModel

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
                            //在这里加一个判断是医生还是病人，然后点击后进入不同的qml文件
                            property string url: initPage.getIdentity() ? "myCases.qml" : "editCases.qml"

                            onClicked: {
                                //.....................
                                stackView.push(url, {
                                patientname:"病人："+patientName,
                                doctorname:"医生："+doctorName+"，"+departmentName,
                                symptomsname:"症状："+symptomsName,
                                diagnosisname:"诊断："+diagnosisName,
                                medicine:"处方："+medicine,
                                datename:"日期："+dateName})
                                //把从后端获得的数据，使用push方法传给页面
                            }
                        }
                        Text{
                            text: dateName + "  " + doctorName + "  " + departmentName
                            font.pixelSize: 20
                            anchors.centerIn: parent
                        }
                    }
                }
            }
        }
    }
}
