#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// field size
#define WIDTH 80
#define HEIGHT 25

#define MAX_SPEED 5
#define MIN_SPEED 1

#define DELAY 1000000
#define SPEED_FACTOR 2

// ANSI magic
#define RAINBOW_COLORS 7
const char *rainbow[] = {
    "\033[38;5;196m", // red
    "\033[38;5;202m", // orange
    "\033[38;5;226m", // yellow
    "\033[38;5;46m",  // green
    "\033[38;5;21m",  // blue
    "\033[38;5;93m",  // violet
    "\033[38;5;201m", // pink
    "\033[38;5;236m"  // grey
};

void clearScreen() { system("clear || cls"); }
int input(int *speed, char field[HEIGHT][WIDTH]);
int setSpeed(int *speed);
int fileFill(char field[HEIGHT][WIDTH]);
void randomFill(char field[HEIGHT][WIDTH], int density);
void updateField(char field[HEIGHT][WIDTH]);
void printField(char field[HEIGHT][WIDTH], int speed, int generation);

int main() {
  int speed, generation = 0;
  char field[HEIGHT][WIDTH];

  printf("Hey, wanna game for a while?\n \
    Okay, then choose input way:\n \
    1 - random fill\n \
    2 - read from file\n");

  if (input(&speed, field))
    return 1;

  while (1) {
    clearScreen();
    updateField(field);
    printField(field, speed, ++generation);
  }
  return 0;
}

int input(int *speed, char field[HEIGHT][WIDTH]) {
  int choice;
  scanf("%d", &choice);

  switch (choice) {
  case 1: // random
    printf("Enter alives count sells (1-100): ");
    int density;
    scanf("%d", &density);
    randomFill(field, density);
    break;
  case 2: // file
    if (fileFill(field)) {
      return 1;
    }
    break;
  default:
    printf("invalid choice. Bye...\n");
    return 1;
  }
  if (setSpeed(speed)) {
    printf("Enter speed from %d to %d\n", MIN_SPEED, MAX_SPEED);
    return 1;
  }
  return 0;
}

void randomFill(char field[HEIGHT][WIDTH], int density) {
  srand(time(NULL));
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      field[i][j] = (rand() % 100 < density) ? '*' : '.';
    }
  }
}

int fileFill(char field[HEIGHT][WIDTH]) {
  printf("Say me filepath: ");
  char filename[256];
  scanf("%s", filename);

  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("Error open file. Bye...\n");
    return 1;
  }

  char c;
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      c = (char)fgetc(file);

      if (c == '\n' || c == EOF) {
        field[i][j] = '.';
        // if the file ends earlier, the array will be filled with dots
        if (c == EOF && i < HEIGHT - 1) {
          for (int k = i + 1; k < HEIGHT; k++) {
            for (int l = 0; l < WIDTH; l++) {
              field[k][l] = '.';
            }
          }
          fclose(file);
          return 0;
        }
        j--;
        continue;
      }
      field[i][j] = (c == '*') ? '*' : '.';
    }
  }
  fclose(file);
  return 0;
}

int setSpeed(int *speed) {
  printf("Speed (%d-%d): ", MIN_SPEED, MAX_SPEED);
  char test;
  if (scanf("%d%c", speed, &test) != 2 || test != '\n')
    return 1;
  if (*speed < MIN_SPEED || *speed > MAX_SPEED)
    return 1;
  *speed = DELAY / (*speed * SPEED_FACTOR);
  return 0;
}

void printField(char field[HEIGHT][WIDTH], int speed, int generation) {
  printf("\033[H");
  printf(
      "%s..............................THE...GAME...OF...LIFE................."
      "............\n",
      rainbow[2]);
  printf("%sAlive: %d | Speed: %d | Ctrl+C to exit\n", rainbow[3], generation,
         DELAY / speed / SPEED_FACTOR);
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      int iColor = (i + j) % RAINBOW_COLORS;
      if (field[i][j] == '*')
        printf("%s•\033[0m", rainbow[iColor]);
      else
        printf("%s·\033[0m", rainbow[7]);
    }
    printf("\n");
  }
  usleep(speed);
}

void updateField(char field[HEIGHT][WIDTH]) {
  char fieldNew[HEIGHT][WIDTH];
  for (int x = 0; x < HEIGHT; x++) {
    for (int y = 0; y < WIDTH; y++) {
      int neighbours = 0;
      // Count live neighbors
      for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
          if (i == 0 && j == 0)
            continue;
          // Toroidal boundary conditions
          int nx = (x + i + HEIGHT) % HEIGHT;
          int ny = (y + j + WIDTH) % WIDTH;

          if (field[nx][ny] == '*') {
            neighbours++;
          }
        }
      }
      if (field[x][y] == '*') {
        fieldNew[x][y] = (neighbours == 2 || neighbours == 3) ? '*' : '.';
      } else {
        fieldNew[x][y] = (neighbours == 3) ? '*' : '.';
      }
    }
  }
  // Update main field with new state
  for (int x = 0; x < HEIGHT; x++) {
    for (int y = 0; y < WIDTH; y++) {
      field[x][y] = fieldNew[x][y];
    }
  }
}