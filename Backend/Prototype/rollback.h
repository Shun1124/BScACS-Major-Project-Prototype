#ifndef ROLLBACK_H 
#define ROLLBACK_H 

#include <string>   
#include <windows.h>  

// Function to save the current registry value before changes are made
bool saveRegistryValue(const HKEY hKey, const std::string& subKey, const std::string& valueName);

// Function to rollback a registry value to its previous state
bool rollbackRegistryValue(const HKEY hKey, const std::string& subKey, const std::string& valueName);

#endif 
