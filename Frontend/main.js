const { app, BrowserWindow, ipcMain } = require('electron');
const { spawn } = require('child_process');
const path = require('path');

// Variables to hold the C++ process and user settings (email and phone number)
let cppProgram;
let email = '';
let phoneNumber = '';

// Utility function to check if the C++ process is running
function isCppProgramRunning() {
    return cppProgram && cppProgram.exitCode === null; // Return true if the C++ process is active
}

// Function to create the Electron window and handle events
function createWindow() {
    // Create a new browser window with specific dimensions and settings
    const win = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            nodeIntegration: true, // Allows node modules in the renderer
            contextIsolation: false // Allows direct access to Electron API from renderer
        }
    });

    // Load the HTML file for the frontend
    win.loadFile(path.join(__dirname, 'index.html'));

    // Handle 'save-settings' event sent from the frontend (renderer)
    ipcMain.on('save-settings', (event, data) => {
        // Update the email and phone number variables with the data received from the frontend
        email = data.email || '';
        phoneNumber = data.phone || '';

        console.log(`[DEBUG] Email received from frontend: ${email}, Phone: ${phoneNumber}`);

        // Send the settings to the C++ program if it's still running
        if (isCppProgramRunning()) {
            cppProgram.stdin.write(`settings ${email} ${phoneNumber}\n`); // Send settings via stdin
            console.log(`[DEBUG] Sent settings to C++: Email = ${email}, Phone = ${phoneNumber}`);
        } else {
            console.error("Failed to send settings: C++ program is not running.");
        }
    });

    // Try to spawn the C++ program
    try {
        const fullPath = path.join(__dirname, '../Backend/Prototype/x64/Debug/Prototype.exe'); // Path to the C++ executable
        const options = {
            cwd: path.resolve(__dirname, '../Backend/Prototype'), // Working directory for the C++ process
            env: Object.assign({}, process.env), // Use the current environment variables
        };

        //console.log("Resolved path to C++ executable: ", fullPath);
        cppProgram = spawn(fullPath, [], options); // Spawn the C++ program

        // Capture stdout (standard output) from the C++ program and send it to the frontend
        cppProgram.stdout.setEncoding('utf-8'); // Ensure the output is properly encoded as UTF-8
        cppProgram.stdout.on('data', (data) => {
            const message = data.toString().trim(); // Convert the data to a string and remove extra spaces
            if (win && !win.isDestroyed()) {
                //console.log(`Sending C++ Output to Renderer: ${message}`);
                win.webContents.send('cpp-output', message); // Send the message to the renderer (frontend)
            }
        });

        // Capture stderr (standard error) from the C++ program and send it to the frontend
        cppProgram.stderr.on('data', (data) => {
            const errorMsg = `C++ Error: ${data.toString()}`;
            console.error(errorMsg); // Log the error
            if (win && !win.isDestroyed()) {
                win.webContents.send('cpp-output', errorMsg); // Send the error message to the frontend
            }
        });

        // Handle when the C++ program exits
        cppProgram.on('exit', (code, signal) => {
            console.log(`C++ program exited with code ${code} and signal ${signal}`);
            cppProgram = null; // Set cppProgram to null once it has exited
        });

    } catch (err) {
        // Handle any errors that occur when trying to spawn the C++ program
        console.error(`Failed to spawn C++ program: ${err.message}`);
        if (win && !win.isDestroyed()) {
            win.webContents.send('cpp-output', `Error: Could not start C++ program.`); // Notify the frontend of the error
        }
    }
}

// When Electron is ready, create the window
app.whenReady().then(createWindow);

// Handle when all windows are closed
app.on('window-all-closed', () => {
    // If the C++ program is running, attempt to close it gracefully
    if (cppProgram && isCppProgramRunning()) {
        cppProgram.stdin.write('exit\n');  // Ask the C++ process to exit gracefully
    }

    setTimeout(() => {
        // If the C++ program is still running after 2 seconds, forcefully terminate it
        if (cppProgram && isCppProgramRunning()) {
            cppProgram.kill('SIGTERM');  // Gracefully terminate the process after 2 seconds
        }
        cppProgram = null;  // Ensure cleanup by setting cppProgram to null
    }, 2000);

    // Quit the app unless running on macOS (where apps typically stay active in the background)
    if (process.platform !== 'darwin') app.quit();
});
