#include <iostream>  
#include <cstdlib>   
#include <string>   

// Set default recipient here
const std::string defaultRecipient = "";
// A default recipient email address is defined as a constant, which can be used if no other recipient is provided.

// Function to send an email alert
bool sendEmailAlert(const std::string& subject, const std::string& body, const std::string& emailAddress);

// Function to send an SMS using Node.js script
bool sendSmsAlert(const std::string& message, const std::string& phoneNumber);
