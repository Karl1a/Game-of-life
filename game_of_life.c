#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // for usleep()
#include <time.h>  // for correct random
#include <sys/types.h>  // for kbhit function

void field_print(char **a, int m, int n);
void death_check(int i, int j, char **a, int neighboors_count);
void birth_check(int i, int j, char **a, int neighboors_count);
int neighboors_count(int rows, int cols, int i, int j, char **a);
void output_rules(char *button);
void game_mode(char **a, int m, int n, char button, int *game_end);
void random_field(char **a, int m, int n);
void key_pressed(int *delay, char button, int *game_end, char **a, int m, int n);
char _kbhit();

int main() {
    system("stty -icanon");  // input without 'enter' key
    int N = 80, M = 25;
    char button;
    int delay = 70000, game_end = 0;
    char **a = (char **) calloc (M, sizeof(char*));
    for (int i = 0; i < M; i++) {
        a[i] = (char *) calloc (N, sizeof(char));
    }
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            a[i][j] = ' ';
        }
    }
    printf("\e[1;1H\e[2J");
    output_rules(&button);
    game_mode(a, M, N, button, &game_end);
    while (1) {
        printf("\e[1;1H\e[2J");
        usleep(delay);
        field_print(a, M, N);
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (a[i][j] == '*') {
                    death_check(i, j, a, neighboors_count(M, N, i, j, a));
                }
                if (a[i][j] == ' ') {
                    birth_check(i, j, a, neighboors_count(M, N, i, j, a));
                }
            }
        }
        button = _kbhit();
        key_pressed(&delay, button, &game_end, a, M, N);
        if (game_end) {
            for (int i = 0; i < M; i++) {
                free(a[i]);
            }
            free(a);
            printf("\e[1;1H\e[2J");
            break;
        }
    }
    return 0;
}
void field_print(char **a, int m, int n) {  // outputs game display
    for (int y = -1; y < m + 1 ; y++) {
        for (int x = -1; x < n + 1; x++) {
            if (x == -1 || y== -1 || y== 25 || x == 80) {
                printf("\033[35m/\033[0m");
            } else if (a[y][x] == '.') {
                a[y][x] = ' ';
                printf("%c", a[y][x]);
            } else if (a[y][x] == '^') {
                a[y][x] = '*';
                printf("%c", a[y][x]);
            } else {
                printf("%c", a[y][x]);
            }
        }
        printf("\n");
    }
}
void game_mode(char **a, int m, int n, char button, int *game_end) {  // choosing gamemode depending on input
    FILE *fPtr;
    int i, j;
    if (button != 'q' && button != '\033') {
        for (int k = 0; k < m; k++) {
            for (int p = 0; p < n; p++) {
                a[k][p] = ' ';
            }
        }
    }
    switch (button) {
        case '1':
            fPtr = fopen("GUNS.txt", "r");
            while (fscanf(fPtr, "%d %d", &i, &j) != EOF) {
                a[i][j] = '*';
            }
            fclose(fPtr);
            break;
        case '2':
            fPtr = fopen("SPACESHIPS.txt", "r");
            while (fscanf(fPtr, "%d %d", &i, &j) != EOF) {
                a[i][j] = '*';
            }
            fclose(fPtr);
            break;
        case '3':
            fPtr = fopen("GUINEAPIGS.txt", "r");
            while (fscanf(fPtr, "%d %d", &i, &j) != EOF) {
                a[i][j] = '*';
            }
            fclose(fPtr);
            break;
        case '4':
            fPtr = fopen("PULSAR.txt", "r");
            while (fscanf(fPtr, "%d %d", &i, &j) != EOF) {
                a[i][j] = '*';
            }
            fclose(fPtr);
            break;
        case '5':
            fPtr = fopen("SOMETHING.txt", "r");
            while (fscanf(fPtr, "%d %d", &i, &j) != EOF) {
                a[i][j] = '*';
            }
            fclose(fPtr);
            break;
        case '6':
            random_field(a, m, n);
            break;
        case 'q':
            *game_end = 1;
    }
}
void random_field(char **a, int m, int n) {  // random display generation
    srand(time(NULL));
    int random;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            random = rand() % 3;
            if (random % 2 == 1) {
                a[i][j] = '*';
            } else {
                a[i][j] = ' ';
            }
        }
    }
}
int neighboors_count(int rows, int cols, int i, int j, char **a) {  // checking neighboor cells
    int count = 0;  // returns number of living cells
    int ind_x, ind_y;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            ind_y = i - 1 + r;
            ind_x = j - 1 + c;
            if (ind_y >= rows) {
                ind_y = 0;
            }
            if (ind_y < 0) {
                ind_y = 24;
            }
            if (ind_x >= cols) {
                ind_x = 0;
            }
            if (ind_x < 0) {
                ind_x = 79;
            }
            if (a[ind_y][ind_x] == '*' || a[ind_y][ind_x] == '.') {
                count += 1;
            }
        }
    }
    if (a[i][j] == '*') {
        count -= 1;
    }
    return count;
}
void death_check(int i, int j, char **a, int neighboors_count) {  // checking if we need to destroy a cell
    if (neighboors_count > 3 || neighboors_count < 2) {
        a[i][j] = '.';
    }
}
void birth_check(int i, int j, char **a, int neighboors_count) {  // checking we need to create a cell
    if (neighboors_count == 3) {
        a[i][j] = '^';
    }
}
void output_rules(char *button) {  // game menu output
    char enter;
    printf("Welcome to the Game of Life!\n\n");
    printf("Rules are the following:\n\n");
    printf("Adjust speed: \'+\' and \'-\'\n");
    printf("Open/close menu: \'m\'\n");
    printf("Exit: \'q\'\n\n");
    printf("Choose starting seed:\n\n");  // rules and instructions
    printf("1 - GUN\n");
    printf("2 - SPACESHIPS\n");
    printf("3 - GUINEA PIGS\n");
    printf("4 - PULSAR\n");
    printf("5 - LOCOMOTIVE TRAIN\n");
    printf("6 - RANDOM\n\n");
    printf("Waiting for input: ");
    do {
        scanf("%c%c", button, &enter);
        if (*button != '1' && *button != '2' && *button != '3' && *button !=  // checking pressed button
        '4' && *button != '5' && *button != '6' && *button != 'q' && *button != '\033') {
            printf("Invalid input. Try again: ");
        }
    } while (*button != '1' && *button != '2' && *button != '3' && *button !=
    '4' && *button != '5' && *button != '6' && *button != 'q' && *button != '\033');
}
void key_pressed(int *delay, char button, int *game_end, char **a, int m, int n) {  // operates pressed button
    switch (button) {  // checking value of pressed button
        case '=': {
            if (*delay >= 20000) {
            *delay -= 10000;
            }
            break;
        }
        case '-': {
            *delay += 10000;
            break;
        }
        case 'q':
            *game_end = 1;
            break;
        case '\033': {
            printf("\e[1;1H\e[2J");
            output_rules(&button);
            game_mode(a, m, n, button, game_end);
        }
        default: break;
    }
}
char _kbhit() {  // checks if some button is pressed on keyboard
    struct timeval tv;  // structure made for time tracking
    fd_set readfds;  // initialising readfds of fd_set data type (file descriptors)
    tv.tv_sec = 0;  // setting timer values to 0
    tv.tv_usec = 0;
    FD_ZERO(&readfds);  // initializes readfds to be empty
    FD_SET(0, &readfds);
    select(1, &readfds, NULL, NULL, &tv);
    if (FD_ISSET(0, &readfds)) {  // FD_ISSET returns nonzero, if first arg is a member of file descriptor
        return getchar();
    }
    return 0;
}
