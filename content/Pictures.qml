import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Dialogs // 添加此行

Item {
    id: pictrues
    readonly property color backgroundColor: "#262626" // 背景色

    // 用于显示图片在右侧right_row
    Component.onCompleted: {
        // 原图信号（如不需要可删掉）
        worker.imageSaved.connect(function(fileUrl){
            // 什么都不做，或留作他用
        });

        // 画框图信号
        worker.boxedImageSaved.connect(function(fileUrl){
            rightImage.source = fileUrl;   // 把画框图显示出来
        });
    }

    // ===== 添加启动函数 =====
    // 修改 startDetection 函数
    function startDetection() {
        // 检查路径是否已设置
        if (ConfigManager.getInputPath() === "" || ConfigManager.getOutputPath() === "") {
            console.warn("请先设置输入和输出路径！");
            progressText.text = "错误：请先设置路径！";
            return;
        }

        // 重置进度显示
        progressText.text = "正在启动子线程...";
        progressText.opacity = 1.0;
        blinkTimer.start();

        // 创建简单的计时器来模拟更新（实际由Worker的signal更新）
        worker.progressChanged.connect(function(msg) {
            progressText.text = msg;
            console.log("进度更新:", msg); // 调试输出
        });

        worker.finished.connect(function() {
            blinkTimer.stop();
            progressText.opacity = 1.0;
            if (progressText.text.indexOf("错误") === -1 &&
                progressText.text.indexOf("完成") === -1) {
                progressText.text = "✅ 检测完成！";
            }
        });

        // 启动 Worker（在子线程中运行）
        worker.start();
    }
    // ========================

    // ===== 替换为正确的 FolderDialog =====
    // 在 onAccepted 中简化调用（无需传参）
    FolderDialog {
        id: inputFolderDialog
        onAccepted: {
            var cleanPath = selectedFolder.toString().replace("file:///", "");
            ConfigManager.setInputPath(cleanPath); // 内部自动保存
            loadImagesFromFolder(selectedFolder);
        }
    }
    // =====================================

    // ===== 替换为正确的 FolderDialog =====
    FolderDialog {
        id: outputFolderDialog
        onAccepted: {
            var cleanPath = selectedFolder.toString().replace("file:///", "");
            ConfigManager.setOutputPath(cleanPath); // 内部自动保存
            right_file_url_diplay.text = cleanPath;
        }
    }
    // =====================================

    function loadImagesFromFolder(folderUrl) {
        picListModel.clear();
        console.log("正在加载文件夹:", folderUrl.toString());

        var imageList = ConfigManager.getImageFiles(folderUrl.toString());
        console.log("C++ 返回的图片数量:", imageList.length);

        for (var i = 0; i < imageList.length; i++) {
            var item = imageList[i];
            console.log("添加图片:", i, item.image_name, item.image_url); // 调试用
            picListModel.append(item);
        }

        console.log("ListView 当前项数:", picListModel.count);
    }

    Item {
        id: root
        width: parent.width * 0.9
        height: parent.height * 0.9
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: parent.width * 0.01
        anchors.topMargin: parent.height * 0.01
        // anchors.rightMargin: 10
        // anchors.bottomMargin: 10
        Row {
            id: three_row
            anchors.fill: parent
            spacing: 2
            Rectangle {
                id: left_row
                width: parent.width * 0.45
                height: parent.height * 0.7
                // radius: 5 // 8
                // color: "#222222"
                Item {
                    id: left_update_display
                    anchors.fill: parent
                    Rectangle {
                        anchors.fill: parent
                        radius: 5
                        color: "#222222"
                        ListView {
                            id: pic_listview
                            anchors.fill: parent // 组件不显式指定大小，是致命的，会看不见。
                            // ===== 修改：使用空模型并添加 id =====
                            model: ListModel {
                                id: picListModel
                            }
                            // =====================================

                            delegate: Item {
                                width: pic_listview.width
                                height: 120

                                // required property string image_url
                                // required property string image_name

                                // 注意：属性名改为 model.image_url 和 model.image_name
                                Row {
                                    spacing: 10
                                    Image {
                                        width: 100
                                        height: 100
                                        source: model.image_url // 修改：从 model 获取
                                        fillMode: Image.PreserveAspectFit
                                    }
                                    Text {
                                        text: model.image_name // 修改：从 model 获取
                                        color: "white"
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }
                    }
                }
                RoundButton {
                    id:left_update_bt
                    width: 150
                    radius: 5
                    anchors.left: left_update_display.left
                    anchors.top: left_update_display.bottom
                    text: "选择图片"
                    // ===== 添加此行 =====
                    onClicked: inputFolderDialog.open()
                    // ====================
                }
            }

            Rectangle {
                id: center_row
                width: (parent.width - left_row.width - right_row.width) * 0.9
                height: left_row.height
                // color: "#222222"
                Column {
                    id: bt_column
                    anchors.fill: parent
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.leftMargin: parent.width * 0.05
                    anchors.topMargin: parent.width * 0.05

                    spacing: 2
                    readonly property int bt_length: bt_column.width * 0.95
                    /* ===== 1. 选择模型 ===== */
                    ZComboBox {
                        id: modelCombo
                        width: parent.bt_length
                        height: 36
                        model: ConfigManager.getModelFolders()
                        currentIndex: 0

                        onCurrentTextChanged: {
                            if (currentIndex >= 0) {
                                ConfigManager.setModelPaths(currentText);
                                console.log("选中模型文件夹：", currentText);

                                // ✅ 刷新类别下拉框
                                classCombo.model = ConfigManager.getCocoNames();
                                classCombo.currentIndex = 0;

                                // ✅ 可选：自动设置 target 为第一个类别
                                if (classCombo.model.length > 0) {
                                    ConfigManager.setTargetClass(classCombo.model[0]);
                                }
                            }
                        }

                        Component.onCompleted: {
                            var folders = ConfigManager.getModelFolders();
                            if (folders.length > 0) {
                                ConfigManager.setModelPaths(folders[0]);
                                classCombo.model = ConfigManager.getCocoNames(); // 初始加载
                            }
                        }
                    }

                    /* ===== 2. 目标类别 ===== */
                    ZComboBox {
                        id: classCombo
                        width: parent.bt_length
                        height: 36
                        model: ConfigManager.getCocoNames()          // ← 动态读取
                        currentIndex: 0
                        Component.onCompleted: {
                            // 恢复上次保存的类别
                            var t = ConfigManager.getTargetClass()
                            if(t !== "")  currentIndex = model.indexOf(t)
                        }
                        onCurrentTextChanged: {
                            console.log("选中类别：", currentText)
                            ConfigManager.setTargetClass(currentText) // ← 立即写 json
                        }
                    }
                    RoundButton {
                        width: parent.bt_length
                        text: "开始筛选"
                        radius: 5
                        // ===== 添加这行 =====
                        onClicked: startDetection()
                        // =====================
                    }
                    RoundButton {
                        width: parent.bt_length
                        text: "终止"
                        radius: 5
                    }
                    RoundButton {
                        width: parent.bt_length
                        text: "001"
                        radius: 5
                    }
                }
            }

            Rectangle {
                id: right_row
                width: parent.width * 0.45
                height: parent.height * 0.7
                radius: 5 // 8
                color: "#222222"
                Image {
                    id:  rightImage          // 唯一用来显示的图
                    anchors.centerIn: parent
                    width:  parent.width  * 0.9
                    height: parent.height * 0.9
                    fillMode: Image.PreserveAspectFit
                    source: ""              // 初始空图
                }
                Row {
                    id: right_file_url
                    width: right_row.width
                    height: left_update_bt.height
                    anchors.left: parent.left
                    anchors.top: parent.bottom
                    Rectangle {
                        width: right_row.width * 0.8
                        height: left_update_bt.height
                        color: "#F5F5F5"
                        Text {
                            id: right_file_url_diplay
                            width: right_row.width * 0.8
                            height: left_update_bt.height
                            text: "输出文件的路径"
                            color: "#222222"
                            font.pixelSize: 15
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    RoundButton {
                        id: right_file_url_bt
                        width: right_row.width * 0.2
                        height: left_update_bt.height
                        text: "选择输出路径"
                        radius: 5
                        // ===== 添加此行 =====
                        onClicked: outputFolderDialog.open()
                        // ====================
                    }
                }
            }
        }
    }
    Rectangle {
        id: progress_bar
        width: parent.width
        height: 40
        // anchors.top: three_row.bottom //不行，也不知道为什么
        // anchors.topMargin: 10
        anchors.left:  parent.left          // 新增
        anchors.right: parent.right         // 新增
        anchors.bottom: parent.bottom       // 关键：锚在 root 底部
        color: "#333333"
        radius: 5

        Text {
            id: progressText
            anchors.centerIn: parent
            text: "等待开始"
            color: "white"
            font.pixelSize: 14
        }

        // 简单计时器动画，让文字看起来在动
        Timer {
            id: blinkTimer
            interval: 500
            running: false
            repeat: true
            onTriggered: {
                progressText.opacity = progressText.opacity === 1.0 ? 0.7 : 1.0;
            }
        }
    }
}
