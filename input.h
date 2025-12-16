#ifndef INPUT_H
#define INPUT_H

double input_read_double(const char *prompt);
double input_read_double_min(const char *prompt, double min_value);
int    input_read_int_range(const char *prompt, int min_value, int max_value);

/* Converts strings with SI suffixes into numerical value */
int    input_parse_number_with_suffix(const char *text, double *out_value);

#endif
