#include "LoginToken.h"
#include "Engine/NetWork/Http/HttpClient.h"
#include <curl/curl.h>
#include <sstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include <imgui.h>

LoginToken::LoginToken()
{
}

LoginToken::LoginToken(NetLogin* login)
{
	name_ = login->GetName();
	password_ = login->GetPassword();
	// トークン取得処理
	token_ = login->LoginToken("https://swgame-gilt.vercel.app/users/login").get();
}

LoginToken::LoginToken(const std::string& name, const std::string& password)
{
	name_ = name;
	password_ = password;
}

void LoginToken::ClearPost(int score)
{
	if (isRequest_) {
		return;
	}

	isRequest_ = true;

    postResult_ = PostScoreAsync(score).get();
    result_ = GetAllFacultiesAsync().get();	
	
}

void LoginToken::ImGuiDraw()
{
    // リクエストが来てなければ表示しない
    if (!isRequest_) return;

    ImGui::Begin("結果");
    std::string s = "送信結果:" + postResult_;
    ImGui::Text(s.c_str());
    ImGui::Text(result_.c_str());

    ImGui::End();
}

std::future<std::string> LoginToken::PostScoreAsync(int score)
{
    return std::async(std::launch::async, [score, this]() -> std::string {
        CURL* curl = curl_easy_init();
        if (!curl) return "CURL初期化エラー";

#pragma warning(push)
#pragma warning(disable : 26495)
        Json body = Json::object();
        body["score"] = score;
#pragma warning(pop)

        std::string bodyStr = body.dump();

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type:application/json");

        std::string authHeader = "Authorization: Bearer " + token_;
        headers = curl_slist_append(headers, authHeader.c_str());

        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, "https://swgame-gilt.vercel.app/scores");
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

        // エラーハンドリング
        if (res != CURLE_OK) {
            return std::string("送信エラー： ") + curl_easy_strerror(res);
        }

        //
        std::stringstream ss;
        ss << "HTTP " << httpCode << ": " << response;
        return ss.str();
        });
}

std::future<std::string> LoginToken::GetAllFacultiesAsync()
{
    return std::async(std::launch::async, [=]() -> std::string {
        CURL* curl = curl_easy_init();
        if (!curl) return "CURL初期化エラー";

#pragma warning(push)
#pragma warning(disable : 26495)
        Json body = Json::object();
        body["userName"] = name_;
#pragma warning(pop)

        std::string bodyStr = body.dump();
        
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type:application/json");

        std::string authHeader = "Authorization: Bearer " + token_;
        headers = curl_slist_append(headers, authHeader.c_str());

        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, "https://swgame-gilt.vercel.app/scores/user");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyStr.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            return std::string("取得エラー: ") + curl_easy_strerror(res);
        }

        return response;
        });
}
