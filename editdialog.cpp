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
#include "xlsxdocument.h"

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
    clearButton = new QPushButton("清空名单", this);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(clearButton);

    // 导入按钮（带菜单）
    importButton = new QPushButton("导入...", this);
    QMenu *importMenu = new QMenu(this);
    importMenu->addAction("导入文本文件 (.txt)", this, &EditDialog::importTxt);
    importMenu->addAction("导入CSV文件 (.csv)", this, &EditDialog::importCsv);
    importMenu->addAction("导入Excel文件 (.xlsx)", this, &EditDialog::importXlsx);
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
    connect(clearButton, &QPushButton::clicked, this, &EditDialog::clearList);
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
    QString filePath = QFileDialog::getOpenFileName(this, "选择CSV文件", "", "CSV文件 (*.csv)");
    if (filePath.isEmpty()) return;

    bool ok; // <--- 用于检查用户是否点击了 "OK"
    int columnIndex = QInputDialog::getInt(
        this,
        "选择姓名列",
        "请输入姓名所在列的序号 (第1列填1，第2列填2...):",
        1, 1, 100, 1, &ok
    );

    if (!ok) return; // <--- 如果用户点击 "Cancel"，则直接返回

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "导入失败", "无法打开文件");
        return;
    }

    QStringList newNames;
    QTextStream in(&file);
    in.setCodec("UTF-8");

    bool isFirstLine = true; // 增加一个选项来询问用户是否跳过第一行（标题行）
    int ret = QMessageBox::question(this, "跳过标题行？", "文件第一行是否为标题行（如“姓名”，“学号”等）？\n选择“是”将跳过第一行。", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (ret == QMessageBox::No) {
        isFirstLine = false;
    }

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (isFirstLine) { // 如果需要跳过标题行，则读完第一行后就跳过本次循环
            isFirstLine = false;
            continue;
        }

        if (line.trimmed().isEmpty()) continue;

        QStringList fields = line.split(',');
        if (fields.size() >= columnIndex) {
            QString name = fields[columnIndex - 1].trimmed();
            // 移除可能存在的引号
            if (name.startsWith('"') && name.endsWith('"')) {
                name = name.mid(1, name.length() - 2);
            }
            if (!name.isEmpty()) {
                newNames.append(name);
            }
        }
    }
    file.close();

    if (!newNames.isEmpty()) {
        QString result = QString("成功从CSV文件导入 %1 个名字").arg(newNames.size());
        nameList += newNames; // 追加到当前名单
        setNameList(nameList);
        QMessageBox::information(this, "导入成功", result);
    } else {
        QMessageBox::warning(this, "导入失败", "在指定列没有找到有效的姓名记录");
    }
}

void EditDialog::importXlsx()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择Excel文件", "", "Excel 文件 (*.xlsx)");
    if (filePath.isEmpty()) return;

    bool ok;
    int columnIndex = QInputDialog::getInt(
        this,
        "选择姓名列",
        "请输入姓名所在列的序号 (A列填1, B列填2...):",
        1, 1, 100, 1, &ok
    );

    if (!ok) return;

    // 使用 QXlsx 打开文档
    QXlsx::Document xlsx(filePath);
    if (!xlsx.load()) {
        QMessageBox::warning(this, "导入失败", "无法打开或解析Excel文件。\n请确保文件未被其他程序占用。");
        return;
    }

    int ret = QMessageBox::question(this, "跳过标题行？", "文件第一行是否为标题行？\n选择“是”将跳过第一行。", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    bool skipFirstLine = (ret == QMessageBox::Yes);

    QStringList newNames;
    // QXlsx 从 1 开始索引行和列
    for (int row = (skipFirstLine ? 2 : 1); row <= xlsx.dimension().lastRow(); ++row) {

        // =======================================================
        //  ↓↓↓  这就是被修复的关键一行  ↓↓↓
        // =======================================================
        auto cell = xlsx.cellAt(row, columnIndex);

        if (cell) { // 检查智能指针是否有效（和检查裸指针不为nullptr一样）
            QString name = cell->value().toString().trimmed(); // 使用方式也一样
            if (!name.isEmpty()) {
                newNames.append(name);
            }
        }
    }

    if (!newNames.isEmpty()) {
        QString result = QString("成功从Excel文件导入 %1 个名字").arg(newNames.size());
        nameList += newNames; // 追加到当前名单
        setNameList(nameList);
        QMessageBox::information(this, "导入成功", result);
    } else {
        QMessageBox::warning(this, "导入失败", "在指定列没有找到有效的姓名记录");
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

void EditDialog::clearList()
{
    // 如果列表本身就是空的，就没必要继续了
    if (nameList.isEmpty()) {
        return;
    }

    // 弹出一个确认对话框，给用户一个反悔的机会
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认清空",
                                  "您确定要清空当前名单中的所有名字吗？\n此操作无法撤销。",
                                  QMessageBox::Yes | QMessageBox::No);

    // 只有当用户点击 "Yes" 按钮时，才执行清空操作
    if (reply == QMessageBox::Yes) {
        // 1. 清空数据模型
        nameList.clear();
        // 2. 清空界面显示
        listWidget->clear();
    }
}
