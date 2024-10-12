const { SESClient, SendEmailCommand } = require("@aws-sdk/client-ses");

// Ensure correct number of command-line arguments
if (process.argv.length < 4) {
    console.error("Usage: node sendEmail.js <Subject> <Body>");
    process.exit(1);
}

// Command-line arguments: [0] -> node, [1] -> script name, [2] -> subject, [3] -> body
const subject = process.argv[2];
const body = process.argv[3];

// Initialize SES client
const sesClient = new SESClient({ region: 'ca-central-1' });

// Define email parameters
const params = {
    Destination: {
        ToAddresses: ['qsjzhdfd130613@gmail.com'],  // Update with recipient email
    },
    Message: {
        Body: {
            Text: { Data: body },  // Email body from command line
        },
        Subject: { Data: subject },  // Email subject from command line
    },
    Source: 'ingridh2630@gmail.com',  // Your verified sender email
};

// Async function to send the email
const run = async () => {
    try {
        const data = await sesClient.send(new SendEmailCommand(params));
        console.log("Email sent successfully:", data);
    } catch (err) {
        console.error("Error sending email:", err.message);
    }
};

// Execute the email send function
run();