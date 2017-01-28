#ifndef SUBAK_H
#define SUBAK_H

#include <QtWidgets/QMainWindow>
#include <QProcess>
#include <qmessagebox.h>
#include <QByteArray>
#include "ui_subak.h"
#include <string>
#include <QtNetwork>
#include <QUrl>
#include <QNetworkAccessManager>
#include <qjsondocument.h>
#include "youtube.h"
#include "soundcloud.h"

class subak : public QMainWindow
{
	Q_OBJECT

public:
	subak(QWidget *parent = 0);
	~subak();

	void downloadDone();
	void readOutput();
	void retrieveSearchData(QString search_query);
	void querySearchDone();
	void buttonClick();

	void retrieve_youtube_search(QString search_query);
	void set_youtube_music_info();
	void set_soundcloud_music_info();

private:
	Ui::subakClass ui;
	QProcess *myProcess;
	QNetworkReply *reply;

	YoutubeAPI youtube;
	SoundCloudAPI soundcloud; 

	std::vector<YoutubeAPI::MusicInfo> youtube_music_list;
	std::vector<SoundCloudAPI::MusicInfo> soundcloud_music_list;
};

#endif // SUBAK_H
