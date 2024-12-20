# Marburg
![image](https://github.com/user-attachments/assets/ff5f3891-6359-4ccd-8286-e991188445cf)

Marburg is a system information collector designed to gather and transmit sensitive data securely. It collects essential system details such as:

- Current user’s username
- Computer name
- Operating system version
- System uptime

This information is sent to a specified webhook URL for further processing or monitoring. To enhance security and prevent unauthorized access or reverse engineering, the webhook URL is obfuscated using the **oxorany** library.

## Features
- **System Information Collection**: Retrieves critical system data including user and computer details, OS version, and uptime.
- **Webhook URL Obfuscation**: Protects the webhook URL by obfuscating it with the **oxorany** library, making it difficult to identify and reverse-engineer.
- **Secure Data Transmission**: Ensures that sensitive information is securely sent to the specified webhook without exposure.p

---

## License

This project is licensed [License](LICENSE).
