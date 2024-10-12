#include "rollback.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include "monitoring.h"
#include "rollback.h"
#include "stringUtils.h"
#include <filesystem>

// Function to save the current registry value before a change is made
bool saveRegistryValue(const HKEY hKey, const std::string& subKey, const std::string& valueName) {
    // Get the current registry value
    std::string currentValue = getRegistryValue(hKey, subKey, valueName);
    if (currentValue.empty()) {
        std::cerr << "Failed to retrieve current value for rollback." << std::endl;
        return false;
    }

    // Open the backup log file in append mode
    std::ofstream backupFile("registry_backup.log", std::ios_base::app);
    if (backupFile.is_open()) {
        // Write the subKey, valueName, and current value to the log file
        backupFile << subKey << "\\" << valueName << ": " << currentValue << std::endl;
        backupFile.close();
        return true;
    }
    else {
        std::cerr << "Failed to open backup log file." << std::endl;
        return false;
    }
}

// Function to apply the wallpaper change
void applyWallpaperChange(const std::string& wallpaperPath) {
    // Convert std::string to std::wstring for the SystemParametersInfoW API call
    std::wstring wWallpaperPath = stringToWString(wallpaperPath);

    // Apply the new wallpaper using the Windows API
    SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (PVOID)wWallpaperPath.c_str(), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
}

// Function to rollback a registry value to its previous state
bool rollbackRegistryValue(const HKEY hKey, const std::string& subKey, const std::string& valueName) {
    // Open the backup log file to retrieve the previous value
    std::ifstream backupFile("registry_backup.log");
    if (!backupFile.is_open()) {
        std::cerr << "Failed to open backup log file." << std::endl;
        return false;
    }

    std::string line;
    std::string restoredValue;
    std::string fullKey = subKey + "\\" + valueName;  // Combine subKey and valueName for easy lookup

    // Read the log file line by line to find the matching key and its previous value
    while (std::getline(backupFile, line)) {
        if (line.find(fullKey) != std::string::npos) {
            restoredValue = line.substr(line.find(":") + 2);  // Extract the value after ": "
            break;
        }
    }

    backupFile.close();

    if (restoredValue.empty()) {
        std::cerr << "No backup found for key: " << fullKey << std::endl;
        return false;
    }

    HKEY hOpenedKey;
    std::wstring wSubKey = stringToWString(subKey);
    std::wstring wValueName = stringToWString(valueName);

    // Open the registry key with the ability to modify the value
    if (RegOpenKeyEx(hKey, wSubKey.c_str(), 0, KEY_SET_VALUE, &hOpenedKey) == ERROR_SUCCESS) {
        // Set the registry value to the restored (previous) value
        if (RegSetValueEx(hOpenedKey, wValueName.c_str(), 0, REG_SZ, (BYTE*)restoredValue.c_str(), restoredValue.size() + 1) == ERROR_SUCCESS) {
            std::cout << "Successfully rolled back " << valueName << " to previous value. " << std::endl;
            //std::cout << "Successfully rolled back " << valueName << " to previous value: " << restoredValue << std::endl;

            // If the key being restored is the "Wallpaper" key, apply the wallpaper change
            if (valueName == "Wallpaper") {
                applyWallpaperChange(restoredValue);
            }

            RegCloseKey(hOpenedKey);
            return true;
        }
        else {
            std::cerr << "Failed to restore value for " << valueName << std::endl;
            RegCloseKey(hOpenedKey);
        }
    }
    else {
        std::cerr << "Failed to open registry key for rollback: " << subKey << std::endl;
    }

    return false;
}
