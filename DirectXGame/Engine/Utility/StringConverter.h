#pragma once
#include <string>

/// <summary>
/// 
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::wstring ConvertString(const std::string str);
/// <summary>
/// 
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::string ConvertString(const std::wstring str);

/// <summary>
/// ログ出力
/// </summary>
void Log(const std::string& message);