#include <windows.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

wstring utf8_to_wstring(const string& s) {
    if (s.empty()) return {};
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), NULL, 0);
    wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), &wstr[0], size_needed);
    return wstr;
}

void typeWithDelay(const wstring& text, int delay) {
    for (wchar_t wc : text) {
        INPUT inp[2] = {};
        inp[0].type = INPUT_KEYBOARD;
        inp[0].ki.wScan = wc;
        inp[0].ki.dwFlags = KEYEVENTF_UNICODE;
        inp[1] = inp[0];
        inp[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
        SendInput(2, inp, sizeof(INPUT));
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    cout << "══════════════════════════════════════════════════\n";
    cout << "               Unknown запущен\n";
    cout << "══════════════════════════════════════════════════\n";
    cout << "   Легендарный автофлудер с душой от @quoterr\n";
    cout << "   Нажми U или u, чтобы начать, Закройте терминал чтобы остановить.\n";
    cout << "══════════════════════════════════════════════════\n";
    cout << "                   @prank\n";
    cout << "══════════════════════════════════════════════════\n";

    auto exePath = fs::current_path();
    auto filePath = exePath / "phrases.txt";
    cout << "Ищу файл по пути: " << filePath.string() << "\n";

    ifstream file(filePath, ios::binary);
    if (!file) {
        cerr << "Не удалось открыть phrases.txt!\n";
        return 1;
    }

    vector<wstring> phrases;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            phrases.push_back(utf8_to_wstring(line));
        }
    }
    file.close();

    if (phrases.empty()) {
        cerr << "Файл пуст или содержит только пустые строки.\n";
        return 1;
    }

    int messageDelay, charDelay;
    cout << "\nВведите задержку между сообщениями (мс): ";
    cin >> messageDelay;
    cout << "Введите задержку между буквами (мс): ";
    cin >> charDelay;
    cin.ignore();

    cout << "Введите текст перед сообщением (или 'No' для пропуска): ";
    string prefix8;
    getline(cin, prefix8);
    wstring prefix = (prefix8 == "No" ? L"" : utf8_to_wstring(prefix8 + " "));

    cout << "\nЧерез 3 секунды переключитесь в окно...\n";
    this_thread::sleep_for(chrono::seconds(3));
    cout << "Нажмите U или u для старта флудинга...\n";

    while (true) {
        if ((GetAsyncKeyState('U') & 0x8000) || (GetAsyncKeyState('u') & 0x8000)) {
            cout << "Старт флудинга!\n";
            break;
        }
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    while (true) {
        for (const auto& wphrase : phrases) {
            if ((GetAsyncKeyState('P') & 0x8000) || (GetAsyncKeyState('p') & 0x8000)) {
                cout << "Флуд остановлен.\n";
                return 0;
            }

            if (!prefix.empty()) {
                typeWithDelay(prefix, charDelay);
            }
            typeWithDelay(wphrase, charDelay);

            INPUT down = { INPUT_KEYBOARD };
            down.ki.wVk = VK_RETURN;
            INPUT up = down;
            up.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &down, sizeof(INPUT));
            SendInput(1, &up,   sizeof(INPUT));

            this_thread::sleep_for(chrono::milliseconds(messageDelay));
        }
    }

    return 0;
}
