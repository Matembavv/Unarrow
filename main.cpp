#include <windows.h>
#include <iostream>
#include <string>


void RestartExplorer() {
    std::cout << "Restarting Explorer..." << std::endl;
    system("taskkill /f /im explorer.exe > nul 2>&1");
    Sleep(10);
    system("start explorer.exe");
    std::cout << "Done!" << std::endl;
}

bool RemoveArrows() {
    const wchar_t* subKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Icons";
    const wchar_t* valueName = L"29";

    wchar_t systemRoot[MAX_PATH];
    GetWindowsDirectoryW(systemRoot, MAX_PATH);
    std::wstring valueData = std::wstring(systemRoot) + L"\\system32\\imageres.dll,197";

    HKEY hKey;
    LONG result = RegCreateKeyExW(
        HKEY_LOCAL_MACHINE, subKey, 0, NULL,
        REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL
    );

    if (result != ERROR_SUCCESS) {
        std::cerr << "Error: cannot open/create registry key. Code: " << result << std::endl;
        return false;
    }

    result = RegSetValueExW(
        hKey, valueName, 0, REG_SZ,
        reinterpret_cast<const BYTE*>(valueData.c_str()),
        (DWORD)((valueData.length() + 1) * sizeof(wchar_t))
    );
    RegCloseKey(hKey);

    if (result != ERROR_SUCCESS) {
        std::cerr << "Error: cannot write registry value. Code: " << result << std::endl;
        return false;
    }

    std::cout << "Shortcut arrows removed." << std::endl;
    return true;
}

bool RestoreArrows() {
    const wchar_t* subKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Icons";
    const wchar_t* valueName = L"29";

    HKEY hKey;
    LONG result = RegOpenKeyExW(
        HKEY_LOCAL_MACHINE, subKey, 0, KEY_SET_VALUE, &hKey
    );

    if (result == ERROR_FILE_NOT_FOUND) {
        std::cout << "Key 'Shell Icons' not found. Nothing to restore." << std::endl;
        return true;
    }

    if (result != ERROR_SUCCESS) {
        std::cerr << "Error: cannot open registry key. Code: " << result << std::endl;
        return false;
    }

    result = RegDeleteValueW(hKey, valueName);
    RegCloseKey(hKey);

    if (result == ERROR_SUCCESS) {
        std::cout << "Shortcut arrows restored." << std::endl;
        return true;
    } else if (result == ERROR_FILE_NOT_FOUND) {
        std::cout << "Value '29' not found. Nothing to restore." << std::endl;
        return true;
    } else {
        std::cerr << "Error: cannot delete registry value. Code: " << result << std::endl;
        return false;
    }
}

bool IsAdmin() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;

    if (AllocateAndInitializeSid(
            &ntAuthority, 2,
            SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS,
            0, 0, 0, 0, 0, 0,
            &adminGroup)) {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    return isAdmin;
}

int main() {
    if (!IsAdmin()) {
        std::cerr << "ERROR: This tool must be run as Administrator." << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }

    while (true) {
        system("cls");
        std::cout << "=====================================\n";
        std::cout << "   Shortcut Arrow Toggle Utility\n";
        std::cout << "=====================================\n";
        std::cout << " 1. Restore default\n";
        std::cout << " 2. Remove arrows from shortcuts\n";
        std::cout << " 0. Exit\n";
        std::cout << "-------------------------------------\n";
        std::cout << "Choose an option: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        bool changed = false;

        switch (choice) {
            case 1:
                changed = RestoreArrows();
                break;
            case 2:
                changed = RemoveArrows();
                break;
            case 0:
                std::cout << "Bye!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid option." << std::endl;
                break;
        }

        if (changed) {
            RestartExplorer();
        }

        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(10000, '\n');
        std::cin.get();
    }
    return 0;
}