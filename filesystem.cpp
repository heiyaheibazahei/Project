#include "filesystem.h"
#include <QStandardPaths>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QDateTime>
#include <QTextCodec>
FileSystem::FileSystem(QObject *parent) : QObject(parent)
{
    // 设置项目存储路径
    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    projectPath = "D:/Project/Project/savedProject/";
    m_luckyPath = "D:/Project/Project/luckyPersons";
  //  projectPath = appDataDir + "/projects/";

    // 确保项目目录存在
    ensureProjectDir();
    //确保中奖记录的路径存在
    ensureDirExists(m_luckyPath);
}

void FileSystem::ensureProjectDir()
{
    QDir dir(projectPath);
    if (!dir.exists()) {
        qDebug() << "Creating project directory: " << projectPath;
        dir.mkpath(".");  // 递归创建目录
    }
}

QString FileSystem::getProjectFilePath(const QString& projectName)
{
    // 生成文件路径：固定路径 + 项目名.names [4]
    QString filename = projectName + ".names";
    return projectPath + filename;
}


bool FileSystem::saveProject(const QString& projectName, const QStringList& names)
{
    if (projectName.isEmpty() || names.isEmpty()) {
        qWarning() << "Invalid project name or empty names list";
        return false;
    }

    QString filePath = getProjectFilePath(projectName);
    QFile file(filePath);

    qDebug() << "Saving project:" << projectName << "at:" << filePath;

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << file.errorString();
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    for (const QString &name : names) {
        out << name << "\n";
    }

    file.close();
    return true;
}

QStringList FileSystem::loadProject(const QString& projectName)
{
    QStringList names;
    QString filePath = getProjectFilePath(projectName);
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return names;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            names.append(line);
        }
    }

    file.close();
    return names;
}

QStringList FileSystem::getProjectNames()
{
    QDir dir(projectPath);
    QStringList projects;

    // 读取固定路径下的所有 .names 文件
    QStringList nameFiles = dir.entryList(QStringList() << "*.names", QDir::Files);

    for (const QString &file : nameFiles) {
        // 移除文件扩展名作为项目名
        projects.append(file.left(file.length() - 6));
    }

    return projects;
}

bool FileSystem::deleteProject(const QString& projectName)
{
    QString filePath = getProjectFilePath(projectName);

    if (QFile::exists(filePath)) {
        if (QFile::remove(filePath)) {
            // 从最近项目中移除
            QSettings settings;
            QStringList recentProjects = settings.value("recentProjects").toStringList();
            recentProjects.removeAll(projectName);
            settings.setValue("recentProjects", recentProjects);

            emit projectDeleted(projectName);
            return true;
        }
    }
    return false;
}

QStringList FileSystem::importTxtFile(const QString& filePath)
{
    QStringList names;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return names;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            names.append(line);
        }
    }

    file.close();
    return names;
}

QStringList FileSystem::importCsvFile(const QString& filePath, int nameColumn)
{
    QStringList names;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return names;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    bool isFirstLine = true;  // 跳过标题行

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue;

        // 简单的CSV解析
        QStringList fields = line.split(',');

        if (fields.size() > nameColumn) {
            QString name = fields[nameColumn].trimmed();
            if (!name.isEmpty() && !isFirstLine) {
                if (name.startsWith('"') && name.endsWith('"')) {
                    name = name.mid(1, name.length() - 2);
                }
                names.append(name);
            }
        }
        isFirstLine = false;
    }

    file.close();
    return names;
}

void FileSystem::ensureDirExists(const QString& path) {
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(path); // 创建目录（包括父目录）
    }
}

bool FileSystem::saveLuckyPersons(const QStringList& winners) {
    if (winners.isEmpty()) return false; // 无中奖人员，无需保存

    // 生成唯一文件名（包含日期时间，避免重复）
    QString fileName = QString("lucky_%1.txt")
        .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QString filePath = QDir(m_luckyPath).filePath(fileName);

    // 写入文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false; // 文件打开失败
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    for (const QString& name : winners) {
        out << name << endl; // 每行一个名字
    }

    file.close();
    return true; // 保存成功
}

QString FileSystem::getLuckyPath(){
    return m_luckyPath;
}
