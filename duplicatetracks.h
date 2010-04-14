#ifndef DUPLICATETRACKS_H
#define DUPLICATETRACKS_H

#include "ui_duplicatetrack.h"
#include "trackParser.h"
#include <QFileDialog>
#include <QStandardItemModel>

class DuplicateTrackWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT;
public:
    DuplicateTrackWindow(QWidget* parent=0);
    bool eventFilter(QObject *target, QEvent *event);
private slots:
    void on_directoryBrowse_clicked();
    void on_findDuplicates_clicked();
public slots:
    void newDuplicate(QList<QStandardItem*> &duplicate);
    void parseFinished();
private:
    void deleteFiles(QModelIndexList &indexList);
    QFileDialog *fileDialog;
    QStandardItemModel* duplicates;
    TrackParser* parser;
    enum State {Stopped, InProgress, Paused};
    State state;
};

#endif // DUPLICATETRACKS_H
