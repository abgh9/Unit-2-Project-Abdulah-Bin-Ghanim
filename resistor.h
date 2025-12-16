#ifndef RESISTOR_H
#define RESISTOR_H

typedef struct {
    int bands; /* either 4 or 5 bands */
    double ohms;
    double tolerance_percent;
    double min_ohms;
    double max_ohms;

    char formatted_value[64];
    char formatted_min[64];
    char formatted_max[64];

    /* saves to history data */
    char history_line[256];
} ResistorResult;

/* Main decoder line from a user input  */
int resistor_decode_from_line(const char *line, ResistorResult *out);

#endif
