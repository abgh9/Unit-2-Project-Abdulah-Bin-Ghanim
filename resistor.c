#include "resistor.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>



static void to_lower_str(char *s)
{
    while (*s) {
        *s = (char)tolower((unsigned char)*s);
        s++;
    }
}

/* Accepts "gray", "grey", and "purple", "violet" */
static void normalise_colour(char *s)
{
    if (strcmp(s, "gray") == 0) strcpy(s, "grey");
    if (strcmp(s, "purple") == 0) strcpy(s, "violet");
}

/* Returns digit 0-9, or -1 if invalid input */
static int colour_to_digit(const char *c)
{
    if (strcmp(c, "black") == 0) return 0;
    if (strcmp(c, "brown") == 0) return 1;
    if (strcmp(c, "red") == 0) return 2;
    if (strcmp(c, "orange") == 0) return 3;
    if (strcmp(c, "yellow") == 0) return 4;
    if (strcmp(c, "green") == 0) return 5;
    if (strcmp(c, "blue") == 0) return 6;
    if (strcmp(c, "violet") == 0) return 7;
    if (strcmp(c, "grey") == 0) return 8;
    if (strcmp(c, "white") == 0) return 9;
    return -1;
}

/* Returns the exponent for multiplier (10^exp) and gold/silver negative exponents. */
static int colour_to_multiplier_exp(const char *c, int *ok)
{
    *ok = 1;
    if (strcmp(c, "gold") == 0) return -1;
    if (strcmp(c, "silver") == 0) return -2;

    {
        int d = colour_to_digit(c);
        if (d >= 0) return d;
    }

    *ok = 0;
    return 0;
}

/* Returns tolerance percent. If invalid returns 0 and ok=0 */
static double colour_to_tolerance(const char *c, int *ok)
{
    *ok = 1;
    if (strcmp(c, "brown") == 0) return 1.0;
    if (strcmp(c, "red") == 0) return 2.0;
    if (strcmp(c, "green") == 0) return 0.5;
    if (strcmp(c, "blue") == 0) return 0.25;
    if (strcmp(c, "violet") == 0) return 0.1;
    if (strcmp(c, "grey") == 0) return 0.05;
    if (strcmp(c, "gold") == 0) return 5.0;
    if (strcmp(c, "silver") == 0) return 10.0;

    *ok = 0;
    return 0.0;
}

static void format_resistance(double ohms, char *out, size_t n)
{
    const char *unit = "ohms";
    double v = ohms;

    if (fabs(v) >= 1e6) { v /= 1e6; unit = "Mohms"; }
    else if (fabs(v) >= 1e3) { v /= 1e3; unit = "kohms"; }

    snprintf(out, n, "%.6g %s", v, unit);
}


int resistor_decode_from_line(const char *line, ResistorResult *out)
{
    char buf[256];
    char *tok;
    char colours[5][32];
    int count = 0;
    int ok;
    int exp;
    double tol;

    if (!line || !out) return 0;

    strncpy(buf, line, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    tok = strtok(buf, " \t");
    while (tok && count < 5) {
        strncpy(colours[count], tok, sizeof(colours[count]) - 1);
        colours[count][sizeof(colours[count]) - 1] = '\0';
        to_lower_str(colours[count]);
        normalise_colour(colours[count]);
        count++;
        tok = strtok(NULL, " \t");
    }

    if (!(count == 4 || count == 5)) return 0;

    memset(out, 0, sizeof(*out));
    out->bands = count;

    if (count == 4) {
        int d1 = colour_to_digit(colours[0]);
        int d2 = colour_to_digit(colours[1]);
        exp = colour_to_multiplier_exp(colours[2], &ok);
        if (d1 < 0 || d2 < 0 || !ok) return 0;

        tol = colour_to_tolerance(colours[3], &ok);
        if (!ok) return 0;

        out->ohms = (double)(d1 * 10 + d2) * pow(10.0, (double)exp);
        out->tolerance_percent = tol;
    } else {
        int d1 = colour_to_digit(colours[0]);
        int d2 = colour_to_digit(colours[1]);
        int d3 = colour_to_digit(colours[2]);
        exp = colour_to_multiplier_exp(colours[3], &ok);
        if (d1 < 0 || d2 < 0 || d3 < 0 || !ok) return 0;

        tol = colour_to_tolerance(colours[4], &ok);
        if (!ok) return 0;

        out->ohms = (double)(d1 * 100 + d2 * 10 + d3) * pow(10.0, (double)exp);
        out->tolerance_percent = tol;
    }

    out->min_ohms = out->ohms * (1.0 - out->tolerance_percent / 100.0);
    out->max_ohms = out->ohms * (1.0 + out->tolerance_percent / 100.0);

    format_resistance(out->ohms, out->formatted_value, sizeof(out->formatted_value));
    format_resistance(out->min_ohms, out->formatted_min, sizeof(out->formatted_min));
    format_resistance(out->max_ohms, out->formatted_max, sizeof(out->formatted_max));

    snprintf(out->history_line, sizeof(out->history_line),
             "Resistor decode (%d-band): %s -> %s, tol ±%.3g%%, range [%s, %s]",
             out->bands, line, out->formatted_value, out->tolerance_percent,
             out->formatted_min, out->formatted_max);

    return 1;
}
