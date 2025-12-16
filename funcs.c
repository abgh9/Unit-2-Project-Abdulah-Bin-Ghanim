#include <stdio.h>
#include <string.h>
#include <math.h>
#include "funcs.h"
#include "input.h"
#include "resistor.h"
#include "history.h"

/* Menu Item 1: Resistor decoder */
void menu_item_1(void)
{
    char line[256];
    ResistorResult rr;
    int ok = 0;

    printf("\n>> Resistor colour code decoder\n");
    printf("Enter colours separated by spaces.\n");
    printf("4-band example:  red violet yellow gold\n");
    printf("5-band example:  brown black black red brown\n");
    printf("Type 'q' to cancel.\n\n");

    for (;;) {
        printf("Colours: ");
        if (!fgets(line, sizeof(line), stdin)) return;
        line[strcspn(line, "\r\n")] = '\0';

        if (strcmp(line, "q") == 0 || strcmp(line, "Q") == 0) {
            printf("Cancelled.\n");
            return;
        }

        ok = resistor_decode_from_line(line, &rr);
        if (!ok) {
            printf("Invalid input. Try again.\n");
            continue;
        }

        /* Prints result */
        printf("\nResult:\n");
        printf("  Bands: %d\n", rr.bands);
        printf("  Resistance: %s\n", rr.formatted_value);
        printf("  Tolerance: ±%.3g%%\n", rr.tolerance_percent);
        printf("  Range: %s to %s\n", rr.formatted_min, rr.formatted_max);

        /* Saves to history data */
        history_append(rr.history_line);

        return;
    }
}

/* Menu Item 2: Series/Parallel calculator  */
void menu_item_2(void)
{
    int n = 0;
    int mode = 0; /* 1=series, 2=parallel */
    int i;
    double r[20];
    double req = 0.0;

    printf("\n>> Series / Parallel resistor calculator\n");

    n = input_read_int_range("How many resistors? (1-20): ", 1, 20);

    for (i = 0; i < n; i++) {
        char prompt[64];
        snprintf(prompt, sizeof(prompt), "R%d (ohms, allow 4.7k, 2M, 10): ", i + 1);
        r[i] = input_read_double_min(prompt, 0.0);
    }

    printf("\n1) Series\n2) Parallel\n");
    mode = input_read_int_range("Choose connection (1-2): ", 1, 2);

    if (mode == 1) {
        req = 0.0;
        for (i = 0; i < n; i++) req += r[i];
        printf("\nEquivalent resistance (series): %.6g ohms\n", req);
        history_append_series_parallel("Series", r, n, req);
    } else {
        double inv = 0.0;
        int any_zero = 0;
        for (i = 0; i < n; i++) {
            if (r[i] == 0.0) { any_zero = 1; break; }
            inv += 1.0 / r[i];
        }
        req = any_zero ? 0.0 : (inv == 0.0 ? 0.0 : (1.0 / inv));

        printf("\nEquivalent resistance (parallel): %.6g ohms\n", req);
        history_append_series_parallel("Parallel", r, n, req);
    }
}

/* Menu Item 3: Ohm's Law */
void menu_item_3(void)
{
    int choice;
    double V = 0.0, I = 0.0, R = 0.0;

    printf("\n>> Ohm's law calculator (V = I * R)\n");
    printf("1) Calculate Voltage (V)\n");
    printf("2) Calculate Current (I)\n");
    printf("3) Calculate Resistance (R)\n");
    choice = input_read_int_range("Choose (1-3): ", 1, 3);

    if (choice == 1) {
        I = input_read_double("Enter current I (A, allow m for milli e.g. 20m): ");
        R = input_read_double_min("Enter resistance R (ohms): ", 0.0);
        V = I * R;
        printf("\nV = %.6g V\n", V);
        history_append_ohms("Solve V", V, I, R);
    } else if (choice == 2) {
        V = input_read_double("Enter voltage V (V): ");
        R = input_read_double_min("Enter resistance R (ohms, must be > 0): ", 1e-12);
        I = V / R;
        printf("\nI = %.6g A\n", I);
        history_append_ohms("Solve I", V, I, R);
    } else {
        V = input_read_double("Enter voltage V (V): ");
        I = input_read_double_min("Enter current I (A, must be > 0): ", 1e-12);
        R = V / I;
        printf("\nR = %.6g ohms\n", R);
        history_append_ohms("Solve R", V, I, R);
    }
}

/* Menu Item 4: Viewing data history */
void menu_item_4(void)
{
    printf("\n>> View history (history.txt)\n\n");
    history_view();
}
