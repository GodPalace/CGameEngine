#include "Logger.h"

FILE* g_logFile = nullptr;

void cutil::Logger(const std::string& message)
{
    if (g_logFile == nullptr)
    {
        g_logFile = fopen("log.txt", "a");
    }

    fprintf(g_logFile, "%s\n", message.c_str());
    fflush(g_logFile);
}
