#pragma once
#include <json.hpp>
#include <future>

class NetLogin
{
public:
	NetLogin();
	NetLogin(const std::string& name, const std::string& password);
private:
	using Json = nlohmann::json;

	std::string className_;
	std::string name_ = "";
	std::string password_ = "";

	int loginStatus_ = 0;
	std::string loginLog_ = "";
	bool isLogin_ = false;

public:
	/// <summary>
	/// ログイン
	/// </summary>
	/// <param name="url"></param>
	/// <returns></returns>
	std::future<int> Login(const std::string& url);

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
};
