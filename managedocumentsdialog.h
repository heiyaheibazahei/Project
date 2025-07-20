#ifndef MANAGEDOCUMENTSDIALOG_H
#define MANAGEDOCUMENTSDIALOG_H

#include <QDialog>
#include <QStringList>

class QListWidget;
class QPushButton;
class QLabel;

class ManageDocumentsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManageDocumentsDialog(QWidget *parent = nullptr);
    void setProjectsList(const QStringList &projects);
    QString selectedProject() const;

signals:
    void projectSelected(const QString &projectName);
    void projectDeleted(const QString &projectName);

private slots:
    void deleteSelectedProject();
    void showProject();

private:
    QListWidget *projectsList;
    QLabel *infoLabel;
    QPushButton *viewButton;
    QPushButton *deleteButton;
};

#endif // MANAGEDOCUMENTSDIALOG_H
