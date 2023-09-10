#include "WinOpenDialogBox.h"

std::string WinODB::ShowOpenDialogBox()
{
    // initialize the COM library
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr))
        return "";

    // create the Common Item Dialog object and get a pointer to the object's IFileOpenDialog interface
    IFileOpenDialog* pFileOpenDialog;
    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpenDialog));
    if (FAILED(hr))
    {
        CoUninitialize();
        return "";
    }

    // show the dialog box to the user. This method blocks until the user dismisses the dialog box
    hr = pFileOpenDialog->Show(NULL);
    if (FAILED(hr))
    {
        pFileOpenDialog->Release();
        CoUninitialize();
        return "";
    }

    // return a pointer to a second COM object, called a Shell item object. 
    // The Shell item, which implements the IShellItem interface, represents the file that the user selected
    IShellItem* pItem;
    hr = pFileOpenDialog->GetResult(&pItem);
    if (FAILED(hr))
    {
        pFileOpenDialog->Release();
        CoUninitialize();
        return "";
    }

    // get the file path, in the form of a string
    PWSTR pszFilePath;
    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
    if (FAILED(hr))
    {
        pItem->Release();
        pFileOpenDialog->Release();
        CoUninitialize();
        return "";
    }

    //  format file path to std::string
    std::wstring path(pszFilePath);
    std::string filePath(path.begin(), path.end());

    // clean up
    CoTaskMemFree(pszFilePath);
    pItem->Release();
    pFileOpenDialog->Release();
    CoUninitialize();

    return ConvertWstringToString(path);
}