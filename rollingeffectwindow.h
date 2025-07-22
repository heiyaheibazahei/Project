#ifndef ROLLINGEFFECTWINDOW_H
#define ROLLINGEFFECTWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QStringList>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class RollingEffectWindow : public QWidget {
    Q_OBJECT
public:
    explicit RollingEffectWindow(const QStringList& names, const QStringList& winners, QWidget *parent = nullptr);
    void startRolling(int duration = 3000); // 开始滚动特效

signals:
    void rollingFinished(); // 滚动结束信号

private slots:
    void updateName(); // 更新显示的名字
    void fadeOut();    // 淡出特效

private:
    QLabel *nameLabel;
    QTimer *timer;
    QStringList allNames;   // 所有参与者的名字
    QStringList winners;    // 中奖者名单
    int currentIndex;
    int rollingDuration;
    int steps;
    int stepCounter;
};

#endif // ROLLINGEFFECTWINDOW_H
