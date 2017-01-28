#pragma once
#include <QtWidgets/QMainWindow>
#include <QString>
#include <QtNetwork>
#include <QUrl>
#include <QNetworkAccessManager>

#include <vector>
#include "HTMLParser.h"

class SoundCloudAPI : public QObject {
	Q_OBJECT

public :
	struct MusicInfo {
		QString title;
		int duration;
		QString video_link;

		MusicInfo(QString title, QString duration, QString video_link)
			: video_link(video_link), title(title) 
		{
			set_duration(duration);
		}

		void set_duration(QString str) {
			int millisec = str.toInt();
			duration = millisec / 1000;
		}
	};

	SoundCloudAPI(QString soundcloud_api_key, int max_connection);
	void search_music(QString music_name);

	void search_music_done();
	void get_music_info_all();
	void get_music_info(QString music_link, int i);
	void get_music_info_done(int i);
	std::vector<MusicInfo>& get_music_info_list() {
		return music_list;
	}

private :
	QString soundcloud_api_key;
	const int max_connection; 

	QNetworkAccessManager* qnam;
	QNetworkReply* music_search_reply;
	std::vector<QNetworkReply*> get_music_reply;
	std::vector<MusicInfo> music_list;

	int read_until; 
	bool parsing_search_result;
	std::vector<int> current_request;

signals:
	void get_music_info_all_done();
};