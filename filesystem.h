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

    // 文件导入
    QStringList importTxtFile(const QString& filePath);
    QStringList importCsvFile(const QString& filePath, int nameColumn = 0);

signals:
    void projectSaved(const QString& projectName);
    void projectDeleted(const QString& projectName);

private:
    QDir projectDir;
    QString projectPath;

    QString getProjectFilePath(const QString& projectName);
    void ensureProjectDir();
};

#endif // FILESYSTEM_H
