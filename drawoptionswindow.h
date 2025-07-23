#ifndef DRAWOPTIONSWINDOW_H
#define DRAWOPTIONSWINDOW_H

#include <QVBoxLayout>
#include <QPushButton>
#include <QStringList>
#include <QWidget>
#include "QLabel"
#include <QDebug>
#include <QMap>
#include "filesystem.h"
#include "rollingeffectwindow.h"
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QString>

class DrawOptionsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DrawOptionsWindow(QWidget *parent = nullptr);

    //第二个界面的标题
   // QLabel *title;

    QVBoxLayout *mainLayout;
    QVBoxLayout *buttonLayout;
    QPushButton *singleDraw;     // 单人点名
    QPushButton *multiDraw; // 多人点名
    void performRolling(RollingEffectWindow *rollingWindow, QStringList &winners);
    RollingEffectWindow *rollingWindow;

    bool isLottery;//为true时就是抽奖模式
    QString lottery();

    void setFileSystem(FileSystem* fs);

    bool m_isFullScreen = false;
    void scaleUI();
    void saveOriginalSizes();

signals:

    //返回主界面的函数
    void backToMainMenu();


public slots:

    //返回按钮的槽函数
    void backButtonClicked();
    void upperButton();
    void bottomButton();
    void resetToInitialState(); // 用于重置
    void setNamesList(const QStringList &names); // 用于从主窗口接收名单



protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupLayout();
    RollingEffectWindow* m_rollingWindow; // 滚动特效窗口

    // 1. 定义状态枚举
    enum class DrawState {
        Initial,              // 初始状态：选择“单人”或“多人”
        Single_ChooseGroup,   // 已选单人，进入选择“分组”或“不分组”
        Multi_ChooseRepeat,   // 已选多人，进入选择“重复”或“不重复”
        Multi_ChooseGroup,    // 已选多人和重复选项，进入选择“分组”或“不分组”
        ReadyToStart,          // 所有选项已确定，准备开始
        GroupsDefined
    };

    // 2. 用一个状态变量替换所有 flags
    DrawState currentState;
    // 新增成员变量来记录用户的选择
    QStringList m_nameList;      // 用于存储当前抽奖/点名的名单
    bool m_isSingleMode;         // true: 单人, false: 多人
    bool m_allowRepeats;         // true: 允许重复, false: 不允许
    bool m_useGroups;            // true: 分组, false: 不分组 (此功能待实现)

    // 用于持久化存储分组结果
    QMap<QString, QStringList> m_definedGroups;

    // 3. 声明一个更新UI的私有函数
    void updateUIForState();

    void createOptionButtons();

    QStringList performLottery(); // 声明核心抽奖函数


    QPushButton *backButton;     // 返回按钮

    FileSystem* m_fileSystem; // 保存FileSystem指针
    QMediaPlayer* cheerPlayer; // 中奖音效播放器
    QMediaPlayer* rollingMusic;

    QSoundEffect *okSound;
    QSoundEffect *backSound;



    // 添加全屏按钮和状态变量
    QPushButton *fullScreenButton;


    // UI缩放数据结构
    struct WidgetData {
        QWidget *widget;
        QSize originalSize;
        QPoint originalPos;
        int originalFontSize;
    };
    QVector<WidgetData> widgetData;
    QSize originalWindowSize;
};

#endif // DRAWOPTIONSWINDOW_H
