
#include <cstdio>

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
        printf("Error executing process");
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

int main()
{
    char *cmd = ParseCommandLine();

    printf("=== %s ===\n", cmd);

    LARGE_INTEGER freq, start, end;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    ExecuteCommand(cmd);

    QueryPerformanceCounter(&end);

    printf("\nExecution time: %.3f s\n",
           static_cast<float>(end.QuadPart - start.QuadPart) / static_cast<float>(freq.QuadPart));

    return EXIT_SUCCESS;
}
