#include "summarydialog.h"
#include "ui_summarydialog.h"

SummaryDialog::SummaryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SummaryDialog)
{
    ui->setupUi(this);
    db.setDatabaseName(dbpath);
    QSqlQuery display_qry(db);
    display_qry.exec("SELECT * FROM Books");
    if (display_qry.next()) {
        QSqlQueryModel *display_qrm = new QSqlQueryModel(this);
        display_qrm->setQuery(display_qry);
        ui->tbvwSummary->setModel(display_qrm);
    }

    int item_total = ui->tbvwSummary->model()->rowCount();
    ui->ledRTotal->setText(QString::number(item_total));
}

SummaryDialog::~SummaryDialog()
{
    delete ui;
}
