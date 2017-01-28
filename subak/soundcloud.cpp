#include "soundcloud.h"

SoundCloudAPI::SoundCloudAPI(QString soundcloud_api_key, int max_connection) :
	max_connection(max_connection), soundcloud_api_key(soundcloud_api_key), 
	get_music_reply(max_connection, NULL), current_request(max_connection, NULL)
{
	qnam = new QNetworkAccessManager(NULL);

	parsing_search_result = false;
}

void SoundCloudAPI::search_music(QString music_name)
{
	QString request_url_str = "http://localhost/search";
	// request_url_str += QUrl::toPercentEncoding(music_name);

	QJsonObject post_data;
	post_data.insert("search_query", music_name);

	QNetworkRequest request = QNetworkRequest(QUrl(request_url_str));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	music_search_reply = qnam->post(request, QJsonDocument(post_data).toJson());

	connect(music_search_reply, &QNetworkReply::finished, this, &SoundCloudAPI::search_music_done);
}

void SoundCloudAPI::search_music_done()
{
	auto s = music_search_reply->error();
	if (s != QNetworkReply::NoError) {
		return;
	}

	music_search_reply->close();

	/*
	QString search_result = music_search_reply->readAll();
	Parser::HTMLParser parser(search_result.toStdWString());

	// SoundCloud Search HTML format 
	// First <ul><li>....</ul> is ignored 
	// Second <ul><li><a href="/../.."></a></li><li><....></li></ul> 
	// is the part where it contains the search result 

	Parser::DOMTree search_result_dom = parser.DOM();
	auto itr = search_result_dom.begin();

	bool first_ul_tag_passed = false; 
	while (itr != search_result_dom.end()) {
		if (itr->tag == L"ul") {
			if (!first_ul_tag_passed) {
				first_ul_tag_passed = true;
			}
			else {
				++itr;
				while (itr != search_result_dom.end()) {
					if (itr->tag == L"a") {
						music_list.push_back(MusicInfo(QString::fromStdWString(itr->attr[L"href"])));
					}
					else if (itr->tag != L"li" && itr->tag != L"h2") break;
					++ itr; 
				}
			}
		}
		++ itr; 
	}

	get_music_info_all();

	*/
	
	QJsonDocument search_result_json_doc = QJsonDocument::fromJson(music_search_reply->readAll());
	QJsonObject search_result_json_obj = search_result_json_doc.object();
	QJsonArray search_result_arr = search_result_json_obj["result"].toArray();

	for (int i = 0; i < search_result_arr.size(); i++) {
		QJsonObject search_item = search_result_arr[i].toObject();

		QString title = search_item.value("title").toString();
		QString duration = search_item.value("duration").toString();
		QString url = search_item.value("permalink_url").toString();

		music_list.push_back(MusicInfo(title, duration, url));
	}

	emit get_music_info_all_done();
}

void SoundCloudAPI::get_music_info_all()
{
	if (parsing_search_result) {
		if (music_list.size() <= read_until) return;
		
		for (int i = 0; i < get_music_reply.size(); i++) {
			if (!get_music_reply[i]) {
				get_music_info(music_list[read_until].video_link, i);
				current_request[i] = read_until;
				break; 
			}
		}
		
		read_until++;
		return;
	}
	else parsing_search_result = true;

	for (read_until = 0; read_until < max_connection; read_until++) {
		if (music_list.size() <= read_until) return;

		get_music_info(music_list[read_until].video_link, read_until);
		current_request[read_until] = read_until;
	}
}

void SoundCloudAPI::get_music_info(QString music_link, int i) 
{
	QString request_url_str = "http://api.soundcloud.com/resolve?url=http://soundcloud.com";
	request_url_str += music_link;
	request_url_str += ("&client_id=" + soundcloud_api_key);

	QNetworkRequest request = QNetworkRequest(QUrl(request_url_str));
	get_music_reply[i] = qnam->get(request);

	connect(get_music_reply[i], &QNetworkReply::finished, this, [this, i] { get_music_info_done(i); });
}

// SoundCloud resolve API redirects to the another page. 
// That means, we need to send another query to get the correct response 
void SoundCloudAPI::get_music_info_done(int i)
{
	if (get_music_reply[i]->error() != QNetworkReply::NoError) {
		get_music_reply[i]->close();
		get_music_reply[i] = NULL;

		// If every slot is available and fully requested every item in the search result
		// Then the request is over 
		for (int i = 0; i < get_music_reply.size(); i++) {
			if (get_music_reply[i] != NULL) {
				get_music_info_all();
				return;
			}
		}
		if (music_list.size() <= read_until) {
			parsing_search_result = false;
			emit get_music_info_all_done();
		}
		return ;
	}

	get_music_reply[i]->close(); 

	auto reply = get_music_reply[i]->readAll();
	QJsonDocument search_result_json_doc = QJsonDocument::fromJson(reply);
	QJsonObject search_result_json_obj = search_result_json_doc.object();

	QString redirect_url = search_result_json_obj.value("location").toString();

	if (redirect_url.length()) {
		QNetworkRequest request = QNetworkRequest(QUrl(redirect_url));
		get_music_reply[i] = qnam->get(request);

		connect(get_music_reply[i], &QNetworkReply::finished, this, [this, i] { get_music_info_done(i); });

		return; 
	}
	else {
		music_list[current_request[i]].set_duration(search_result_json_obj.value("duration").toString());
		music_list[current_request[i]].title = search_result_json_obj.value("title").toString();
	}

	get_music_reply[i] = NULL;

	// If every slot is available and fully requested every item in the search result
	// Then the request is over 
	for (int i = 0; i < get_music_reply.size(); i++) {
		if (get_music_reply[i] != NULL) {
			get_music_info_all();
			return;
		}
	}
	if (music_list.size() <= read_until) {
		parsing_search_result = false;
		emit get_music_info_all_done();
	}
}