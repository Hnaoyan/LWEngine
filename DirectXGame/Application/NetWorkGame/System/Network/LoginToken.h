#pragma once
#include "Engine/NetWork/Login/NetLogin.h"

class LoginToken
{
public:
	LoginToken();
	LoginToken(NetLogin* login);
	LoginToken(const std::string& name, const std::string& password);
public:
	// POST（データ送信）/POST(データ受信）
	void ClearPost(int score);

	std::future<std::string> PostScoreAsync(int score);
	std::future<std::string> GetAllFacultiesAsync();

	void ImGuiDraw();

private:
	using Json = nlohmann::json;

	std::string name_ = "";
	std::string password_ = "";
	std::string token_ = "";

	bool isRequest_ = false;

	std::string postResult_ = "";
	std::string result_ = "";
};
