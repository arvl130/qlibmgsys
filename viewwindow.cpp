#include "viewwindow.h"
#include "ui_viewwindow.h"

ViewWindow::ViewWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewWindow)
{
    ui->setupUi(this);

    db.setDatabaseName(dbpath);

    loadCompTable();
    loadListViews();

    connect(ui->tbvwBooks, SIGNAL(clicked(QModelIndex)), this, SLOT(loadItemInfo()));
    connect(ui->lsvwCategory, SIGNAL(clicked(QModelIndex)), this, SLOT(loadTruncTable_category()));
    connect(ui->lsvwGenre, SIGNAL(clicked(QModelIndex)), this, SLOT(loadTruncTable_genre()));
}

ViewWindow::~ViewWindow()
{
    delete ui;
}

void ViewWindow::loadCompTable() {
    db.open();

    QSqlQueryModel *tbl_model = new QSqlQueryModel(this);
    tbl_model->setQuery(db.exec("SELECT Title, Genre FROM Books"));
    ui->tbvwBooks->setModel(tbl_model);
    ui->tbvwBooks->horizontalHeader()->setStretchLastSection(true);
    ui->tbvwBooks->verticalHeader()->setVisible(false);
    ui->tbvwBooks->setColumnWidth(0, ui->tbvwBooks->width() * 3 / 4);

    db.close();
}

void ViewWindow::loadTruncTable_category() {
    QModelIndex curr_category_index = ui->lsvwCategory->currentIndex();
    QString curr_category = curr_category_index.data(Qt::DisplayRole).toString();

    db.open();

    QSqlQueryModel *tbl_model = new QSqlQueryModel(this);
    tbl_model->setQuery(db.exec("SELECT Title, Genre FROM Books WHERE Category = '" + curr_category + "'"));
    ui->tbvwBooks->setModel(tbl_model);
    ui->tbvwBooks->horizontalHeader()->setStretchLastSection(true);
    ui->tbvwBooks->verticalHeader()->setVisible(false);
    ui->tbvwBooks->setColumnWidth(0, ui->tbvwBooks->width() * 3 / 4);

    db.close();

    ui->btnResetTable->setEnabled(true);
}

void ViewWindow::loadTruncTable_genre() {
    QModelIndex curr_genre_index = ui->lsvwGenre->currentIndex();
    QString curr_genre = curr_genre_index.data(Qt::DisplayRole).toString();

    db.open();

    QSqlQueryModel *tbl_model = new QSqlQueryModel(this);
    tbl_model->setQuery(db.exec("SELECT Title, Genre FROM Books WHERE Genre = '" + curr_genre + "'"));
    ui->tbvwBooks->setModel(tbl_model);
    ui->tbvwBooks->horizontalHeader()->setStretchLastSection(true);
    ui->tbvwBooks->verticalHeader()->setVisible(false);
    ui->tbvwBooks->setColumnWidth(0, ui->tbvwBooks->width() * 3 / 4);

    db.close();

    ui->btnResetTable->setEnabled(true);
}

void ViewWindow::loadListViews() {
    db.open();

    QSqlQuery category_qry(db);
    category_qry.exec("SELECT DISTINCT Category FROM Books");
    if (category_qry.next()) {
        QSqlQueryModel *category_qrm = new QSqlQueryModel(this);
        category_qrm->setQuery(category_qry);
        ui->lsvwCategory->setModel(category_qrm);
    }

    QSqlQuery genre_qry(db);
    genre_qry.exec("SELECT DISTINCT Genre FROM Books");
    if (genre_qry.next()) {
        QSqlQueryModel *genre_qrm = new QSqlQueryModel();
        genre_qrm->setQuery(genre_qry);
        ui->lsvwGenre->setModel(genre_qrm);
    }

    db.close();
}

// This function treats book 'Title' as the primary key
// It has to be refactored, later..
void ViewWindow::loadItemInfo() {
    QModelIndex item_index = ui->tbvwBooks->currentIndex();
    QString item_name = item_index.sibling(item_index.row(), 0).data().toString();
    qDebug().noquote() << item_name;

    db.open();

    // Title
    QSqlQuery title_qry(db);
    title_qry.exec("SELECT Title FROM Books where Title = '" + item_name + "'");
    if (title_qry.next()) {
        ui->ledTitle->setText(title_qry.value(0).toString());
    }

    // Author
    QSqlQuery author_qry(db);
    author_qry.exec("SELECT Author FROM Books WHERE Title = '" + item_name + "'");
    if (author_qry.next()) {
        ui->ledAuthor->setText(author_qry.value(0).toString());
    }

    // Publisher
    QSqlQuery publisher_qry(db);
    publisher_qry.exec("SELECT Publisher FROM Books WHERE Title = '" + item_name + "'");
    if (publisher_qry.next()) {
        ui->ledPublisher->setText(publisher_qry.value(0).toString());
    }

    // Published
    QSqlQuery published_qry(db);
    published_qry.exec("SELECT DatePublished FROM Books WHERE Title = '" + item_name + "'");
    if (published_qry.next()) {
        ui->ledPublished->setText(published_qry.value(0).toString());
    }

    // ISBN
    QSqlQuery isbn_qry(db);
    isbn_qry.exec("SELECT ISBN FROM Books WHERE Title = '" + item_name + "'");
    if (isbn_qry.next()) {
        ui->ledISBN->setText(isbn_qry.value(0).toString());
    }

    // Synopsis
    QSqlQuery synopsis_qry(db);
    synopsis_qry.exec("SELECT Synopsis FROM Books WHERE Title = '" + item_name + "'");
    if (synopsis_qry.next()) {
        ui->ptxtSynopsis->setPlainText(synopsis_qry.value(0).toString());
    }

    // Cover
    QSqlQuery cover_qry(db);
    cover_qry.exec("SELECT Image FROM Books WHERE Title = '" + item_name + "'");
    if (cover_qry.next()) {
        if (cover_qry.value(0).toString().isEmpty()) {
            ui->lblCoverPhoto->setText("No Image Data");
        }
        else {
            QString cover_path = cover_qry.value(0).toString();
            QPixmap cover_img(cover_path);
            int cover_w = ui->lblCoverPhoto->width();
            int cover_h = ui->lblCoverPhoto->height();
            ui->lblCoverPhoto->setPixmap(cover_img.scaled(cover_w, cover_h, Qt::KeepAspectRatio));
        }
    }

    db.close();
}

void ViewWindow::addItem(QString title, QString author, QString publisher, QString published, QString isbn, QString category, QString genre, QString synopsis, QString cover) {
    db.open();
    db.transaction();

    QSqlQuery insert_qry(db);
    if (insert_qry.exec("INSERT INTO Books VALUES (" + isbn + ", '" + title + "', '" + author + "', '" + publisher + "', '" + published + "', '" + genre + "', '" + category + "', '" + synopsis + "', '" + cover + "')")) {
        qDebug().noquote() << "INSERT INTO Books VALUES (" + isbn + ", '" + title + "', '" + author + "', '" + publisher + "', '" + published + "', '" + genre + "', '" + category + "', '" + synopsis + "', '" + cover + "')";
        db.commit();
    }
    else {
        // missing error message
        qDebug().noquote() << "Insertion failed! Rolling back..";
        db.rollback();
    }

    loadCompTable();
    loadListViews();

    db.close();

    ui->btnResetTable->setEnabled(false);
}

void ViewWindow::deleteItem() {
    db.open();
    db.transaction();

    QMessageBox::StandardButton deletePrompt = QMessageBox::question(this, "Delete Item", "The current entry will be deleted. Continue?", QMessageBox::Yes|QMessageBox::No);

    if (deletePrompt == QMessageBox::Yes) {
        QSqlQuery delete_qry(db);
        if (delete_qry.exec("DELETE FROM Books WHERE Title = '" + ui->ledTitle->text() + "'")) {
            qDebug().noquote() << "DELETE FROM Books WHERE Title = '" + ui->ledTitle->text() + "'";
            db.commit();
            ui->ledTitle->clear();
            ui->ledAuthor->clear();
            ui->ledPublisher->clear();
            ui->ledPublished->clear();
            ui->ledISBN->clear();
            ui->ptxtSynopsis->clear();
            ui->lblCoverPhoto->setText("No Image Data");
        }
        else {
            // missing error message
            qDebug().noquote() << "Deletion failed! Rolling back..";
            db.rollback();
        }
    }

    loadCompTable();
    loadListViews();

    db.close();

    ui->btnResetTable->setEnabled(false);
}

void ViewWindow::updateItem(QString title, QString author, QString publisher, QString published, QString isbn, QString category, QString genre, QString synopsis, QString cover) {
    db.open();
    db.transaction();

    QModelIndex item_index = ui->tbvwBooks->currentIndex();
    QString item_name = item_index.sibling(item_index.row(), 0).data().toString();

    QSqlQuery update_title_qry(db),
                update_author_qry(db),
                update_publisher_qry(db),
                update_published_qry(db),
                update_isbn_qry(db),
                update_category_qry(db),
                update_genre_qry(db),
                update_synopsis_qry(db),
                update_cover_qry(db);

    if ( update_title_qry.exec("UPDATE Books SET Title = '" + title + "' WHERE Title = '" + item_name + "'") &&
         update_author_qry.exec("UPDATE Books SET Author = '" + author + "' WHERE Title = '" + item_name + "'") &&
         update_publisher_qry.exec("UPDATE Books SET Publisher = '" + publisher + "' WHERE Title = '" + item_name + "'") &&
         update_published_qry.exec("UPDATE Books SET DatePublished = '" + published + "' WHERE Title = '" + item_name + "'") &&
         update_isbn_qry.exec("UPDATE Books SET ISBN = '" + isbn + "' WHERE Title = '" + item_name + "'") &&
         update_category_qry.exec("UPDATE Books SET Category = '" + category + "' WHERE Title = '" + item_name + "'") &&
         update_genre_qry.exec("UPDATE Books SET Genre = '" + genre + "' WHERE Title = '" + item_name + "'") &&
         update_synopsis_qry.exec("UPDATE Books SET Synopsis = '" + synopsis + "' WHERE Title = '" + item_name + "'") &&
         update_cover_qry.exec("UPDATE Books SET Image = '" + cover + "' WHERE Title = '" + item_name + "'") ) {
        db.commit();
    }
    else {
        // missing error message
        qDebug() << "Update failed! Rolling back..";
        db.rollback();
    }

    loadCompTable();
    loadListViews();
    loadItemInfo();

    db.close();

    ui->btnResetTable->setEnabled(false);
    ui->ledTitle->clear();
    ui->ledAuthor->clear();
    ui->ledPublisher->clear();
    ui->ledPublished->clear();
    ui->ledISBN->clear();
    ui->ptxtSynopsis->clear();
    ui->lblCoverPhoto->clear();
    ui->lblCoverPhoto->setText("No Image Data");
}

void ViewWindow::disableAdminBtns() {
    ui->btnAdd->setEnabled(false);
    ui->actionAdd_item->setEnabled(false);
    ui->btnDelete->setEnabled(false);
    ui->actionDelete_item->setEnabled(false);
    ui->btnUpdate->setEnabled(false);
    ui->actionUpdate_item->setEnabled(false);
}

void ViewWindow::on_btnAdd_clicked()
{
    ad = new AddDialog(this);
    ad->setModal(true);
    ad->show();
    connect(ad, SIGNAL(addItem(QString, QString, QString, QString, QString, QString, QString, QString, QString)), this, SLOT(addItem(QString, QString, QString, QString, QString, QString, QString, QString, QString)));
}

void ViewWindow::on_btnDelete_clicked()
{
    deleteItem();
}

void ViewWindow::on_btnUpdate_clicked()
{
    ud = new UpdateDialog(this);
    ud->setModal(true);
    ud->show();
    connect(this, SIGNAL(loadCurrInfo(QString, QString, QString, QString, QString, QString, QString, QString, QString)), ud, SLOT(loadCurrInfo(QString, QString, QString, QString, QString, QString, QString, QString, QString)));

    QModelIndex item_index = ui->tbvwBooks->currentIndex();
    QString item_name = item_index.sibling(item_index.row(), 0).data().toString();

    db.open();

    // Category
    QString category;
    QSqlQuery category_qry(db);
    category_qry.exec("SELECT Category FROM Books WHERE Title = '" + item_name + "'");
    if (category_qry.next()) {
        category = category_qry.value(0).toString();
    }

    // Genre
    QString genre;
    QSqlQuery genre_qry(db);
    genre_qry.exec("SELECT Genre FROM Books WHERE Title = '" + item_name + "'");
    if (genre_qry.next()) {
        genre = genre_qry.value(0).toString();
    }

    // Cover
    QString cover;
    QSqlQuery cover_qry(db);
    cover_qry.exec("SELECT Image FROM Books WHERE + Title = '" + item_name + "'");
    if (cover_qry.next()) {
        qDebug().noquote() << "on_btnUpdate_clicked: cover: " << cover_qry.value(0).toString();
        cover = cover_qry.value(0).toString();
    }

    db.close();

    emit loadCurrInfo(ui->ledTitle->text(), ui->ledAuthor->text(), ui->ledPublisher->text(), ui->ledPublished->text(), ui->ledISBN->text(), category, genre, ui->ptxtSynopsis->toPlainText(), cover);

    connect(ud, SIGNAL(updateItem(QString, QString, QString, QString, QString, QString, QString, QString, QString)), this, SLOT(updateItem(QString, QString, QString, QString, QString, QString, QString, QString, QString)));
}

void ViewWindow::on_actionLogout_triggered()
{
    emit unhideWindow();
    this->close();
}

void ViewWindow::on_actionAdd_item_triggered()
{
    ad = new AddDialog(this);
    ad->setModal(true);
    ad->show();
    connect(ad, SIGNAL(addItem(QString, QString, QString, QString, QString, QString, QString, QString, QString)), this, SLOT(addItem(QString, QString, QString, QString, QString, QString, QString, QString, QString)));
}

void ViewWindow::on_actionDelete_item_triggered()
{
    deleteItem();
}

void ViewWindow::on_actionUpdate_item_triggered()
{
    ud = new UpdateDialog(this);
    ud->setModal(true);
    ud->show();
    connect(this, SIGNAL(loadCurrInfo(QString, QString, QString, QString, QString, QString, QString, QString, QString)), ud, SLOT(loadCurrInfo(QString, QString, QString, QString, QString, QString, QString, QString, QString)));

    QModelIndex item_index = ui->tbvwBooks->currentIndex();
    QString item_name = item_index.sibling(item_index.row(), 0).data().toString();

    db.open();

    // Category
    QString category;
    QSqlQuery category_qry(db);
    category_qry.exec("SELECT Category FROM Books WHERE Title = '" + item_name + "'");
    if (category_qry.next()) {
        category = category_qry.value(0).toString();
    }

    // Genre
    QString genre;
    QSqlQuery genre_qry(db);
    genre_qry.exec("SELECT Genre FROM Books WHERE Title = '" + item_name + "'");
    if (genre_qry.next()) {
        genre = genre_qry.value(0).toString();
    }

    // Cover
    QString cover;
    QSqlQuery cover_qry(db);
    cover_qry.exec("SELECT Image FROM Books WHERE + Title = '" + item_name + "'");
    if (cover_qry.next()) {
        cover = cover_qry.value(0).toString();
        qDebug().noquote() << "Cover:" << cover;
    }

    db.close();

    emit loadCurrInfo(ui->ledTitle->text(), ui->ledAuthor->text(), ui->ledPublisher->text(), ui->ledPublished->text(), ui->ledISBN->text(), category, genre, ui->ptxtSynopsis->toPlainText(), cover);

    connect(ud, SIGNAL(updateItem(QString, QString, QString, QString, QString, QString, QString, QString, QString)), this, SLOT(updateItem(QString, QString, QString, QString, QString, QString, QString, QString, QString)));
}

void ViewWindow::on_actionExit_triggered()
{
    QApplication::quit();
}



void ViewWindow::on_ledSearch_textChanged(const QString &srch)
{

        db.open();
        db.exec("select USN, FIRSTNAME from STUDENTS where USN like '" + srch + "%'");
        QSqlQueryModel *display_qrm = new QSqlQueryModel;
        display_qrm -> setQuery(db.exec("select Title, Genre from Books where Title like '%" + srch + "%'"));
        ui ->tbvwBooks->setModel(display_qrm);
        db.close();

}

void ViewWindow::on_btnResetTable_clicked()
{
    loadCompTable();
    ui->btnResetTable->setEnabled(false);
}

void ViewWindow::on_actionSummary_triggered()
{
    sd = new SummaryDialog(this);
    sd->setModal(true);
    sd->show();
}
