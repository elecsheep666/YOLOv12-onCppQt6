#ifndef TEST001_H
#define TEST001_H

#include <QObject>
#include <QThread>

class Worker : public QObject {
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();

public slots:
    void doWork();
    Q_INVOKABLE void start();

signals:
    void finished();
    void progressChanged(QString message);  // 添加：进度文本信号
    void imageSaved(QString fileUrl);   // <-- 新增 显示图片到pictures的right_row上 // 已删掉，后期用作别的
    void boxedImageSaved(QString fileUrl);   // 画框图路径 显示图片到pictures的right_row上

private:
    QThread *m_thread;
};

#endif // TEST001_H
