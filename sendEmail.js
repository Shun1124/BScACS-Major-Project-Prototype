const { SESClient, SendEmailCommand } = require("@aws-sdk/client-ses");
// Import SESClient and SendEmailCommand from the AWS SDK to interact with Amazon SES.

const fs = require('fs');
// Import the file system module to read the configuration file.

const path = require('path');
// Import the path module to handle file and directory paths.

// Get the current directory of this script (where sendEmail.js is located)
const currentDir = path.dirname(__filename);
// `__filename` gives the current file's path, and `path.dirname` extracts the directory.

// Load AWS credentials and region from the config file (assuming awsconfig.json is in the same directory as sendEmail.js)
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

const subject = process.argv[2];
// The email subject is passed as the second argument when running the script.

const body = process.argv[3];
// The email body is passed as the third argument when running the script.

const emailAddress = process.argv[4];
// The recipient's email address is passed as the fourth argument.

console.log("Sending email to:", emailAddress);
// Output the recipient's email address for debugging purposes.

// Create the SES client with credentials from the config file
const sesClient = new SESClient({
    region: config.region,
    // Use the region specified in the awsconfig.json file.
    credentials: {
        accessKeyId: config.accessKeyId,
        // Access key from the config file.
        secretAccessKey: config.secretAccessKey
        // Secret access key from the config file.
    }
});

const params = {
    Destination: {
        ToAddresses: [emailAddress],
        // Set the recipient email address.
    },
    Message: {
        Body: {
            Text: { Data: body },
            // Set the email body text.
        },
        Subject: { Data: subject },
        // Set the email subject.
    },
    Source: 'ingridh2630@gmail.com',
    // Set the sender's email address (must be verified in SES).
};

// Asynchronous function to send the email using SES
const run = async () => {
    try {
        await sesClient.send(new SendEmailCommand(params));
        // Send the email with the specified parameters.
        console.log("Email sent successfully.");
        // Output a success message if the email is sent.
    } catch (err) {
        console.error("Error sending email:", err.message);
        // Output an error message if something goes wrong.
    }
};

run();
// Execute the asynchronous function to send the email.
