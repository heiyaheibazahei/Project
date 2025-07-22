#ifndef LUCKYRECORDSWINDOW_H
#define LUCKYRECORDSWINDOW_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QPushButton>
class LuckyRecordsWindow : public QWidget {
    Q_OBJECT
public:
    explicit LuckyRecordsWindow(QWidget *parent = nullptr);
    QPushButton *backToMenu;

private:
    QListWidget *recordsList;
    const QString luckyPath = "D:/Project/Project/luckyPersons"; // 复用现有路径

    void loadRecords();
private slots:
    void back();
};

#endif // LUCKYRECORDSWINDOW_H
