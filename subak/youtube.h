#pragma once
#include <QtWidgets/QMainWindow>
#include <QtNetwork>
#include <QUrl>
#include <QNetworkAccessManager>

#include <vector>
#include <utility>
#include "util.h"

class YoutubeAPI : public QObject {
	Q_OBJECT

public :
	struct MusicInfo {
		QString title;
		int duration;
		QString videoId;

		MusicInfo(QString title, QString videoId) 
			: title(title), videoId(videoId) {}
		void set_duration(QString dur) {
			duration = Util::Utility::iso8601_to_second(dur.toStdString());
		}
	};

	YoutubeAPI(QString youtube_api_key, int max_connection);

	void search_music(QString music_name);
	void search_music_done();


	void get_music_info_all();
	void get_music_info(QString video_id, int i);
	void get_music_info_done(int i);

	std::vector<MusicInfo>& get_music_info_list() {
		return search_result;
	}

	~YoutubeAPI();

private:
	QString youtube_api_key;
	QNetworkAccessManager *qnam;

	QNetworkReply *music_search_reply;
	std::vector<QNetworkReply*> get_music_reply;

	const int max_connection;

	// first : video title, second : videoId
	std::vector<MusicInfo> search_result;
	std::vector<int> current_request;

	bool parsing_search_result;
	int read_until;

signals:
	void get_music_info_all_done();
};