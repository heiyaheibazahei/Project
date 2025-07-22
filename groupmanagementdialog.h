#ifndef GROUPMANAGEMENTDIALOG_H
#define GROUPMANAGEMENTDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QMap>

// 前置声明，减少头文件依赖
class QListWidget;
class QComboBox;
class QPushButton;

class GroupManagementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GroupManagementDialog(QWidget *parent = nullptr);

    // 用于从外部接收初始的总名单
    void setInitialNames(const QStringList &names);

    // 用于让外部获取最终分好的所有组
    QMap<QString, QStringList> getGroups() const;

private slots:
    void createNewGroup();
    void deleteCurrentGroup();
    void switchActiveGroup(int index);
    void addMemberToGroup();
    void removeMemberFromGroup();

private:
    void updateGroupMembersList();

    // UI 控件
    QListWidget *unassignedListWidget; // 未分配人员列表
    QListWidget *groupMembersListWidget; // 当前选中小组的成员列表
    QComboBox *groupsComboBox;         // 用于选择和显示所有小组的下拉框
    QPushButton *newGroupButton;
    QPushButton *deleteGroupButton;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *doneButton;

    // 数据存储
    QStringList m_unassignedNames; // 存储未分配的姓名
    QMap<QString, QStringList> m_groups; // 核心数据结构：Key是组名，Value是组成员名单
};

#endif // GROUPMANAGEMENTDIALOG_H
