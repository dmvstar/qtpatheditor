#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QItemSelectionModel>
#include <QFileInfo>
#include <QFileDialog>
#include <vector>
#include <string>
#include "cpathreader.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_twUserPath_clicked(const QModelIndex &index);

    void on_twSystemPath_clicked(const QModelIndex &index);

    void on_actionAbout_triggered();

    void on_actionAbout_QT_triggered();

    void on_btRemoveUserPath_clicked();

    void on_btRemoveSystemPath_clicked();

    void on_btSelectUserPath_clicked();

    void on_btSelectSystemPath_clicked();

    void on_btAddUserPath_clicked();

    void on_btAddSystemPath_clicked();

    void on_btUpUserPath_clicked();

    void on_btDnUserPath_clicked();

    void on_btUpSystemPath_clicked();

    void on_btDnSystemPath_clicked();

    void on_btUpdUserPath_clicked();

    void on_btUpdSystemPath_clicked();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;

    QIcon icon_exist_path;//(":/dir_exist.png");
    QIcon icon_error_path;//(":/dir_exist.png");

    bool m_bIsAdmin;
    bool dataCahnged;
    //StringListT m_str_list;
    QStringList mStrPathList;

    CPathReader m_reader;

    void loadUserPath();
    void loadSystemPath();
    void loadResources();
    void addDirToPath(QString dir, QTableWidget* tw, QLineEdit* line);
    bool findPath(QTableWidget* tw, QString dir);

    QString expandEnvironmentStrings(const QString &curPath);
    QTableWidgetItem* getTableWidgetItemIconForPath(const QString &curPath);

    void    moveItemUp(QTableWidget* tw);
    void    moveItemDn(QTableWidget* tw);
    void    updateItemFor(QTableWidget* tw, QLineEdit* edit);
    void    saveRegistryUserPath();
    void    saveRegistrySystemPath();


};

#endif // MAINWINDOW_H
