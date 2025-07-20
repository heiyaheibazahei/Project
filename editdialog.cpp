#include "editdialog.h"
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QInputDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

EditDialog::EditDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("编辑名单");
    setMinimumSize(600, 400);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 列表控件
    listWidget = new QListWidget(this);
    listWidget->setAlternatingRowColors(true);
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &EditDialog::itemDoubleClicked);

    // 输入编辑框
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("输入姓名");

    // 按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    addButton = new QPushButton("添加", this);
    editButton = new QPushButton("编辑", this);
    removeButton = new QPushButton("删除", this);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(removeButton);

    // 导入按钮（带菜单）
    importButton = new QPushButton("导入...", this);
    QMenu *importMenu = new QMenu(this);
    importMenu->addAction("导入文本文件", this, &EditDialog::importTxt);
    importMenu->addAction("导入CSV文件", this, &EditDialog::importCsv);
    importButton->setMenu(importMenu);

    // 底部按钮
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    saveButton = new QPushButton("保存为项目", this);
    QPushButton *closeButton = new QPushButton("关闭", this);
    bottomLayout->addWidget(importButton);
    bottomLayout->addStretch();
    bottomLayout->addWidget(saveButton);
    bottomLayout->addWidget(closeButton);

    // 添加到主布局
    mainLayout->addWidget(listWidget);
    mainLayout->addWidget(nameEdit);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(bottomLayout);

    // 信号连接
    connect(addButton, &QPushButton::clicked, this, &EditDialog::addName);
    connect(editButton, &QPushButton::clicked, this, &EditDialog::editName);
    connect(removeButton, &QPushButton::clicked, this, &EditDialog::removeName);
    connect(saveButton, &QPushButton::clicked, this, &EditDialog::saveProject);
    connect(closeButton, &QPushButton::clicked, this, &EditDialog::saveAndClose);
}

EditDialog::~EditDialog() {}

void EditDialog::setNameList(const QStringList &names)
{
    nameList = names;
    listWidget->clear();
    listWidget->addItems(nameList);
}

QStringList EditDialog::getNameList() const
{
    return nameList;
}

void EditDialog::addName()
{
    QString name = nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "添加失败", "姓名不能为空");
        return;
    }

    nameList.append(name);
    listWidget->addItem(name);
    nameEdit->clear();
}

void EditDialog::editName()
{
    QListWidgetItem *item = listWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "编辑失败", "请先选择一个姓名");
        return;
    }

    QString currentName = item->text();
    QString newName = QInputDialog::getText(this, "编辑姓名", "新姓名:", QLineEdit::Normal, currentName);

    if (!newName.isEmpty() && newName != currentName) {
        nameList.replace(listWidget->row(item), newName);
        item->setText(newName);
    }
}

void EditDialog::removeName()
{
    QListWidgetItem *item = listWidget->currentItem();
    if (item) {
        int row = listWidget->row(item);
        delete listWidget->takeItem(row);
        nameList.removeAt(row);
    }
}

void EditDialog::saveAndClose()
{
    accept();
}

void EditDialog::saveProject()
{
    bool ok;
    QString projectName = QInputDialog::getText(
        this,
        "保存项目",
        "项目名称:",
        QLineEdit::Normal,
        "",
        &ok
    );

    if (ok && !projectName.isEmpty()) {
        // 发射信号，传递项目名称和当前名单
        emit projectSaved(projectName, nameList);
        accept();  // 关闭对话框
    }
}

void EditDialog::importFile()
{
    // 打开文件对话框
}

void EditDialog::importTxt()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "选择文本文件",
        "",
        "文本文件 (*.txt)"
    );

    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "导入失败", "无法打开文件");
        return;
    }

    QStringList newNames;
    QTextStream in(&file);
    in.setCodec("UTF-8");

    while (!in.atEnd()) {
        QString name = in.readLine().trimmed();
        if (!name.isEmpty()) {
            newNames.append(name);
        }
    }

    if (!newNames.isEmpty()) {
        QString result;
        result = QString("成功导入 %1 个名字").arg(newNames.size());
        if (nameList.isEmpty()) {
            result += "\n替换当前名单";
            nameList = newNames;
        } else {
            result += QString("\n添加到当前名单 (%1 个名字)").arg(nameList.size() + newNames.size());
            nameList += newNames;
        }

        setNameList(nameList);
        QMessageBox::information(this, "导入成功", result);
    } else {
        QMessageBox::warning(this, "导入失败", "文件中没有有效的姓名");
    }
}

void EditDialog::importCsv()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "选择CSV文件",
        "",
        "CSV文件 (*.csv)"
    );

    if (filePath.isEmpty()) return;

    // 询问姓名列序号（0开始）
    int columnIndex = QInputDialog::getInt(
        this,
        "选择姓名列",
        "请输入姓名所在列的序号 (1,2,3,...):",
        1, 1, 100, 1
    );

    if (columnIndex <= 0) return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "导入失败", "无法打开文件");
        return;
    }

    QStringList newNames;
    bool isFirstLine = true; // 跳过标题行
    QTextStream in(&file);
    in.setCodec("UTF-8");

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue;

        QStringList fields = line.split(',');
        if (fields.size() >= columnIndex) {
            QString name = fields[columnIndex - 1].trimmed();
            if (!isFirstLine && !name.isEmpty()) {
                if (name.startsWith('"') && name.endsWith('"')) {
                    name = name.mid(1, name.length() - 2);
                }
                newNames.append(name);
            }
        }
        isFirstLine = false;
    }

    if (!newNames.isEmpty()) {
        QString result;
        result = QString("成功导入 %1 个名字").arg(newNames.size());
        if (nameList.isEmpty()) {
            result += "\n替换当前名单";
            nameList = newNames;
        } else {
            result += QString("\n添加到当前名单 (%1 个名字)").arg(nameList.size() + newNames.size());
            nameList += newNames;
        }

        setNameList(nameList);
        QMessageBox::information(this, "导入成功", result);
    } else {
        QMessageBox::warning(this, "导入失败", "文件中没有有效的姓名记录");
    }
}

void EditDialog::itemDoubleClicked(QListWidgetItem *item)
{
    if (!item) return;

    QString currentName = item->text();
    QString newName = QInputDialog::getText(this, "编辑姓名", "新姓名:", QLineEdit::Normal, currentName);

    if (!newName.isEmpty() && newName != currentName) {
        nameList.replace(listWidget->row(item), newName);
        item->setText(newName);
    }
}
