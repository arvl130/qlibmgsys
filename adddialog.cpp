#include "adddialog.h"
#include "ui_adddialog.h"

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
    ui->btnOKCancel->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->ledTitle->setStyleSheet("border: 1px solid red");
    ui->ledISBN->setStyleSheet("border: 1px solid red");

    connect(ui->ledTitle, SIGNAL(textChanged(QString)), this, SLOT(toggleOKBtn()));
    connect(ui->ledISBN, SIGNAL(textChanged(QString)), this, SLOT(toggleOKBtn()));

}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::toggleOKBtn() {
    if (ui->ledISBN->text().isEmpty()) {
        ui->ledISBN->setStyleSheet("border: 1px solid red");
    }
    else {
        ui->ledISBN->setStyleSheet("");
    }

    if (ui->ledTitle->text().isEmpty()) {
        ui->ledTitle->setStyleSheet("border: 1px solid red");
    }
    else {
        ui->ledTitle->setStyleSheet("");
    }

    if (ui->ledISBN->text().isEmpty() || ui->ledTitle->text().isEmpty()) {
        ui->btnOKCancel->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else {
        ui->btnOKCancel->button(QDialogButtonBox::Ok)->setEnabled(true);
    }

}

void AddDialog::on_btnOKCancel_accepted()
{
    QString title = ui->ledTitle->text();
    QString author = ui->ledAuthor->text();
    QString publisher = ui->ledPublisher->text();
    QString published = ui->ledPublished->text();
    QString isbn = ui->ledISBN->text();
    QString category = ui->ledCategory->text();
    QString genre = ui->ledGenre->text();
    QString synopsis = ui->ptxtSynopsis->toPlainText();
    QString cover = ui->ledCoverPhoto->text();

    emit addItem(title, author, publisher, published, isbn, category, genre, synopsis, cover);
}

void AddDialog::on_btnCoverPhoto_clicked()
{

    ui->ledCoverPhoto->setText(QFileDialog::getOpenFileName(this, "Choose a cover image", homeDir));
    if (ui->ledCoverPhoto->text().isEmpty()) {
        ui->lblCoverPhoto->clear();
        ui->ledCoverPhoto->setEnabled(false);
    }
    else {
        ui->lblCoverPhoto->setPixmap(QPixmap(ui->ledCoverPhoto->text()).scaled(ui->lblCoverPhoto->width(), ui->lblCoverPhoto->height(), Qt::KeepAspectRatio));
        ui->ledCoverPhoto->setEnabled(true);
    }
}
