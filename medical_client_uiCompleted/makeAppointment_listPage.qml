import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "mycomponent"

Item {
    width: 850
    height: 680

    property int departmentIndex
    property var dateList: ["全部日期"]
    property var doctorList: ["全部医生"]
    property var scheduleList
    property bool initialized: false
    property var departmentName: ["内科", "外科", "儿科", "妇科", "皮肤科", "眼科", "耳鼻喉科", "骨科"]
    Connections {
        target: initPage
        onReturnSchedule: {
            scheduleList = schedule;
            appointmentListModel.clear() // 清空现有数据
            for (var i = 0; i < scheduleList.length; i++) {
                var parts = scheduleList[i].split("_");

                if(!initialized){
                    // 检查 doctorList 中是否已经包含此医生名字
                    if (doctorList.indexOf(parts[0]) === -1) {
                        doctorList.push(parts[0]); // 如果不包含，添加到 doctorList 中
                    }
                    // 检查 dateList 中是否已经包含此日期
                    if (dateList.indexOf(parts[2]) === -1) {
                        dateList.push(parts[2]); // 如果不包含，添加到 dateList 中
                    }
                }

                appointmentListModel.append({
                    name: parts[0],
                    date: parts[2],
                    slots: parts[3]
                });
            }
            dateFilter.model = dateList
            doctorFilter.model = doctorList
            initialized = true
        }

        onAppointResult: {
            if(isSuccess){
                appointSuccessText.text = "预约成功"
                appointSuccessText.color = "green"
                getScheduleList(dateFilter.currentText, doctorFilter.currentText)
            }
            else {
                appointSuccessText.text = "预约失败"
                appointSuccessText.color = "red"
            }
            appointButton.enabled = false
            timer.start()
        }
    }

    Component.onCompleted: {
        initPage.getSchedule(departmentName[departmentIndex], "全部日期", "全部医生")
    }

    function getScheduleList(date, doctor){
        initPage.getSchedule(departmentName[departmentIndex], date, doctor)
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        Button {
            text: "<返回"
            Layout.alignment: Qt.AlignLeft
            onClicked: {
                stackView.pop()
            }
        }

        RowLayout {
            spacing: 10
            Layout.fillWidth: true

            ComboBox {
                id: dateFilter
                Layout.fillWidth: true
                onCurrentTextChanged: {
                    if(initialized) {
                        getScheduleList(dateFilter.currentText, doctorFilter.currentText)
                    }
                }
            }

            ComboBox {
                id: doctorFilter
                Layout.fillWidth: true
                onCurrentTextChanged: {
                    if(initialized) {
                        getScheduleList(dateFilter.currentText, doctorFilter.currentText)
                    }
                }
            }
        }
        ListModel {
            id: appointmentListModel
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: appointmentListModel

            delegate: Item {
                width: parent.width
                height: 80
                Rectangle {
                    anchors.fill: parent
                    color: "#F0F0F0"
                    radius: 10
                    border.color: "#CCCCCC"
                    border.width: 1

                    RowLayout {
                        anchors.fill: parent
                        spacing: 10
                        anchors.margins: 5

                        CircularAvatar {
                            identity: false
                            width: 60
                            height: 60
                        }

                        ColumnLayout {
                            spacing: 5
                            Layout.fillWidth: true

                            Text {
                                text: "医生：" + name
                                font.pixelSize: 18
                                font.bold: true
                            }

                            Text {
                                text: "可预约日期: " + date
                                font.pixelSize: 14
                            }

                            Text {
                                text: "剩余名额: " + slots
                                font.pixelSize: 14
                                color: slots > 0 ? "green" : "red"
                            }
                        }

                        Button {
                            text: "查看医生信息"
                            onClicked: {
                                stackView.push(Qt.resolvedUrl("doctorInfo.qml"), { doctorName: name });
                            }
                        }

                        Button {
                            text: "预约"
                            enabled: slots > 0
                            Layout.alignment: Qt.AlignRight
                            onClicked: {
                                // 显示预约提示框
                                confirmDialog.doctorName = name
                                confirmDialog.date = date
                                confirmDialog.open()
                            }
                        }    
                    }
                }
            }
        }

        Dialog {
            id: confirmDialog
            modal: true
            title: "预约确认"
            width: 400
            height: 300
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2

            property string doctorName
            property string date

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                Text {
                    text: "医生: " + confirmDialog.doctorName
                    font.pixelSize: 20
                    horizontalAlignment: Text.AlignHCenter
                }

                Text {
                    text: "预约日期: " + confirmDialog.date
                    font.pixelSize: 20
                    horizontalAlignment: Text.AlignHCenter
                }
                Text {
                    id: appointSuccessText
                    text: ""
                    font.pixelSize: 20
                    horizontalAlignment: Text.AlignHCenter
                }

                Button {
                    id:appointButton
                    text: "确定"
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: {
                        initPage.doAppointment(departmentName[departmentIndex], confirmDialog.date, confirmDialog.doctorName)
                    }
                }

                Timer {
                    id: timer
                    interval: 2000
                    repeat: false
                    onTriggered: {
                        appointSuccessText.text = ""
                        appointButton.enabled = true
                        confirmDialog.close()
                    }
                }
            }
        }
    }
}
