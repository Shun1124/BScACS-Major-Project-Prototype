#include <iostream>
#include <thread>
#include <vector>
#include "monitoring.h"
#include "registryKeys.h"
#include "userSettings.h"

int main(int argc, char* argv[]) {
    // Output the total number of registry keys to be monitored as soon as the program starts
    std::cout << "Total keys to monitor: " << sizeof(monitoredKeys) / sizeof(monitoredKeys[0]) << std::endl;

    std::vector<std::thread> threads; // Vector to store threads for each key being monitored

    // Loop over each monitored key in the monitoredKeys array
    for (const auto& key : monitoredKeys) {
        // Output a message indicating a new thread is being created to monitor this specific key
        std::cout << "Starting thread for: " << key.keyPath << std::endl;

        // Create a new thread to monitor the registry key, and add it to the threads vector
        threads.emplace_back(std::thread(monitorRegistryKey, key));
    }

    // Start a separate thread to handle user input for settings (email and phone)
    std::thread inputThread(handleSettingsInput);  // Ensure this thread runs concurrently with monitoring threads

    // Wait for each monitoring thread to finish (join all threads)
    for (auto& t : threads) {
        t.join();  // Blocks the main thread until the thread t finishes execution
    }

    // Wait for the input thread to finish (join the input thread)
    if (inputThread.joinable()) {
        inputThread.join();  // Ensure the input thread is properly joined if it's still running
    }

    // Output a message indicating the program is terminating
    std::cout << "Program terminated." << std::endl;
    return 0;  // Return 0 to indicate successful program execution
}
