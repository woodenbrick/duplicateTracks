#ifndef ID3TAG_H
#define ID3TAG_H

#include <QFile>
class ID3Tag : public QObject
{
public:
    ID3Tag(QString file, QObject* parent=0);
    ~ID3Tag();
    QString getArtist() const;
    QString getTitle() const;
    QString getAlbum() const;
    QString getFullPath() const;
    bool operator==(const ID3Tag &other) const;
private:
    QString title;
    QString artist;
    QString album;
    QString year;
    QString comment;
    QString genre;
    QFile *file;
};

#endif // ID3TAG_H
