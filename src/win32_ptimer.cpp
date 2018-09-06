
#include <Windows.h>

#define internal static

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define INVALID_INDEX -1

internal void ExecuteCommand(char *cmd)
{
    STARTUPINFOA si        = {};
    si.cb                  = sizeof(si);
    PROCESS_INFORMATION pi = {};

    if (!CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        ExitProcess(EXIT_FAILURE);
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
}

internal char *ParseCommandLine()
{
    LPSTR cmdLine = GetCommandLineA();

    while (*cmdLine && *cmdLine != ' ' && *cmdLine != '\t') {
        ++cmdLine;
    }

    while (*cmdLine == ' ' || *cmdLine == '\t') {
        cmdLine++;
    }

    return cmdLine;
}

float Round(float val, int precision)
{
    float roundBy = 0.5f;

    for (int i = 0; i < precision; ++i) {
        roundBy *= 0.1f;
    }

    val += roundBy;

    return val;
}

internal void FloatToString(float val, char *str, int precision)
{
    val = Round(val, precision);

    if (val < 0) {
        *str++ = '-';
        val *= -1;
    }

    int whole = static_cast<int>(val);

    if (val < 1.f) {
        *str++ = '0';
    }

    int wholeTmp = 0;
    float valTmp = static_cast<float>(whole);
    int counter  = 0;
    while (valTmp > 1.f) {
        valTmp *= 0.1f;
        counter++;
    }

    for (int i = 0; i < counter; ++i) {
        valTmp -= wholeTmp;
        valTmp *= 10;
        wholeTmp = static_cast<char>(valTmp);

        *str++ = static_cast<char>(wholeTmp) + 0x30;
    }

    *str++ = '.';

    for (int i = 0; i < precision; ++i) {
        val -= whole;
        val *= 10;
        whole = static_cast<char>(val);

        *str++ = static_cast<char>(whole) + 0x30;
    }
}

internal void Win32WriteConsole(const char *str)
{
    DWORD charsWritten;
    DWORD len = lstrlenA(str);
    if (!WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), str, len, &charsWritten, NULL)
        || charsWritten != len) {
        ExitProcess(EXIT_FAILURE);
    }
}

internal void PrintCommandLine(const char *str)
{
    Win32WriteConsole("=== ");
    Win32WriteConsole(str);
    Win32WriteConsole(" ===\n");
}

internal void PrintElapsedTime(float t)
{
    char timeStr[36] = {};
    lstrcpyA(timeStr, "\nExecution time: ");
    FloatToString(t, timeStr + lstrlenA(timeStr), 3);
    lstrcpyA(timeStr + lstrlenA(timeStr), " s\n");

    Win32WriteConsole(timeStr);
}

int main()
{

    LARGE_INTEGER freq, start, end;
    char *cmd = ParseCommandLine();

    PrintCommandLine(cmd);

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    ExecuteCommand(cmd);

    QueryPerformanceCounter(&end);

    PrintElapsedTime(static_cast<float>(end.QuadPart - start.QuadPart)
                     / static_cast<float>(freq.QuadPart));

    return EXIT_SUCCESS;
}
