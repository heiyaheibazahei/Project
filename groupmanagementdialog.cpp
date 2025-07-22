#include "groupmanagementdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QComboBox>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>

GroupManagementDialog::GroupManagementDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("分组管理");
    setMinimumSize(800, 500);

    // --- 布局和控件初始化 ---
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    QVBoxLayout *middleLayout = new QVBoxLayout();
    QVBoxLayout *rightLayout = new QVBoxLayout();

    // 左侧：未分配人员
    leftLayout->addWidget(new QLabel("未分配人员"));
    unassignedListWidget = new QListWidget();
    unassignedListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection); // 允许多选
    leftLayout->addWidget(unassignedListWidget);

    // 中间：操作按钮
    addButton = new QPushButton(">>");
    removeButton = new QPushButton("<<");
    middleLayout->addStretch();
    middleLayout->addWidget(addButton);
    middleLayout->addWidget(removeButton);
    middleLayout->addStretch();

    // 右侧：小组管理
    QHBoxLayout *groupControlLayout = new QHBoxLayout();
    groupsComboBox = new QComboBox();
    newGroupButton = new QPushButton("新建小组");
    deleteGroupButton = new QPushButton("删除小组");
    groupControlLayout->addWidget(groupsComboBox, 1); // 占据更多空间
    groupControlLayout->addWidget(newGroupButton);
    groupControlLayout->addWidget(deleteGroupButton);

    rightLayout->addWidget(new QLabel("小组成员"));
    rightLayout->addLayout(groupControlLayout);
    groupMembersListWidget = new QListWidget();
    groupMembersListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection); // 允许多选
    rightLayout->addWidget(groupMembersListWidget);

    doneButton = new QPushButton("完成分组");
    rightLayout->addWidget(doneButton);

    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(middleLayout);
    mainLayout->addLayout(rightLayout, 1);

    // --- 信号和槽连接 ---
    connect(newGroupButton, &QPushButton::clicked, this, &GroupManagementDialog::createNewGroup);
    connect(deleteGroupButton, &QPushButton::clicked, this, &GroupManagementDialog::deleteCurrentGroup);
    connect(addButton, &QPushButton::clicked, this, &GroupManagementDialog::addMemberToGroup);
    connect(removeButton, &QPushButton::clicked, this, &GroupManagementDialog::removeMemberFromGroup);
    connect(doneButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(groupsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GroupManagementDialog::switchActiveGroup);
}

void GroupManagementDialog::setInitialNames(const QStringList &names)
{
    m_unassignedNames = names;
    unassignedListWidget->addItems(m_unassignedNames);
}

QMap<QString, QStringList> GroupManagementDialog::getGroups() const
{
    return m_groups;
}

void GroupManagementDialog::createNewGroup()
{
    bool ok;
    QString groupName = QInputDialog::getText(this, "新建小组", "请输入小组名称:", QLineEdit::Normal, "", &ok);
    if (ok && !groupName.isEmpty() && !m_groups.contains(groupName)) {
        m_groups[groupName] = QStringList(); // 创建一个空的小组
        groupsComboBox->addItem(groupName);
        groupsComboBox->setCurrentText(groupName); // 自动切换到新创建的组
    }
}

void GroupManagementDialog::deleteCurrentGroup()
{
    if (groupsComboBox->count() == 0) return;

    QString currentGroup = groupsComboBox->currentText();
    int ret = QMessageBox::warning(this, "确认删除",
                                   QString("您确定要删除 '%1' 小组吗？\n组内成员将返回到“未分配人员”列表。").arg(currentGroup),
                                   QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        // 将组成员移回未分配列表
        m_unassignedNames.append(m_groups[currentGroup]);
        m_unassignedNames.sort(); // 保持排序
        unassignedListWidget->clear();
        unassignedListWidget->addItems(m_unassignedNames);

        // 从数据和UI中移除该组
        m_groups.remove(currentGroup);
        groupsComboBox->removeItem(groupsComboBox->currentIndex());
    }
}

void GroupManagementDialog::switchActiveGroup(int index)
{
    if (index < 0) {
        groupMembersListWidget->clear();
        return;
    }
    updateGroupMembersList();
}

void GroupManagementDialog::addMemberToGroup()
{
    if (groupsComboBox->count() == 0) {
        QMessageBox::warning(this, "操作失败", "请先新建一个小组！");
        return;
    }
    QList<QListWidgetItem*> selectedItems = unassignedListWidget->selectedItems();
    if (selectedItems.isEmpty()) return;

    QString currentGroup = groupsComboBox->currentText();
    for (QListWidgetItem* item : selectedItems) {
        QString name = item->text();
        m_groups[currentGroup].append(name); // 添加到数据模型
        m_unassignedNames.removeAll(name);   // 从数据模型中移除
        delete unassignedListWidget->takeItem(unassignedListWidget->row(item)); // 从UI移除
    }
    updateGroupMembersList(); // 更新右侧UI
}

void GroupManagementDialog::removeMemberFromGroup()
{
    QList<QListWidgetItem*> selectedItems = groupMembersListWidget->selectedItems();
    if (selectedItems.isEmpty()) return;

    QString currentGroup = groupsComboBox->currentText();
    for (QListWidgetItem* item : selectedItems) {
        QString name = item->text();
        m_groups[currentGroup].removeAll(name); // 从数据模型移除
        m_unassignedNames.append(name);       // 添加回未分配列表
        delete groupMembersListWidget->takeItem(groupMembersListWidget->row(item)); // 从UI移除
    }
    // 更新左侧UI
    m_unassignedNames.sort();
    unassignedListWidget->clear();
    unassignedListWidget->addItems(m_unassignedNames);
}

void GroupManagementDialog::updateGroupMembersList()
{
    groupMembersListWidget->clear();
    QString currentGroup = groupsComboBox->currentText();
    if (m_groups.contains(currentGroup)) {
        m_groups[currentGroup].sort();
        groupMembersListWidget->addItems(m_groups[currentGroup]);
    }
}
