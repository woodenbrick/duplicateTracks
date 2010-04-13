#ifndef TRACKPARSER_H
#define TRACKPARSER_H

#include <QDirIterator>
#include "id3tag.h"
#include <QStandardItem>

class TrackParser : public QObject
{
    Q_OBJECT;
public:
    TrackParser(QString dir);
    ~TrackParser();
    void start();
    bool isRunning();
    void setRunning(bool);
signals:
    void newDuplicateFound(QList<QStandardItem*>&);
    void tracksCounted(int);
    void finished();
public slots:
    void searchPaused();
    void searchContinued();
private:
    QList<ID3Tag*> songs;
    QDirIterator *dirIter;
    QStringList filetypes;
    int trackCount;
    void makeStandardItem(const ID3Tag*);
    bool running;
};

#endif // TRACKPARSER_H
