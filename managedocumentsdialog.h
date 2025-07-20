#ifndef MANAGEDOCUMENTSDIALOG_H
#define MANAGEDOCUMENTSDIALOG_H

#include <QDialog>
#include <QStringList>
//这个managedocumentsdialog文件用于实现文档管理的界面和功能

class QListWidget;
class QPushButton;
class QLabel;

class ManageDocumentsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManageDocumentsDialog(QWidget *parent = nullptr);
    void setProjectsList(const QStringList &projects);
    QString selectedProject() const;

signals:
    void projectSelected(const QString &projectName);//选择文件的信号
    void projectDeleted(const QString &projectName);//删除文件的信号
     void projectImportedToList(const QString &projectName); // 新增：导入到名单信号

private slots:
    void deleteSelectedProject();//删除项目的函数
    void showProject();//展示项目的函数
    void importToSelectedList();        // 新增：导入到名单槽函数

private:
    QListWidget *projectsList;
    QLabel *infoLabel;
    QPushButton *viewButton;//打开项目的按钮
    QPushButton *deleteButton;//删除项目的按钮
     QPushButton *importToListButton;//导入项目的按钮
};

#endif // MANAGEDOCUMENTSDIALOG_H
