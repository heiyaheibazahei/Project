#include "drawoptionswindow.h"
#include "config.h"
#include "groupmanagementdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <random>       // 用于 mt19937 和相关工具
#include <QInputDialog> // 用于获取用户输入的抽奖人数
#include <QMessageBox>  // 用于显示抽奖结果
#include <algorithm>    // 用于 std::shuffle
#include <chrono>
#include "groupmanagementdialog.h" // <-- 添加这一行

DrawOptionsWindow::DrawOptionsWindow(QWidget *parent)
    : QWidget(parent)
{
    // 设置窗口属性
    setWindowTitle("抽奖选项");
    setFixedSize(WIN_WIDTH, WIN_HEIGHT);
    setWindowIcon(QPixmap(WIN_PATH));

    isLottery=false; // 默认点名模式

    // 创建标题
    title = new QLabel("选择抽奖模式", this);
    QFont titleFont("Microsoft YaHei", 28, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "QLabel {"
        "  color: #e67e22;"
        "  background-color: rgba(255, 255, 255, 0.85);"
        "  border-radius: 15px;"
        "  padding: 15px;"
        "}"
    );

    // 创建选项按钮
    createOptionButtons();

    // 返回按钮
    backButton = new QPushButton("返回主菜单", this);
    backButton->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    backButton->setStyleSheet(
                "QPushButton {"
                                 "  font-size: 18px;"
                                "  font-weight: bold;"
                                "  color: white;"
                                 "  padding: 12px 25px;"
                                  "  border-radius: 12px;"
                                   "  border: 2px solid #34495e;" // 深色边框
                                      "  background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
                                     "   stop:0 #e74c3c, stop:1 #c0392b);" // 红色渐变色
                                    "}"
                                   "QPushButton:hover {"
                                      "  background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
                                       "            stop:0 #ff6b6b, stop:1 #e74c3c);" // 鼠标悬停时亮红色
                                  "  border: 2px solid #2c3e50;"
                                 "}"
                               "QPushButton:pressed {"
                              "  background-color: #c0392b;" // 按下时的颜色
                               "}"
    );
    // 布局管理
    setupLayout();
    // 连接信号和槽
    connect(singleDraw,&QPushButton::clicked,this,&DrawOptionsWindow::upperButton);
    connect(multiDraw,&QPushButton::clicked,this,&DrawOptionsWindow::bottomButton);
    connect(backButton, &QPushButton::clicked, this, &DrawOptionsWindow::backButtonClicked);

    // 初始化状态
    currentState = DrawState::Initial;
    updateUIForState(); // 根据初始状态设置UI

    // 设置背景
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QBrush(QPixmap(MAINMENU_PATH).scaled(WIN_WIDTH, WIN_HEIGHT)));
    this->setPalette(palette);
}

void DrawOptionsWindow::createOptionButtons()
{
        singleDraw = new QPushButton("单人点名", this);
        multiDraw = new QPushButton("多人点名", this);

        QString buttonStyle = "QPushButton {"
                                  "  font-size: 22px;"
                                  "  font-weight: bold;"
                                  "  color: white;"
                                  "  padding: 15px 30px;"
                                  "  border-radius: 15px;"
                                  "  border: 2px solid #34495e;" // 深色边框
                                  "  background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
                                  "            stop:0 #3498db, stop:1 #2980b9);" // 蓝色渐变色
                                  "}"
                                  "QPushButton:hover {"
                                  "  background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
                                  "            stop:0 #3cb0fd, stop:1 #3498db);" // 鼠标悬停时亮蓝色
                                  "  border: 2px solid #2c3e50;"
                                  "}"
                                  "QPushButton:pressed {"
                                  "  background-color: #2980b9;" // 按下时的颜色
                                  "}";
        QString backButtonStyle = "QPushButton {"
                                      "  font-size: 18px;"
                                      "  font-weight: bold;"
                                      "  color: white;"
                                      "  padding: 12px 25px;"
                                      "  border-radius: 12px;"
                                      "  border: 2px solid #34495e;" // 深色边框
                                      "  background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
                                      "            stop:0 #e74c3c, stop:1 #c0392b);" // 红色渐变色
                                      "}"
                                      "QPushButton:hover {"
                                      "  background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
                                      "            stop:0 #ff6b6b, stop:1 #e74c3c);" // 鼠标悬停时亮红色
                                      "  border: 2px solid #2c3e50;"
                                      "}"
                                      "QPushButton:pressed {"
                                      "  background-color: #c0392b;" // 按下时的颜色
                                      "}";

    // 设置按钮样式
    singleDraw->setStyleSheet(buttonStyle);
    //"background-color: #3498db;"
    multiDraw->setStyleSheet(buttonStyle);
   // backButton->setStyleSheet(backButtonStyle);

    // 设置按钮大小
    int btnWidth = static_cast<int>(BUTTON_WIDTH);
    int btnHeight = static_cast<int>(BUTTON_HEIGHT);

    //固定大小
    singleDraw->setFixedSize(btnWidth, btnHeight);
    multiDraw->setFixedSize(btnWidth, btnHeight);
}

void DrawOptionsWindow::setupLayout()
{
    // 主垂直布局
    mainLayout = new QVBoxLayout(this);

    // 创建按钮布局（垂直排列）
    buttonLayout = new QVBoxLayout();

    // 垂直伸缩器 - 顶部占位
    mainLayout->addStretch(1);

    // 添加主按钮到按钮布局
    buttonLayout->addWidget(singleDraw, 0, Qt::AlignHCenter);
    buttonLayout->addSpacing(30); // 按钮间距

    buttonLayout->addWidget(multiDraw, 0, Qt::AlignHCenter);
    buttonLayout->addSpacing(60); // 更多的间距到返回按钮

    // 将按钮布局添加到主布局中，居中显示
    mainLayout->addLayout(buttonLayout, 1); // 第二个参数设置为1表示该部分会扩展

    // 创建返回按钮的水平布局（让它能停留在底部）
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch(); // 左侧弹性空间
    bottomLayout->addWidget(backButton);
    bottomLayout->addStretch(); // 右侧弹性空间

    // 添加底部布局到主布局
    mainLayout->addLayout(bottomLayout);

    // 添加底部弹性空间
    mainLayout->addStretch(1);

    // 设置主布局
    setLayout(mainLayout);

    // 设置布局间距和边距
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(0);
    mainLayout->setContentsMargins(20, 20, 20, 40); // 整体窗口的边距
    mainLayout->setSpacing(30);
}

//---------------------------------------------
void DrawOptionsWindow::upperButton() {
    switch (currentState) {
        case DrawState::Initial:
            m_isSingleMode = true;
            currentState = DrawState::Single_ChooseGroup;
            break;
        case DrawState::Single_ChooseGroup:
            m_useGroups = true;
            currentState = DrawState::ReadyToStart;
            break;
        case DrawState::Multi_ChooseRepeat:
            m_allowRepeats = true;
            currentState = DrawState::Multi_ChooseGroup;
            break;
        case DrawState::Multi_ChooseGroup:
            m_useGroups = true;
            currentState = DrawState::ReadyToStart;
            break;
        case DrawState::ReadyToStart:
            if (m_useGroups) {
                // 启动分组管理...
                GroupManagementDialog dialog(this);
                dialog.setInitialNames(m_nameList);
                if (dialog.exec() == QDialog::Accepted) {
                    m_definedGroups = dialog.getGroups();
                    if (m_definedGroups.isEmpty()) {
                        QMessageBox::information(this, "提示", "您没有创建任何小组，请重新设置。");
                        resetToInitialState();
                    } else {
                        currentState = DrawState::GroupsDefined;
                    }
                }
            } else {
                // 不分组，直接抽奖
                QStringList winners = performLottery();
                if (!winners.isEmpty()) {
                    QMessageBox::information(this, (isLottery ? "抽奖结果" : "点名结果"), winners.join("\n"));
                }
            }
            break;

        // --- 这是被修正的逻辑 ---
        case DrawState::GroupsDefined:
            // 唯一的作用就是“开始抽奖”，程序会根据已有的 m_isSingleMode 设置来执行
            {
                QStringList winners = performLottery();
                if (!winners.isEmpty()) {
                    QMessageBox::information(this, (isLottery ? "抽奖结果" : "点名结果"), winners.join("\n"));
                }
            }
            break;
    }
    updateUIForState();
}

//----------------------------------------
void DrawOptionsWindow::bottomButton() {
    switch (currentState) {
        case DrawState::Initial:
            m_isSingleMode = false;
            currentState = DrawState::Multi_ChooseRepeat;
            break;
        case DrawState::Single_ChooseGroup:
            m_useGroups = false;
            currentState = DrawState::ReadyToStart;
            break;
        case DrawState::Multi_ChooseRepeat:
            m_allowRepeats = false;
            currentState = DrawState::Multi_ChooseGroup;
            break;
        case DrawState::Multi_ChooseGroup:
            m_useGroups = false;
            currentState = DrawState::ReadyToStart;
            break;
        case DrawState::ReadyToStart:
            resetToInitialState();
            break;

        case DrawState::GroupsDefined:
            // 唯一的作用就是“重置所有设置”
            resetToInitialState();
            break;
    }
    updateUIForState();
}

void DrawOptionsWindow::backButtonClicked() {
    switch (currentState) {
        case DrawState::Initial:
            emit backToMainMenu();
            break;
        case DrawState::Single_ChooseGroup:
        case DrawState::Multi_ChooseRepeat:
            currentState = DrawState::Initial;
            break;
        case DrawState::Multi_ChooseGroup:
            currentState = DrawState::Multi_ChooseRepeat;
            break;
        case DrawState::ReadyToStart:
            resetToInitialState();
            break;
        // --- 新增逻辑 ---
        case DrawState::GroupsDefined:
            // "重新管理分组"按钮，再次打开分组对话框
            // 这次我们传入已经定义好的分组，方便用户修改而不是从头开始
            {
                GroupManagementDialog dialog(this);
                // 注意：这里需要修改 GroupManagementDialog 以支持接收并显示已有的分组
                // 为了简化当前步骤，我们先实现简单的重新创建
                dialog.setInitialNames(m_nameList);
                if (dialog.exec() == QDialog::Accepted) {
                    m_definedGroups = dialog.getGroups();
                    if (m_definedGroups.isEmpty()) {
                         QMessageBox::information(this, "提示", "您没有创建任何小组，请重新设置。");
                         resetToInitialState();
                    }
                }
            }
            break;
    }
    updateUIForState();
}

// 实现重置函数
void DrawOptionsWindow::resetToInitialState() {
    currentState = DrawState::Initial;
    // 重置所有记录的变量
    m_isSingleMode = true;
    m_allowRepeats = false;
    m_useGroups = false;
    m_definedGroups.clear(); // 确保清空已保存的分组
    updateUIForState();
}

// 实现新增的 setNamesList 函数
void DrawOptionsWindow::setNamesList(const QStringList &names)
{
    m_nameList = names;
}

void DrawOptionsWindow::updateUIForState()
{
    QString modeText = isLottery ? "抽奖" : "点名";

    switch (currentState) {
        // ... (Initial, Single_ChooseGroup, Multi_ChooseRepeat, Multi_ChooseGroup, ReadyToStart 的 case 保持不变) ...

        case DrawState::Initial:
            title->setText("选择" + modeText + "模式");
            singleDraw->setText("单人" + modeText);
            multiDraw->setText("多人" + modeText);
            backButton->setText("返回主菜单");
            singleDraw->show();
            multiDraw->show();
            break;

        case DrawState::Single_ChooseGroup:
            title->setText("是否需要分组?");
            singleDraw->setText("分组" + modeText);
            multiDraw->setText("不分组" + modeText);
            backButton->setText("返回上一级");
            singleDraw->show();
            multiDraw->show();
            break;

        case DrawState::Multi_ChooseRepeat:
            title->setText("是否允许重复?");
            singleDraw->setText("允许重复");
            multiDraw->setText("不允许重复");
            backButton->setText("返回上一级");
            singleDraw->show();
            multiDraw->show();
            break;

        case DrawState::Multi_ChooseGroup:
            title->setText("是否需要分组?");
            singleDraw->setText("分组" + modeText);
            multiDraw->setText("不分组" + modeText);
            backButton->setText("返回上一级");
            singleDraw->show();
            multiDraw->show();
            break;

        case DrawState::ReadyToStart:
            title->setText("设置完成，可以开始");
            singleDraw->setText("开始" + modeText);
            multiDraw->setText("重新设置模式");
            backButton->setText("返回上一级");
            singleDraw->show();
            multiDraw->show();
            break;

        case DrawState::GroupsDefined:
            // 此时 m_isSingleMode 已经被确定，我们根据它来显示标题
            title->setText(QString("分组已就绪 (%1)").arg(m_isSingleMode ? "单人模式" : "多人模式"));
            singleDraw->setText("开始从小组" + modeText); // 唯一的“开始”按钮
            multiDraw->setText("重置所有设置");          // 下方的按钮用于重置
            backButton->setText("重新管理分组");
            singleDraw->show();
            multiDraw->show();
            break;
    }
}

QStringList DrawOptionsWindow::performLottery()
{
    // 抽奖前的名单检查
    if (m_nameList.isEmpty()) {
        QMessageBox::warning(this, "操作失败", "当前名单为空，请先导入或加载一个名单！");
        return QStringList();
    }
    if (m_useGroups && m_definedGroups.isEmpty()) {
        QMessageBox::warning(this, "操作失败", "分组信息为空，请先设置分组！");
        return QStringList();
    }

    // 使用高精度系统时钟作为随机数种子
    unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);

    QStringList winners;
    QStringList targetNameList; // 这是本次抽奖真正要操作的目标名单

    if (m_useGroups) {
        // 如果是分组模式，从已保存的分组中选择一个
        bool ok;
        QString selectedGroup = QInputDialog::getItem(this, "选择小组", "请选择要进行抽奖/点名的小组：", m_definedGroups.keys(), 0, false, &ok);
        if (!ok || selectedGroup.isEmpty()) {
            return QStringList(); // 用户取消
        }
        targetNameList = m_definedGroups.value(selectedGroup);
    } else {
        // 非分组模式，使用总名单
        targetNameList = m_nameList;
    }

    // =================================================================================
    //  ↓↓↓ 核心抽奖逻辑 (完全复用您的代码，但操作对象是 targetNameList) ↓↓↓
    // =================================================================================
    if (targetNameList.isEmpty()){
        QMessageBox::warning(this, "操作失败", "选中的小组内没有任何成员！");
        return QStringList();
    }

    std::vector<QString> nameVector;
    for(const QString& name : targetNameList) {
        nameVector.push_back(name);
    }

    if (m_isSingleMode) {
        // --- 单人抽奖/点名 ---
        std::uniform_int_distribution<int> distribution(0, nameVector.size() - 1);
        int randomIndex = distribution(generator);
        winners.append(nameVector[randomIndex]);
    } else {
        // --- 多人抽奖/点名 ---
        bool ok;
        int maxNum = nameVector.size();

        int numToDraw = QInputDialog::getInt(this, "设置人数",
                                             QString("请从 '%1' (共%2人) 中抽取几人:").arg(m_useGroups ? m_definedGroups.key(targetNameList) : "总名单").arg(maxNum),
                                             1, 1, (m_allowRepeats ? 1000 : maxNum), 1, &ok);

        if (!ok) {
            return QStringList();
        }

        if (m_allowRepeats) { // 多人允许重复
            for (int i = 0; i < numToDraw; i++) {
                std::uniform_int_distribution<int> distribution(0, nameVector.size() - 1);
                int randomIndex = distribution(generator);
                winners.append(nameVector[randomIndex]);
            }
        } else { // 多人不允许重复
            if (numToDraw > nameVector.size()) {
                 QMessageBox::warning(this, "设置错误", "非重复抽奖的人数不能超过名单总人数！");
                 return QStringList();
            }
            std::shuffle(nameVector.begin(), nameVector.end(), generator);
            for (int i = 0; i < numToDraw; i++) {
                winners.append(nameVector[i]);
            }
        }
    }

    if (m_fileSystem && !winners.isEmpty()) {
            if (m_fileSystem->saveLuckyPersons(winners)) {
               /* QMessageBox::information(this, "保存成功",
                    QString("中奖人员已保存到：\n%1").arg(m_fileSystem->getLuckyPath())); */
            } else {
                QMessageBox::warning(this, "保存失败", "无法保存中奖人员名单");
            }
        }


    return winners;
}

void DrawOptionsWindow::setFileSystem(FileSystem* fs) {
    m_fileSystem = fs;
}
