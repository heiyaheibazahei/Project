#include "fireworkeffect.h"
#include <QTime>
#include <cstdlib>
#include <QPainterPath>
#include <QDateTime>

FireworkEffect::FireworkEffect(QWidget *parent)
    : QWidget(parent) {
    // 设置窗口属性
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);

    // 覆盖整个父窗口
    if (parent) {
        setGeometry(parent->geometry());
    }

    // 初始化随机种子
    qsrand(QDateTime::currentMSecsSinceEpoch());

    // 创建定时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        updateFireworks();
        update();
        frameCount++;

        // 自动关闭
        if (frameCount > 100) { // 大约10秒后关闭
            timer->stop();
            hide();
            deleteLater();
        }
    });
}

void FireworkEffect::startFireworks() {
    // 清除现有烟花
    fireworks.clear();
    frameCount = 0;

    // 创建初始烟花
    for (int i = 0; i < 5; ++i) {
        createFirework();
    }

    // 开始动画
    timer->start(30); // 约30fps
    show();
}

void FireworkEffect::createFirework() {
    Firework firework;
    firework.exploded = false;

    // 随机位置（屏幕上半部分）
    int x = 100 + qrand() % (width() - 200);
    int y = 100 + qrand() % (height() / 2);
    firework.position = QPointF(x, y);

    // 随机颜色
    int hue = qrand() % 360;
    firework.color = QColor::fromHsv(hue, 255, 255, 200);

    fireworks.append(firework);
}

void FireworkEffect::explodeFirework(Firework &firework) {
    int particleCount = 80 + qrand() % 50;

    for (int i = 0; i < particleCount; ++i) {
        Particle particle;
        particle.position = firework.position;

        // 随机方向
        double angle = (qrand() % 360) * M_PI / 180.0;
        double speed = 2.0 + (qrand() % 100) / 50.0;
        particle.velocity = QPointF(cos(angle) * speed, sin(angle) * speed);

        // 随机颜色（基于烟花颜色）
        int hue = firework.color.hue() + (qrand() % 60 - 30);
        if (hue < 0) hue += 360;
        if (hue >= 360) hue -= 360;
        particle.color = QColor::fromHsv(hue, 255, 255, 180);

        // 随机生命周期
        particle.life = particle.maxLife = 50 + qrand() % 50;

        firework.particles.append(particle);
    }

    firework.exploded = true;
}

void FireworkEffect::updateFireworks() {
    // 随机添加新烟花
    if (qrand() % 100 < 10) { // 10%的概率添加新烟花
        createFirework();
    }

    // 更新现有烟花
    for (int i = fireworks.size() - 1; i >= 0; i--) {
        Firework &firework = fireworks[i];

        if (!firework.exploded) {
            // 烟花上升
            firework.position.setY(firework.position.y() - 3);

            // 随机爆炸
            if (qrand() % 100 < 5 || firework.position.y() < 50) {
                explodeFirework(firework);
            }
        } else {
            // 更新粒子
            for (int j = firework.particles.size() - 1; j >= 0; j--) {
                Particle &particle = firework.particles[j];

                // 更新位置
                particle.position += particle.velocity;

                // 应用重力
                particle.velocity.setY(particle.velocity.y() + 0.1);

                // 减少生命值
                particle.life--;

                // 移除死亡粒子
                if (particle.life <= 0) {
                    firework.particles.removeAt(j);
                }
            }

            // 移除没有粒子的烟花
            if (firework.particles.isEmpty()) {
                fireworks.removeAt(i);
            }
        }
    }
}

void FireworkEffect::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 绘制所有烟花
    for (const Firework &firework : fireworks) {
        if (!firework.exploded) {
            // 绘制未爆炸的烟花
            painter.setPen(Qt::NoPen);
            painter.setBrush(firework.color);

            // 烟花主体
            painter.drawEllipse(firework.position, 4, 4);

            // 烟花尾迹
            for (int i = 0; i < 5; i++) {
                QPointF trailPos = firework.position + QPointF(0, i * 3);
                QColor trailColor = firework.color;
                trailColor.setAlpha(200 - i * 40);
                painter.setBrush(trailColor);
                painter.drawEllipse(trailPos, 3 - i * 0.5, 3 - i * 0.5);
            }
        } else {
            // 绘制爆炸粒子
            for (const Particle &particle : firework.particles) {
                // 根据生命周期设置透明度
                QColor color = particle.color;
                color.setAlpha(color.alpha() * particle.life / particle.maxLife);

                painter.setPen(Qt::NoPen);
                painter.setBrush(color);

                // 绘制粒子
                painter.drawEllipse(particle.position, 2, 2);
            }
        }
    }

    // 绘制背景文字（可选）
    if (frameCount < 150) {
        QFont font;
        font.setPointSize(40);
        font.setBold(true);
        painter.setFont(font);

        QColor textColor(255, 255, 255, 200 - frameCount * 1.33);
        painter.setPen(textColor);

        QString message = "恭喜中奖人员!";
        QRect textRect = rect();
        textRect.setTop(textRect.top() + 50);
        painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignTop, message);
    }
}
