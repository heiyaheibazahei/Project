#ifndef DRAWOPTIONSWINDOW_H
#define DRAWOPTIONSWINDOW_H

#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include "QLabel"
#include <QDebug>

class DrawOptionsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DrawOptionsWindow(QWidget *parent = nullptr);

    //第二个界面的标题
    QLabel *title;

    QVBoxLayout *mainLayout;
    QVBoxLayout *buttonLayout;
    QPushButton *singleDraw;     // 单人点名
    QPushButton *multiDraw;   // 多人点名

//    int multi_flag; //三个flag用来判断是选择哪种算法，点击上面的按钮统一设置为1，下面的统一设置为0
//    int repeat_flag;
//    int group_flag;
    bool isLottery;//默认点名模式
    QString lottery();

signals:

    //返回主界面的函数
    void backToMainMenu();

public slots:

    //返回按钮的槽函数
    void backButtonClicked();
    void upperButton();
    void bottomButton();
    void resetToInitialState(); // 用于重置

private:
    // 1. 定义状态枚举
    enum class DrawState {
        Initial,              // 初始状态：选择“单人”或“多人”
        Single_ChooseGroup,   // 已选单人，进入选择“分组”或“不分组”
        Multi_ChooseRepeat,   // 已选多人，进入选择“重复”或“不重复”
        Multi_ChooseGroup,    // 已选多人和重复选项，进入选择“分组”或“不分组”
        ReadyToStart          // 所有选项已确定，准备开始
    };

    // 2. 用一个状态变量替换所有 flags
    DrawState currentState;

    // 3. 声明一个更新UI的私有函数
    void updateUIForState();

    void createOptionButtons();
    void setupLayout();


    QPushButton *backButton;     // 返回按钮
};

#endif // DRAWOPTIONSWINDOW_H
