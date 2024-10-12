#include "alerts.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include "userSettings.h"
#include <algorithm>
#include <cctype>  

// Externally declared global variables for storing email and phone number
extern std::string emailAddress;
extern std::string phoneNumber;

// Helper function to trim and sanitize input strings
std::string trimAndSanitize(const std::string& str) {
    std::string result = str;

    // Trim whitespace from the beginning of the string
    result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char ch) {
        return !std::isspace(ch);  // Remove leading whitespace
        }));

    // Trim whitespace from the end of the string
    result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) {
        return !std::isspace(ch);  // Remove trailing whitespace
        }).base(), result.end());

    // Remove control characters from the string
    result.erase(std::remove_if(result.begin(), result.end(), [](unsigned char c) {
        return std::iscntrl(c);  // Check for control characters (e.g., newline, tab)
        }), result.end());

    return result;  // Return the sanitized result
}

// Function to send an email alert
bool sendEmailAlert(const std::string& subject, const std::string& body, const std::string& emailAddress) {
    // Sanitize the email address
    std::string cleanEmail = trimAndSanitize(emailAddress);

    // Validate the email address
    if (!isValidEmail(cleanEmail)) {  // Assuming isValidEmail is a function that checks the format
        logToFile("[ERROR] Invalid email address: " + cleanEmail);  // Log error if email is invalid
        std::cerr << "[ERROR] Invalid email address: " << cleanEmail << std::endl;
        return false;  // Exit the function with failure if the email is invalid
    }

    // Sanitize the email subject
    std::string cleanSubject = trimAndSanitize(subject);

    // Dynamically construct the command to run sendEmail.js
    std::string executableDir = getExecutablePath();  // Get the current executable's directory
    std::string command = "node \"" + executableDir + "/sendEmail.js\" \"" + cleanSubject + "\" \"" + body + "\" \"" + cleanEmail + "\"";

    logToFile("[DEBUG] Executing command: " + command);  // Log the constructed command
    //std::cout << "[DEBUG] Executing command: " << command << std::endl;

    // Execute the command to send the email
    int result = std::system(command.c_str());
    if (result == 0) {  // Check if the command succeeded (returns 0)
        logToFile("[INFO] Email alert sent.");  // Log success
        std::cout << "[INFO] Email alert sent." << std::endl;
        return true;  // Return success
    }
    else {  // If the command failed
        logToFile("[ERROR] Failed to send email alert with result code: " + std::to_string(result));  // Log failure
        std::cerr << "[ERROR] Failed to send email alert. Result code: " << result << std::endl;
        return false;  // Return failure
    }
}

// Function to send an SMS alert
bool sendSmsAlert(const std::string& message, const std::string& phoneNumber) {
    // Sanitize the phone number
    std::string cleanPhone = trimAndSanitize(phoneNumber);

    // Dynamically construct the command to run sendSms.js
    std::string executableDir = getExecutablePath();  // Get the current executable's directory
    std::string command = "node \"" + executableDir + "/sendSms.js\" \"" + message + "\" \"" + cleanPhone + "\"";

    logToFile("[DEBUG] Executing command: " + command);  // Log the constructed command
    std::cout << "[DEBUG] Executing command: " << command << std::endl;

    // Execute the command to send the SMS
    int result = std::system(command.c_str());
    if (result == 0) {  // Check if the command succeeded (returns 0)
        logToFile("[INFO] SMS alert sent.");  // Log success
        std::cout << "[INFO] SMS alert sent." << std::endl;
        return true;  // Return success
    }
    else {  // If the command failed
        logToFile("[ERROR] Failed to send SMS alert.");  // Log failure
        std::cerr << "[ERROR] Failed to send SMS alert." << std::endl;
        return false;  // Return failure
    }
}
