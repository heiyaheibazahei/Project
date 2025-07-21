#ifndef DRAWOPTIONSWINDOW_H
#define DRAWOPTIONSWINDOW_H

#include <QWidget>
#include <QPushButton>
#include "QLabel"
#include <QVBoxLayout>

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

    int multi_flag; //三个flag用来判断是选择哪种算法，点击上面的按钮统一设置为1，下面的统一设置为0
    int repeat_flag;
    int group_flag;
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

private:
    void createOptionButtons();
    void setupLayout();


    QPushButton *backButton;     // 返回按钮
};

#endif // DRAWOPTIONSWINDOW_H
