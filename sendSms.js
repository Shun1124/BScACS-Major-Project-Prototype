const { SNSClient, PublishCommand } = require("@aws-sdk/client-sns");
// Import the SNSClient and PublishCommand from AWS SDK to interact with Amazon SNS for sending SMS.

const fs = require('fs');
// Import the file system module to read the configuration file.

const path = require('path');
// Import the path module to handle file and directory paths.

// Get the current directory of this script (where sendSms.js is located)
const currentDir = path.dirname(__filename);
// `__filename` gives the current file's path, and `path.dirname` extracts the directory.

// Load AWS credentials and region from the config file (assuming awsconfig.json is in the same directory as sendSms.js)
const configFilePath = path.resolve(currentDir, 'awsconfig.json');
// Resolve the absolute path to the `awsconfig.json` file.

//console.log("Resolved config file path:", configFilePath);
// Output the resolved path for debugging purposes.

let config;
try {
    // Read and parse the awsconfig.json file to load AWS credentials and region.
    config = JSON.parse(fs.readFileSync(configFilePath, 'utf-8'));
    //console.log("AWS Credentials loaded:", config);
    // Output the loaded configuration for debugging.
} catch (err) {
    console.error("Error reading the config file:", err.message);
    // Print an error if the config file cannot be read.
    process.exit(1);
    // Exit the program if the config file can't be read or parsed.
}

// Get the message text and phone number from command-line arguments
const message = process.argv[2];
const phoneNumber = process.argv[3];

// Create the SNS client with credentials from the config file
const snsClient = new SNSClient({
    region: config.region,
    // Use the region specified in the awsconfig.json file.
    credentials: {
        accessKeyId: config.accessKeyId,
        // Access key from the config file.
        secretAccessKey: config.secretAccessKey
        // Secret access key from the config file.
    }
});

// Parameters for the PublishCommand to send the SMS message
const params = {
    Message: message,  // The message to be sent via SMS.
    PhoneNumber: phoneNumber,  // The recipient's phone number.
};

// Asynchronous function to send the SMS using SNS
const run = async () => {
    try {
        await snsClient.send(new PublishCommand(params));
        console.log("SMS sent successfully.");
        // Output a success message if the SMS is sent.
    } catch (err) {
        console.error("Error sending SMS:", err.message);
        // Output an error message if something goes wrong.
    }
};

run();
// Execute the asynchronous function to send the SMS.
