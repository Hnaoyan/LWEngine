#pragma once
#include <string>
#include <future>

//---ASyncを使った非同期関数---//

/// <summary>
/// 全ての情報をまとめて取得する
/// </summary>
/// <returns></returns>
std::future<std::string> GetAllFacultiesAsync();
/// <summary>
/// IDの情報を取得する
/// </summary>
/// <param name="id"></param>
/// <returns></returns>
std::future<std::string> GetFacultyByIdAsync(int id);
/// <summary>
/// 新規作成
/// </summary>
/// <param name="name"></param>
/// <returns></returns>
std::future<std::string> PostFacultyAsync(const std::string& name);
/// <summary>
/// 更新
/// </summary>
/// <param name="id"></param>
/// <param name="newName"></param>
/// <returns></returns>
std::future<std::string> PatchFacultyAsync(int id, const std::string& newName);
/// <summary>
/// 削除
/// </summary>
/// <param name="id"></param>
/// <returns></returns>
std::future<std::string> DeleteFacultyAsync(int id);
