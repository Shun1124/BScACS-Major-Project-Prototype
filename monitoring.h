#ifndef MONITORING_H 
#define MONITORING_H  

#include <string>  
#include <windows.h>  
#include "registryKeys.h" 

// Function declaration for monitoring a registry key
// Takes a const reference to a RegistryKey structure as an argument
void monitorRegistryKey(const RegistryKey& key);

#endif 
