#include "stringUtils.h"  
#include <Windows.h>      
#include <string>       

// Function to convert std::string to std::wstring
std::wstring stringToWString(const std::string& str) {
    // Use MultiByteToWideChar to determine the size of the buffer needed for the wide string
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);

    // Create a std::wstring buffer of the required size
    std::wstring wstrTo(size_needed, 0);

    // Perform the actual conversion from std::string to std::wstring
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);

    // Return the converted wide string
    return wstrTo;
}

// Function to convert std::wstring to std::string
std::string wstringToString(const std::wstring& wstr) {
    // Use WideCharToMultiByte to determine the size of the buffer needed for the narrow string
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);

    // Create a std::string buffer of the required size
    std::string strTo(size_needed, 0);

    // Perform the actual conversion from std::wstring to std::string
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);

    // Return the converted narrow string
    return strTo;
}
