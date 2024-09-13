import QtQuick 2.12

Rectangle {
      width: 100
      height: 100
      radius: width / 2
      clip: true
      color: identity ? "#FF8784" : "#6BB1FB"
      border.color: "gray"
      border.width: 0.5
      property bool identity: false

      Image {
          id: avatarImage
          anchors.fill: parent
          anchors.margins: 5
          source: identity ? "qrc:/images/patient.png" : "qrc:/images/doctor.png"
          fillMode: Image.PreserveAspectCrop
      }
  }


