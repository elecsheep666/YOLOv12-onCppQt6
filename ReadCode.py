# ReadCode.py
import os

# 需要读取的文件列表
files_to_read = [
    "ConfigManager.cpp",
    "ConfigManager.h",
    "main.cpp",
    "Main.qml",
    "test001.cpp",
    "test001.h",
    "content/Mainwindows.qml",
    "content/Pictures.qml",
    "content/ZComboBox.qml",
    "content/Videos.qml",
    "YOLO12.hpp"
]

# 输出文件
output_file = "WriteCode.txt"

# 以写入模式打开输出文件
with open(output_file, "w", encoding="utf-8") as out_f:
    for file_name in files_to_read:
        file_path = os.path.join(os.getcwd(), file_name)
        # 写入分隔符和文件名提示
        out_f.write(f"########################\n")
        out_f.write(f"{file_name}内容：\n")
        out_f.write(f"########################\n")
        
        # 尝试读取文件内容
        try:
            with open(file_path, "r", encoding="utf-8") as in_f:
                content = in_f.read()
                out_f.write(content)
        except FileNotFoundError:
            out_f.write(f"[错误：文件 {file_name} 未找到]\n")
        except Exception as e:
            out_f.write(f"[读取文件 {file_name} 时出错：{e}]\n")
        
        # 每个文件内容后换行，方便阅读
        out_f.write("\n")

print(f"所有文件已读取并写入到 {output_file}")