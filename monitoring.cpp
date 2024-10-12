#include "monitoring.h"
#include "alerts.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <system_error>
#include "rollback.h"
#include "registryKeys.h"
#include "stringUtils.h"
#include "userSettings.h"
#include <filesystem>

// Global variables for email and phone number
std::string emailAddress = "";  // Holds the email address for alerts
std::string phoneNumber = "";   // Holds the phone number for SMS alerts

// Function to monitor a specific registry key
void monitorRegistryKey(const RegistryKey& key) {
    HKEY hKey;  // Handle to the opened registry key
    LONG result;  // Stores the result of registry operations

    // Convert the registry key path from std::string to std::wstring
    std::wstring wKeyPath = stringToWString(key.keyPath);

    // Open the registry key for reading
    result = RegOpenKeyEx(key.hive, wKeyPath.c_str(), 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS) {  // If the key couldn't be opened
        DWORD lastError = GetLastError();
        std::cerr << "Failed to open registry key: " << key.keyPath
            << ". Error: " << lastError << " ("
            << std::system_category().message(lastError) << ")\n";  // Print an error message
        return;  // Exit the function if the key cannot be opened
    }

    //std::cout << "Successfully opened registry key: " << key.keyPath << "\n";  // Notify successful key opening

    // Create an event for monitoring registry changes
    HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (!hEvent) {  // Check if the event creation failed
        std::cerr << "Failed to create event. Error: " << GetLastError() << "\n";  // Print an error message
        RegCloseKey(hKey);  // Close the registry key handle
        return;  // Exit if the event couldn't be created
    }

    // Get the current value of the registry key
    std::string currentValue = getRegistryValue(key.hive, key.keyPath, key.valueName);

    // If the registry key is critical, save its current value for rollback purposes
    if (key.isCritical) {
        saveRegistryValue(key.hive, key.keyPath, key.valueName);
    }

    // Infinite loop to continuously monitor changes to the registry key
    while (true) {
        // Monitor changes to the registry key
        result = RegNotifyChangeKeyValue(
            hKey,
            FALSE,  // Do not monitor subkeys
            REG_NOTIFY_CHANGE_LAST_SET,  // Monitor changes to the key's value
            hEvent,  // Use the created event for notifications
            TRUE  // Asynchronous notification
        );

        if (result == ERROR_SUCCESS) {
            // Wait for the event to be triggered or timeout after 2 seconds
            DWORD waitResult = WaitForSingleObject(hEvent, 2000);

            if (waitResult == WAIT_OBJECT_0) {  // If the event was triggered
                // Get the new value of the registry key
                std::string newValue = getRegistryValue(key.hive, key.keyPath, key.valueName);

                // Check if the value has changed
                if (newValue != currentValue) {
                    std::string message = key.valueName + " registry value changed.";
                    //std::string message = key.valueName + " registry value changed. New value: " + newValue;

                    // Log non-critical changes to the console
                    if (!key.isCritical) {
                        std::cout << "[INFO] " << message << std::endl;
                    }

                    // If the key is critical, log the change and send alerts
                    if (key.isCritical) {
                        std::string criticalMessage = "[CRITICAL] Critical change detected for " + key.valueName + "!";

                        // Log the critical change
                        std::cout << criticalMessage << std::endl;

                        // Send an SMS alert if a phone number is provided
                        if (!phoneNumber.empty()) {
                            std::cout << "[DEBUG] Sending SMS alert to " << phoneNumber << std::endl;
                            bool smsSuccess = sendSmsAlert(criticalMessage, phoneNumber);
                            if (smsSuccess) {
                                std::cout << "[INFO] SMS alert sent to " << phoneNumber << std::endl;
                            }
                            else {
                                std::cerr << "[ERROR] Failed to send SMS alert to " << phoneNumber << std::endl;
                            }
                        }
                        else {
                            std::cout << "[DEBUG] No phone number provided." << std::endl;
                        }

                        // Send an email alert if an email address is provided
                        if (!emailAddress.empty()) {
                            std::cout << "[DEBUG] Sending email alert to " << emailAddress << std::endl;
                            bool emailSuccess = sendEmailAlert("Registry Change Detected", criticalMessage, emailAddress);
                            if (emailSuccess) {
                                std::cout << "[INFO] Email alert sent to " << emailAddress << std::endl;
                            }
                            else {
                                std::cerr << "[ERROR] Failed to send email alert to " << emailAddress << std::endl;
                            }
                        }
                        else {
                            std::cout << "[DEBUG] No email address provided." << std::endl;
                        }

                        // Rollback the registry value to its previous state
                        rollbackRegistryValue(key.hive, key.keyPath, key.valueName);

                        // Update the current value after the rollback to avoid triggering the same alert again
                        currentValue = getRegistryValue(key.hive, key.keyPath, key.valueName);
                    }
                    else {
                        // Update the current value for non-critical changes
                        currentValue = newValue;
                    }
                }
            }
        }
    }

    // Clean up: Close the event handle and registry key handle
    CloseHandle(hEvent);
    RegCloseKey(hKey);
    std::cout << "Finished monitoring key: " << key.keyPath << "\n";  // Notify that monitoring for this key is done
}
