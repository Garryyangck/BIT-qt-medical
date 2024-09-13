import QtQuick 2.12

// 健康自测数据输入框
Rectangle {
    id: inputColumn
    width: parent.width*0.6
    height: parent.height
    anchors.verticalCenter: parent.verticalCenter
    radius: 2
    color: "lightgrey"
    border.color: "grey"
    border.width: 1
    MouseArea{
        anchors.fill: parent
        cursorShape: "IBeamCursor"
    }

    property string userInput:input.text

    TextInput{
        id: input
        width: parent.width*0.97
        height: parent.height*0.9
        anchors.centerIn: parent
        font.pixelSize: 36
        focus: true
        maximumLength: 11
        inputMethodHints: Qt.ImhDigitsOnly
        //限制只能输入数字
        onTextChanged: {
            var regex = /[^\d.]/g;
            text = text.replace(regex, ""); // 移除非数字和小数点的字符

            // 确保小数点只出现一次
            var firstDotIndex = text.indexOf('.');
            if (firstDotIndex !== -1) {
                text = text.substring(0, firstDotIndex + 1) + text.substring(firstDotIndex + 1).replace(/\./g, "");
            }

            // 确保小数点不在第一位
            if (text.startsWith('.')) {
                text = '0' + text; // 在小数点前添加 0
            }
        }
    }
}
