#include <exception>
#include <logging.h>
#include <stdio.h>

FILE *Logger::logger = NULL;

DataFlowException::DataFlowException(const char *type, const char *error)
{
    sprintf(msg, "Throwing Exception: (%s): %s", type, error);
    Logger::LogEvent(msg);
}

void Logger::LogEvent(const char *event)
{
    if (logger == NULL)
    {
	logger = fopen("logger", "w");
    }
//printf("%s\n", event);
    fprintf(logger,"%s\n", event);
}

void Logger::Finalize()
{
    if (logger != NULL)
	fclose(logger);
}
