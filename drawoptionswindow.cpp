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

    multi_flag=-1;
    repeat_flag=-1;
    group_flag=-1;
    isLottery=false;
    //三个flag的初始化

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
    connect(backButton, &QPushButton::clicked, this, &DrawOptionsWindow::backButtonClicked);
    connect(singleDraw,&QPushButton::clicked,this,&DrawOptionsWindow::upperButton);
    connect(multiDraw,&QPushButton::clicked,this,&DrawOptionsWindow::bottomButton);

    // 布局管理
    setupLayout();

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

//---------------------------------------
void DrawOptionsWindow::backButtonClicked()
{
    if(multi_flag==-1&&repeat_flag==-1&&group_flag==-1){
        multi_flag==-1;
        repeat_flag==-1;
        group_flag==-1;
        emit backToMainMenu();
    }
    else if(multi_flag==1&&repeat_flag==-1&&group_flag==-1&&!isLottery){
        //点名模式，选择了单人点名模式之后选择返回上一级
        multi_flag=-1;
        singleDraw->setText("单人点名");
        multiDraw->setText("多人点名");
        backButton->setText("返回主菜单");
    }
    else if(multi_flag==1&&repeat_flag==-1&&group_flag==-1&&isLottery){
        //抽奖模式，选择了单人抽奖之后选择返回上一级
        multi_flag=-1;
        singleDraw->setText("单人抽奖");
        multiDraw->setText("多人抽奖");
        backButton->setText("返回主菜单");
    }
    else if(multi_flag==1&&repeat_flag==-1&&group_flag!=-1&&!isLottery){
        //点名模式，选择了单人点名之后选择任意分组抽奖之后选择返回上一级
        group_flag=-1;
        singleDraw->setText("分组点名");
        multiDraw->setText("不分组点名");
        backButton->setText("返回上一级");
    }
    else if(multi_flag==1&&repeat_flag==-1&&group_flag!=-1&&isLottery){
        //抽奖模式，选择了单人抽奖之后选择返回上一级
        group_flag=-1;
        singleDraw->setText("分组抽奖");
        multiDraw->setText("不分组抽奖");
        backButton->setText("返回上一级");
    }
    else if(multi_flag==0&&repeat_flag==-1&&group_flag==-1&&!isLottery){
        //点名模式，选择了多人点名之后选择返回上一级
        multi_flag=-1;
        singleDraw->setText("单人点名");
        multiDraw->setText("多人点名");
        backButton->setText("返回主菜单");
    }
    else if(multi_flag==0&&repeat_flag==-1&&group_flag==-1&&isLottery){
        //抽奖模式，选择了多人抽奖之后选择返回上一级
        multi_flag=-1;
        singleDraw->setText("单人抽奖");
        multiDraw->setText("多人抽奖");
        backButton->setText("返回主菜单");
    }
    else if(multi_flag==0&&repeat_flag!=-1&&group_flag==-1&&!isLottery){
        //点名模式，选择了多人点名之后选择任意重复点名之后选择返回上一级
        repeat_flag=-1;
        singleDraw->setText("重复点名");
        multiDraw->setText("不重复点名");
        backButton->setText("返回上一级");
    }
    else if(multi_flag==0&&repeat_flag!=-1&&group_flag==-1&&isLottery){
        //抽奖模式，选择了多人抽奖之后选择任意重复抽奖之后选择返回上一级
        repeat_flag=-1;
        singleDraw->setText("重复抽奖");
        multiDraw->setText("不重复抽奖");
        backButton->setText("返回上一级");
    }
    else if(multi_flag==0&&repeat_flag!=-1&&group_flag!=-1&&!isLottery){
        //点名模式，选择了多人点名之后选择任意重复点名之后选择任意分组点名之后选择返回上一级
        group_flag=-1;
        singleDraw->setText("分组点名");
        multiDraw->setText("不分组点名");
        backButton->setText("返回上一级");
    }
    else if(multi_flag==0&&repeat_flag!=-1&&group_flag!=-1&&isLottery){
        //抽奖模式，选择了多人抽奖之后选择任意重复抽奖之后选择任意分组抽奖之后选择返回上一级
        group_flag=-1;
        singleDraw->setText("分组抽奖");
        multiDraw->setText("不分组抽奖");
        backButton->setText("返回上一级");
    }
}

//---------------------------------------------
void DrawOptionsWindow::upperButton(){
    if(multi_flag==-1&&repeat_flag==-1&&group_flag==-1&&!isLottery){
        //点名模式，点击单人点名
        multi_flag=1;
        singleDraw->setText("分组点名");
        multiDraw->setText("不分组点名");
        backButton->setText("返回上一级");
    }
    else if(multi_flag==-1&&repeat_flag==-1&&group_flag==-1&&isLottery){
        //抽奖模式，点击单人抽奖
        multi_flag=1;
        singleDraw->setText("分组抽奖");
        multiDraw->setText("不分组抽奖");
        backButton->setText("返回上一级");
    }
    else if(multi_flag==1&&repeat_flag==-1&&group_flag==-1&&!isLottery){
        //点名模式，点击了单人点名之后点击分组点名
        group_flag=1;
        singleDraw->setText("开始点名");
        multiDraw->setText("重新设置模式");
    }
    else if(multi_flag==1&&repeat_flag==-1&&group_flag==-1&&isLottery){
        //抽奖模式，点击单人抽奖之后点击分组抽奖
        group_flag=1;
        singleDraw->setText("开始抽奖");
        multiDraw->setText("重新设置模式");
    }

    else if(multi_flag==0&&repeat_flag==-1&&group_flag==-1&&!isLottery){
        //点名模式，点击多人点名之后点击重复点名
        repeat_flag=1;
        singleDraw->setText("分组点名");
        multiDraw->setText("不分组点名");
    }

    else if(multi_flag==0&&repeat_flag==-1&&group_flag==-1&&isLottery){
        //抽奖模式，点击多人抽奖之后点击重复抽奖
        repeat_flag=1;
        singleDraw->setText("分组抽奖");
        multiDraw->setText("不分组抽奖");
    }


    else if(multi_flag==0&&repeat_flag!=-1&&group_flag==-1&&!isLottery){
        //点名模式，点击多人点名之后点击任意重复点名之后点击分组点名
        group_flag=1;
        singleDraw->setText("开始抽奖");
        multiDraw->setText("重新设置模式");
    }

    else if(multi_flag==0&&repeat_flag!=-1&&group_flag==-1&&isLottery){
        //抽奖模式，点击多人抽奖之后点击任意重复抽奖之后点击分组抽奖
        group_flag=1;
        singleDraw->setText("开始抽奖");
        multiDraw->setText("重新设置模式");
    }





    else if(multi_flag==1&&repeat_flag==-1&&group_flag==1&&!isLottery){
        //开始单人分组点名
    }

    else if(multi_flag==1&&repeat_flag==-1&&group_flag==1&&isLottery){
        //开始单人分组抽奖
    }

    else if(multi_flag==1&&repeat_flag==-1&&group_flag==1&&!isLottery){
        //开始单人不分组点名
    }

    else if(multi_flag==1&&repeat_flag==-1&&group_flag==1&&isLottery){
        //开始单人不分组抽奖
    }

    else if(multi_flag==0&&repeat_flag==0&&group_flag==0&&!isLottery){
        //开始多人不重复不分组点名
    }

    else if(multi_flag==0&&repeat_flag==0&&group_flag==1&&!isLottery){
        //开始多人不重复 分组点名
    }

    else if(multi_flag==0&&repeat_flag==1&&group_flag==0&&!isLottery){
        //开始多人重复不分组点名
    }

    else if(multi_flag==0&&repeat_flag==1&&group_flag==1&&!isLottery){
        //开始多人重复分组点名
    }

    else if(multi_flag==0&&repeat_flag==0&&group_flag==0&&isLottery){
        //开始多人不重复不分组抽奖
    }

    else if(multi_flag==0&&repeat_flag==0&&group_flag==1&&isLottery){
        //开始多人不重复 分组抽奖
    }

    else if(multi_flag==0&&repeat_flag==1&&group_flag==0&&isLottery){
        //开始多人重复不分组抽奖
    }

    else if(multi_flag==0&&repeat_flag==1&&group_flag==1&&isLottery){
        //开始多人重复分组抽奖
    }


}


//----------------------------------------
void DrawOptionsWindow::bottomButton(){
    if(multi_flag==-1&&repeat_flag==-1&&group_flag==-1&&!isLottery){
        //点名模式，点击多人点名
        multi_flag=0;
        singleDraw->setText("重复点名");
        multiDraw->setText("不重复点名");
        backButton->setText("返回上一级");

    }
    else if(multi_flag==-1&&repeat_flag==-1&&group_flag==-1&&isLottery){
        //抽奖模式，点击多人抽奖
        multi_flag=0;
        singleDraw->setText("重复抽奖");
        multiDraw->setText("不重复抽奖");
        backButton->setText("返回上一级");

    }
    else if(multi_flag==0&&repeat_flag==-1&&group_flag==-1&&!isLottery){
        //点名模式，点击了多人点名之后点击不重复点名
        repeat_flag=0;
        singleDraw->setText("分组点名");
        multiDraw->setText("不分组点名");

    }
    else if(multi_flag==0&&repeat_flag==-1&&group_flag==-1&&isLottery){
        //抽奖模式，点击了多人抽奖之后点击不重复抽奖
        repeat_flag=0;
        singleDraw->setText("分组抽奖");
        multiDraw->setText("不分组抽奖");

    }


    else if(multi_flag==1&&repeat_flag==-1&&group_flag==-1&&!isLottery){
        //点名模式，点击了单人点名之后点击不分组点名
        group_flag=0;
        singleDraw->setText("开始点名");
        multiDraw->setText("重新设置模式");

    }

    else if(multi_flag==1&&repeat_flag==-1&&group_flag==-1&&isLottery){
        //抽奖模式，点击了单人抽奖之后点击不分组抽奖
        group_flag=0;
        singleDraw->setText("开始抽奖");
        multiDraw->setText("重新设置模式");

    }

    else if(multi_flag==0&&repeat_flag!=-1&&group_flag==-1&&!isLottery){
        //点名模式，点击了多人点名之后点击重复点名之后点击不分组点名
        group_flag=0;
        singleDraw->setText("开始点名");
        multiDraw->setText("重新设置模式");

    }

    else if(multi_flag==0&&repeat_flag!=-1&&group_flag==-1&&isLottery){
        //抽奖模式，点击了多人抽奖之后点击重复抽奖之后点击不分组抽奖
        group_flag=0;
        singleDraw->setText("开始抽奖");
        multiDraw->setText("重新设置模式");

    }
   /* else if(multi_flag==0&&repeat_flag==0&&group_flag==-1&&!isLottery){
        //点名模式，点击了多人点名之后点击不重复点名之后点击不分组点名
        group_flag=0;
        singleDraw->setText("开始点名");
        multiDraw->setText("重新设置模式");

    }
    else if(multi_flag==0&&repeat_flag==0&&group_flag==-1&&isLottery){
        //抽奖模式，点击了多人抽奖之后点击重复抽奖之后点击不分组抽奖
        group_flag=0;
        singleDraw->setText("开始抽奖");
        multiDraw->setText("重新设置模式");

    }*/

    else if(multi_flag==0&&repeat_flag!=-1&&group_flag!=-1&&!isLottery){
        //点名模式，点击多人点名之后重新设置模式
        group_flag=-1;
        multi_flag=-1;
        repeat_flag=-1;
        singleDraw->setText("单人点名");
        multiDraw->setText("多人点名");
        backButton->setText("返回主菜单");

    }

    else if(multi_flag==0&&repeat_flag!=-1&&group_flag!=-1&&isLottery){
        //抽奖模式，点击多人抽奖之后重新设计模式
        group_flag=-1;
        multi_flag=-1;
        repeat_flag=-1;
        singleDraw->setText("单人抽奖");
        multiDraw->setText("多人抽奖");
        backButton->setText("返回主菜单");

    }

    else if(multi_flag==1&&repeat_flag==-1&&group_flag!=-1&&!isLottery){
        //点名模式，点击单人点名之后重新设置模式
        group_flag=-1;
        multi_flag=-1;
        repeat_flag=-1;
        singleDraw->setText("单人点名");
        multiDraw->setText("多人点名");
        backButton->setText("返回主菜单");

    }

    else if(multi_flag==1&&repeat_flag==-1&&group_flag!=-1&&isLottery){
        //抽奖模式，点击单人抽奖之后重新设置模式
        group_flag=-1;
        multi_flag=-1;
        repeat_flag=-1;
        singleDraw->setText("单人点名");
        multiDraw->setText("多人点名");
        backButton->setText("返回主菜单");

    }

}
