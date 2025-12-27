# YOLOv12-onCppQt6

YOLOv12-onCppQt6 🔍📦 把 YOLOv12 塞进 100 MB 的跨端“小盒子”——纯 C++/Qt6 + ONNX Runtime CPU，零 Python、零 CUDA、零依赖，Windows 下单文件 exe 即点即跑。 支持批量图片类别筛选、实时进度反馈、画框结果可视化；源码级注释，课堂/科研/毕业设计拿来就能改。 拷贝即可运行，U 盘级交付，让视觉算法真正“轻”下来。



快速使用：
1、手动下载onnxruntime-win-x64-1.22.0源码，使用minGW编译出来，替换此文件夹

2、手动下载opencv4.8.1目录源码，使用minGW编译出来，替换opencv文件夹

3、值得注意的是，由于opencv官方并不兼容mingw，opencv编译之后，还需要进行调整在”opencv\\x64\\mingw\\bin“中的”libopencv\_world481.dll“我使用了别的特殊方式编译，我将它放在了opencv文件夹下，需要使用它替换原有的libopencv\_world481.dll
具体参考：https://blog.csdn.net/JCYAO\_/article/details/148140672

QQ交流群：870790039

