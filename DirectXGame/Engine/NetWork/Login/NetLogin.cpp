#include "NetLogin.h"
#include "Engine/NetWork/Http/HttpClient.h"
#include "imgui.h"
#include <curl/curl.h>
#include <sstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include <typeinfo>


NetLogin::NetLogin()
{
	className_ = typeid(NetLogin).name();
}

NetLogin::NetLogin(const std::string& name, const std::string& password)
{
	className_ = typeid(NetLogin).name();
	name_ = name;
	password_ = password;
}

std::future<int> NetLogin::Login(const std::string& url)
{
	// -1：エラーハンドリング
	return std::async(std::launch::async, [url, this]() -> int {
		CURL* curl = curl_easy_init();

		if (!curl) return -1;

		Json body = Json::object();
		body["name"] = name_;
		body["password"] = password_;

		std::string response;
		// 送る
		std::string bodyStr = body.dump();

		struct curl_slist* headers = nullptr;
		headers = curl_slist_append(headers, "Content-Type:application/json");

		std::string token =
			"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJuYW1lIjoiVGVzdCIsImlhdCI6MTc1NjM1MTc5MywiZXhwIjoxNzU2MzU1MzkzfQ.7e47x5mBItOEcFMmC-slCoIOxJVT9lQcWEbHruRNQLA";
		std::string authHeader = "Authorization: Bearer " + token;
		headers = curl_slist_append(headers, authHeader.c_str());

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyStr.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		CURLcode res = curl_easy_perform(curl);
		// HTTPステータスコードを取得
		long httpCode = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
		// 送信エラー
		if (res != CURLE_OK) {
			return -1;
		}

		// データ取得
		try {
			Json data = Json::parse(response);
			// ある場合
			if (data.contains("login_status") && data["login_status"].is_string()) {
				std::string result = data["login_status"].get<std::string>();

				if (result == "success") {
					return 200;
				}
				else if (result == "faild") {
					return 0;
				}
				else {
					return -1;
				}
			}

			return -1;
		}
		catch (std::exception& e){
			std::cerr << "Parce Error:" << e.what() << std::endl;
			return -1;
		}

		});
}

std::future<std::string> NetLogin::LoginToken(const std::string& url)
{
	// -1：エラーハンドリング
	return std::async(std::launch::async, [url, this]() -> std::string {
		CURL* curl = curl_easy_init();

		if (!curl) return "Error";

		Json body = Json::object();
		body["name"] = name_;
		body["password"] = password_;

		std::string response;
		// 送る
		std::string bodyStr = body.dump();

		struct curl_slist* headers = nullptr;
		headers = curl_slist_append(headers, "Content-Type:application/json");

		//std::string token =
		//	"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJuYW1lIjoiVGVzdCIsImlhdCI6MTc1NjM1MTc5MywiZXhwIjoxNzU2MzU1MzkzfQ.7e47x5mBItOEcFMmC-slCoIOxJVT9lQcWEbHruRNQLA";
		//std::string authHeader = "Authorization: Bearer " + token;
		//headers = curl_slist_append(headers, authHeader.c_str());

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyStr.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		CURLcode res = curl_easy_perform(curl);
		// HTTPステータスコードを取得
		long httpCode = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
		// 送信エラー
		if (res != CURLE_OK) {
			return "Error";
		}

		// データ取得
		try {
			Json data = Json::parse(response);
			// ある場合
			if (data.contains("login_status") && data["login_status"].is_string()) {
				std::string result = data["token"].get<std::string>();

				return result;
			}

			return "None";
		}
		catch (std::exception& e) {
			std::cerr << "Parce Error:" << e.what() << std::endl;
			return "Error";
		}

		});
}

void NetLogin::ImGuiDraw()
{
	ImGui::Begin(className_.c_str());
	ImGui::Text(loginLog_.c_str());
	ImGui::Checkbox("IsLogin", &isLogin_);

	if (ImGui::Button("LOGIN")) {
		if (!isLogin_) {
			try {
				loginStatus_ = Login("https://swgame-gilt.vercel.app/users/login").get();
				if (loginStatus_ == 200) {
					loginLog_ = "SUCCESS";
					isLogin_ = true;
				}
				else {
					loginLog_ = "FAILED";
				}

			}
			catch (std::exception& e) {
				loginLog_ = "Error exception" + std::string(e.what());
			}
		}
	}
	static std::string token = "";
	if (ImGui::Button("トークン")) {
		if (!isLogin_) {
			try {
				token = LoginToken("https://swgame-gilt.vercel.app/users/login").get();
				if (token != "Error") {
					isLogin_ = true;
				}

			}
			catch (std::exception& e) {
				loginLog_ = "Error exception" + std::string(e.what());
			}
		}
	}
	char buffer[256];
	strncpy_s(buffer, sizeof(buffer), token.c_str(), _TRUNCATE);

	if (ImGui::InputText("トークンテキスト", buffer, sizeof(buffer))) {
		token = buffer; // std::string に反映
	}	
	
	ImGui::End();

}
