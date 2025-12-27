#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QVariantMap>

class ConfigManager : public QObject {
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = nullptr);
    Q_INVOKABLE QString getTargetClass() const;
    Q_INVOKABLE bool    getOutputTemp() const;
    Q_INVOKABLE QString makeDateTimeFolderName() const;

    // 下拉菜单
    Q_INVOKABLE QStringList getCocoNames() const;   // 给 QML 提供下拉列表
    Q_INVOKABLE void setTargetClass(const QString &cls); // 写 target 字段

    // 读取配置
    Q_INVOKABLE QVariantMap loadConfig();
    // 保存配置
    // 修改函数签名，移除参数
    Q_INVOKABLE void saveConfig(); // 不再接收 QVariantMap 参数

    // 快捷方法
    Q_INVOKABLE QString getInputPath() const;
    Q_INVOKABLE QString getOutputPath() const;
    Q_INVOKABLE void setInputPath(const QString &path);
    Q_INVOKABLE void setOutputPath(const QString &path);

    // modelpath、namespath
    Q_INVOKABLE QStringList getModelFolders() const;
    Q_INVOKABLE void setModelPaths(const QString &folderName);

    // 获取图片列表（返回 ListModel 能用的格式）
    Q_INVOKABLE QVariantList getImageFiles(const QString &folderPath) const;

private:
    QString m_configFilePath;
    QVariantMap m_config;
    void ensureConfigDir();
    void ensureDefaultValues();   // 保证必需字段都存在
};

#endif
