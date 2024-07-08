#include <curses.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 10
#define COLS 5
#define MAX_INPUT_LENGTH 100

void displaySpreadsheet(char spreadsheet[ROWS][COLS][MAX_INPUT_LENGTH], int selectedRow, int selectedCol) {
    clear();
    for (int j = 0; j < COLS; j++) {
        mvprintw(0, (j + 1) * 10, "%c", 'A' + j);
    }
    for (int i = 0; i < ROWS; i++) {
        mvprintw((i + 1) * 2, 0, "%d", i + 1);
        for (int j = 0; j < COLS; j++) {
            if (i == selectedRow && j == selectedCol) {
                attron(A_REVERSE);
            }
            mvprintw((i + 1) * 2, (j + 1) * 10, "%s", spreadsheet[i][j]);
            if (i == selectedRow && j == selectedCol) {
                attroff(A_REVERSE);
            }
        }
    }
    refresh();
}

void inputCellValue(char spreadsheet[ROWS][COLS][MAX_INPUT_LENGTH], int row, int col) {
    char value[MAX_INPUT_LENGTH];
    echo();
    mvprintw((row + 1) * 2, (col + 1) * 10, "");
    getnstr(value, MAX_INPUT_LENGTH - 1);
    strcpy(spreadsheet[row][col], value);
    noecho();
}

int main() {
    char spreadsheet[ROWS][COLS][MAX_INPUT_LENGTH] = {{{0}}};
    int selectedRow = 0, selectedCol = 0;
    int ch;

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    mousemask(ALL_MOUSE_EVENTS, NULL);

    displaySpreadsheet(spreadsheet, selectedRow, selectedCol);

    while ((ch = getch()) != 'q') {
        MEVENT event;
        switch (ch) {
            case KEY_UP:
                if (selectedRow > 0) selectedRow--;
                break;
            case KEY_DOWN:
                if (selectedRow < ROWS - 1) selectedRow++;
                break;
            case KEY_LEFT:
                if (selectedCol > 0) selectedCol--;
                break;
            case KEY_RIGHT:
                if (selectedCol < COLS - 1) selectedCol++;
                break;
            case KEY_MOUSE:
                if (getmouse(&event) == OK) {
                    if (event.bstate & BUTTON1_PRESSED) {
                        int row = event.y / 2 - 1;
                        int col = event.x / 10 - 1;
                        if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
                            selectedRow = row;
                            selectedCol = col;
                        }
                    }
                }
                break;
            case '\n':
                inputCellValue(spreadsheet, selectedRow, selectedCol);
                break;
        }
        displaySpreadsheet(spreadsheet, selectedRow, selectedCol);
    }

    endwin();
    return 0;
}
