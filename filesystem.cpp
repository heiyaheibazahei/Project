#include "filesystem.h"
#include <QStandardPaths>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

FileSystem::FileSystem(QObject *parent) : QObject(parent)
{
    // 设置项目存储路径
    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    projectDir = QDir(appDataDir);
    projectPath = appDataDir + "/projects/";

    // 确保项目目录存在
    ensureProjectDir();
}

void FileSystem::ensureProjectDir()
{
    QDir dir(projectPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

QString FileSystem::getProjectFilePath(const QString& projectName)
{
    QString filename = projectName + ".names";
    return projectPath + filename;
}

bool FileSystem::saveProject(const QString& projectName, const QStringList& names)
{
    if (names.isEmpty() || projectName.isEmpty()) {
        return false;
    }

    QString filePath = getProjectFilePath(projectName);
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    for (const QString &name : names) {
        out << name << "\n";
    }

    file.close();

    // 保存到最近项目列表
    QSettings settings;
    QStringList recentProjects = settings.value("recentProjects").toStringList();

    // 如果项目已存在则先移除
    recentProjects.removeAll(projectName);

    // 添加项目到列表顶部
    recentProjects.prepend(projectName);

    // 限制最多保存10个项目
    while (recentProjects.size() > 10) {
        recentProjects.removeLast();
    }

    settings.setValue("recentProjects", recentProjects);

    emit projectSaved(projectName);
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
    QStringList projects;

    // 最近项目列表
    QSettings settings;
    projects = settings.value("recentProjects").toStringList();

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
