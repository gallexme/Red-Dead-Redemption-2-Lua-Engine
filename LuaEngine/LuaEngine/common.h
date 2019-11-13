// stdafx.h: Includedatei für Include-Standardsystemdateien
// oder häufig verwendete projektspezifische Includedateien,
// die nur in unregelmäßigen Abständen geändert werden.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Selten verwendete Komponenten aus Windows-Headern ausschließen
// Windows-Headerdateien
#include <windows.h>
#include "framework.h"
#include <string>
#include <iostream>
#include <cstdint>
#include <vector>
#include "alt-log.h"

#include <lua.hpp>
#define DLL_EXPORT __declspec(dllexport)

using namespace alt;

template<typename First, typename ... T>
bool is_in(First&& first, T&& ... t)
{
    return ((first == t) || ...);
}

static std::wstring GetModulePath(HMODULE module)
{
    DWORD size = MAX_PATH;
    std::vector<wchar_t> buffer(size);

    do
    {
        buffer.resize(size);
        GetModuleFileNameW(module, buffer.data(), size);
        size *= 1.5;
    } while (GetLastError() == ERROR_INSUFFICIENT_BUFFER);

    std::wstring modulePath = std::wstring(buffer.begin(), buffer.end());

    size_t slashPos = modulePath.size();
    for (int i = modulePath.size() - 1; i >= 0; --i)
    {
        if (modulePath[i] == L'/' || modulePath[i] == L'\\') {
            slashPos = i;
            break;
        }
    }

    std::wstring moduleDir = modulePath.substr(0, slashPos);
    return moduleDir;
}


// Hier auf zusätzliche Header verweisen, die das Programm benötigt.
