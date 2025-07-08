#pragma once
#include <json.hpp>
#include "pplx/pplxtasks.h"

class NetLogin
{
private:
	using Json = nlohmann::json;

public:
	pplx::task<int> Login(const std::wstring& url);

};
