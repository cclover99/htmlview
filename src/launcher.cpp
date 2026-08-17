#include <windows.h>
#include <iostream>
#include <string>

std::string RegGet(HKEY root, const char* subKey, const char* value){
    HKEY hKey;
    char buf[512];
    DWORD size = sizeof(buf);
    if (RegOpenKeyExA(root, subKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS) return "";
    LONG r = RegQueryValueExA(hKey, value, nullptr, nullptr, (LPBYTE)buf, &size);
    RegCloseKey(hKey);
    return (r == ERROR_SUCCESS) ? std::string(buf) : "";
}

std::string SanitizePath(const std::string& cmd){
    if (cmd.empty()) return "";

    // Take what's between the quotes
    if (cmd[0] == '"'){
        size_t end = cmd.find('"', 1);
        return cmd.substr(1, end - 1);
    }

    // Cut right after ".exe" (case-insensitive)
    std::string lower = cmd;
    for (auto& c : lower) c = (char)tolower((unsigned char)c);
    size_t end = lower.find(".exe");

    if (end != std::string::npos) return cmd.substr(0, end + 4);

    // Fallback: up to first space
    return cmd.substr(0, cmd.find(' '));
}

std::string GetBrowser(){
    std::string progId = RegGet(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\Shell\\Associations\\UrlAssociations\\http\\UserChoice", "ProgId");

    std::string cmd = RegGet(HKEY_CLASSES_ROOT, (progId + "\\shell\\open\\command").c_str(), "");
    
    std::string clean_cmd = SanitizePath(cmd);

    return clean_cmd;
}

int main(int argc, char* argv[]){
    if (argc < 2 || argc > 3) {
        std::cout << "Usage: launcher.exe <path> <path>" << std::endl;
        return 1;
    }

    std::string path = GetBrowser();
    std::string args = std::string("file://") + argv[1] + "?file=\"" + argv[2] + "\"";

    ShellExecuteA(nullptr, "open", path.c_str(), args.c_str(), nullptr, SW_SHOWNORMAL);
}