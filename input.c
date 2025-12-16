#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Remove extra spaces from the start and end*/
static void trim(char *s)
{
    size_t len;
    char *p = s;

    while (*p && isspace((unsigned char)*p)) p++;
    if (p != s) memmove(s, p, strlen(p) + 1);

    len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[len - 1] = '\0';
        len--;
    }
}

int input_parse_number_with_suffix(const char *text, double *out_value)
{
    char buf[128];
    char *endptr;
    double val, mult = 1.0;
    size_t len;

    if (!text || !out_value) return 0;

    strncpy(buf, text, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    trim(buf);

    if (buf[0] == '\0') return 0;

    len = strlen(buf);

    /* Suffix Handling */
    {
        char last = buf[len - 1];
        if (isalpha((unsigned char)last)) {
            if (last == 'k' || last == 'K') mult = 1e3; /* Kilo */
            else if (last == 'M') mult = 1e6; /* Mega */
            else if (last == 'G') mult = 1e9; /* Giga */
            else if (last == 'm') mult = 1e-3; /* milli */
            else if (last == 'u' || last == 'U') mult = 1e-6; /* micro */
            else return 0;

            buf[len - 1] = '\0';
            trim(buf);
            if (buf[0] == '\0') return 0;
        }
    }

    val = strtod(buf, &endptr);
    if (endptr == buf) return 0;

    /* Makes sure no extra characters were entered */
    while (*endptr) {
        if (!isspace((unsigned char)*endptr)) return 0;
        endptr++;
    }

    *out_value = val * mult;
    return 1;
}

double input_read_double(const char *prompt)
{
    char line[128];
    double val;
    int ok;

    for (;;) {
        printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin)) return 0.0;
        line[strcspn(line, "\r\n")] = '\0';

        ok = input_parse_number_with_suffix(line, &val);
        if (!ok) {
            printf("Invalid number. Try again.\n");
            continue;
        }
        return val;
    }
}

double input_read_double_min(const char *prompt, double min_value)
{
    double v;
    for (;;) {
        v = input_read_double(prompt);
        if (v < min_value) {
            printf("Value must be >= %.6g\n", min_value);
            continue;
        }
        return v;
    }
}

int input_read_int_range(const char *prompt, int min_value, int max_value)
{
    char line[128];
    char *endptr;
    long val;

    for (;;) {
        printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin)) return min_value;
        line[strcspn(line, "\r\n")] = '\0';

        /* Trim */
        {
            char *p = line;
            while (*p && isspace((unsigned char)*p)) p++;
            if (p != line) memmove(line, p, strlen(p) + 1);
        }

        if (line[0] == '\0') {
            printf("Enter a number.\n");
            continue;
        }

        val = strtol(line, &endptr, 10);
        if (endptr == line) {
            printf("Enter a valid integer.\n");
            continue;
        }
        while (*endptr) {
            if (!isspace((unsigned char)*endptr)) {
                printf("Enter a valid integer.\n");
                goto next_try;
            }
            endptr++;
        }

        if (val < min_value || val > max_value) {
            printf("Enter a number in range %d to %d.\n", min_value, max_value);
            continue;
        }
        return (int)val;

    next_try:
        continue;
    }
}
