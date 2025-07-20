#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include <QIcon>
#include <QMessageBox>
#include <QFontDatabase>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>
#include "drawoptionswindow.h"
#include <QFile>
#include <QFileDialog>
#include <QList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    //在构造函数之中新增添一个初始化指针

    fileSystem(new FileSystem(this)), // 添加文件系统实例
  drawOptionsWindow(nullptr)
{
    ui->setupUi(this);


    //主界面的初始化
    mainMenuinitialize();
    setMenuBackground();
    setMenuTitle();
    // 添加其余按钮功能

    //文件系统的初始化
    //setupFileManager();

    // 连接文件系统信号
       connect(fileSystem, &FileSystem::projectSaved, [this](const QString& projectName) {
           QMessageBox::information(this, "保存成功",
                                   QString("项目 '%1' 已保存").arg(projectName));
       });

       connect(fileSystem, &FileSystem::projectDeleted, [this](const QString& projectName) {
               QMessageBox::information(this, "已删除",
                                       QString("项目 '%1' 已删除").arg(projectName));
           });

    // 最下方实现退出按钮
    exit = new QPushButton("退出", this);
    exitButton();

    //随后最上面实现开始抽奖按钮
    start= new QPushButton("抽奖,启动!",this);
    startButton();

    //再下面实现导入文件的按钮
    import= new QPushButton("导入名单",this);
    importButton_1();

    //最后就是文件管理的按钮
    fileManage= new QPushButton("管理文档",this);
    fileManageButton();
}

MainWindow::~MainWindow()
{
    //析构函数的重写
    if (drawOptionsWindow) {
        delete drawOptionsWindow;
    }
    delete ui;
}

void MainWindow::exitButton()
{
    // 设置按钮位置和大小
    setSize(*exit);
    exit->move(static_cast<int>(WIN_WIDTH * 0.5 - exit->width() * 0.5),
               WIN_HEIGHT - BUTTON_HEIGHT - BOTTOM_MARGIN);  // 底部留边

    // 设置按钮样式
    exit->setStyleSheet(
        "QPushButton {"
        "  background-color: #e7495a;"
        "  color: white;"
        "  border-radius: 10px;"
        "  font-size: 24px;"
        "  padding: 10px 20px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #c03040;"
        "}"
    );

    // 连接点击事件
    connect(exit, &QPushButton::clicked, this, [this]() {

        // 创建对话框
        QMessageBox exitDialog;
        exitDialog.setWindowTitle("确认退出");
        exitDialog.setText("你真的要退出吗?");
        exitDialog.setIconPixmap(QPixmap(SURETOEXIT_PATH).scaled(64, 64, Qt::KeepAspectRatio));
        exitDialog.setWindowIcon(QPixmap(WIN_PATH));

        // 添加按钮
        QAbstractButton* exitBtn = exitDialog.addButton("退出", QMessageBox::AcceptRole);
        QAbstractButton* cancelBtn = exitDialog.addButton("还是不退出了", QMessageBox::RejectRole);

        exitBtn->setStyleSheet("background-color: #e7495a; color: white;");
        cancelBtn->setStyleSheet("background-color: #4CAF50; color: white;");

        // 显示对话框
        //不能用show函数，如果是show函数那么只会一闪而过
        exitDialog.exec();

        if (exitDialog.clickedButton() == exitBtn) {
            this->close(); // 如果按下退出键那就关闭主窗口
        }
        // 如果点击取消按钮或关闭对话框，不做任何操作
    });
}

void MainWindow::mainMenuinitialize(){
    // 窗口的设置
    setWindowTitle("抽奖系统");
    setFixedSize(WIN_WIDTH, WIN_HEIGHT);
    setWindowIcon(QIcon(WIN_PATH));
}

void MainWindow::setMenuBackground(){
    // 设置主窗口背景
        QPalette palette = this->palette();
       palette.setBrush(QPalette::Window, QBrush(QPixmap(MAINMENU_PATH).scaled(WIN_WIDTH, WIN_HEIGHT)));
       this->setPalette(palette);
}

void MainWindow::startButton(){
    setSize(*start);
    start->move(static_cast<int>(WIN_WIDTH*0.5-start->width()*0.5),BUTTON_HEIGHT*3);

    start->setStyleSheet(
        "QPushButton {"
        "  background-color: #2ecc71;"
        "  color: white;"
        "  border-radius: 10px;"
        "  font-size: 24px;"
        "  padding: 10px 20px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #27ae60;"
        "}"
    );

    // 添加按钮点击事件
    connect(start, &QPushButton::clicked, this, &MainWindow::showDrawOptions);
}

void MainWindow::setMenuTitle(){
    QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        //添加中央部件

        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
            mainLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
            mainLayout->setSpacing(50);
            mainLayout->setContentsMargins(0, TOP_MARGIN, 0, 0);
            //创建主布局

            QLabel *titleLabel = new QLabel("多功能抽奖和点名系统", centralWidget);
                titleLabel->setAlignment(Qt::AlignCenter);

                // 控制字体
                QFont mainTitleFont("Arial", 20, QFont::Bold);
                titleLabel->setFont(mainTitleFont);

                // 设置样式 - 深蓝色调更显正式
                titleLabel->setStyleSheet(
                    "QLabel {"
                    "  color: red;"  // 深蓝灰色
                    "  padding-top: -px;"
                    "  background: transparent;"  // 透明背景
                    "}"
                );

                //设置字体阴影
                QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
                    effect->setBlurRadius(8);
                    effect->setOffset(4);
                    effect->setColor(QColor(0, 0, 0, 160));
                    titleLabel->setGraphicsEffect(effect);

         mainLayout->addWidget(titleLabel);
                //添加title
}

void MainWindow::setSize(QPushButton &btn){
    btn.setFixedSize(BUTTON_WIDTH,BUTTON_HEIGHT);
}

void MainWindow::importButton_1(){
    setSize(*import);
    import->move(static_cast<int>(WIN_WIDTH*0.5-import->width()*0.5),BUTTON_HEIGHT*4.7);
    import->setStyleSheet(
        "QPushButton {"
        "  background-color: blue;"
        "  color: white;"
        "  border-radius: 10px;"
        "  font-size: 24px;"
        "  padding: 10px 20px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: midnightblue;"
        "}"
    );
    connect(import, &QPushButton::clicked, this, &MainWindow::onImportButtonClicked);
}

void MainWindow::fileManageButton(){
    setSize(*fileManage);
    fileManage->move(static_cast<int>(WIN_WIDTH*0.5-fileManage->width()*0.5),BUTTON_HEIGHT*6.5);
    fileManage->setStyleSheet(
        "QPushButton {"
        "  background-color: darkgrey;"
        "  color: white;"
        "  border-radius: 10px;"
        "  font-size: 24px;"
        "  padding: 10px 20px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: grey;"
        "}"
    );
    connect(fileManage, &QPushButton::clicked, this, &MainWindow::onManageButtonClicked);

}

void MainWindow::showDrawOptions()
{
    if (!drawOptionsWindow) {
        drawOptionsWindow = new DrawOptionsWindow();

        //如果在抽奖选项页面选择返回主菜单的话，那么首先点击按钮
        //按钮会发出clicked信号，对应槽函数又会发出backToMenu的信号，再调用backToMainWindow的槽函数
        connect(drawOptionsWindow, &DrawOptionsWindow::backToMainMenu,
                this, &MainWindow::backToMainWindow);
    }

    // 隐藏主窗口
    this->hide();

    // 显示抽奖选项窗口
    drawOptionsWindow->show();
}

void MainWindow::backToMainWindow()
{
    //如果这个drawOptinosWindow被打开了，那么它就隐藏
    //转换到mainwindow打开
    if (drawOptionsWindow) {
        drawOptionsWindow->hide();
    }
    this->show();
}

/*void MainWindow::setupFileManager()
{
    // 导入名单按钮
    importButton = new QPushButton("导入名单", this);
    importButton->setObjectName("importButton");
    importButton->move(static_cast<int>(WIN_WIDTH*0.5-import->width()*0.5),BUTTON_HEIGHT*4.7);
    setSize(*importButton);
    importButton->setStyleSheet("font-size: 16px; font-weight: bold;");
    importButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #e7495a;"
        "  color: white;"
        "  border-radius: 10px;"
        "  font-size: 24px;"
        "  padding: 10px 20px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #c03040;"
        "}"
    );
    connect(importButton, &QPushButton::clicked, this, &MainWindow::onImportButtonClicked);

    // 管理文档按钮
    manageButton = new QPushButton("管理文档", this);
    manageButton->setObjectName("manageButton");
    setSize(*manageButton);
    manageButton->move(static_cast<int>(WIN_WIDTH*0.5-import->width()*0.5),BUTTON_HEIGHT*6.5);
    manageButton->setStyleSheet("font-size: 16px; font-weight: bold;");
    connect(manageButton, &QPushButton::clicked, this, &MainWindow::onManageButtonClicked);

    // 根据原有布局添加到合适位置
   /* // 假设我们添加到主界面的底部
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(importButton);
    buttonLayout->addWidget(manageButton);

    // 查看原有代码使用的布局
    // 这里需要根据实际情况添加到布局中
    if (ui->verticalLayout) {
        ui->verticalLayout->addLayout(buttonLayout);
    } else {
        // 创建新的布局并添加
        QWidget *buttonWidget = new QWidget(this);
        buttonWidget->setLayout(buttonLayout);
        ui->verticalLayout->addWidget(buttonWidget);
    }

}*/

void MainWindow::onImportButtonClicked()
{
    EditDialog dialog(this);
    dialog.setNameList(currentNameList);

    // 连接保存信号
    connect(&dialog, &EditDialog::projectSaved,
            this, &MainWindow::handleProjectSaved);

    if (dialog.exec() == QDialog::Accepted) {
        currentNameList = dialog.getNameList();
    }
}

void MainWindow::onManageButtonClicked()
{
    // 获取固定路径下的项目列表
    QStringList projects = fileSystem->getProjectNames();

    ManageDocumentsDialog dialog(this);
    dialog.setProjectsList(projects);

    // 连接信号
    connect(&dialog, &ManageDocumentsDialog::projectSelected,
            this, &MainWindow::showProjectContent);

    connect(&dialog, &ManageDocumentsDialog::projectDeleted,
            fileSystem, &FileSystem::deleteProject);

    connect(&dialog,&ManageDocumentsDialog::projectImportedToList,
            this,&MainWindow::onProjectImportedToList);

    dialog.exec();
}

void MainWindow::showProjectContent(const QString& projectName)
{
    // 从文件系统加载项目
    QStringList names = fileSystem->loadProject(projectName);

    // 在编辑对话框中显示
    EditDialog dialog(this);
    dialog.setNameList(names);
    dialog.setWindowTitle("查看项目: " + projectName);

    // 禁用编辑功能（只查看）
    dialog.setEnabled(false);

    dialog.exec();
}

void MainWindow::handleProjectSaved(const QString &projectName, const QStringList &names)
{
    // 使用FileSystem保存项目
    if (fileSystem->saveProject(projectName, names)) {
        QMessageBox::information(this, "保存成功",
                                QString("项目 '%1' 已保存到固定路径").arg(projectName));
    } else {
        QMessageBox::warning(this, "保存失败", "保存项目失败，请重试");
    }
}

void MainWindow::onProjectImportedToList(const QString &projectName)
{
    // 1. 从文件系统加载选中项目的名单（复用原有loadProject函数）
    QStringList names = fileSystem->loadProject(projectName);
    if (names.isEmpty()) {
        QMessageBox::warning(this, "导入失败", "项目中没有有效名单");
        return;
    }

    // 2. 打开“导入名单”界面（EditDialog），设置加载的名单（复用原有setNameList函数）
    EditDialog dialog(this);
    dialog.setNameList(names); // 将项目名单设置到“导入名单”界面
    dialog.setWindowTitle("导入名单 - 来自项目: " + projectName); // 可选：修改窗口标题以区分

    // 3. 连接保存信号（复用原有handleProjectSaved函数，保持保存逻辑不变）
    connect(&dialog, &EditDialog::projectSaved, this, &MainWindow::handleProjectSaved);

    // 4. 显示“导入名单”界面（允许用户编辑）
    if (dialog.exec() == QDialog::Accepted) {
        // 可选：更新主窗口的当前名单（根据原有逻辑，可保留）
        currentNameList = dialog.getNameList();
    }
}
