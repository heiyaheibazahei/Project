#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QObject>
#include <QStringList>
#include <QSettings>
#include <QFile>
#include <QDir>

class FileSystem : public QObject
{
    Q_OBJECT

public:
    explicit FileSystem(QObject *parent = nullptr);

    // 项目管理
    bool saveProject(const QString& projectName, const QStringList& names);
    QStringList loadProject(const QString& projectName);
    QStringList getProjectNames();
    bool deleteProject(const QString& projectName);
    bool saveLuckyPersons(const QStringList& winners); // 新增：保存中奖人员

    // 文件导入
    QStringList importTxtFile(const QString& filePath);
    QStringList importCsvFile(const QString& filePath, int nameColumn = 0);
    QString getLuckyPath();

signals:
    void projectSaved(const QString& projectName);
    void projectDeleted(const QString& projectName);

private:
    QDir projectDir;
    QString projectPath;

    QString getProjectFilePath(const QString& projectName);
    void ensureProjectDir();
    QString luckyPath;   // 中奖人员保存路径
    void ensureDirExists(const QString& path); // 确保保存中奖记录的目录存在
};

#endif // FILESYSTEM_H
