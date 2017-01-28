#include "youtube.h"

YoutubeAPI::YoutubeAPI(QString youtube_api_key, int max_connection) 
	: youtube_api_key(youtube_api_key), max_connection(max_connection),
	  get_music_reply(max_connection, NULL), current_request(max_connection, NULL)
{
	qnam = new QNetworkAccessManager(NULL);

	parsing_search_result = false;
	read_until = 0;
}

void YoutubeAPI::search_music(QString music_name)
{
	QString request_url_str = "https://www.googleapis.com/youtube/v3/search?part=snippet&maxResults=30&q=";
	request_url_str += QUrl::toPercentEncoding(music_name);
	request_url_str += ("&key=" + youtube_api_key);


	QNetworkRequest request = QNetworkRequest(QUrl(request_url_str));
	music_search_reply = qnam->get(request);

	connect(music_search_reply, &QNetworkReply::finished, this, &YoutubeAPI::search_music_done);
}
void YoutubeAPI::search_music_done()
{
	// Network Error occured
	auto s = music_search_reply->error();
	if (s != QNetworkReply::NoError) {
		return;
	}

	music_search_reply->close();
	
	QJsonDocument search_result_json_doc = QJsonDocument::fromJson(music_search_reply->readAll());
	QJsonObject search_result_json_obj = search_result_json_doc.object();
	QJsonArray search_result_arr = search_result_json_obj["items"].toArray();
	
	search_result.clear(); 

	for (int i = 0; i < search_result_arr.size(); i++) {
		QJsonObject search_item = search_result_arr[i].toObject();

		QJsonObject search_item_snippet = search_item["snippet"].toObject();
		QJsonObject search_item_id = search_item["id"].toObject();

		QString title = search_item_snippet.value("title").toString();
		QString videoId = search_item_id.value("videoId").toString();
		search_result.push_back(MusicInfo(title, videoId));
	}

	get_music_info_all();
}

// Find the duration and detailed info of the videos from the search result 
void YoutubeAPI::get_music_info_all()
{
	if (parsing_search_result) {
		// If there is nothing more to read
		if (search_result.size() <= read_until) return;

		// Look for possible connection 
		for (int i = 0; i < get_music_reply.size(); i++) {
			if (!get_music_reply[i]) {
				get_music_info(search_result[read_until].videoId, i);
				current_request[i] = read_until;
				break;
			}
		}

		read_until++;
		return;
	}
	else parsing_search_result = true;

	// Remove the search result that does not return the video ID
	for (int i = 0; i < search_result.size(); i++) {
		if (search_result[i].videoId.length() == 0) {
			search_result.erase(search_result.begin() + i);
			i--; 
		}
	}

	for (read_until = 0; read_until < max_connection; read_until ++) {
		if (search_result.size() <= read_until) return;

		get_music_info(search_result[read_until].videoId, read_until);
		current_request[read_until] = read_until;
	}
}

void YoutubeAPI::get_music_info(QString video_id, int i)
{
	QString request_url_str = "https://www.googleapis.com/youtube/v3/videos?part=snippet,contentDetails&id=";
	request_url_str += video_id;
	request_url_str += "&key=" + youtube_api_key;

	QNetworkRequest request = QNetworkRequest(QUrl(request_url_str));
	get_music_reply[i] = qnam->get(request);

	connect(get_music_reply[i], &QNetworkReply::finished, this, [this, i] { get_music_info_done(i); });
}
void YoutubeAPI::get_music_info_done(int i)
{
	// Network Error occured
	if (get_music_reply[i]->error() != QNetworkReply::NoError) {
		get_music_reply[i] = NULL;

		// If every slot is available and fully requested every item in the search result
		// Then the request is over 
		for (int i = 0; i < get_music_reply.size(); i++) {
			if (get_music_reply[i] != NULL) {
				get_music_info_all();
				return;
			}
		}
		if (search_result.size() <= read_until) {
			parsing_search_result = false;
			emit get_music_info_all_done();
		}

		return;
	}
	get_music_reply[i]->close();

	QJsonDocument search_result_json_doc = QJsonDocument::fromJson(get_music_reply[i]->readAll());
	QJsonObject search_result_json_obj = search_result_json_doc.object();
	QJsonArray search_result_arr = search_result_json_obj["items"].toArray();

	QJsonObject music_info_item = search_result_arr[0].toObject();
	QJsonObject music_info_detail = music_info_item["contentDetails"].toObject();

	QString duration = music_info_detail.value("duration").toString();
	search_result[current_request[i]].set_duration(duration);

	// Make the slot available for the another request
	get_music_reply[i] = NULL;

	// If every slot is available and fully requested every item in the search result
	// Then the request is over 
	for (int i = 0; i < get_music_reply.size(); i++) {
		if (get_music_reply[i] != NULL) {
			get_music_info_all();
			return;
		}
	}
	if (search_result.size() <= read_until) {
		parsing_search_result = false;
		emit get_music_info_all_done();
	}
}
YoutubeAPI::~YoutubeAPI()
{
	delete qnam;
}