#include "managedocumentsdialog.h"
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

ManageDocumentsDialog::ManageDocumentsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("管理文档");
    setMinimumSize(400, 300);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 标题
    QLabel *titleLabel = new QLabel("项目列表", this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");

    // 项目列表
    projectsList = new QListWidget(this);

    // 信息标签
    infoLabel = new QLabel("已选择 0 个项目");

    // 按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
//    viewButton = new QPushButton("查看项目", this);
    viewButton = new QPushButton("加载使用", this);// <--- 修改文本
    deleteButton = new QPushButton("删除项目", this);
    QPushButton *closeButton = new QPushButton("关闭", this);

    buttonLayout->addWidget(viewButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    // 添加到主布局
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(projectsList, 1); // 使用拉伸因子
    mainLayout->addWidget(infoLabel);
    mainLayout->addLayout(buttonLayout);

    // 信号连接
    connect(projectsList, &QListWidget::itemSelectionChanged, [this]() {
        int count = projectsList->selectedItems().count();
        infoLabel->setText(QString("已选择 %1 个项目").arg(count));

        viewButton->setEnabled(count == 1);
        deleteButton->setEnabled(count > 0);
    });

    connect(viewButton, &QPushButton::clicked, this, &ManageDocumentsDialog::showProject);
    connect(deleteButton, &QPushButton::clicked, this, &ManageDocumentsDialog::deleteSelectedProject);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
}

void ManageDocumentsDialog::setProjectsList(const QStringList &projects)
{
    projectsList->clear();
    projectsList->addItems(projects);
}

QString ManageDocumentsDialog::selectedProject() const
{
    QList<QListWidgetItem*> selected = projectsList->selectedItems();
    if (selected.isEmpty()) {
        return QString();
    }
    return selected[0]->text();
}

void ManageDocumentsDialog::deleteSelectedProject()
{
    QString project = selectedProject();
    if (project.isEmpty()) return;

    int ret = QMessageBox::question(this, "确认删除",
                                   QString("确定要删除项目 '%1' 吗?").arg(project),
                                   QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        emit projectDeleted(project);
        // 从列表中移除
        for (int i = 0; i < projectsList->count(); ++i) {
            if (projectsList->item(i)->text() == project) {
                delete projectsList->takeItem(i);
                break;
            }
        }
//        QMessageBox::information(this, "已删除", QString("项目 '%1' 已删除").arg(project));
    }
}

void ManageDocumentsDialog::showProject()
{
    QString project = selectedProject();
    if (!project.isEmpty()) {
        emit projectSelected(project);
        accept();
    }
}
