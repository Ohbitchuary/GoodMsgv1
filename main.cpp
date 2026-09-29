#include <windows.h>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstdlib>

// ==========================================
// CONFIGURATION
// ==========================================
const int BIRTHDAY_MONTH = 9; // September
const int BIRTHDAY_DAY = 30;  // September 30th

// 25% daily chance = averages ~1 or 2 surprise messages per week
const int RANDOM_PERCENT = 25;

// SIMPLE SINGLE-POPUP BIRTHDAY MESSAGES
const std::vector<std::wstring> BIRTHDAY_MESSAGES = {
    L"Happy birthday, Raven! I hope today is as amazing as you are. I wrote this program for you as a gift. Pretty cool, right?\nI suck at words, enjoy <3",
    L"It's your birthday again, how are you feeling? Do ya feel old yet? Heh.. happy birthday, lovely. *mwah*",
    L"BIRTHDAY GIRL ALERT!!\nBIRTHDAY GIRL ALERT!!\nBIRTHDAY GIRL ALERT!!",
    L"Another birthday? You're growing up, and I'm ever so proud of how far you've come. Kepp going",
    L"Happy birthday! Idk if you still have this program installed anymore, but.. Happy birthday. I love you.",
    L"Happy Birthday, babes! May all your wishes come true <3",
    L"Another year older, wiser\nand still tolerating me.\nYou deserve an award.",
    L"Today's a nice day for a birthday, isn't it.\nHappy birthday, Raven! I love you :)",
};

// SIMPLE SINGLE-POPUP HOLIDAYS
const std::wstring NEW_YEAR_MESSAGE = L"Sheesh, I can't believe that 2026 is done already. I'm sure 2027 is gonna be your year for sure, come hell or high water. <3";
const std::wstring CHRISTMAS_MESSAGE = L"It's Christmas already? What a hell of a year so far, but we made it. It's been a year of shit; the drama and that turdmelon Chloe, etc etc. But here we are near the end of 2026.\nMerry Christmans, Raven. Love, Nova/Kaysu";

// SIMPLE SINGLE-POPUP RANDOM MESSAGES
const std::vector<std::wstring> RANDOM_MESSAGES = {
    L"Hey babes, Nova here..\nListen, I know shit is wack right now and it seems like an endless tunnel of bullshit, but I promise you it will get better.\nI am so proud of you for making it this far, and holding on when everything inside you screams to give up.\n\nAs I say, you fall down, you get up again.\nYou stall, you start again. You got this.",
    L"If nobody's told you today\nyou're amazing and\nI'm so proud of you, babes",
    L"Make sure to step back when things get too much, you're not a machine",
    L"Per infernum ad astra\n(Through hell to the stars)",
    L"You are a wonderful human being,\nand I am happy you're in my life.",
    L"This program is beta,\nbut each message comes form the heart -Nova",
    L"Age 32 (as of this box),\nand I STILL SUCK AT WORDS.\nI LOVE YOU DAMMIT. XD",
    L"AWEBO AWEBO AWEBO\nAWEBO AWEBO AWEBO\nAWEBO AWEBO AWEBO",
    L"In case nobody has tol you today:\nyou're beautiful and I'm so proud of you",
    L"Nova says: Don't forget to hydrate and take your meds (if needed) <3",
    L"Nova was here, telling you that\nshe loves you veeeery much,\nlike SO MUCH! <3",
    L"Hey hun.. You okay? Are you *really* okay? It's okay if you're not..\nI'll always be here if you need me, okay?\nOne day, we will meet and I'm gonna give you\nthe biggest hug in the world okay? It's gonna be okay <3",
    L"It's ya girl Nova here!\nSorry for any bad text formatting, I'm new(ish) to C++.\nMight be fixed in a future update.. Maybe",
    L"Remeber that you come first. Don't forget your medication\n and don't forget to eat okay? Did you drink water and not Monster today?",
    L"So.. many.. lines of code..\nyou are worth it. *mwah*"
};

const std::wstring TASK_NAME = L"GoodMsg by Nova";

std::string GetLogFilePath() {
    char userProfile[MAX_PATH];
    ExpandEnvironmentStringsA("%USERPROFILE%", userProfile, MAX_PATH);
    return std::string(userProfile) + "\\.GoodLog.txt";
}

// Single popup box—she clicks OK once and it closes completely
void ShowPopup(const std::wstring& title, const std::wstring& message) {
    MessageBoxW(NULL, message.c_str(), title.c_str(), MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
}

void InstallTask() {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);

    std::wstring cmd = L"schtasks /create /tn \"" + TASK_NAME + L"\" /tr \"\\\"" + exePath + L"\\\"\" /sc ONLOGON /rl LIMITED /f";

    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    if (CreateProcessW(NULL, &cmd[0], NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        MessageBoxW(NULL, L"GoodMsg installed!", L"Success", MB_OK | MB_ICONINFORMATION);
    } else {
        MessageBoxW(NULL, L"Failed to create scheduled task.", L"Error", MB_OK | MB_ICONERROR);
    }
}

void UninstallTask() {
    std::wstring cmd = L"schtasks /delete /tn \"" + TASK_NAME + L"\" /f";

    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    if (CreateProcessW(NULL, &cmd[0], NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    DeleteFileA(GetLogFilePath().c_str());
    MessageBoxW(NULL, L"The Raven App has been completely removed.", L"Uninstalled", MB_OK | MB_ICONINFORMATION);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    time_t t = time(0);
    tm* now = localtime(&t);
    int month = now->tm_mon + 1;
    int day = now->tm_mday;
    int year = now->tm_year + 1900;

    std::string todayStr = std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
    std::string logPath = GetLogFilePath();

    // 1. First-time auto-install check (Task scheduler registration)
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);

    std::wstring checkCmd = L"schtasks /query /tn \"" + TASK_NAME + L"\" >nul 2>&1";
    if (system(std::string(checkCmd.begin(), checkCmd.end()).c_str()) != 0) {
        std::wstring installCmd = L"schtasks /create /tn \"" + TASK_NAME + L"\" /tr \"\\\"" + exePath + L"\\\"\" /sc ONLOGON /rl LIMITED /f >nul 2>&1";
        system(std::string(installCmd.begin(), installCmd.end()).c_str());

        // Show the initial welcome message on first install
        ShowPopup(L"GoodMsg v1.0", L"Welcome to GoodMsg v1.1, Raven!\nMade by Nova with love from Toronto,Canada! <3");
    }

    // 2. Standard Daily Log Check
    std::ifstream logFileIn(logPath);
    bool ranToday = false;
    if (logFileIn.is_open()) {
        std::string lastRun;
        logFileIn >> lastRun;
        logFileIn.close();
        if (lastRun == todayStr) {
            ranToday = true;
        }
    }

    // For testing: uncomment the line below to force popups every time you run it
    // ranToday = false;

    if (!ranToday) {
        std::ofstream logFileOut(logPath);
        if (logFileOut.is_open()) {
            logFileOut << todayStr;
            logFileOut.close();
        }

        srand(static_cast<unsigned int>(time(0)));

        // --- ALL MESSAGE CHECKS RUN HERE UNIFORMLY ---

        if (month == BIRTHDAY_MONTH && day == BIRTHDAY_DAY) {
            int idx = rand() % BIRTHDAY_MESSAGES.size();
            ShowPopup(L"Happy Birthday, Gorgeous!", BIRTHDAY_MESSAGES[idx]);
        }

        if (month == 1 && day == 1) {
            ShowPopup(L"New year, new you? :)", NEW_YEAR_MESSAGE);
        }

        if (month == 12 && day == 25) {
            ShowPopup(L"Christmas already? Damn.", CHRISTMAS_MESSAGE);
        }

        if ((rand() % 50) < RANDOM_PERCENT) {
            int idx = rand() % RANDOM_MESSAGES.size();
            ShowPopup(L"Made by Nova, pls report any bugs <3", RANDOM_MESSAGES[idx]);
        }
    }

    // Keep the process alive so it stays visible in Task Manager
    while (true) {
        Sleep(10000); // 0% CPU usage loop
    }

    return 0;
}
