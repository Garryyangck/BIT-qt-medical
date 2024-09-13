import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    width: 850
    height: 680


    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: Rectangle {
            width: 850
            height: 680
            color: "transparent"
            border.width: 0

            ColumnLayout {
                anchors.fill: parent
                spacing: 20

                Text {
                    text: "请选择科室"
                    font.pixelSize: 24
                    Layout.alignment: Qt.AlignHCenter
                }

                GridView {
                    id: departmentGridView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    cellWidth: 200
                    cellHeight: 250

                    model: [
                        { name: "内科", imageSource: "images/internalMedicine.png" },
                        { name: "外科", imageSource: "images/surgery.png" },
                        { name: "儿科", imageSource: "images/pediatrics.png" },
                        { name: "妇科", imageSource: "images/gynaecology.png" },
                        { name: "皮肤科", imageSource: "images/dermatology.png" },
                        { name: "眼科", imageSource: "images/ophthalmology.png" },
                        { name: "耳鼻喉科", imageSource: "images/otolaryngology.png" },
                        { name: "骨科", imageSource: "images/orthopedics.png" }
                    ]

                    delegate: Item {
                        width: departmentGridView.cellWidth
                        height: departmentGridView.cellHeight
                        Rectangle {
                            id: gridborder
                            anchors.fill: parent
                            color: "transparent"
                            border.width: 2
                            border.color: "transparent"

                            Rectangle {
                                anchors.fill: parent
                                anchors.margins: 10
                                radius: 10
                                color: "#E6E6E6"
                                border.width: 0


                                Image {
                                    source: modelData.imageSource
                                    anchors.centerIn: parent
                                    anchors.verticalCenterOffset: -20
                                    height: 180
                                    width: 180
                                    fillMode: Image.PreserveAspectCrop
                                }

                                Text {
                                    text: modelData.name
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: 10
                                    font.pixelSize: 30
                                    color: "black"
                                    font.bold: true
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onClicked: {
                                        console.log(index);
                                        stackView.push(Qt.resolvedUrl("makeAppointment_listPage.qml"), {departmentIndex: index});
                                    }

                                    onEntered: {
                                        gridborder.border.color = "#5B8DFF"
                                    }

                                    onExited: {
                                        gridborder.border.color = "transparent"
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
