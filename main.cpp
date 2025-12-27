#include <QGuiApplication>
#include <QQmlApplicationEngine>
// #include <QQuickStyle> 导入失败，是因为在CMake配置中没有找到对应的Qt模块。
#include <QQuickStyle>
#include <test001.h>
#include <QThread>
#include <QQmlContext> // 添加这个头文件
#include "ConfigManager.h" // 添加这个头文件

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName("test");

    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Basic");

    /* ==========  1. 先构造 ConfigManager（读 coco.names） ========== */
    ConfigManager configManager;   // 堆对象，生命周期随 app

    QQmlApplicationEngine engine;

    // ===== 注册 ConfigManager =====
    // ConfigManager configManager;
    engine.rootContext()->setContextProperty("ConfigManager", &configManager);

    // ===== 注册 Worker =====
    Worker *worker = new Worker(&app); // 传入父对象，让Qt自动管理内存
    engine.rootContext()->setContextProperty("worker", worker);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("FirstFukingTest", "Main");

    return app.exec();
}
