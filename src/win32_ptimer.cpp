
#include <Windows.h>

#define internal static

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define INVALID_INDEX -1

extern "C" {
int _fltused;
#pragma function(memset)
void *memset(void *dest, int c, size_t count)
{
    char *bytes = (char *)dest;
    while (count--) {
        *bytes++ = (char)c;
    }
    return dest;
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

internal void Win32ExecuteCommand(char *cmd)
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

internal float Pow(float base, int exponent)
{
    if (exponent == 0) {
        return 1.f;
    }

    if (exponent == 1) {
        return base;
    }

    float pwr = Pow(base, exponent / 2);

    if ((exponent % 2) == 0) {
        return pwr * pwr;
    }
    else {
        return base * pwr * pwr;
    }
}

internal void Reverse(char *str, int len)
{
    int i = 0, j = len - 1;
    char tmp;
    while (i < j) {
        tmp    = str[i];
        str[i] = str[j];
        str[j] = tmp;
        ++i;
        --j;
    }
}

internal int Itoa(int val, char *str, int precision)
{
    int i = 0;

    if (val == 0) {
        str[i++] = '0';
    }
    else {
        while (val) {
            str[i++] = (val % 10) + '0';
            val /= 10;
        }

        while (i < precision) {
            str[i++] = '0';
        }

        Reverse(str, i);
    }

    str[i] = '\0';

    return i;
}

internal void Ftoa(float val, char *str, int precision = 0)
{
    int whole = static_cast<int>(val);
    float dec = val - whole;

    int idx = Itoa(whole, str, 0);

    if (precision != 0) {
        str[idx] = '.';

        dec = dec * Pow(10, precision);

        Itoa(static_cast<int>(dec), str + idx + 1, precision);
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
    Ftoa(t, timeStr + lstrlenA(timeStr), 3);
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

    Win32ExecuteCommand(cmd);

    QueryPerformanceCounter(&end);

    PrintElapsedTime(static_cast<float>(end.QuadPart - start.QuadPart)
                     / static_cast<float>(freq.QuadPart));

    return EXIT_SUCCESS;
}
