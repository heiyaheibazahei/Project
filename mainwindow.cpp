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
#include <QVector>
#include "luckyrecordswindow.h"
#include <QTime>
#include <cstdlib>
#include <QMediaPlayer>
QVector<QString> currentNameList;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    //在构造函数之中新增添一个初始化指针

    fileSystem(new FileSystem(this)), // 添加文件系统实例
  drawOptionsWindow(nullptr)
{
    qsrand(QTime::currentTime().msec());
    ui->setupUi(this);


    //主界面的初始化
    m_modeCheckBox=new QCheckBox("切换模式",this);
    drawOptionsWindow = new DrawOptionsWindow();
    background=MAINMENU_PATH;
    mainMenuinitialize();
    setMenuBackground();
    setMenuTitle();


    // 初始化并播放背景音乐
    m_backgroundMusicPlayer = new QMediaPlayer(this);
    m_backgroundMusicPlayer->setMedia(QUrl(BACKGROUND_MUSIC_PATH));
    // 设置循环播放
    connect(m_backgroundMusicPlayer, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            m_backgroundMusicPlayer->play();
        }
    });
    m_backgroundMusicPlayer->play();
    // 添加其余按钮功能

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
    start= new QPushButton("点名,启动!",this);
    startButton();

    //再下面实现导入文件的按钮
    import= new QPushButton("导入名单",this);
    importButton_1();

    //之后就是文件管理的按钮
    fileManage= new QPushButton("管理文档",this);
    fileManageButton();

    //最后是中奖记录的按钮
    luckyRecords=new QPushButton("抽取记录",this);
    luckyRecordsButton();

    changeBackground=new QPushButton("更改背景",this);
    changeBackgroundButton();


}

MainWindow::~MainWindow()
{
    //析构函数的重写
    if (drawOptionsWindow) {
        delete drawOptionsWindow;
    }
    delete ui;
}

void MainWindow::changeBackgroundButton(){
    changeBackground->setFixedSize(120,40);
    changeBackground->move(WIN_WIDTH-changeBackground->width(),WIN_HEIGHT-changeBackground->height());
    changeBackground->setStyleSheet(
        "QPushButton {"
        "  background-color: #FF69B4;"
        "  color: white;"
        "  border-radius: 10px;"
        "  font-size: 16px;"
        "  padding: 10px 20px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #FF1493;"
        "}"
        );
    connect(changeBackground,&QPushButton::clicked,this,&MainWindow::changeBackgroundClicked);
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
    connect(m_modeCheckBox,&QCheckBox::clicked,this,&MainWindow::onModeChanged);
    m_modeCheckBox->setStyleSheet(R"(
                                     /* 圆形指示器样式 */
                                      QCheckBox::indicator {
                                      width: 22px;
                                      height: 22px;
                                      border-radius: 11px; /* 宽度的一半，形成圆形 */
                                      border: 2px solid #ddd;
                                      background-color: white;
                                      }
                                      /* 勾选状态样式 */
                                     QCheckBox::indicator:checked {
                                         background-color: red;
                                         border-color: red;
                                     }
                                     QCheckBox {
                                         font-size: 14px;
                                         color: white;
                                         cursor: pointer;
                                     }
                                 )");
    QPalette palette_2 = drawOptionsWindow->palette();
    palette_2.setBrush(QPalette::Window, QBrush(QPixmap(background).scaled(WIN_WIDTH, WIN_HEIGHT)));
    drawOptionsWindow->setPalette(palette_2);
}

void MainWindow::setMenuBackground(){
    // 设置主窗口背景
        QPalette palette = this->palette();
       palette.setBrush(QPalette::Window, QBrush(QPixmap(background).scaled(WIN_WIDTH, WIN_HEIGHT)));
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
    }

    //按钮会发出clicked信号，对应槽函数又会发出backToMenu的信号，再调用backToMainWindow的槽函数
    connect(drawOptionsWindow, &DrawOptionsWindow::backToMainMenu,
            this, &MainWindow::backToMainWindow);

    // 切换到抽奖窗口前，暂停背景音乐
    m_backgroundMusicPlayer->pause();

    // 新增：在显示窗口前，将当前名单传递过去
    drawOptionsWindow->setNamesList(this->currentNameList);
    this->hide();// 隐藏主窗口
    drawOptionsWindow->setNamesList(this->currentNameList);

    // 新增：传递FileSystem实例
    drawOptionsWindow->setFileSystem(this->fileSystem); // 需在DrawOptionsWindow中添加setFileSystem方法

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
    // 返回主窗口时，继续播放背景音乐
    m_backgroundMusicPlayer->play();
    this->show();

}

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
//    connect(&dialog, &ManageDocumentsDialog::projectSelected,
//            this, &MainWindow::showProjectContent);
    connect(&dialog, &ManageDocumentsDialog::projectSelected,
            this, &MainWindow::loadProjectForUse);

    connect(&dialog, &ManageDocumentsDialog::projectDeleted,
            fileSystem, &FileSystem::deleteProject);

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

void MainWindow::loadProjectForUse(const QString& projectName)
{
    // 从文件系统加载名单
    QStringList names = fileSystem->loadProject(projectName);

    if (names.isEmpty()) {
        QMessageBox::warning(this, "加载失败", QString("项目 '%1' 是空的或不存在。").arg(projectName));
        return;
    }

    // 将加载的名单更新为当前使用中的名单
    this->currentNameList = names;

    // 弹窗告诉用户加载成功
    QMessageBox::information(this, "加载成功",
                             QString("项目 '%1' 已成功加载，包含 %2 个名字。\n现在可以开始抽奖了。")
                             .arg(projectName)
                             .arg(names.size()));
}

void MainWindow::onModeChanged() {
    if(m_isLotteryMode) {
        m_isLotteryMode=false;
        drawOptionsWindow->isLottery=false;
    }
    else {
        m_isLotteryMode=true;
        drawOptionsWindow->isLottery=true;
    }
    // 切换按钮文本与勾选框提示
    if (m_isLotteryMode) {
        QMessageBox::information(this,"模式切换提示","已切换至抽奖模式");
        start->setText("抽奖,启动!");         // 启动按钮显示抽奖
        drawOptionsWindow->singleDraw->setText("单人抽奖");
        drawOptionsWindow->multiDraw->setText("多人抽奖");
    } else {
        QMessageBox::information(this,"模式切换提示","已切换至点名模式");
        start->setText("点名,启动!");   // 启动按钮显示点名
        drawOptionsWindow->singleDraw->setText("单人点名");
        drawOptionsWindow->multiDraw->setText("多人点名");

    }
}

void MainWindow::showLuckyRecords() {
    LuckyRecordsWindow *window = new LuckyRecordsWindow(this);
    window->show();
}

void MainWindow::luckyRecordsButton(){
    luckyRecords->setStyleSheet(R"(
        QPushButton {
            background-color: #FF9800;
            color: white;
            border-radius: 10px;
            font-size: 16px;
            padding: 10px 20px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #F57C00;
        }
    )");
    luckyRecords->resize(120,40);
    luckyRecords->move(this->width()-luckyRecords->width(),0);
     connect(luckyRecords, &QPushButton::clicked, this, &MainWindow::showLuckyRecords);
}

QString MainWindow::getRandomName() {
    if (currentNameList.isEmpty()) return "";

    // 使用 qrand() 替代 QRandomGenerator
    int index = qrand() % currentNameList.size();
    return currentNameList[index];
}

QStringList MainWindow::getRandomNames(int count) {
    QStringList tempList = currentNameList;
    QStringList winners;

    for (int i = 0; i < count && !tempList.isEmpty(); ++i) {
        // 使用 qrand() 替代 QRandomGenerator
        int index = qrand() % tempList.size();
        winners.append(tempList.takeAt(index));
    }

    return winners;
}

void MainWindow::changeBackgroundClicked(){
    background= QFileDialog::getOpenFileName(this,"选择图片",QDir::homePath(),"(*.jpg *.png)");
    if(background==NULL){
        background=MAINMENU_PATH;
    }
    qDebug()<<background<<endl;

    QPixmap pixmap(background);
    if (pixmap.isNull()) {
        qDebug() << "QPixmap 加载失败或为空";
        return;
    }
    //QFile *backgroundFile= new QFile(background);
    QPalette palette=this->palette();
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(WIN_WIDTH, WIN_HEIGHT)));
    this->setPalette(palette);
    drawOptionsWindow->setPalette(palette);
}
