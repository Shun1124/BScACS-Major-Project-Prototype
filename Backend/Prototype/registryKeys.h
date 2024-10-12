#ifndef REGISTRYKEYS_H 
#define REGISTRYKEYS_H  

#include <windows.h>  
#include <iostream>  
#include <fstream>    
#include <string>    
#include <chrono>     
#include <thread>    

// Function to get the current date and time as a string
std::string getCurrentDateTime();  // Returns a formatted string of the current date and time

// Function to log changes to a file
void logChange(const std::string& message);  // Logs a given message to a log file, including the current date and time

// Function to retrieve current registry value for any key
std::string getRegistryValue(HKEY hKey, const std::string& subKey, const std::string& valueName);
// Retrieves the value of a specified registry key and value name and returns it as a std::string

// Structure to define each registry key and its criticality
struct RegistryKey {
    HKEY hive;                 // Registry hive (e.g., HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE)
    std::string keyPath;        // Path to the registry key (e.g., "Control Panel\\Desktop")
    std::string valueName;      // Name of the value in the registry key (e.g., "Wallpaper")
    bool isCritical;            // Whether the key is critical (true means critical, false means non-critical)
};

// Define a list of registry keys to monitor
const RegistryKey monitoredKeys[] = {
    {HKEY_CURRENT_USER, "Control Panel\\Desktop", "Wallpaper", true},  // Non-critical: Monitor Wallpaper changes
    {HKEY_CURRENT_USER, "Control Panel\\Mouse", "DoubleClickSpeed", false},  // Critical: Monitor Mouse double-click speed
    {HKEY_CURRENT_USER, "Control Panel\\Desktop", "CursorBlinkRate", false}  // Non-critical: Monitor Keyboard repeat rate
};

#endif 
