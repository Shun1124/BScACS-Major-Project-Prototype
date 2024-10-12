#include "registryKeys.h"
#include "stringUtils.h"

// Function to get the current date and time as a string
std::string getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();  // Get the current system time
    std::time_t timeNow = std::chrono::system_clock::to_time_t(now);  // Convert system time to time_t
    char buffer[26];  // Buffer to hold the formatted date-time string (ctime_s requires at least 26 bytes)
    ctime_s(buffer, sizeof(buffer), &timeNow);  // Convert the time_t value to a formatted date-time string (secure version of ctime)
    return std::string(buffer);  // Convert the buffer to a std::string and return
}

// Function to log changes to a file and trigger alerts
void logChange(const std::string& message) {
    // Open the log file in append mode so new changes are added at the end of the file
    std::ofstream logFile("registry_changes.log", std::ios_base::app);
    if (logFile.is_open()) {  // Check if the file was successfully opened
        logFile << getCurrentDateTime() << ": " << message << std::endl;  // Write the current date-time and message to the log file
    }
    else {
        std::cerr << "Unable to open log file!" << std::endl;  // If the file couldn't be opened, print an error message to the console
    }
}

// Function to retrieve the current value of a registry key
std::string getRegistryValue(HKEY hKey, const std::string& subKey, const std::string& valueName) {
    HKEY hOpenedKey;  // Handle to the opened registry key
    std::string valueData = "";  // String to hold the value data
    std::wstring wSubKey = stringToWString(subKey);  // Convert the subKey string to wide string (wstring)
    std::wstring wValueName = stringToWString(valueName);  // Convert the valueName string to wide string (wstring)

    // Open the registry key with read access
    if (RegOpenKeyEx(hKey, wSubKey.c_str(), 0, KEY_READ, &hOpenedKey) == ERROR_SUCCESS) {
        DWORD dataSize = MAX_PATH;  // Maximum size for the data buffer
        char data[MAX_PATH];  // Buffer to hold the value data

        // Query the value of the registry key
        if (RegQueryValueEx(hOpenedKey, wValueName.c_str(), NULL, NULL, (LPBYTE)&data, &dataSize) == ERROR_SUCCESS) {
            valueData = std::string(data, dataSize - 1);  // Convert the data to std::string and store it in valueData
        }
        RegCloseKey(hOpenedKey);  // Close the opened registry key handle
    }

    return valueData;  // Return the retrieved value data as a string
}
