#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

#include <QMainWindow>

#include <QtSql>
#include <QDebug>
#include <QMessageBox>
#include <QTableWidgetItem>

#include <adddialog.h>
#include <updatedialog.h>

namespace Ui {
class ViewWindow;
}

class ViewWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewWindow(QWidget *parent = nullptr);
    ~ViewWindow();

private slots:
    void loadCompTable();
    void loadListViews();
    void loadItemInfo();
    void loadTruncTable_category();
    void loadTruncTable_genre();
    void addItem(QString, QString, QString, QString, QString, QString, QString, QString, QString);
    void deleteItem();
    void updateItem(QString, QString, QString, QString, QString, QString, QString, QString, QString);
    void disableAdminBtns();

    void on_btnAdd_clicked();

    void on_btnDelete_clicked();

    void on_btnUpdate_clicked();

    void on_actionLogout_triggered();


    void on_actionAdd_item_triggered();

    void on_actionDelete_item_triggered();

    void on_actionUpdate_item_triggered();

    void on_actionExit_triggered();

signals:
    void loadCurrInfo(QString, QString, QString, QString, QString, QString, QString, QString, QString);
    void unhideWindow();

private:
    Ui::ViewWindow *ui;

    QSqlDatabase db = QSqlDatabase::database("db_conn");

    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString dbpath = appDataDir + "/items.db";

    AddDialog *ad;
    UpdateDialog *ud;
};

#endif // VIEWWINDOW_H
