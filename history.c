#include "history.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

static void timestamp(char *out, size_t n)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(out, n, "%Y-%m-%d %H:%M", tm_info);
}

void history_append(const char *line)
{
    FILE *f;
    char ts[32];

    if (!line) return;

    f = fopen("history.txt", "a");
    if (!f) {
        /* If file can't be opened, the program keeps running */
        return;
    }

    timestamp(ts, sizeof(ts));
    fprintf(f, "[%s] %s\n", ts, line);
    fclose(f);
}

void history_append_series_parallel(const char *mode, const double *r, int n, double req)
{
    char buf[512];
    int i;
    size_t pos = 0;

    pos += (size_t)snprintf(buf + pos, sizeof(buf) - pos, "%s resistors: ", mode);

    for (i = 0; i < n; i++) {
        pos += (size_t)snprintf(buf + pos, sizeof(buf) - pos, "%g", r[i]);
        if (i != n - 1) pos += (size_t)snprintf(buf + pos, sizeof(buf) - pos, ", ");
    }

    snprintf(buf + pos, sizeof(buf) - pos, " -> Req=%g ohms", req);
    history_append(buf);
}

void history_append_ohms(const char *label, double V, double I, double R)
{
    char buf[256];
    snprintf(buf, sizeof(buf), "Ohm's law (%s): V=%g V, I=%g A, R=%g ohms", label, V, I, R);
    history_append(buf);
}

void history_view(void)
{
    FILE *f = fopen("history.txt", "r");
    char line[256];

    if (!f) {
        printf("No history yet (history.txt not found).\n");
        return;
    }

    while (fgets(line, sizeof(line), f)) {
        fputs(line, stdout);
    }

    fclose(f);
}