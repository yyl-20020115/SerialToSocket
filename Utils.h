#pragma once
#include <string>

// 使用CRT库的wcstombs()函数将wstring转string，平台无关，需设定locale,这种方法可以兼容中文
std::string wstring_to_string(const std::wstring& str);
std::wstring string_to_wstring(const std::string& str);
