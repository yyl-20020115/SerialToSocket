#pragma once
#include <string>

// ʹ��CRT���wcstombs()������wstringתstring��ƽ̨�޹أ����趨locale,���ַ������Լ�������
std::string wstring_to_string(const std::wstring& str);
std::wstring string_to_wstring(const std::string& str);
