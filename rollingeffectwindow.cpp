#include "rollingeffectwindow.h"
#include <QFontDatabase>
#include <QHBoxLayout>
#include <cstdlib>
#include <QTime>
#include <QMediaPlayer>
#include "config.h"

RollingEffectWindow::RollingEffectWindow(const QStringList& names, const QStringList& winnersList, QWidget *parent)
    : QWidget(parent), allNames(names), winners(winnersList) {
    // 设置窗口为全屏
    setWindowState(Qt::WindowFullScreen);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setFixedSize(400,60);

    //全透明的背景
    setStyleSheet("background-color: transparent");


    // 创建标签用于显示名字
    nameLabel = new QLabel(this);
    nameLabel->setAlignment(Qt::AlignCenter);

    // 设置字体样式
    QFont font;
    font.setPointSize(18);
    font.setBold(true);

    // 加载中文字体
    int fontId = QFontDatabase::addApplicationFont(":/fonts/simhei.ttf");
    if (fontId != -1) {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.isEmpty()) {
            font.setFamily(fontFamilies.at(0));
        }
    }

    nameLabel->setFont(font);
    nameLabel->setStyleSheet("color: darkred;");

    // 布局设置
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(nameLabel);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &RollingEffectWindow::updateName);

    // 初始化随机数种子
    qsrand(QTime::currentTime().msec());

    // 初始化滚动音乐
   /* rollingMusic = new QMediaPlayer(this);
    rollingMusic->setMedia(QUrl(ROLLING_MUSIC_PATH));*/
}

void RollingEffectWindow::startRolling(int duration) {
    rollingDuration = duration;

    if (allNames.isEmpty()) {
        emit rollingFinished();
        return;
    }

    // 随机选择起始位置
    currentIndex = qrand() % allNames.size();
    nameLabel->setText(allNames.at(currentIndex));

    // 计算更新间隔和步数
    stepCounter = 0;
    steps = rollingDuration / 50;
    timer->start(50);
    //rollingMusic->play();
}

void RollingEffectWindow::updateName() {
    stepCounter++;

    // 更新名字
    currentIndex = (currentIndex + 1) % allNames.size();
    nameLabel->setText(allNames.at(currentIndex));

    // 接近结束时减慢速度
    if (stepCounter > steps * 0.7) {
        timer->setInterval(timer->interval() + 10);
    }

    // 结束滚动
    if (stepCounter >= steps) {
        timer->stop();
        //rollingMusic->stop();

        // 确保最后显示的是中奖者之一
        if (!winners.isEmpty()) {
            int winnerIndex = qrand() % winners.size();
            nameLabel->setText(winners.at(winnerIndex));
        }

        // 淡出效果
        QTimer::singleShot(2000, this, &RollingEffectWindow::fadeOut);
    }
}

void RollingEffectWindow::fadeOut() {
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(opacityEffect);

    QPropertyAnimation *fadeAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    fadeAnimation->setDuration(1000);
    fadeAnimation->setStartValue(1.0);
    fadeAnimation->setEndValue(0.0);
    fadeAnimation->setEasingCurve(QEasingCurve::OutCubic);

    connect(fadeAnimation, &QPropertyAnimation::finished, this, [this]() {
        hide();
        emit rollingFinished();
        deleteLater();
    });

    fadeAnimation->start();
}
