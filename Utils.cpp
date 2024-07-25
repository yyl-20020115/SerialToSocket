#include "pch.h"
#include "Utils.h"
#include <locale>

// ʹ��CRT���wcstombs()������wstringתstring��ƽ̨�޹أ����趨locale,���ַ������Լ�������
std::string wstring_to_string(const std::wstring& str)
{
	char* loc = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "");

	const wchar_t* _Source = str.c_str();
	size_t _Dsize = 2 * str.size() + 1;
	char* _Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	wcstombs(_Dest, _Source, _Dsize);

	std::string result = _Dest;
	delete[] _Dest;

	setlocale(LC_ALL, loc);
	return result;
}
std::wstring string_to_wstring(const std::string& str)
{
	char* loc = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "");

	const char* _Source = str.c_str();
	size_t _Dsize = str.size();
	wchar_t* _Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);

	std::wstring result = _Dest;
	delete[] _Dest;

	setlocale(LC_ALL, loc);
	return result;
}
