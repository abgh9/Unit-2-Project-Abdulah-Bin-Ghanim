//Main Menu 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funcs.h"

/*Prototypes */
static void main_menu(void);
static void print_main_menu(void);
static int  get_user_input(void);
static void select_menu_item(int input);
static void go_back_to_main(void);
static int  is_integer(const char *s);

int main(void)
{
    for (;;) {
        main_menu();
    }
    return 0;
}

static void main_menu(void)
{
    print_main_menu();
    {
        int input = get_user_input();
        select_menu_item(input);
    }
}

static int get_user_input(void)
{
    enum { MENU_ITEMS = 5 }; /* 1 to 4 = items, 5 = Exit */
    char buf[128];
    int valid_input = 0;
    int value = 0;

    do {
        printf("\nSelect item: ");
        if (!fgets(buf, sizeof(buf), stdin)) {
            puts("\nInput error. Exiting.");
            exit(1);
        }

        buf[strcspn(buf, "\r\n")] = '\0';

        if (!is_integer(buf)) {
            printf("Enter an integer!\n");
            valid_input = 0;
        } else {
            value = (int)strtol(buf, NULL, 10);
            if (value >= 1 && value <= MENU_ITEMS) {
                valid_input = 1;
            } else {
                printf("Invalid menu item!\n");
                valid_input = 0;
            }
        }
    } while (!valid_input);

    return value;
}

static void select_menu_item(int input)
{
    switch (input) {
        case 1:
            menu_item_1(); /* Resistor decoder */
            go_back_to_main();
            break;
        case 2:
            menu_item_2(); /* Series/Parallel calculator */
            go_back_to_main();
            break;
        case 3:
            menu_item_3(); /* Ohm's law calculator*/
            go_back_to_main();
            break;
        case 4:
            menu_item_4(); /* Viewing history */
            go_back_to_main();
            break;
        default:
            printf("Bye!\n");
            exit(0);
    }
}

static void print_main_menu(void)
{
    printf("\n----------- Electronics Helper -----------\n");
    printf("\n"
           "\t1. Resistor colour code decoder\n"
           "\t2. Series / Parallel resistor calculator\n"
           "\t3. Ohm's law calculator\n"
           "\t4. View history (history.txt)\n"
           "\t5. Exit\n");
    printf("-----------------------------------------\n");
}

static void go_back_to_main(void)
{
    char buf[64];
    do {
        printf("\nEnter 'b' or 'B' to go back to main menu: ");
        if (!fgets(buf, sizeof(buf), stdin)) {
            puts("\nInput error. Exiting.");
            exit(1);
        }
        buf[strcspn(buf, "\r\n")] = '\0';
    } while (!(buf[0] == 'b' || buf[0] == 'B') || buf[1] != '\0');
}


static int is_integer(const char *s)
{
    if (!s || !*s) return 0;
    if (*s == '+' || *s == '-') s++;
    if (!isdigit((unsigned char)*s)) return 0;

    while (*s) {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}
