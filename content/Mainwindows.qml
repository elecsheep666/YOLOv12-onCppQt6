import QtQuick 2.15
import QtQuick.Controls

Image {
    id: shadow
    anchors.fill: parent
    visible: true
    source: "qrc:/data/content/images/image002.png"
    anchors.centerIn: parent
    fillMode: Image.Stretch
    property bool videoPressed: false
    property bool picturePressed: false
    Item {
        height: parent.height / 3
        width: parent.width / 3
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: (parent.height - height) / 2
        anchors.leftMargin: (parent.width - width) / 2
        Row {
            anchors.centerIn: parent
            spacing: 50  // 按钮间距
            Button {
                id: bt1
                text: "视频"
                /*
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: (parent.height - height) / 2
                anchors.leftMargin: (parent.width - width) / 5*/
                onClicked: {
                    shadow.visible = false
                    shadow.videoPressed = true
                    // QML语法区别：
                    // 属性声明使用冒号 :：visible: true
                    // 赋值语句使用等号 =：shadow.visible = false
                }
            }
            Button {
                id: bt2
                text: "图片"
                onClicked: {
                    shadow.visible = false
                    shadow.picturePressed = true
                }
                /*
                anchors.left: bt1.right
                anchors.leftMargin: (parent.width - width) / 5*/
            }
        }
    }
}
