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
signals:

    //返回主界面的函数
    void backToMainMenu();

public slots:

    //返回按钮的槽函数
    void backButtonClicked();

private:
    void createOptionButtons();
    void setupLayout();

    QPushButton *repeatDraw;     // 重复抽奖
    QPushButton *noRepeatDraw;   // 不重复抽奖
    QPushButton *backButton;     // 返回按钮
};

#endif // DRAWOPTIONSWINDOW_H
