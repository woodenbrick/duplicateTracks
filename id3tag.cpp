#include "id3tag.h"
//#include "taglib/taglib.h"
//#include "taglib/fileref.h"
//#include "taglib/tag.h"
#include <QDebug>

ID3Tag::ID3Tag(QString songfile, QObject* parent) : QObject(parent)
{
    file = new QFile(songfile);
    if(! file->open(QIODevice::ReadOnly))
    {
        //deal with error
        qDebug() << file->errorString();
    }
    else
    {
        //    the layout is
        //    ID3v1 identification -> 3 characters (the word "TAG")
        //    Song Title -> 30 characters
        //    Artist -> 30 characters
        //    Album -> 30 characters
        //    Year -> 4 characters
        //    Comment -> 30 characters
        //    Genre -> 1 byte

        file->seek(file->size() - 128);
        QByteArray info = file->read(3);
        if(info != "TAG")
        {
            qDebug() << "No ID3Tag" << file->fileName();
//            //check if its ID3v2
//            file->close();
//            TagLib::FileRef f(songfile.toUtf8());
//            if(f.tag()->isEmpty())
//            {
//                qDebug() << "No ID3v2";
//                return;
//            }
//            title = f.tag()->title().toCString(true);
//            artist = f.tag()->artist().toCString(true);
//            album = f.tag()->album().toCString(true);
//            year = QString::number(f.tag()->year());
//            comment = f.tag()->comment().toCString(true);
//            genre = f.tag()->genre().toCString(true);
        }
        else
        {
            title = file->read(30);
            artist = file->read(30);
            album = file->read(30);
            year = file->read(4);
            comment = file->read(30);
            genre = file->read(1);
        }
    file->close();
    }
}

ID3Tag::~ID3Tag()
{
    delete file;
}

bool ID3Tag::operator==(const ID3Tag &other) const
{
    if(other.artist == artist && other.title == title)
        return true;
    return false;
}

QString ID3Tag::getArtist() const
{
    return artist;
}

QString ID3Tag::getTitle() const
{
    return title;
}

QString ID3Tag::getAlbum() const
{
    return album;
}

QString ID3Tag::getFullPath() const
{
    return file->fileName();
}
