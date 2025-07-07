#pragma once
#include <string>
#include <future>
#include "json.hpp"

class HttpScoreClient
{
private:
	using Json = nlohmann::json;
public:
	std::future<std::string> PostScoreAsync(int score);
	std::future<std::string> GetAllScoreAsync();

};
