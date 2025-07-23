#ifndef FIREWORKEFFECT_H
#define FIREWORKEFFECT_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QPainter>
#include <QColor>
#include <cmath>

class FireworkEffect : public QWidget {
    Q_OBJECT
public:
    explicit FireworkEffect(QWidget *parent = nullptr);
    void startFireworks();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    struct Particle {
        QPointF position;
        QPointF velocity;
        QColor color;
        int life;
        int maxLife;
    };

    struct Firework {
        QPointF position;
        QColor color;
        QVector<Particle> particles;
        bool exploded;
    };

    QVector<Firework> fireworks;
    QTimer *timer;
    int frameCount = 0;

    void createFirework();
    void updateFireworks();
    void explodeFirework(Firework &firework);
};

#endif // FIREWORKEFFECT_H
