#include "ConfigManager.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>
#include <QDebug>


QStringList ConfigManager::getModelFolders() const {
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp();
    QString modelDirPath = appDir.absoluteFilePath("temp/modle");
    QDir modelDir(modelDirPath);
    return modelDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
}

void ConfigManager::setModelPaths(const QString &folderName) {
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp();
    QString base = appDir.absoluteFilePath("temp/modle/" + folderName);

    QString onnxPath;
    QString namesPath;

    QDir dir(base);
    for (const QFileInfo &fi : dir.entryInfoList({"*.onnx"}, QDir::Files)) {
        onnxPath = fi.absoluteFilePath();
        break;
    }
    for (const QFileInfo &fi : dir.entryInfoList({"*.names"}, QDir::Files)) {
        namesPath = fi.absoluteFilePath();
        break;
    }

    if (!onnxPath.isEmpty()) m_config["modelpath"] = onnxPath;
    if (!namesPath.isEmpty()) m_config["namespath"] = namesPath;
    saveConfig();
}

ConfigManager::ConfigManager(QObject *parent) : QObject(parent) {
    // 设置 config.json 路径（与 test001.cpp 逻辑一致）
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp(); // 回到项目根目录
    QString tempPath = appDir.absoluteFilePath("temp");
    ensureConfigDir();
    m_configFilePath = tempPath + "/config.json";
    m_config = loadConfig(); // 加载现有配置
    ensureDefaultValues(); // ② 补全缺失字段（不覆盖已有）
}

QStringList ConfigManager::getCocoNames() const {
    QStringList list;
    QString namePath = m_config.value("namespath").toString();
    if (namePath.isEmpty()) return list;

    QFile f(namePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return list;

    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty())
            list.append(line);
    }
    return list;
}

void ConfigManager::setTargetClass(const QString &cls)
{
    m_config["target"] = cls.trimmed();
    saveConfig();   // 内部已经做了 ensureDefaultValues + 非覆盖写
}

// 新增：只补缺失字段
void ConfigManager::ensureDefaultValues()
{
    // 如果文件里本来就没有这些 key，才写入默认值
    if (!m_config.contains("input"))
        m_config["input"] = "";
    if (!m_config.contains("output"))
        m_config["output"] = "";
    if (!m_config.contains("target"))
        m_config["target"] = "";
    if (!m_config.contains("outputtemp"))
        m_config["outputtemp"] = false;
    // 以后再加字段，继续在这里补即可
}

QVariantMap ConfigManager::loadConfig() {
    QFile file(m_configFilePath);
    if (!file.exists()) {
        qDebug() << "Config file not found, will create new one:" << m_configFilePath;
        return QVariantMap();
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open config file for reading:" << m_configFilePath;
        return QVariantMap();
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.object().toVariantMap();
}

// 完全重写 saveConfig 函数
// 重写 saveConfig：先补全再落盘
void ConfigManager::saveConfig()
{
    ensureDefaultValues();   // 保证落盘前不缺字段
    QFile file(m_configFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法写入配置文件:" << m_configFilePath;
        return;
    }
    QJsonDocument doc(QJsonObject::fromVariantMap(m_config));
    file.write(doc.toJson());
    file.close();
    qDebug() << "配置已保存:" << m_configFilePath;
}

QString ConfigManager::getInputPath() const {
    return m_config.value("input", "").toString();
}

QString ConfigManager::getOutputPath() const {
    return m_config.value("output", "").toString();
}

// 同时修改 setInputPath 和 setOutputPath，改为自动保存
void ConfigManager::setInputPath(const QString &path) {
    m_config["input"] = path;
    saveConfig(); // 自动保存
}

void ConfigManager::setOutputPath(const QString &path) {
    m_config["output"] = path;
    saveConfig(); // 自动保存
}

QString ConfigManager::getTargetClass() const
{
    return m_config.value("target").toString().trimmed();
}
bool ConfigManager::getOutputTemp() const
{
    return m_config.value("outputtemp").toBool();
}
/* 返回 “yyMMdd-HHmmss” 格式的文件夹名 */
QString ConfigManager::makeDateTimeFolderName() const
{
    return QDateTime::currentDateTime().toString("yyMMdd-HHmmss");
}

QVariantList ConfigManager::getImageFiles(const QString &folderPath) const {
    QVariantList result;

    QString cleanPath = folderPath;
    if (cleanPath.startsWith("file:///")) {
        cleanPath = cleanPath.remove(0, 8);
    }

    QDir dir(cleanPath);
    if (!dir.exists()) {
        qWarning() << "文件夹不存在:" << cleanPath;
        return result;
    }

    QStringList filters{"*.jpg", "*.jpeg", "*.png", "*.bmp", "*.JPG", "*.PNG"};
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);

    // ===== 使用 std::as_const 避免 detach =====
    // 改为：
    for (const QFileInfo &fi : std::as_const(files)) {
        // ==========================================
        QVariantMap item;
        item["image_url"] = "file:///" + fi.absoluteFilePath();
        item["image_name"] = fi.fileName();
        result.append(item);
    }
    qDebug() << "从" << cleanPath << "加载了" << result.size() << "张图片";
    return result;
}

void ConfigManager::ensureConfigDir() {
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp();
    QDir().mkpath(appDir.absoluteFilePath("temp"));
}
