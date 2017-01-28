#include "subak.h"

subak::subak(QWidget *parent)
	: QMainWindow(parent), youtube("AIzaSyB8xCOwiCzkQPI_KAo8YFteWU5pJ3XIIAc", 5),
	soundcloud("7Syc3T5ayTlpAMXafqQoIIku9F3ZXXip", 5)
{
	ui.setupUi(this);

	QString program = "youtube-dl.exe";
	QStringList arguments;
	arguments << "https://www.youtube.com/watch?v=EEVeFhiyoFw -v";

	myProcess = new QProcess(parent);
	myProcess->start(program, arguments);
	connect(myProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &subak::downloadDone);
	connect(myProcess, &QProcess::readyReadStandardOutput, this, &subak::readOutput);
	connect(ui.pushButton, &QPushButton::clicked, this, &subak::buttonClick);
}

void subak::retrieve_youtube_search(QString search_query)
{
	youtube.search_music(search_query);
	soundcloud.search_music(search_query);

	connect(&youtube, &YoutubeAPI::get_music_info_all_done, this, &subak::set_youtube_music_info);
	connect(&soundcloud, &SoundCloudAPI::get_music_info_all_done, this, &subak::set_soundcloud_music_info);
}

void subak::set_youtube_music_info()
{
	youtube_music_list = youtube.get_music_info_list();

	if (soundcloud_music_list.size()) {
		ui.tableWidget2->clearContents();
		ui.tableWidget2->setColumnCount(3);
		ui.tableWidget2->setRowCount(youtube_music_list.size() + soundcloud_music_list.size());

		int i; 
		for (i = 0; i < youtube_music_list.size(); i++) {
			auto item = youtube_music_list[i];

			ui.tableWidget2->setItem(i, 0, new QTableWidgetItem(item.title));
			ui.tableWidget2->setItem(i, 1, new QTableWidgetItem(item.videoId));
			ui.tableWidget2->setItem(i, 2, new QTableWidgetItem(QString::number(item.duration)));
		}

		for (int j = 0; j < soundcloud_music_list.size(); j++) {
			auto item = soundcloud_music_list[j];

			ui.tableWidget2->setItem(i + j, 0, new QTableWidgetItem(item.title));
			ui.tableWidget2->setItem(i + j, 1, new QTableWidgetItem(item.video_link));
			ui.tableWidget2->setItem(i + j, 2, new QTableWidgetItem(QString::number(item.duration)));
		}
	}
}

void subak::set_soundcloud_music_info()
{
	soundcloud_music_list = soundcloud.get_music_info_list();

	if (youtube_music_list.size()) {
		ui.tableWidget2->clearContents();
		ui.tableWidget2->setColumnCount(3);
		ui.tableWidget2->setRowCount(youtube_music_list.size() + soundcloud_music_list.size());

		int i;
		for (i = 0; i < youtube_music_list.size(); i++) {
			auto item = youtube_music_list[i];

			ui.tableWidget2->setItem(i, 0, new QTableWidgetItem(item.title));
			ui.tableWidget2->setItem(i, 1, new QTableWidgetItem(item.videoId));
			ui.tableWidget2->setItem(i, 2, new QTableWidgetItem(QString::number(item.duration)));
		}

		for (int j = 0; j < soundcloud_music_list.size(); j++) {
			auto item = soundcloud_music_list[j];

			ui.tableWidget2->setItem(i + j, 0, new QTableWidgetItem(item.title));
			ui.tableWidget2->setItem(i + j, 1, new QTableWidgetItem(item.video_link));
			ui.tableWidget2->setItem(i + j, 2, new QTableWidgetItem(QString::number(item.duration)));
		}
	}
}

void subak::buttonClick() {
	QString search_query = ui.textEdit->document()->toPlainText();
	retrieveSearchData(search_query);
	retrieve_youtube_search(search_query);
}

// Get the search result from the query 
void subak::retrieveSearchData(QString search_query)
{
	QNetworkRequest request = QNetworkRequest(QUrl("http://appsvr.melon.com/cds/search/player/searchsong_searchBySong2.json?SubSearchType=0&SortFlag=1&SearchData="));

	request.setRawHeader("User-Agent", "MelOn Player 5.16.1004.18 Windows8 6.3");
	request.setRawHeader("Host", "appsvr.melon.com");
	request.setRawHeader("Content-Type", "application/json; charset=UTF-8");

	QByteArray request_body = R"(
		{
		  "DETAILSEARCHDATA": "",
		  "GROUPBYKEY": "",
		  "ID": "Unknown",
		  "MENUADDSTYLE": 0,
		  "ORDERTYPE": 1,
		  "PAGESIZE": 100,
		  "PERIODID": "",
		  "REQUESTURI": "searchBySong2",
		  "SEARCHDATA":
	)";
	request_body.append("\"" + search_query + "\",\n");
	request_body.append(R"(
		"SELECTTYPE": 0,
		  "SORTFLAG": 1,
		  "STARTINDEX": 1,
		  "STATICID": "43010101",
		  "STATISTICID": "43010101",
		  "SUBSEARCHTYPE": 0,
		  "TYPE": 0,
		  "USERNO": "0"
		}
	)");
	
	QNetworkAccessManager* qnam = new QNetworkAccessManager(NULL);
	reply = qnam->post(request, request_body);
	connect(reply, &QNetworkReply::finished, this, &subak::querySearchDone);
}
void subak::querySearchDone()
{
	// Network Error occured
	if (reply->error() != QNetworkReply::NoError) {
		QMessageBox *box = new QMessageBox;
		box->show();
		return;
	}

	reply->close();

	QByteArray data = reply->readAll();

	QJsonDocument data_json = QJsonDocument::fromJson(data);
	QJsonObject json_obj = data_json.object();
	QJsonArray json_arr = json_obj["LIST"].toArray();

	ui.tableWidget->clearContents();
	ui.tableWidget->setColumnCount(3);
	ui.tableWidget->setRowCount(json_arr.size());

	for (int i = 0; i < json_arr.size(); i++) {
		QJsonObject inside_obj = json_arr[i].toObject();
		QString val = inside_obj.value("ALBUMNAME").toString();
		
		QString artist_name = "";
		QJsonArray artist_info = inside_obj["ARTISTINFO"].toArray();
		for (int j = 0; j < artist_info.size(); j++) {
			QJsonObject artist_obj = artist_info[j].toObject();
			QJsonValue v_artist_name = artist_obj.value("ARTISTNAME");

			if (j > 0) artist_name += ", ";
			artist_name += v_artist_name.toString();
		}

		QString play_time = inside_obj.value("PLAYTIME").toString();

		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(artist_name));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(val));
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(play_time));
	}

	ui.plainTextEdit->document()->setPlainText(data);
}
void subak::downloadDone()
{

}

void subak::readOutput() {

}

subak::~subak()
{

}

/* 

	Melon POST Request Format 

	http://appsvr.melon.com/cds/search/player/searchsong_searchBySong2.json?SubSearchType=0&SortFlag=1&SearchData=

	Headers 
		User-Agent		MelOn Player 5.16.1004.18 Windows8 6.3
		Host			appsvr.melon.com
		Content-Type	application/json; charset=UTF-8

	Body
		{
		  "DETAILSEARCHDATA": "",
		  "GROUPBYKEY": "",
		  "ID": "Unknown",
		  "MENUADDSTYLE": 0,
		  "ORDERTYPE": 1,
		  "PAGESIZE": 100,
		  "PERIODID": "",
		  "REQUESTURI": "searchBySong2",
		  "SEARCHDATA": "don't stop me now",
		  "SELECTTYPE": 0,
		  "SORTFLAG": 1,
		  "STARTINDEX": 1,
		  "STATICID": "43010101",
		  "STATISTICID": "43010101",
		  "SUBSEARCHTYPE": 0,
		  "TYPE": 0,
		  "USERNO": "0"
		}
*/