#include "duplicatetracks.h"
#include <QHeaderView>
#include <QEvent>
#include <QKeyEvent>
#include <QModelIndexList>
#include <QMessageBox>
DuplicateTrackWindow::DuplicateTrackWindow(QWidget* parent) : QMainWindow(parent)
{
    setupUi(this);
    fileCountEnd->setVisible(false);
    duplicatesFoundEnd->setVisible(false);
    directoryPath->setText(QDir::homePath());
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
        //if user started and stopped a search we need to update the button
        state = Stopped;
        findDuplicates->setText("Find Duplicates");
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
        duplicates->clear();
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
        connect(this, SIGNAL(destroyed()), parser, SLOT(searchPaused()));
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
    //tableView->resizeColumnsToContents();
    //tableView->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
}



void DuplicateTrackWindow::parseFinished()
{
    state = Stopped;
    findDuplicates->setText("Find duplicates");
    tableView->resizeColumnsToContents();
    tableView->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    delete parser;
}

bool DuplicateTrackWindow::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key() == Qt::Key_Delete)
        {
            if(tableView->selectionModel()->hasSelection())
            {
                QModelIndexList indexList(tableView->selectionModel()->selectedRows(2));
                if(QMessageBox::question(this, "Delete files",
                                      QString("Delete %1 files?").arg(indexList.size()),
                                      QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok)
                    deleteFiles(indexList);
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(target, event);
}

void DuplicateTrackWindow::deleteFiles(QModelIndexList &indexList)
{
    QModelIndex index;
    foreach(index, indexList)
    {
        QString path = duplicates->data(index, Qt::DisplayRole).toString();
        //noncontiguous selections create empty paths
        if(path != NULL)
            QFile::remove(path);
        duplicates->removeRow(index.row(), QModelIndex());
    }
    //TODO we should clear any items left that are no longer duplicates

}
