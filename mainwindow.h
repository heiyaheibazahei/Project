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
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //主界面四个按钮的设置
    QPushButton *exit;
    QPushButton *import;
    QPushButton *fileManage;
    QPushButton *start;
    void setSize(QPushButton &btn);

    //主界面初始化
    void mainMenuinitialize();

    //主界面的背景
    void setMenuBackground();
    QString background;

    //主界面的大标题
    void setMenuTitle();

    //模式切换按钮
    QCheckBox *m_modeCheckBox;
    // 模式状态，false就是点名模式
     bool m_isLotteryMode = false;

    //退出按钮的功能
    void exitButton();

    //开始抽奖按钮的功能
    void startButton();

    //导入名单的按钮的功能
    void importButton_1();

    //管理名单的按钮
    void fileManageButton();

private slots:
    void onImportButtonClicked();
    void onManageButtonClicked();
//    void showProjectContent(const QString& projectName);
    void loadProjectForUse(const QString& projectName);
    void handleProjectSaved(const QString &projectName, const QStringList &names);
    void onModeChanged();

private:

    FileSystem *fileSystem; // 添加文件系统
    QPushButton *importButton;// 添加两个新按钮到界面
    QPushButton *manageButton;


    void setupFileManager();    // 在UI初始化时创建这些按钮


    QStringList currentNameList;  // 当前名单

    DrawOptionsWindow *drawOptionsWindow; // 抽奖选项窗口，将其加到mainwindow类下
    void showDrawOptions();               // 显示抽奖选项窗口
    void backToMainWindow();              // 返回主窗口


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
