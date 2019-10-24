#ifndef SUMMARYDIALOG_H
#define SUMMARYDIALOG_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class SummaryDialog;
}

class SummaryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SummaryDialog(QWidget *parent = nullptr);
    ~SummaryDialog();

private:
    Ui::SummaryDialog *ui;
    QSqlDatabase db = QSqlDatabase::database("db_conn");

    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString dbpath = appDataDir + "/items.db";
};

#endif // SUMMARYDIALOG_H
