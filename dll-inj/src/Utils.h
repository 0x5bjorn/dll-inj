#pragma once

#include <string>
#include <codecvt>
#include <windows.h>

using convert_type = std::codecvt_utf8<wchar_t>;
static std::wstring_convert<convert_type, wchar_t> converter;

static std::string ConvertWcharToString(WCHAR* wchar)
{
    std::wstring wstringToCovnert(wchar);
    return converter.to_bytes(wstringToCovnert);
}

static std::string ConvertWstringToString(std::wstring wstring)
{
    return converter.to_bytes(wstring);
}