
#include <cstdint>
#include <cstdio>
#include <string>

#include <Windows.h>

#define internal static

using namespace std;

struct CommandLineInput {
    bool echo = true;
    string command;
};

internal CommandLineInput GetCommandStringFromInput(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: ptimer <process command>");
        ExitProcess(EXIT_FAILURE);
    }

    CommandLineInput input = {};

    int index     = 1; // @note(eb): start index at 1 because element 0 is the exe name
    input.command = argv[index++];

    if (input.command.compare("-q") == 0) {
        input.echo    = false;
        input.command = argv[index++];
    }

    for (; index < argc; ++index) {
        input.command.append(" ").append(argv[index]);
    }

    return input;
}

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

int main(int argc, char **argv)
{
    CommandLineInput input = GetCommandStringFromInput(argc, argv);

    if (input.echo) {
        printf("=== %s ===\n", input.command.c_str());
    }

    LARGE_INTEGER freq, start, end;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    ExecuteCommand(input.command.data());

    QueryPerformanceCounter(&end);

    printf("\nExecution time: %.3f s\n",
           static_cast<float>(end.QuadPart - start.QuadPart) / static_cast<float>(freq.QuadPart));

    return EXIT_SUCCESS;
}
