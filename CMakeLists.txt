# cmake_minimum_required(VERSION 3.16)
# project(FirstFukingTest VERSION 0.1 LANGUAGES CXX)

# # Qt 6 要求编译器支持 C++ 17 或更新版本。
# # 通过设置CMAKE_CXX_STANDARD,CMAKE_CXX_STANDARD_REQUIRED 变量，
# # 如果编译器太旧，CMake 会打印错误信息。
# # set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # opencv有关
# set(PROJECT_SOURCES

# )

# # 这将告诉 CMake 查找 Qt 6 并使Core 模块可用。Gui Qml Quick QuickControls2也是模块
# # 如果CMake 无法找到该模块，就没有继续的必要了，
# # 所以我们要设置REQUIRED 标志，让 CMake 在这种情况下中止。
# find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2)

# set(CMAKE_AUTORCC ON)  # 关键！启用自动资源编译 没有这句话就加载不出图片 很烦
# # 为什么 Calqlatr 没有这句也能工作？
# # 因为 Calqlatr 项目没有使用 .qrc 文件，而是直接在 RESOURCES 参数中指定了资源文件路径，如下：
# # RESOURCES
# #     "content/images/backspace.svg"
# #     "content/images/backspace_fill.svg"

# # 具体是什么不知道，反正必须写
# # 一个自动化进行配置的东西，可以省略代码
# # 应该在所有 Qt CMake 项目的 find_package(Qt6) 之后立即调用。
# qt_standard_project_setup(REQUIRES 6.8)

# # 告诉 CMake 我们要构建一个名为appFirstFukingTest 的可执行文件（所以不是库）作为目标。
# # 目标应从 C++ 源文件main.cpp 生成。
# qt_add_executable(appFirstFukingTest
#     main.cpp
# )

# # 对应find_package的各种包
# target_link_libraries(appFirstFukingTest PRIVATE
#     Qt6::Core
#     Qt6::Gui
#     Qt6::Qml
#     Qt6::Quick
#     Qt6::QuickControls2
# )

# # qt_add_qml_module(appFirstFukingTest
# #     URI FirstFukingTest
# #     QML_FILES
# #         Main.qml
# #         QML_FILES
# #         RESOURCES Resources.qrc # 这是语法错误！qt_add_qml_module() 的参数格式被你写乱了。
# #         QML_FILES content/videos.qml
# #         QML_FILES content/pictures.qml
# # )

# qt_add_qml_module(appFirstFukingTest
#     URI FirstFukingTest
#     QML_FILES
#         Main.qml
#     RESOURCES
#         Resources.qrc
#         QML_FILES content/Videos.qml
#         QML_FILES content/Mainwindows.qml
#         QML_FILES content/Pictures.qml
# )

# # qt_add_qml_module(appFirstFukingTest
# #     URI FirstFukingTest
# #     QML_FILES
# #         Main.qml
# #         QML_FILES # ❌ 错误！重复写了 QML_FILES
# #         QML_FILES
# #         QML_FILES
# #         RESOURCES Resources.qrc
# #         RESOURCES # ❌ 错误！重复写了 RESOURCES
# #         QML_FILES content/videos.qml
# #         QML_FILES content/pictures.qml
# # )

# # Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# # If you are developing for iOS or macOS you should consider setting an
# # explicit, fixed bundle identifier manually though.
# set_target_properties(appFirstFukingTest PROPERTIES
# #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appFirstFukingTest
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#     MACOSX_BUNDLE TRUE
#     WIN32_EXECUTABLE TRUE
# )

# target_link_libraries(appFirstFukingTest
#     PRIVATE Qt6::Quick
# )

# include(GNUInstallDirs)
# install(TARGETS appFirstFukingTest
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )
#########################################
######下方是ai整合的，支持openai的（原教程在yolo11+qt里）
#########################################
cmake_minimum_required(VERSION 3.16)
project(FirstFukingTest VERSION 0.1 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt6模块
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2)

set(CMAKE_AUTORCC ON)  # 关键！启用自动资源编译 没有这句话就加载不出图片 很烦
# ！！！！！！！！！！！！！！！
# 注意，这跟下面这句是有差别的，草了，看了半天没发现，原来是这个原因
# ！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
# 设置自动功能
set(CMAKE_AUTOMOC ON)

qt_standard_project_setup(REQUIRES 6.8)

# ======================= OpenCV 配置 ========================
# 或者使用传统方式
set(OpenCV_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencv/include")
set(OpenCV_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencv/x64/mingw/lib")
set(OpenCV_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencv/x64/mingw/bin")

    # 手动添加包含目录，包含头文件
    include_directories(${OpenCV_INCLUDE_DIR})
    include_directories(${OpenCV_INCLUDE_DIR}/opencv2)

    # 链接库目录
    link_directories(${OpenCV_LIB_DIR})

    # 设置要链接的库
    # set(OpenCV_LIBS opencv_world481d)
    # set(OpenCV_LIBS opencv_world481)

    # 显式链接
    # 显式链接 .a 文件（注意文件名）
    # target_link_libraries(appFirstFukingTest PRIVATE
    #     ${OpenCV_LIB_DIR}/libopencv_world481.dll.a
    # )
    # 注意，必须在下方这个之后定义，因为只有包含了这些文件，才能链接，具体原因也不知道，反正就是能用了，原因有待深究
    # ======================= 创建可执行文件 ========================
    # 创建Qt Quick应用程序
    # qt_add_executable(appFirstFukingTest
    #     ${PROJECT_SOURCES}
    # )


# ======================= 源文件配置 ========================
# 设置项目源文件
set(PROJECT_SOURCES
    main.cpp
    # 如果还有其他C++文件，继续添加
    test001.cpp
    test001.h
    YOLO12.hpp
    ConfigManager.h
    ConfigManager.cpp
)

# ======================= 创建可执行文件 ========================
# 创建Qt Quick应用程序
qt_add_executable(appFirstFukingTest
    ${PROJECT_SOURCES}
)

# 显式链接
# 显式链接 .a 文件（注意文件名）
target_link_libraries(appFirstFukingTest PRIVATE # 按住ctrl点击，会跳转到上方的 # 创建Qt Quick应用程序 的appFirstFukingTest
    ${OpenCV_LIB_DIR}/libopencv_world481.dll.a
)

# ======================= 链接库 ========================
# 链接Qt模块
target_link_libraries(appFirstFukingTest PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
)

# 链接OpenCV
target_link_libraries(appFirstFukingTest PRIVATE ${OpenCV_LIBS})

# ================= ONNX Runtime 现成 DLL 模式 =================
set(ONNXRUNTIME_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/onnxruntime-win-x64-1.22.0")
set(ONNXRUNTIME_INC  "${ONNXRUNTIME_ROOT}/include")
set(ONNXRUNTIME_LIB  "${ONNXRUNTIME_ROOT}/lib/onnxruntime.lib")   # 导入库
set(ONNXRUNTIME_DLL  "${ONNXRUNTIME_ROOT}/lib/onnxruntime.dll")   # 运行库

# 让编译器能找到头文件
target_include_directories(appFirstFukingTest PRIVATE ${ONNXRUNTIME_INC})

# 链接导入库（MinGW 的 ld 能识别 .lib 导入库）
target_link_libraries(appFirstFukingTest PRIVATE ${ONNXRUNTIME_LIB})

# 构建后把 dll 放到 exe 同目录
add_custom_command(TARGET appFirstFukingTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ONNXRUNTIME_DLL}
            $<TARGET_FILE_DIR:appFirstFukingTest>)

# =============================================yolov12hpp========
set(YOLO12_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/YOLOv12-ONNX-CPP-main/include")
set(YOLO12_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/YOLOv12-ONNX-CPP-main/src")

# 手动添加包含目录(这里面都是头文件)
include_directories(${Onnx_INCLUDE_DIR})
# include_directories(${Onnx_INCLUDE_DIR}/YOLO12.hpp)

# 链接库目录
link_directories(${YOLO12_LIB_DIR})


# ======================= QML模块配置 ========================
# 添加QML模块
qt_add_qml_module(appFirstFukingTest
    URI FirstFukingTest
    VERSION 1.0
    QML_FILES
        Main.qml
        content/Videos.qml
        content/Mainwindows.qml
        content/Pictures.qml
        content/ZComboBox.qml
    RESOURCES
        Resources.qrc
)


# ======================= 平台特定配置 ========================
# Windows配置
if(WIN32)
    set_target_properties(appFirstFukingTest PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# ======================= 安装配置 ========================
include(GNUInstallDirs)
install(TARGETS appFirstFukingTest
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
