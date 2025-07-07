#include "HttpScoreClient.h"
#include "Engine/Utility/StringConverter.h"
#include "HttpClient.h"
#include <curl/curl.h>
#include <sstream>
#include <iostream>
#include <locale>
#include <codecvt>

std::future<std::string> HttpScoreClient::PostScoreAsync(int score)
{
    return std::async(std::launch::async, [score]() -> std::string {
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

        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/scores");
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

std::future<std::string> HttpScoreClient::GetAllScoreAsync()
{
    return std::async(std::launch::async, []() -> std::string {
        CURL* curl = curl_easy_init();
        if (!curl) return "CURL初期化エラー";

        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/scores");
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
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
