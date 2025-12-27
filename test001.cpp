#include "test001.h"
#include "YOLO12.hpp"
#include "ConfigManager.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <opencv2/imgcodecs.hpp>
#include <QCoreApplication>
#include <QDateTime>
#include <QUrl>

// 修改构造函数：不移动自身到线程
Worker::Worker(QObject *parent) : QObject(parent) {
    // 空构造函数，不在这里创建线程
}

// 析构函数
Worker::~Worker() {
    // 清理线程
    if (m_thread && m_thread->isRunning()) {
        m_thread->quit();
        m_thread->wait();
    }
}

// QML调用：启动检测线程
void Worker::start() {
    // 创建新线程（每次调用都新建）
    m_thread = new QThread();
    QObject* workerObj = new QObject(); // 创建一个临时工作对象

    // 将工作对象移动到线程
    workerObj->moveToThread(m_thread);

    // 连接：线程启动→执行工作
    connect(m_thread, &QThread::started, workerObj, [this, workerObj]() {
        doWork();  // 真正的工作函数

        // 工作完成后，清理并退出线程
        workerObj->deleteLater();
        m_thread->quit();
    });

    // 连接：线程完成→清理
    connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);

    // 启动线程
    m_thread->start();
}

// 核心检测逻辑
void Worker::doWork() {
    try {
        // 发送进度信息
        emit progressChanged("========== YOLOv12 批量检测 ==========");
        QCoreApplication::processEvents(); // 处理事件，让UI更新

        /* 1. 读配置 ----------------------------------------------------*/
        ConfigManager cfg;
        QString inDir      = cfg.getInputPath();
        QString outDir     = cfg.getOutputPath();
        QString targetName = cfg.getTargetClass();          // 要保留的类别
        bool    saveTemp   = cfg.getOutputTemp();           // 是否保存画框图
        if (targetName.isEmpty()) {
            emit progressChanged("❌ config.json 中 target 不能为空");
            emit finished(); return;
        }

        /* 2. 模型路径 --------------------------------------------------*/
        QDir root(QCoreApplication::applicationDirPath()); root.cdUp();
        QString modelPath = cfg.loadConfig()["modelpath"].toString();
        QString namePath  = cfg.loadConfig()["namespath"].toString();
        if (!QFile::exists(modelPath) || !QFile::exists(namePath)) {
            emit progressChanged("❌ 模型或 names 文件缺失"); emit finished(); return;
        }

        /* 3. 加载类别名 ------------------------------------------------*/
        std::vector<std::string> classNames = utils::getClassNames(namePath.toStdString());
        int targetId = -1;
        for (size_t i = 0; i < classNames.size(); ++i) {
            if (QString::fromStdString(classNames[i]).trimmed() == targetName) {
                targetId = static_cast<int>(i); break;
            }
        }
        if (targetId < 0) {
            emit progressChanged("❌ names 文件中找不到目标类别：" + targetName);
            emit finished(); return;
        }

        /* 4. 创建输出目录 ----------------------------------------------*/
        QString dateFolder = cfg.makeDateTimeFolderName();
        QString rawSaveDir = outDir + "/" + dateFolder;          // 原图保存处
        QString boxSaveDir = outDir + "/" + dateFolder + "-temp"; // 画框图保存处
        QDir().mkpath(rawSaveDir);
        if (saveTemp) QDir().mkpath(boxSaveDir);

        /* 5. 获取待处理图片 -------------------------------------------*/
        QDir inputDir(inDir);
        QStringList flt{"*.jpg","*.jpeg","*.png","*.bmp","*.JPG","*.PNG"};
        QFileInfoList files = inputDir.entryInfoList(flt, QDir::Files);
        if (files.isEmpty()) {
            emit progressChanged("❌ 输入目录无图片"); emit finished(); return;
        }

        emit progressChanged(QString("共 %1 张图片，目标类别：%2")
                                 .arg(files.size()).arg(targetName));
        QCoreApplication::processEvents();

        /* 6. 加载模型 --------------------------------------------------*/
        YOLO12Detector detector(modelPath.toStdString(), namePath.toStdString(), false);

        /* 7. 主循环 ----------------------------------------------------*/
        for (int i = 0; i < files.size(); ++i) {
            const QFileInfo &fi = files[i];
            QString msg = QString("处理 %1/%2  %3")
                              .arg(i+1).arg(files.size()).arg(fi.fileName());
            emit progressChanged(msg);
            QCoreApplication::processEvents();

            cv::Mat img = cv::imread(fi.absoluteFilePath().toStdString());
            if (img.empty()) { emit progressChanged("跳过空图"); continue; }

            std::vector<Detection> dets = detector.detect(img);

            /* 7.1 是否包含目标类别 */
            bool hasTarget = false;
            for (const auto &d : dets) {
                if (d.classId == targetId && d.conf >= CONFIDENCE_THRESHOLD) {
                    hasTarget = true; break;
                }
            }

            if (!hasTarget) {
                emit progressChanged("  未检测到 " + targetName + "，跳过");
                continue;
            }

            /* 7.2 保存原图 */
            QString rawName = rawSaveDir + "/" + fi.fileName();
            bool ok = cv::imwrite(rawName.toStdString(), img);
            if (ok) {
                emit progressChanged("  ✅ 原图已保存：" + rawName);
                emit imageSaved(QUrl::fromLocalFile(rawName).toString()); // 关键
            } else {
                emit progressChanged("  ❌ 原图保存失败");
            }

            /* 7.3 保存画框图（可选） */
            if (saveTemp) {
                cv::Mat boxed = img.clone();
                detector.drawBoundingBox(boxed, dets);
                QString boxName = boxSaveDir + "/" + fi.baseName() + "_detected.jpg";
                if (cv::imwrite(boxName.toStdString(), boxed)) {
                    emit progressChanged("  ✅ 画框图已保存：" + boxName);
                    emit boxedImageSaved(QUrl::fromLocalFile(boxName).toString());   // <-- 新增
                } else
                    emit progressChanged("  ❌ 画框图保存失败");
            }

            QThread::msleep(50);   // 让进度条看得清
        }

        emit progressChanged("✅ 全部处理完成！");
    }
    catch (const std::exception &e) {
        emit progressChanged(QString("异常：") + e.what());
    }
    catch (...) {
        emit progressChanged("未知异常");
    }
    emit finished();
}
