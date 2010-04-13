#include "duplicatetracks.h"
#include <QDebug>
#include <QHeaderView>
#include <QEvent>
#include <QKeyEvent>
DuplicateTrackWindow::DuplicateTrackWindow(QWidget* parent) : QMainWindow(parent)
{
    setupUi(this);
    fileCountEnd->setVisible(false);
    duplicatesFoundEnd->setVisible(false);
    directoryPath->setText(QDir::homePath() + "/music/MGMT - Congratulations [2010]/");
    state = Stopped;
    duplicates = new QStandardItemModel();
    tableView->setModel(duplicates);
    tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->installEventFilter(this);
    duplicates->setHorizontalHeaderItem(0, new QStandardItem("Title"));
    duplicates->setHorizontalHeaderItem(1, new QStandardItem("Artist"));
    duplicates->setHorizontalHeaderItem(2, new QStandardItem("Path"));
    tableView->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
}

void DuplicateTrackWindow::on_directoryBrowse_clicked()
{
    fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::DirectoryOnly);
    fileDialog->setDirectory(QDir(directoryPath->text()));
    if(fileDialog->exec())
    {
        directoryPath->setText(fileDialog->selectedFiles().at(0));
    }
}

void DuplicateTrackWindow::on_findDuplicates_clicked()
{
    if(state == InProgress)
    {
        state = Paused;
        parser->searchPaused();
        findDuplicates->setText("Continue");
    }
    else if(state == Stopped)
    {
        state = InProgress;
        parser = new TrackParser(directoryPath->text());
        duplicatesFoundCount->setNum(0);
        fileCount->setNum(0);
        fileCountEnd->setVisible(true);
        duplicatesFoundEnd->setVisible(true);
        findDuplicates->setText("Stop");
        connect(parser, SIGNAL(tracksCounted(int)), fileCount, SLOT(setNum(int)));
        connect(parser, SIGNAL(newDuplicateFound(QList<QStandardItem*>&)),
                this, SLOT(newDuplicate(QList<QStandardItem*>&)));
        connect(parser, SIGNAL(finished()), this, SLOT(parseFinished()));
        parser->start();
    }
    else
    {
        state = InProgress;
        parser->searchContinued();
        findDuplicates->setText("Stop");
    }
}

void DuplicateTrackWindow::newDuplicate(QList<QStandardItem*> &duplicate)
{
    QString path = duplicate.at(2)->text();
    QList<QStandardItem*> items = duplicates->findItems(path, Qt::MatchExactly, 2);
    if(items.isEmpty())
    {
        duplicates->appendRow(duplicate);
    }
    duplicatesFoundCount->setNum(duplicates->rowCount());
}



void DuplicateTrackWindow::parseFinished()
{
    state = Stopped;
    findDuplicates->setText("Find duplicates");
    tableView->resizeColumnsToContents();
    delete parser;
}

bool DuplicateTrackWindow::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key() == Qt::Key_Delete)
        {
            qDebug() << "Delete selected";
            return true;
        }
    }
    return QMainWindow::eventFilter(target, event);
}
