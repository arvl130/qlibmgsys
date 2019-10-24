#include "updatedialog.h"
#include "ui_updatedialog.h"

UpdateDialog::UpdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDialog)
{
    ui->setupUi(this);

    ui->btnOKCancel->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->ledTitle->setStyleSheet("border: 1px solid red");
    ui->ledISBN->setStyleSheet("border: 1px solid red");

    connect(ui->ledTitle, SIGNAL(textChanged(QString)), this, SLOT(toggleOKBtn()));
    connect(ui->ledISBN, SIGNAL(textChanged(QString)), this, SLOT(toggleOKBtn()));

}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}

void UpdateDialog::toggleOKBtn() {
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

void UpdateDialog::loadCurrInfo(QString title, QString author, QString publisher, QString published, QString isbn, QString category, QString genre, QString synopsis, QString cover) {
    ui->ledTitle->setText(title);
    ui->ledAuthor->setText(author);
    ui->ledPublisher->setText(publisher);
    ui->ledPublished->setText(published);
    ui->ledISBN->setText(isbn);
    ui->ledCategory->setText(category);
    ui->ledGenre->setText(genre);
    ui->ptxtSynopsis->setPlainText(synopsis);
    ui->ledCoverPhoto->setText(cover);
    if (cover.isEmpty()) {
        ui->lblCoverPhoto->setText("No Image Data");
    }
    else {
        QPixmap cover_img(cover);
        int cover_w = ui->lblCoverPhoto->width();
        int cover_h = ui->lblCoverPhoto->height();
        ui->lblCoverPhoto->setPixmap(cover_img.scaled(cover_w, cover_h, Qt::KeepAspectRatio));
        ui->ledCoverPhoto->setEnabled(true);
    }

}

void UpdateDialog::on_btnOKCancel_accepted()
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

    if (ui->ledGenre->text().isEmpty()) {
        genre = "(unsorted)";
    }

    if (ui->ledCategory->text().isEmpty()) {
        category = "(unsorted)";
    }

    emit updateItem(title, author, publisher, published, isbn, category, genre, synopsis, cover);
}

void UpdateDialog::on_btnCoverPhoto_clicked()
{
    ui->ledCoverPhoto->setText(QFileDialog::getOpenFileName(this, "Choose a cover image", homeDir));
    if (ui->ledCoverPhoto->text().isEmpty()) {
        ui->lblCoverPhoto->clear();
        ui->lblCoverPhoto->setText("No Image Data");
        ui->ledCoverPhoto->setEnabled(false);
    }
    else {
        ui->lblCoverPhoto->setPixmap(QPixmap(ui->ledCoverPhoto->text()).scaled(ui->lblCoverPhoto->width(), ui->lblCoverPhoto->height(), Qt::KeepAspectRatio));
        ui->ledCoverPhoto->setEnabled(true);
    }
}
