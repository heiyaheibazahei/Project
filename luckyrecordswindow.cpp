#include "luckyrecordswindow.h"
#include <QTextCodec>
#include <QTextStream>
LuckyRecordsWindow::LuckyRecordsWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("中奖记录");
    setFixedSize(800, 600);

    // 创建布局和列表
    QVBoxLayout *layout = new QVBoxLayout(this);
    recordsList = new QListWidget(this);
    layout->addWidget(recordsList);

    // 加载记录
    loadRecords();
    backToMenu=new QPushButton("关闭",this);
    connect(backToMenu,&QPushButton::clicked,this,&LuckyRecordsWindow::back);
    backToMenu->move(400-backToMenu->width()*0.5,600-backToMenu->height()*1.5);
}

void LuckyRecordsWindow::loadRecords() {
    // 确保目录存在
    QDir dir(luckyPath);
    if (!dir.exists()) {
        dir.mkpath(luckyPath);
    }

    // 获取所有记录文件
    QStringList filters;
    filters << "lucky_*.txt";
    QStringList recordFiles = dir.entryList(filters, QDir::Files, QDir::Time);

    if (recordFiles.isEmpty()) {
        recordsList->addItem("暂无记录");
        return;
    }

    // 显示记录文件
    for (const QString &file : recordFiles) {
        recordsList->addItem(file);
    }

    // 双击查看详情
    connect(recordsList, &QListWidget::itemDoubleClicked, [this, dir](QListWidgetItem *item) {
        QString filePath = dir.filePath(item->text());
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            in.setCodec("UTF-8"); // 设置编码为 UTF-8,解决乱码问题
            QMessageBox::information(this, "记录详情", in.readAll());
            file.close();
        } else {
            QMessageBox::warning(this, "错误", "无法打开记录文件");
        }
    });
}

void LuckyRecordsWindow::back(){
    this->hide();
}
