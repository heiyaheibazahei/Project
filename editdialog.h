#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include <QStringList>

class QListWidget;
class QListWidgetItem;
class QLineEdit;
class QPushButton;

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(QWidget *parent = nullptr);
    ~EditDialog();

    void setNameList(const QStringList &names);
    QStringList getNameList() const;

signals:
    // 添加信号声明
    void projectSaved(const QString& projectName, const QStringList &names);

private slots:
    void addName();
    void editName();
    void removeName();
    void saveAndClose();
    void saveProject();
    void importFile();
    void importTxt();
    void importCsv();
    void itemDoubleClicked(QListWidgetItem *item);

private:
    QListWidget *listWidget;
    QLineEdit *nameEdit;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *removeButton;
    QPushButton *saveButton;
    QPushButton *importButton;

    QStringList nameList;
};

#endif // EDITDIALOG_H
