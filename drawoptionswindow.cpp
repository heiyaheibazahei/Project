#include "drawoptionswindow.h"
#include "config.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>

DrawOptionsWindow::DrawOptionsWindow(QWidget *parent)
    : QWidget(parent)
{
    // 设置窗口属性
    setWindowTitle("抽奖选项");
    setFixedSize(WIN_WIDTH, WIN_HEIGHT);
    setWindowIcon(QPixmap(WIN_PATH));

    isLottery=false; // 默认点名模式

    // 创建标题
    title = new QLabel("选择抽奖模式", this);
    QFont titleFont("Microsoft YaHei", 28, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "QLabel {"
        "  color: #e67e22;"
        "  background-color: rgba(255, 255, 255, 0.85);"
        "  border-radius: 15px;"
        "  padding: 15px;"
        "}"
    );

    // 创建选项按钮
    createOptionButtons();

    // 返回按钮
    backButton = new QPushButton("返回主菜单", this);
    backButton->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    backButton->setStyleSheet(
                "QPushButton {"
                                 "  font-size: 18px;"
                                "  font-weight: bold;"
                                "  color: white;"
                                 "  padding: 12px 25px;"
                                  "  border-radius: 12px;"
                                   "  border: 2px solid #34495e;" // 深色边框
                                      "  background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
                                     "   stop:0 #e74c3c, stop:1 #c0392b);" // 红色渐变色
                                    "}"
                                   "QPushButton:hover {"
                                      "  background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
                                       "            stop:0 #ff6b6b, stop:1 #e74c3c);" // 鼠标悬停时亮红色
                                  "  border: 2px solid #2c3e50;"
                                 "}"
                               "QPushButton:pressed {"
                              "  background-color: #c0392b;" // 按下时的颜色
                               "}"
    );
    // 布局管理
    setupLayout();
    // 连接信号和槽
    connect(singleDraw,&QPushButton::clicked,this,&DrawOptionsWindow::upperButton);
    connect(multiDraw,&QPushButton::clicked,this,&DrawOptionsWindow::bottomButton);
    connect(backButton, &QPushButton::clicked, this, &DrawOptionsWindow::backButtonClicked);

    // 初始化状态
    currentState = DrawState::Initial;
    updateUIForState(); // 根据初始状态设置UI

    // 设置背景
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QBrush(QPixmap(MAINMENU_PATH).scaled(WIN_WIDTH, WIN_HEIGHT)));
    this->setPalette(palette);
}

void DrawOptionsWindow::createOptionButtons()
{
        singleDraw = new QPushButton("单人点名", this);
        multiDraw = new QPushButton("多人点名", this);

        QString buttonStyle = "QPushButton {"
                                  "  font-size: 22px;"
                                  "  font-weight: bold;"
                                  "  color: white;"
                                  "  padding: 15px 30px;"
                                  "  border-radius: 15px;"
                                  "  border: 2px solid #34495e;" // 深色边框
                                  "  background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
                                  "            stop:0 #3498db, stop:1 #2980b9);" // 蓝色渐变色
                                  "}"
                                  "QPushButton:hover {"
                                  "  background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
                                  "            stop:0 #3cb0fd, stop:1 #3498db);" // 鼠标悬停时亮蓝色
                                  "  border: 2px solid #2c3e50;"
                                  "}"
                                  "QPushButton:pressed {"
                                  "  background-color: #2980b9;" // 按下时的颜色
                                  "}";
        QString backButtonStyle = "QPushButton {"
                                      "  font-size: 18px;"
                                      "  font-weight: bold;"
                                      "  color: white;"
                                      "  padding: 12px 25px;"
                                      "  border-radius: 12px;"
                                      "  border: 2px solid #34495e;" // 深色边框
                                      "  background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
                                      "            stop:0 #e74c3c, stop:1 #c0392b);" // 红色渐变色
                                      "}"
                                      "QPushButton:hover {"
                                      "  background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
                                      "            stop:0 #ff6b6b, stop:1 #e74c3c);" // 鼠标悬停时亮红色
                                      "  border: 2px solid #2c3e50;"
                                      "}"
                                      "QPushButton:pressed {"
                                      "  background-color: #c0392b;" // 按下时的颜色
                                      "}";

    // 设置按钮样式
    singleDraw->setStyleSheet(buttonStyle);
    //"background-color: #3498db;"
    multiDraw->setStyleSheet(buttonStyle);
   // backButton->setStyleSheet(backButtonStyle);

    // 设置按钮大小
    int btnWidth = static_cast<int>(BUTTON_WIDTH);
    int btnHeight = static_cast<int>(BUTTON_HEIGHT);

    //固定大小
    singleDraw->setFixedSize(btnWidth, btnHeight);
    multiDraw->setFixedSize(btnWidth, btnHeight);
}

void DrawOptionsWindow::setupLayout()
{
    // 主垂直布局
    mainLayout = new QVBoxLayout(this);

    // 创建按钮布局（垂直排列）
    buttonLayout = new QVBoxLayout();

    // 垂直伸缩器 - 顶部占位
    mainLayout->addStretch(1);

    // 添加主按钮到按钮布局
    buttonLayout->addWidget(singleDraw, 0, Qt::AlignHCenter);
    buttonLayout->addSpacing(30); // 按钮间距

    buttonLayout->addWidget(multiDraw, 0, Qt::AlignHCenter);
    buttonLayout->addSpacing(60); // 更多的间距到返回按钮

    // 将按钮布局添加到主布局中，居中显示
    mainLayout->addLayout(buttonLayout, 1); // 第二个参数设置为1表示该部分会扩展

    // 创建返回按钮的水平布局（让它能停留在底部）
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch(); // 左侧弹性空间
    bottomLayout->addWidget(backButton);
    bottomLayout->addStretch(); // 右侧弹性空间

    // 添加底部布局到主布局
    mainLayout->addLayout(bottomLayout);

    // 添加底部弹性空间
    mainLayout->addStretch(1);

    // 设置主布局
    setLayout(mainLayout);

    // 设置布局间距和边距
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(0);
    mainLayout->setContentsMargins(20, 20, 20, 40); // 整体窗口的边距
    mainLayout->setSpacing(30);
}

//---------------------------------------------
void DrawOptionsWindow::upperButton() {
    switch (currentState) {
        case DrawState::Initial:
            currentState = DrawState::Single_ChooseGroup;
            break;
        case DrawState::Single_ChooseGroup:
            // group_flag = 1
            currentState = DrawState::ReadyToStart;
            break;
        case DrawState::Multi_ChooseRepeat:
            // repeat_flag = 1
            currentState = DrawState::Multi_ChooseGroup;
            break;
        case DrawState::Multi_ChooseGroup:
            // group_flag = 1
            currentState = DrawState::ReadyToStart;
            break;
        case DrawState::ReadyToStart:
            // TODO: 在这里添加“开始抽奖/点名”的逻辑
            qDebug() << "开始" << (isLottery ? "抽奖" : "点名");
            // lottery();
            break;
    }
    updateUIForState();
}

//----------------------------------------
void DrawOptionsWindow::bottomButton() {
    switch (currentState) {
        case DrawState::Initial:
            currentState = DrawState::Multi_ChooseRepeat;
            break;
        case DrawState::Single_ChooseGroup:
            // group_flag = 0
            currentState = DrawState::ReadyToStart;
            break;
        case DrawState::Multi_ChooseRepeat:
            // repeat_flag = 0
            currentState = DrawState::Multi_ChooseGroup;
            break;
        case DrawState::Multi_ChooseGroup:
            // group_flag = 0
            currentState = DrawState::ReadyToStart;
            break;
        case DrawState::ReadyToStart:
            // “重新设置模式”按钮
            resetToInitialState();
            break;
    }
    updateUIForState();
}

void DrawOptionsWindow::backButtonClicked() {
    switch (currentState) {
        case DrawState::Initial:
            emit backToMainMenu();
            break;
        case DrawState::Single_ChooseGroup:
        case DrawState::Multi_ChooseRepeat:
            currentState = DrawState::Initial;
            break;
        case DrawState::Multi_ChooseGroup:
            currentState = DrawState::Multi_ChooseRepeat;
            break;
        case DrawState::ReadyToStart:
            // 从准备界面返回时，需要判断是从单人流程还是多人流程来的
            // 为了简单起见，我们统一回到初始状态，或者你可以引入更复杂的前一状态记录
            resetToInitialState(); // 直接重置更简单
            break;
    }
    updateUIForState();
}

// 实现重置函数
void DrawOptionsWindow::resetToInitialState() {
    currentState = DrawState::Initial;
    // 如果需要，在这里重置记录选择的变量，如 group_flag 等
    updateUIForState();
}

void DrawOptionsWindow::updateUIForState()
{
    QString modeText = isLottery ? "抽奖" : "点名";

    switch (currentState) {
        case DrawState::Initial:
            title->setText("选择" + modeText + "模式");
            singleDraw->setText("单人" + modeText);
            multiDraw->setText("多人" + modeText);
            backButton->setText("返回主菜单");
            singleDraw->show();
            multiDraw->show();
            break;

        case DrawState::Single_ChooseGroup:
            title->setText("是否需要分组?");
            singleDraw->setText("分组" + modeText);
            multiDraw->setText("不分组" + modeText);
            backButton->setText("返回上一级");
            singleDraw->show();
            multiDraw->show();
            break;

        case DrawState::Multi_ChooseRepeat:
            title->setText("是否允许重复?");
            singleDraw->setText("允许重复" + modeText);
            multiDraw->setText("不允许重复" + modeText);
            backButton->setText("返回上一级");
            singleDraw->show();
            multiDraw->show();
            break;

        case DrawState::Multi_ChooseGroup:
            title->setText("是否需要分组?");
            singleDraw->setText("分组" + modeText);
            multiDraw->setText("不分组" + modeText);
            backButton->setText("返回上一级");
            singleDraw->show();
            multiDraw->show();
            break;

        case DrawState::ReadyToStart:
            title->setText("设置完成");
            singleDraw->setText("开始" + modeText);
            multiDraw->setText("重新设置模式");
            backButton->setText("返回上一级");
            singleDraw->show();
            multiDraw->show();
            break;
    }
}
