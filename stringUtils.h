#ifndef STRINGUTILS_H 
#define STRINGUTILS_H  

#include <string>  

// Function to convert std::string to std::wstring
std::wstring stringToWString(const std::string& str);

// Function to convert std::wstring to std::string
std::string wstringToString(const std::wstring& wstr);

#endif 
