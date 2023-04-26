#pragma once

#include "stack_walker.h"
#include <stdio.h>
#include <tchar.h>
#include <minidumpapiset.h>
#include "ve_log_lite.h"

// secure-CRT_functions are only available starting with VC8
#if _MSC_VER < 1400
#define _tcscpy_s _tcscpy
#define _tcscat_s _tcscat
#define _stprintf_s _stprintf
#define strcpy_s(a, b, c) strcpy(a, c)
#endif


// Specialized stackwalker-output classes
// Console (printf):
class StackWalkerToLog : public StackWalker {
protected:
    virtual void OnOutput(LPCSTR szText) {
        //printf("%s", szText);
        veLOGE(szText);
    }
};

static bool CreateCrashDumpFile(TCHAR* const file, EXCEPTION_POINTERS* pExPtrs) {
    // 创建文件
    HANDLE file_dump = CreateFile(file, GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file_dump == INVALID_HANDLE_VALUE) {
        DWORD err = GetLastError();
        veLOGE("CreateFile failed with last error: {}", err);
        return false;
    }

    MINIDUMP_EXCEPTION_INFORMATION mei;
    mei.ThreadId = GetCurrentThreadId();
    mei.ExceptionPointers = pExPtrs;
    mei.ClientPointers = true;

    // 写入文件
    BOOL ret = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file_dump,
        MINIDUMP_TYPE::MiniDumpWithFullMemory, &mei, nullptr, nullptr);
    if (!ret) {
        DWORD err = GetLastError();
        veLOGE("MiniDumpWriteDump failed with last error: {}", err);
        CloseHandle(file_dump);
        return false;
    }

    // 关闭文件
    CloseHandle(file_dump);
    return true;
}

// For more info about "PreventSetUnhandledExceptionFilter" see:
// "SetUnhandledExceptionFilter" and VC8
// http://blog.kalmbachnet.de/?postid=75
// and
// Unhandled exceptions in VC8 and above?for x86 and x64
// http://blog.kalmbach-software.de/2008/04/02/unhandled-exceptions-in-vc8-and-above-for-x86-and-x64/
// Even better: http://blog.kalmbach-software.de/2013/05/23/improvedpreventsetunhandledexceptionfilter/

#if defined(_M_X64) || defined(_M_IX86)
static BOOL PreventSetUnhandledExceptionFilter() {
    HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));
    if (hKernel32 == NULL)
        return FALSE;
    void* pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
    if (pOrgEntry == NULL)
        return FALSE;

#ifdef _M_IX86
    // Code for x86:
    // 33 C0                xor         eax,eax
    // C2 04 00             ret         4
    unsigned char szExecute[] = { 0x33, 0xC0, 0xC2, 0x04, 0x00 };
#elif _M_X64
    // 33 C0                xor         eax,eax
    // C3                   ret
    unsigned char szExecute[] = { 0x33, 0xC0, 0xC3 };
#else
#error "The following code only works for x86 and x64!"
#endif

    DWORD dwOldProtect = 0;
    BOOL  bProt = VirtualProtect(pOrgEntry, sizeof(szExecute), PAGE_EXECUTE_READWRITE, &dwOldProtect);

    SIZE_T bytesWritten = 0;
    BOOL   bRet = WriteProcessMemory(GetCurrentProcess(), pOrgEntry, szExecute, sizeof(szExecute),
        &bytesWritten);

    if ((bProt != FALSE) && (dwOldProtect != PAGE_EXECUTE_READWRITE)) {
        DWORD dwBuf;
        VirtualProtect(pOrgEntry, sizeof(szExecute), dwOldProtect, &dwBuf);
    }
    return bRet;
}
#else
#pragma message("This code works only for x86 and x64!")
#endif

static TCHAR s_szExceptionLogFileName[_MAX_PATH] = _T("\\exceptions.log"); // default
static BOOL  s_bUnhandledExeptionFilterSet = FALSE;
static LONG __stdcall CrashHandlerExceptionFilter(EXCEPTION_POINTERS* pExPtrs) {
#ifdef _M_IX86
    if (pExPtrs->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW) {
        static char MyStack[1024 * 128]; // be sure that we have enough space...
        // it assumes that DS and SS are the same!!! (this is the case for Win32)
        // change the stack only if the selectors are the same (this is the case for Win32)
        //__asm push offset MyStack[1024*128];
        //__asm pop esp;
        __asm mov eax, offset MyStack[1024 * 128];
        __asm mov esp, eax;
    }
#endif

    StackWalkerToLog sw; // output to console
    sw.ShowCallstack(GetCurrentThread(), pExPtrs->ContextRecord);

    CreateCrashDumpFile(s_szExceptionLogFileName, pExPtrs);

    TCHAR lString[500];
    _stprintf_s(lString,
        _T("*** Unhandled Exception! See log output for more infos!\n")
        _T("   ExpCode: 0x%8.8X\n")
        _T("   ExpFlags: %d\n")
#if _MSC_VER >= 1900
        _T("   ExpAddress: 0x%8.8p\n")
#else
        _T("   ExpAddress: 0x%8.8X\n")
#endif
        _T("   Please report!"),
        pExPtrs->ExceptionRecord->ExceptionCode, pExPtrs->ExceptionRecord->ExceptionFlags,
        pExPtrs->ExceptionRecord->ExceptionAddress);
    FatalAppExit(-1, lString);
    return EXCEPTION_CONTINUE_SEARCH;
}

static void InitUnhandledExceptionFilter() {
    TCHAR szModName[_MAX_PATH];
    if (GetModuleFileName(NULL, szModName, sizeof(szModName) / sizeof(TCHAR)) != 0) {
        _tcscpy_s(s_szExceptionLogFileName, szModName);
        _tcscat_s(s_szExceptionLogFileName, _T(".dmp"));
    }
    if (s_bUnhandledExeptionFilterSet == FALSE) {
        // set global exception handler (for handling all unhandled exceptions)
        SetUnhandledExceptionFilter(CrashHandlerExceptionFilter);
#if defined _M_X64 || defined _M_IX86
        PreventSetUnhandledExceptionFilter();
#endif
        s_bUnhandledExeptionFilterSet = TRUE;
    }
}

