#ifndef HISTORY_H
#define HISTORY_H

/* Appends a plain text line to history.txt and adds timestamp automatically */
void history_append(const char *line);

/* formatiing for arrangment */
void history_append_series_parallel(const char *mode, const double *r, int n, double req);
void history_append_ohms(const char *label, double V, double I, double R);

/* Prints history.txt to screen */
void history_view(void);

#endif
