# COMP 8800 Major Project - Windows Registry Monitor Prototype

This project monitors Windows registry keys for changes, detects critical and non-critical changes, and sends alerts via email and SMS using AWS services (SES and SNS). It also provides a rollback feature for critical registry changes. The project consists of a C++ backend for monitoring registry changes and an Electron-based frontend for displaying log information and managing user settings.

## Table of Contents
[Features](#features)\
[Prerequisites](#prerequisites)\
[Running the Project](#running-the-project)\
[Testing the Project](#testing-the-project)\
[Code Structures](#code-structures)

## Features
**Real-Time Monitoring**: WRM monitors critical registry keys using `RegNotifyChangeKeyValue`. Detected changes are logged and processed immediately.\
**Rollback System**:  Critical changes are reverted within 5 seconds using `RegSetValueEx`, ensuring system integrity is maintained.\
**Customizable Alerts**: Alerts are sent to system administrators within 5 seconds of detecting critical changes, ensuring timely intervention.

## Prerequisites
### Tools and Technologies Required
#### Windows System
**Operating System**: Windows 10 or later (64-bit).\
**Development Environment** Visual Studio 2022 (Community Edition is sufficient).\
**Node.js**: v16.0.0 or higher\
**AWS Account** (Optional): For sending email and SMS alerts through SES (Simple Email Service) and SNS (Simple Notification Service). (Optional)

#### Libraries and Frameworks
**Electron**: Used for the graphical user interface (GUI)\
**AWS SDK for JavaScript**: Used for sending emails and SMS via AWS SES and SNS.\
**Concurrently**: For running both the Electron frontend and the C++ backend simultaneously.\

### Installation Instructions
#### Step 1: Install Node.js
- [Download and Install Node.js] (https://nodejs.org/en) (v16.0.0 or higher).
- Verify installation by running `node -v` and `npm -v` in the terminal.
#### Step 2: Install Visual Studio 2022
- [Download Visual Studio 2022] (https://visualstudio.microsoft.com/downloads/) and install it.
- During installation, ensure you select **Desktop development with C++**.
- Install **vcpkg** for dependency management:
    - Follow this [Installing vcpkg and curl](#installing-vcpkg-and-curl).
    - Make sure `curl` is installed for fetching packages through vcpkg. Windows 10+ typically has it pre-installed. You can verify this by running `curl --version` in the terminal. 

#### Step 3: Install Electron
- Open a terminal and run:
```
npm install -g electron
```

#### Step 4: Set up AWS Services
- Signup for an AWS account and set up SES and SNS.
- **SES**: Verify your email address (required by AWS SES to send emails).
- **SNS**: Register and set up SMS service in the AWS console.

#### Step 5: Install AWS SDK and Concurrently
- In the project directory, install required dependencies by running: 
```
npm install @aws-sdk/client-ses @aws-sdk/client-sns concurrently
```

#### Step 6: Configure AWS Credentials (Optional)
The project comes with a pre-configured  `awsconfig.json` file that contains access key information for a restricted IAM user with full access to SNS and SES. If you want to use your own AWS account instead, you can modify the existing `awsconfig.json` with your own credentials in the project's root directory with your own access information.
```
{
  "accessKeyId": "YOUR_ACCESS_KEY_ID",
  "secretAccessKey": "YOUR_SECRET_ACCESS_KEY",
  "region": "YOUR_AWS_REGION"
}
```
- Ensure the file is placed in the directory where `sendEmail.js` and `sendSms.js` scripts reside.

### Configuration Settings
**AWS Configuration**
- **AWS SES**: Requires a verified email address to send alerts. This can be set up in the AWS Console under the SES service.
- **AWS SNS**: Will use a phone number for SMS notifications. Ensure the number is registered in AWS SNS.
- If you plan to use your own AWS Console account, remember to verify your email and phone number first.

## Running the Project
Follow these steps to run the project:

### Step 1: Clone the Repository 
Clone the project repository to local machine, `cd` to the cloned project folder:
```
git clone https://github.com/your-repo/registry-monitoring-system.git
cd BScACS-Major-Project-Prototype
```

### Step 2: Install Dependencies
Install all required Node.js dependencies by running:
```
npm install
```

### Step 3: Build the C++ Code
- Open the solution file (`Prototype.sln`) in **Visual Studio 2022**.
- Build the project in **Debug mode** for the `x64` platform.

### Step 4: Run the Project
Start the entire project (Electron frontend + C++ backend) by running:
```
npm start
```
This will open an Electron window and start monitoring registry keys. The GUI will display log information, and alerts will be sent if critical changes are detected.

### Step 5: Set Email and Phone Number
1. Enter your email and phone number in the Electron GUI
2. Click **Save Settings** to update your preferences.
Once saved, alerts will be sent via SMS and email when critical registry key changes occur.

## Testing the Project
Here are a few test cases to verify the project works as expected:

### Test Case 1: Registry Key Change Detection
1. Modify the wallpaper on the system (this is monitored by default).
2. The console and the Electron log should display the change as non-critical.
3. Change the **DoubleClickSpeed** (a critical registry key).
4. The console should display `[CRITICAL]` and roll back the change automatically.

### Test Case 2: Email and SMS Alerts
1. Modify the **DoubleClickSpeed** key with email and phone settings saved.
2. You should receive an email and SMS alert indicating a critical change.

### Test Case 3: Rollback Functionality
1. Change the **DoubleClickSpeed** key.
2. After the alert, the value should automatically rollback to its original state.
3. Verify the rollback by checking the double-click speed setting on the your system.

## Code Structures
### Frontend (Electron Dashboard)
- **index.html**: The primary entry point for the user interface. It contains the HTML structure and elements displayed to the user.

- **app.js**: Handles user interactions in the frontend and communicates with the backend to get real-time registry changes and update the interface.

- **main.js**: Responsible for setting up the Electron application, bridging the frontend (user interface) and backend (monitoring processes).

- **.css**: Styles for the frontend, defining the layout and visual presentation of elements in the user interface.

### Backend (C++ Monitoring System)
- **main.cpp**: Starts the monitoring process, creates threads to monitor multiple registry keys, and initializes the user settings and alert systems.

- **regsitryKey.h**: Defines the structure and attributes of the registry keys that will be monitored.

- **registryKeys.cpp**: Implements the logic for loading and accessing the registry keys being monitored.

- **monitoring.h**: Declares the real-time monitoring functions.

- **monitoring.cpp**: Implements real-time tracking of registry changes, logs changes, and triggers alerts or rollbacks for critical keys.

- **rollback.h**: Declares functions for handling rollback functionality.

- **rollback.cpp**: Implements the rollback mechanism that restores the previous state of a registry key when a critical change is detected.

- **alerts.h**: Defines the alert system's interfaces.

- **alerts.cpp**: Manages the logic for sending notifications (emails and SMS) to the user when critical registry changes are detected.

- **sendEmail.js**: A Node.js script for sending email notifications via AWS SES.

- **sendSms.js**: A Node.js script for sending SMS notifications via AWS SNS.

- **stringUtils.h**: Declares utility functions for string manipulation.

- **stringUtils.cpp**: Implements string conversions and utilities used throughout the backend for handling registry key paths and values.

- **userSettings.h**: Declares functions for managing user preferences and settings.

- **userSettings.cpp**: Implements functions for handling user settings such as saving/loading preferences, managing email and phone inputs, and allowing dynamic changes during runtime.

## Installing vcpkg and curl
### Step 1: Install curl
1. Check if **curl** is already installed by running the following in the command prompt:
```
curl --version
```
If you see a version number, **curl** is already installed. If not, follow the steps below.

2. For Windows 10 or Later:
- **curl** is typically pre-instlaled. If it's missing, download the installer from (https://curl.se/windows/).
- Extract the zip file and place the `curl.exe` file somewhere convenient (e.g., `C:\curl\`).
3. Add curl to your **PATH**:
- Right-click This **PC** or **Computer**, select **Properties**.
- Click **Advanced system settings**, then click **Environment Variables**.
- In the **System Variables** section, find **Path**, and click Edit.
- Add the directory where you placed `curl.exe` (e.g., `C:\curl\`) to the PATH.
- Open a new terminal window and run `curl --version` to ensure it's installed.

### Step 2: Install vcpkg
1. **Clone vcpkg repository**: Open the command prompt (as Administrator) and run the following:
```
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
```
2. **Bootstrap vcpkg**: After navigating to the `vcpkg` directory, run: 
```
.\bootstrap-vcpkg.bat
```
3. **Add vcpkg to PATH** (Optional): You can add **vcpkg** to your system PATH for easier access:
- Follow the same steps as for **curl** above.
- Add the full path to the **vcpkd** executable (e.g., `C:\path\to\vcpkg`).

4. **Install Required Libraries**: Now that **vcpkg** is installed, you can use it to install necessary libraries:
```
.\vcpkg install curl
```
This ensure that **curl** and any other required dependencies are managed through **vcpkg**.

5. **Integrate vcpkg with Visual Studio**: To integrate **vcpkg** into Visual Studio 2022, run:
```
.\vcpkg integrate install
```
Now, **vcpkg** and **curl** are installed, and you can proceed building the project using Visual Studio as described in the earlier steps.
