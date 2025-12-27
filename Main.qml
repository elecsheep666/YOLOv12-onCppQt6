import QtQuick
import QtQuick.Controls
import "content" // 直接导入整个包，才能检测到自定义控件

// 错误示例：
// import "content/Videos.qml"
// import "qrc:/Source Files/Videos.qml"

Window {
    visible: true
    width: 1200
    height: 700
    minimumWidth: 600
    minimumHeight: 450
    // color: "#222222"
    Item {
        anchors.fill: parent
        // 导入失败，找不到，因为没导入，或者导入自定义包有问题（看最上面，导入语句） 正确做法：直接导入整个文件夹 // import "content"
        Mainwindows {
            id: mainwindows
        }

        Videos {
            id: videos
            visible: mainwindows.videoPressed
        }

        Pictures {
            anchors.fill: parent
            id: pictures
            visible: mainwindows.picturePressed
        }
    }
}



// Window {
//     visible: true
//     width: 640
//     height: 480
//     color: "#222222"

//     // 使用 Rectangle 而不是 Item，这样可以看到布局范围
//     Rectangle {
//         id: mainItem
//         anchors.fill: parent
//         color: "transparent"  // 透明背景

//         property var modelData: ListModel {} // 定义ListModel作为数据源

//         Column {
//             anchors.centerIn: parent
//             spacing: 10
//             width: parent.width * 0.8  // 设置宽度为父元素的80%

//             // 输入框用于输入新文本
//             TextField {
//                 id: inputField
//                 placeholderText: "输入文本"
//                 width: parent.width
//             }

//             // 按钮，点击后将输入框中的文本添加到model中
//             Button {
//                 text: "添加文本"
//                 width: parent.width
//                 onClicked: {
//                     if (inputField.text.trim() !== "") {
//                         mainItem.modelData.append({"text": inputField.text})
//                         inputField.text = "" // 清空输入框
//                         // 直接在这里调用 positionViewAtEnd() ！
//                         listView.positionViewAtEnd()
//                     }
//                 }
//             }

//             // ListView用于展示添加的文本
//             ListView {
//                 id: listView  // 添加 id 以便引用
//                 width: parent.width
//                 height: 300
//                 model: mainItem.modelData
//                 clip: true

//                 // 方法1：监听模型变化
//                 onCountChanged: positionViewAtEnd()

//                 // 方法2：监听 contentHeight 变化（更准确）
//                 onContentHeightChanged: positionViewAtEnd()

//                 delegate: Rectangle {
//                     height: 40
//                     width: ListView.view.width
//                     color: index % 2 ? "#f0f0f0" : "#d0d0d0"
//                     Row {
//                         anchors.fill: parent
//                         // 左边文本（占50%）
//                         Text {
//                             width: parent.width * 0.5
//                             id: fuckright
//                             text: "fuckingtest"
//                             color: index % 2 ? "#d0d0d0" : "#f0f0f0"
//                         }

//                         // 右边文本（占50%）
//                         Text {
//                             id: fuckleft
//                             width: parent.width * 0.5  // 改为 width 而不是 anchors
//                             text: model.text
//                             color: index % 2 ? "#d0d0d0" : "#f0f0f0"
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }
