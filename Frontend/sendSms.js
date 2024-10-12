// Correct import for AWS SDK v3
const { SNSClient, PublishCommand } = require("@aws-sdk/client-sns");

// Ensure correct number of command-line arguments
if (process.argv.length < 3) {
    console.error("Usage: node sendSms.js <Message>");
    process.exit(1);
}

// Command-line argument: [0] -> node, [1] -> script name, [2] -> message
const message = process.argv[2];

// Initialize SNS client
const snsClient = new SNSClient({ region: "ca-central-1" });

// Define SMS parameters
const params = {
    Message: message,  // The SMS message content
    PhoneNumber: '+16045000796',  // Update with your verified phone number in E.164 format
};

// Async function to send the SMS
const run = async () => {
    try {
        const data = await snsClient.send(new PublishCommand(params));
        console.log("SMS sent successfully:", data);
    } catch (err) {
        console.error("Error sending SMS:", err.message);
    }
};

// Execute the SMS send function
run();