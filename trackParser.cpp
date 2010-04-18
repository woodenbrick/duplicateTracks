
#include <QFile>
#include <QDirIterator>
#include <QStandardItem>
#include <QCoreApplication>
#include "trackParser.h"

TrackParser::TrackParser(QString dir)
{
    running = false;
    trackCount = 0;
    dirIter = new QDirIterator(dir, QDirIterator::Subdirectories);
}

TrackParser::~TrackParser()
{
    songs.clear();
}

void TrackParser::start()
{
    running = true;
    while(dirIter->hasNext())
    {
        if(!running)
            return;
        QString file(dirIter->next());
        if (!file.endsWith(".mp3"))
        {
            //stop nonresponsive gui during check of files that arent mp3s
            QCoreApplication::processEvents();
            continue;
        }
        ID3Tag* song = new ID3Tag(file, this);
        if (song->getTitle() == "")
            continue;
        trackCount++;
        if(trackCount % 100 == 0)
            emit tracksCounted(trackCount);
        QCoreApplication::processEvents();
        for(int i=0; i<songs.size(); i++)
        {
            if(*song == *songs.at(i))
            {
                makeStandardItem(song);
                makeStandardItem(songs.at(i));
                break;
            }
        }
        songs.append(song);
    }
    emit tracksCounted(trackCount);
    emit finished();
}

void TrackParser::searchPaused()
{
    running = false;
}

void TrackParser::searchContinued()
{
    running = true;
    start();
}

bool TrackParser::isRunning()
{
    return running;
}

void TrackParser::makeStandardItem(const ID3Tag* song)
{
    QList<QStandardItem*> info;
    info.append(new QStandardItem(song->getTitle()));
    info.append(new QStandardItem(song->getArtist()));
    info.append(new QStandardItem(song->getFullPath()));
    emit newDuplicateFound(info);
}


