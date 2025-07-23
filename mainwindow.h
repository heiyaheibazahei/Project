#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QPushButton>
#include <QMainWindow>
#include <QString>
#include "drawoptionswindow.h"
#include <QtMultimedia/QSoundEffect>
#include "filesystem.h"
#include "editdialog.h"
#include "managedocumentsdialog.h"
#include <QCheckBox>

class QMediaPlayer;

class QSoundEffect;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //主界面五个按钮的设置
    QPushButton *exit;
    QPushButton *import;
    QPushButton *fileManage;
    QPushButton *start;
    QPushButton *luckyRecords;//中奖记录按钮

    //更换背景图片
    QPushButton *changeBackground;
    QString backgroundPic;


    //调整按钮大小样式这些
    void setSize(QPushButton &btn);

    //主界面初始化
    void mainMenuinitialize();

    //主界面布局
    void setUpLayout();

    //主界面的背景
    void setMenuBackground();
    QString background;

    //主界面的大标题
    void setMenuTitle();

    //模式切换按钮
    QCheckBox *modeCheckBox;

    // 模式状态，false就是点名模式
    bool isLotteryMode = false;

    // 添加全屏状态变量
    bool isFullScreen() const { return m_isFullScreen; }

    //退出按钮的功能
    void exitButton();

    //开始抽奖按钮的功能
    void startButton();

    //导入名单的按钮的功能
    void importButton_1();

    //管理名单的按钮
    void fileManageButton();

    void luckyRecordsButton();

    void changeBackgroundButton();

    QString getRandomName();

    QStringList getRandomNames(int count);


private slots:
    void onImportButtonClicked();
    void onManageButtonClicked();
//    void showProjectContent(const QString& projectName);
    void loadProjectForUse(const QString& projectName);
    void handleProjectSaved(const QString &projectName, const QStringList &names);
    void onModeChanged();//模式切换
    void showLuckyRecords();//展示中奖记录
    void changeBackgroundClicked();
private:

    FileSystem *fileSystem; // 添加文件系统
    QPushButton *importButton;// 添加两个新按钮到界面
    QPushButton *manageButton;

    QSoundEffect *okSound;     // 确认按钮音效
    QSoundEffect *backSound;   // 返回按钮音效
    QSoundEffect *switchSound;

    QMediaPlayer *backgroundMusicPlayer;

    void setupFileManager();    // 在UI初始化时创建这些按钮


    QStringList currentNameList;  // 当前名单

    DrawOptionsWindow *drawOptionsWindow; // 抽奖选项窗口，将其加到mainwindow类下
    void showDrawOptions();               // 显示抽奖选项窗口
    void backToMainWindow();              // 返回主窗口

    QLabel *titleLabel = nullptr; // 添加标题标签成员
    #define BASE_TITLE_SIZE 20    // 添加基准标题字体大小

private:
    Ui::MainWindow *ui;

    // 添加全屏按钮和状态变量
    QPushButton *fullScreenButton;
    bool m_isFullScreen = false;

    // 添加用于UI缩放的数据结构
    struct WidgetData {
        QWidget *widget;
        QSize originalSize;
        QPoint originalPos;
        int originalFontSize;
    };
    QVector<WidgetData> widgetData;

    // 原始窗口大小
    QSize originalWindowSize;
};

#endif // MAINWINDOW_H
