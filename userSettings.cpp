#include "alerts.h"  
#include <iostream>  
#include <cstdlib>  
#include <string>  
#include <filesystem> 
#include "userSettings.h" 
#include <algorithm> 
#include <cctype> 
#include <atomic> 
#include <mutex> 
#include <fstream> 

// Externally declared variables for email and phone number
extern std::string emailAddress;  // Email address declared elsewhere
extern std::string phoneNumber;   // Phone number declared elsewhere

// Atomic flag to control the input thread (thread-safe variable)
std::atomic<bool> running(true);  // Used to keep the input thread running until stopped

// Helper function to validate the email format
bool isValidEmail(const std::string& email) {
    size_t atPos = email.find('@');  // Find the position of '@' in the email
    size_t dotPos = email.find('.', atPos);  // Find the position of '.' after the '@'

    // Ensure that the email has an '@' and a domain with at least one '.'
    return atPos != std::string::npos && dotPos != std::string::npos && dotPos > atPos;
}

// Function to get the directory of the executable
std::string getExecutablePath() {
    // Return the current working directory as a string
    return std::filesystem::current_path().string();
}

// Helper function to trim whitespace from a string
std::string trim(const std::string& str) {
    std::string result = str;  // Make a copy of the input string
    // Remove leading whitespace
    result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
    // Remove trailing whitespace
    result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), result.end());
    return result;  // Return the trimmed string
}

// Thread-safe log to a file for debugging purposes
void logToFile(const std::string& message) {
    static std::mutex logMutex;  // A static mutex to control access to the log file
    std::lock_guard<std::mutex> lock(logMutex);  // Lock the mutex for thread-safe writing

    // Open the log file in append mode
    std::ofstream logFile("settings_log.txt", std::ios_base::app);
    if (logFile.is_open()) {
        logFile << message << std::endl;  // Write the message to the log file
        logFile.close();  // Close the file
    }
    else {
        std::cerr << "[ERROR] Could not open settings_log.txt for writing." << std::endl;  // Log an error if the file can't be opened
    }
}

// Function to save email and phone number settings
void saveAlertSettings(const std::string& newEmailAddress, const std::string& newPhoneNumber) {
    // Log the input email and phone number for debugging
    logToFile("[DEBUG] Input email: " + newEmailAddress + ", Input phone: " + newPhoneNumber);

    // Update the global email address if a new one is provided
    if (!newEmailAddress.empty()) {
        emailAddress = newEmailAddress;
    }

    // Update the global phone number if a new one is provided
    if (!newPhoneNumber.empty()) {
        phoneNumber = newPhoneNumber;
    }

    // Log the updated settings
    logToFile("[INFO] Updated settings: Email = " + emailAddress + ", Phone = " + phoneNumber);
    // Print the updated settings to the console
    std::cout << "[INFO] Updated settings: Email = " << emailAddress << ", Phone = " << phoneNumber << std::endl;
}

// Function to handle input commands (running in a separate thread)
void handleSettingsInput() {
    std::string input;  // Variable to hold user input
    // Loop while the `running` flag is true
    while (running) {
        // Read input from the console
        if (std::getline(std::cin, input)) {
            // Log the received input for debugging
            logToFile("[DEBUG] Received input: " + input);
            std::cout << "[DEBUG] Received input: " << input << std::endl;

            // If the input starts with "settings", try to parse and update settings
            if (input.find("settings") == 0) {
                size_t firstSpace = input.find(' ');  // Find the first space after "settings"
                size_t secondSpace = input.find(' ', firstSpace + 1);  // Find the second space after the email

                // Ensure that at least one of email or phone number is present in the input
                if (firstSpace != std::string::npos) {
                    std::string newEmailAddress = "";
                    std::string newPhoneNumber = "";

                    // Extract and trim the email and phone number if they are present
                    if (secondSpace != std::string::npos) {
                        newEmailAddress = trim(input.substr(firstSpace + 1, secondSpace - firstSpace - 1));
                        newPhoneNumber = trim(input.substr(secondSpace + 1));
                    }
                    else {
                        newPhoneNumber = trim(input.substr(firstSpace + 1));
                    }

                    // Validate the email format only if an email is provided
                    if (!newEmailAddress.empty()) {
                        if (!isValidEmail(newEmailAddress)) {
                            logToFile("[ERROR] Invalid email format: " + newEmailAddress);  // Log error for invalid email
                            std::cerr << "[ERROR] Invalid email format." << std::endl;
                            continue;  // Skip to the next iteration if the email is invalid
                        }
                    }

                    // Save the alert settings (email and/or phone number)
                    saveAlertSettings(newEmailAddress, newPhoneNumber);
                }
                else {
                    logToFile("[ERROR] Invalid input format for 'settings' command.");  // Log error for invalid input format
                    std::cerr << "[ERROR] Invalid input format. Expected: settings <email> <phone>" << std::endl;
                }
            }
            // If the input is "clear", clear the email and phone settings
            else if (input == "clear") {
                clearSettings();  // Clear the settings
            }
            // If the input is "exit", stop the thread and exit
            else if (input == "exit") {
                running = false;  // Stop the input thread by setting the atomic flag to false
                logToFile("[INFO] Program exit requested.");  // Log that the exit was requested
                std::cout << "[INFO] Exiting program." << std::endl;
            }
            // Handle unknown commands
            else {
                logToFile("[WARNING] Unknown command received: " + input);  // Log a warning for unknown command
                std::cerr << "[WARNING] Unknown command: " << input << std::endl;
            }
        }
    }
}


// Function to clear settings (email and phone number)
void clearSettings() {
    emailAddress = "";  // Clear the email address
    phoneNumber = "";  // Clear the phone number
    logToFile("[INFO] Settings cleared.");  // Log that the settings were cleared
    std::cout << "[INFO] Settings cleared." << std::endl;  // Print confirmation to the console
}
