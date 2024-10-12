#ifndef USERSETTINGS_H 
#define USERSETTINGS_H 

#include <string>  

// Logs a message to a file. This function is thread-safe using a mutex to ensure only one thread writes to the file at a time.
void logToFile(const std::string& message);

// Saves the provided email address and phone number as alert settings.
void saveAlertSettings(const std::string& newEmailAddress, const std::string& newPhoneNumber);

// Handles input commands for settings in a separate thread. Continuously listens for commands like 'settings', 'clear', and 'exit'.
void handleSettingsInput();

// Clears the email and phone number settings.
void clearSettings();  // Declaration of the clearSettings function.

// Validates the format of the provided email. Returns true if the email format is valid.
bool isValidEmail(const std::string& email);

// Returns the path of the current executable directory.
std::string getExecutablePath();

#endif  
