const { ipcRenderer } = require('electron'); 
// Import the `ipcRenderer` module from Electron, which allows communication between the renderer process (frontend) and the main process (backend).

document.addEventListener('DOMContentLoaded', () => { 
    // Add an event listener to the document to run the enclosed code when the DOM content has fully loaded.

    const logArea = document.getElementById('log-area'); 
    // Get a reference to the log area element where log messages will be displayed.
    
    const alertContainer = document.getElementById('alert-container'); 
    // Get a reference to the alert container element where critical alerts will be shown.
    
    const emailInput = document.getElementById('email'); 
    // Get a reference to the email input field where users can enter their email.
    
    const phoneInput = document.getElementById('phone'); 
    // Get a reference to the phone input field where users can enter their phone number.
    
    const saveSettingsButton = document.getElementById('save-settings-button'); 
    // Get a reference to the save button that allows the user to save their settings (email or phone number).

    // Ensure DOM elements are ready
    function initializeLogArea() { 
        // Initialize the log area with an initial log message and ensure the log is always scrolled to the bottom.
        setTimeout(() => { 
            // Use a timeout to simulate a delay before displaying the initialization message.
            logArea.value += "[INFO] System monitoring initialized.\n"; 
            // Append an initialization message to the log area.
            logArea.scrollTop = logArea.scrollHeight; 
            // Automatically scroll to the bottom of the log area to show the most recent message.
        }, 1000); 
        // Wait for 1 second before executing the function to allow other UI elements to load.
    }

    // Enable or disable the Save button based on whether email or phone is provided
    function updateSaveButtonState() { 
        // Update the save button state (enabled/disabled) based on the presence of an email or phone number.
        const email = emailInput ? emailInput.value.trim() : ''; 
        // Get the trimmed value from the email input field, or an empty string if it's not available.
        
        const phone = phoneInput ? phoneInput.value.trim() : ''; 
        // Get the trimmed value from the phone input field, or an empty string if it's not available.
        
        saveSettingsButton.disabled = !email && !phone; 
        // Disable the save button if both email and phone are empty, otherwise enable it.
    }

    if (emailInput) emailInput.addEventListener('input', updateSaveButtonState); 
    // Add an event listener to the email input field that triggers the button state update when the user types.
    
    if (phoneInput) phoneInput.addEventListener('input', updateSaveButtonState); 
    // Add an event listener to the phone input field that triggers the button state update when the user types.

    if (saveSettingsButton) { 
        // Check if the save button exists.
        saveSettingsButton.addEventListener('click', () => { 
            // Add an event listener to the save button that is triggered when the button is clicked.
            const email = emailInput ? emailInput.value.trim() : ''; 
            // Get the trimmed value from the email input field.
            
            const phone = phoneInput ? phoneInput.value.trim() : ''; 
            // Get the trimmed value from the phone input field.

            console.log(`[DEBUG] Email entered: '${email}', Phone entered: '${phone}'`); 
            // Log the email and phone inputs to the console for debugging purposes.

            if (!email && !phone) { 
                // Check if both email and phone are empty.
                console.log('No email or phone provided.'); 
                // Log a message indicating that no contact information was provided.
                return; 
                // Exit the function early if no email or phone is provided.
            }

            ipcRenderer.send('save-settings', { email, phone }); 
            // Send the email and phone settings to the main process using `ipcRenderer`.
            
            console.log(`Settings sent: Email = ${email}, Phone = ${phone}`); 
            // Log a confirmation that the settings were sent.
        });
    }

    ipcRenderer.on('cpp-output', (event, message) => { 
        // Listen for messages from the C++ backend via `ipcRenderer`.
        const time = new Date().toLocaleTimeString(); 
        // Get the current time in a human-readable format.

        logArea.value += `[${time}] ${message}\n`; 
        // Append the received message along with the current time to the log area.
        logArea.scrollTop = logArea.scrollHeight; 
        // Scroll to the bottom of the log area to show the most recent message.

        if (message.includes("[CRITICAL]")) { 
            // Check if the message contains the word "[CRITICAL]", indicating a critical event.
            const criticalMessage = message.split('\n').find(line => line.includes("[CRITICAL]")); 
            // Find the line within the message that contains the critical alert.

            if (criticalMessage) { 
                // If a critical message is found:
                const alertDiv = document.createElement('div'); 
                // Create a new `div` element for the critical alert.
                alertDiv.classList.add('alert'); 
                // Add a CSS class to style the alert.
                alertDiv.textContent = criticalMessage; 
                // Set the alert's content to the critical message text.
                alertContainer.innerHTML = ''; 
                // Clear any existing alerts in the alert container.
                alertContainer.appendChild(alertDiv); 
                // Add the new alert to the alert container.
            }
        } else { 
            // If the message does not contain "[CRITICAL]":
            alertContainer.innerHTML = ''; 
            // Clear any critical alerts from the alert container.
        }
    });

    updateSaveButtonState(); 
    // Call the function to initialize the save button state based on the current inputs.
    initializeLogArea(); 
    // Call the function to initialize the log area with a startup message.
});
