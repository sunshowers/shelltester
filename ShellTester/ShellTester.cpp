#include <Windows.h>
#include <Shlwapi.h>
#include <Shlobj.h>
#include <cstdio>

int wmain(int argc, wchar_t** argv) {
    if (argc != 2) {
        wprintf(L"Usage: %s <url>", argv[0]);
        return 1;
    }

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        printf("CoInitialize failed with status %x\n", hr);
        return 1;
    }

    wchar_t* url = argv[1];
    SHELLEXECUTEINFOW sinfo;
    memset(&sinfo, 0, sizeof(sinfo));
    sinfo.cbSize = sizeof(sinfo);
    sinfo.fMask = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_FLAG_NO_UI;
    sinfo.hwnd = NULL;
    sinfo.lpVerb = L"open";
    sinfo.nShow = SW_SHOWNORMAL;

    LPITEMIDLIST pidl = NULL;
    SFGAOF sfgao;
    hr = ::SHParseDisplayName(url, NULL, &pidl, 0, &sfgao);
    if (FAILED(hr)) {
        printf("SHParseDisplayName failed with status %x\n", hr);
        CoUninitialize();
        return 1;
    }

    sinfo.lpIDList = pidl;
    sinfo.fMask = SEE_MASK_INVOKEIDLIST;

    BOOL result = ::ShellExecuteExW(&sinfo);
    if (!result) {
        hr = HRESULT_FROM_WIN32(::GetLastError());
        printf("ShellExecuteExW failed with status %x\n", hr);
        CoUninitialize();
        return 1;
    }

    CoUninitialize();
}