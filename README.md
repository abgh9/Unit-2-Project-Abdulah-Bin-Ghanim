### 1 Electronics Helper Program
Overview:
This project is a menu‑driven C application designed to help with common basic electronics calculations and view the results and store it in persistant history.


### 2 Features
Resistor Colour Code Decoder (4‑band and 5‑band resistors)

Series and Parallel Resistor Calculator

Ohm’s Law Calculator (V, I, R)

Calculation History saved to a text file

Input Handling (handles invalid and non‑numeric input)


### 3 How the Program Works
1.The program starts in main.c and displays a menu.

2.User input is validated using helper functions from input.c.

3.Each menu option is routed through funcs.c to the correct module.

4.Calculations are performed in their respective source files.

5.Results are displayed to the user and appended to history.txt.

6.The user can view saved history at any time from the menu.


### 4 Input Handling
-All user input is read using fgets to avoid buffer issues.

-Numeric input supports engineering suffixes:

k (kilo = *10^3)

M (Mega= *10^6)

m (Milli= *10^-3)

u (Micro= *10^-6)

-When invalid input is detected and the user is re‑prompted.


### 5 History File (File Input/Output)
-All completed calculations are appended to history.txt.

-Each entry is written on a new line.

-If the history file does not exist, the program displays a clear message.

### 6 Running
Compile the program using the Makefile:
make
Run the program:
./main.out


Author,
Abdullah Bin Ghanim201860162