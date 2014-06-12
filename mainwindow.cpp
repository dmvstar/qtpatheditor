/* Copyright (c) 2014, Dmitry Starzhynskyi
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cpathreader.h"
#include "utils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mIsAdmin = IsProcessAdmin( ::GetCurrentProcess());
    ui->setupUi(this);

    dataUserChanged = false;
    dataSystemChanged = false;
    loadResources();
    loadUserPath();
    loadSystemPath();
}

void MainWindow::loadResources(){

    icon_exist_path = QIcon(":/dir_exist.png");
    icon_error_path = QIcon(":/dir_non_exist.png");

}

void MainWindow::loadSystemPath(){
    if(mIsAdmin) {

        QStringList header;
        header<<"*"<<tr("Path");

        QTableWidget *tw;
        tw = ui->twSystemPath;
        tw->clear();

        tw->setColumnCount(2);
        tw->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
        tw->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
        tw->setHorizontalHeaderLabels(header);

        mPathReader = CPathReader( HKEY_LOCAL_MACHINE,
                                L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment",
                                L"Path" );
        mPathReader.Read( mStrPathList );

        tw->setRowCount(mStrPathList.size());

        for( std::size_t count = 0; count < mStrPathList.size(); ++count)
        {
            qDebug() << "System:" <<  mStrPathList[count];//     QString::fromStdWString(m_str_list[count]);
            QString wsCurPath = mStrPathList[count];//QString::fromStdWString(m_str_list[count]);
            tw->setItem( count, 0,  getTableWidgetItemIconForPath( wsCurPath ) );
            //tw->setItem( count, 0,  new QTableWidgetItem( icon_error_path, "", QTableWidgetItem::Type ) );
            tw->setItem( count, 1,  new QTableWidgetItem( wsCurPath, QTableWidgetItem::Type ) );
        }

    }
}

void MainWindow::loadUserPath(){
    QStringList header;
    header<<"*"<<tr("Path");

    QTableWidget *tw;
    tw = ui->twUserPath;
    tw->clear();

    tw->setColumnCount(2);
    tw->setHorizontalHeaderLabels(header);
    tw->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    tw->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);


    mPathReader = CPathReader( HKEY_CURRENT_USER, L"Environment", L"Path" );
    mPathReader.Read( mStrPathList );

    tw->setRowCount(mStrPathList.size());

    for( std::size_t count = 0; count < mStrPathList.size(); ++count)
    {
        qDebug() << "User:" << mStrPathList[count];//QString::fromStdWString(m_str_list[count]);
        QString wsCurPath = mStrPathList[count];//QString::fromStdWString(m_str_list[count]);
        //tw->setItem( count, 0, new QTableWidgetItem( icon_exist_path, "", QTableWidgetItem::Type ) );
        tw->setItem( count, 0,  getTableWidgetItemIconForPath( wsCurPath ) );
//        tw->setItem( count, 1, new QTableWidgetItem( QString::fromStdWString(m_str_list[count]), QTableWidgetItem::Type ) );
        tw->setItem( count, 1, new QTableWidgetItem( wsCurPath, QTableWidgetItem::Type ) );
    }

    return;
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_twUserPath_clicked(const QModelIndex &index)
{
    qDebug() << "on_twUserPath_clicked:" << index;
    QString str = ui->twUserPath->item(index.row(),1)->text();
    qDebug() << "on_twUserPath_clicked:" << str;
    ui->lineEditUserPath->setText( str );
}

void MainWindow::on_twSystemPath_clicked(const QModelIndex &index)
{
    qDebug() << "on_twUserPath_clicked:" << index;
    QString str = ui->twSystemPath->item(index.row(),1)->text();
    qDebug() << "on_twUserPath_clicked:" << str;
    ui->lineEditSystemPath->setText( str );
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About QT Path Editor"),
            tr("Simple Windows Path Editor.")+" "+QLocale::system().name());
}

void MainWindow::on_actionAbout_QT_triggered()
{
    QMessageBox::aboutQt(this);
}

QTableWidgetItem* MainWindow::getTableWidgetItemIconForPath(const QString &wsCurPath){

    QString fullPath = expandEnvironmentStrings(wsCurPath);

    QFileInfo fi = QFileInfo(fullPath);

    QTableWidgetItem* qtwItem = new QTableWidgetItem( fi.exists()?icon_exist_path:icon_error_path, "", QTableWidgetItem::Type );

    return qtwItem;
}

void MainWindow::on_btRemoveUserPath_clicked()
{
    QItemSelectionModel* sm = ui->twUserPath->selectionModel();
    qDebug() << "on_btRemoveUserPath_clicked:" << sm->hasSelection();
    if( sm->hasSelection() ){
        QModelIndexList indexList = sm->selectedRows();
        qDebug() << "on_btRemoveUserPath_clicked count:" << indexList.length();
        int row;
        foreach (QModelIndex index, indexList) {
            row = index.row();
            qDebug() << "on_btRemoveUserPath_clicked remove row:" << row;
            ui->twUserPath->removeRow(row);
            ui->lineEditUserPath->clear();
        }
        dataUserChanged = true;
    }
}

void MainWindow::on_btRemoveSystemPath_clicked()
{
    QItemSelectionModel* sm = ui->twSystemPath->selectionModel();
    qDebug() << "on_btRemoveSystemPath_clicked:" << sm->hasSelection();
    if( sm->hasSelection() ){
        QModelIndexList indexList = sm->selectedRows();
        qDebug() << "on_btRemoveSystemPath_clicked count:" << indexList.length();
        int row;
        foreach (QModelIndex index, indexList) {
            row = index.row();
            qDebug() << "on_btRemoveSystemPath_clicked remove row:" << row;
            ui->twSystemPath->removeRow(row);
            ui->lineEditSystemPath->clear();
        }
        dataSystemChanged = true;
    }
}

QString MainWindow::expandEnvironmentStrings(const QString &curPath){
    QString ret = curPath;
    if(curPath.indexOf("%")>=0){
        int start = curPath.indexOf("%");
        int stops = curPath.indexOf("%", start+1);
        QString envVar = curPath.mid(start+1,stops-start-1);
        QString envVarValue = qgetenv(envVar.toStdString().c_str() );
        qDebug() << "expandEnvironmentStrings ["<<envVar<<"]["<<envVarValue<<"]";
        ret = curPath.left(start)+envVarValue+curPath.right(curPath.length()-stops-1);
        qDebug() << "expandEnvironmentStrings ["<<envVar<<"]["<<envVarValue<<"]["<<ret <<"]";

        qDebug() << "expandEnvironmentStrings ["<<envVar<<"]["<<parse(curPath)<<"]";

    }
    return ret;
}

void MainWindow::on_btSelectUserPath_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    ".",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(dir.length()>0){
        addDirToPath(dir, ui->twUserPath, ui->lineEditUserPath);
    }
    qDebug() << "on_btSelectUserPath_clicked ["<<dir<<"]";
}


void MainWindow::on_btSelectSystemPath_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    ".",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(dir.length()>0){
        addDirToPath(dir, ui->twSystemPath, ui->lineEditSystemPath);
    }
    qDebug() << "on_btSelectSystemPath_clicked ["<<dir<<"]";
}

void MainWindow::on_btAddUserPath_clicked()
{
    if( ui->lineEditUserPath->text().length()>0){
        addDirToPath(ui->lineEditUserPath->text(), ui->twUserPath, ui->lineEditUserPath);
    }
}


void MainWindow::on_btAddSystemPath_clicked()
{
    if( ui->lineEditSystemPath->text().length()>0){
        addDirToPath(ui->lineEditSystemPath->text(), ui->twSystemPath, ui->lineEditSystemPath);
    }
}

void MainWindow::on_btUpUserPath_clicked()
{
    dataUserChanged = moveItemUp(ui->twUserPath);
}

void MainWindow::on_btDnUserPath_clicked()
{
    dataUserChanged = moveItemDn(ui->twUserPath);
}

void MainWindow::on_btUpSystemPath_clicked()
{
    dataSystemChanged = moveItemUp(ui->twSystemPath);
}

void MainWindow::on_btDnSystemPath_clicked()
{
    dataSystemChanged = moveItemDn(ui->twSystemPath);
}

bool MainWindow::addDirToPath(QString dir, QTableWidget* tw, QLineEdit* line ){
    bool ret = false;
    if(!findPath(tw, dir)) {
        tw->setRowCount( tw->rowCount()+1 );

        tw->setItem( tw->rowCount()-1, 0,  getTableWidgetItemIconForPath( dir ) );
        tw->setItem( tw->rowCount()-1, 1, new QTableWidgetItem( dir, QTableWidgetItem::Type ) );

        tw->selectRow(tw->rowCount()-1);
        line->setText( dir );

        ret = true;
    }
    return ret;
}

bool MainWindow::findPath(QTableWidget* tw, QString dir){
    int rows = tw->rowCount();
    bool found = false;
    for(int i = 0; i < rows; ++i)
    {
        if(tw->item(i, 1)->text() == dir)
        {
            found = true;
            break;
        }
    }
    return found;
}

bool    MainWindow::moveItemUp(QTableWidget* tw){
    bool ret = false;
    QItemSelectionModel* sm = tw->selectionModel();
    QModelIndexList indexList = tw->selectionModel()->selectedIndexes();
    if( sm->hasSelection()){
        QModelIndex index = indexList.at(0);
        if(index.row()>0){
            QTableWidgetItem *item10 = tw->takeItem(index.row()-1,  0);
            QTableWidgetItem *item11 = tw->takeItem(index.row()-1,  1);

            QTableWidgetItem *item20 = tw->takeItem(index.row(),    0);
            QTableWidgetItem *item21 = tw->takeItem(index.row(),    1);

            tw->setItem(index.row()-1,  0, item20);
            tw->setItem(index.row()-1,  1, item21);
            tw->setItem(index.row(),    0, item10);
            tw->setItem(index.row(),    1, item11);
            tw->selectRow(index.row()-1);
        }
        ret = true;
    }
    return ret;
}

bool    MainWindow::moveItemDn(QTableWidget* tw){
    bool ret = false;
    QItemSelectionModel* sm = tw->selectionModel();
    QModelIndexList indexList = tw->selectionModel()->selectedIndexes();
    if( sm->hasSelection()){
        QModelIndex index = indexList.at(0);
        if(index.row()<tw->rowCount()-1 ){
            QTableWidgetItem *item10 = tw->takeItem(index.row()+1,  0);
            QTableWidgetItem *item11 = tw->takeItem(index.row()+1,  1);

            QTableWidgetItem *item20 = tw->takeItem(index.row(),    0);
            QTableWidgetItem *item21 = tw->takeItem(index.row(),    1);

            tw->setItem(index.row()+1,  0, item20);
            tw->setItem(index.row()+1,  1, item21);
            tw->setItem(index.row(),    0, item10);
            tw->setItem(index.row(),    1, item11);
            tw->selectRow(index.row()+1);
            ret = true;
        }
    }
    return ret;
}




void MainWindow::on_btUpdUserPath_clicked()
{
    dataUserChanged     = updateItemFor(ui->twUserPath, ui->lineEditUserPath);
}

void MainWindow::on_btUpdSystemPath_clicked()
{
    dataSystemChanged   = updateItemFor(ui->twSystemPath, ui->lineEditSystemPath);
}


bool MainWindow::updateItemFor(QTableWidget* tw, QLineEdit* edit ){
    bool ret = false;
    QItemSelectionModel* sm = tw->selectionModel();
    QModelIndexList indexList = tw->selectionModel()->selectedIndexes();
    if( sm->hasSelection()){
        QModelIndex index = indexList.at(0);
        if(index.row()>=0 && edit->text().length()>0 && !findPath( tw, edit->text() )){
            QTableWidgetItem *item = tw->takeItem(index.row(),  1);
            item->setText( edit->text() );
            tw->setItem(index.row(),    0, getTableWidgetItemIconForPath(edit->text()));
            tw->setItem(index.row(),    1, item);
        }
        ret = true;
    }
    return ret;
}

void MainWindow::on_actionSave_triggered()
{
    qDebug() << "Save All data";
    if(dataUserChanged) {
        saveRegistryUserPath();
        dataUserChanged = false;
    }

    if(dataSystemChanged) {
        saveRegistrySystemPath();
        dataSystemChanged = false;
    }

}

void MainWindow::saveRegistryUserPath(){
    QStringList pathList;
    QTableWidget* tw = ui->twUserPath;
    for(int row=0; row!=tw->rowCount(); ++row){
        QTableWidgetItem *item = tw->item(row,1);
        pathList << item->text();
    }
    QString path = pathList.join(";");
//    qDebug() << "Save User data " << path;
    mPathReader = CPathReader( HKEY_CURRENT_USER, L"Environment", L"Path" );
    mPathReader.Write(pathList);
}


void MainWindow::saveRegistrySystemPath(){
    QStringList pathList;
    QTableWidget* tw = ui->twSystemPath;
    for(int row=0; row!=tw->rowCount(); ++row){
        QTableWidgetItem *item = tw->item(row,1);
        pathList << item->text();
    }
    QString path = pathList.join(";");
//    qDebug() << "Save System data " << path;
    mPathReader = CPathReader( HKEY_LOCAL_MACHINE,
                            L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment",
                            L"Path" );
    mPathReader.Write(pathList);
}
