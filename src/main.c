#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define MAX_SPEED 5
#define MIN_SPEED 1

void initField(char field[HEIGHT][WIDTH]);
int setSpeed(int *speed);
void analysis(char field[HEIGHT][WIDTH]);
void printField(char field[HEIGHT][WIDTH], int speed, int generation);
void clearScreen();
void randomFill(char field[HEIGHT][WIDTH], int density);
int fileFill(char field[HEIGHT][WIDTH]);

int main() {
  int speed, generation = 0;
  char field[HEIGHT][WIDTH];

  printf("Hey, wanna game for a while?\n \
    Okay, then choose input:\n \
    1 - random fill\n \
    2 - read from file\n");

  int choice;
  scanf("%d", &choice);

  switch (choice) {
  case 1:
    printf("enter alives count sells(1-100): ");
    int density;
    scanf("%d", &density);
    randomFill(field, density);
    break;
  case 2:
    if (fileFill(field)) {
      return 1;
    }
    break;
  default:
    printf("invalid choice. Bye...\n");
    return 1;
  }

  if (setSpeed(&speed)) {
    printf("enter speed from %d to %d\n", MIN_SPEED, MAX_SPEED);
    return 1;
  }

  clearScreen();
  while (1) {
    analysis(field);
    printField(field, speed, ++generation);
  }

  return 0;
}

void clearScreen() { printf("\033[2J\033[H"); }

void initField(char field[HEIGHT][WIDTH]) {
  printf("\033[H\033[J");
  char c;
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      c = (char)fgetc(stdin);
      if (c == '\n') {
        j--;
        continue;
      } else if (c == EOF) {
        field[i][j] = '.';
        continue;
      }
      field[i][j] = (c == '*') ? '*' : '.';
    }
  }
  freopen("/dev/tty", "r", stdin);
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
  printf("say filename: ");
  char filename[256];
  scanf("%s", filename);
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("error open file. bye...\n");
    return 1;
  }
  char c;
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      c = (char)fgetc(file);
      if (c == '\n' || c == EOF) {
        field[i][j] = '.';
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
  if (scanf("%d%c", speed, &test) != 2 || test != '\n') {
    return 1;
  }
  if (*speed < MIN_SPEED || *speed > MAX_SPEED) {
    return 1;
  }
  *speed = 1000000 / (*speed * 2);
  return 0;
}

void printField(char field[HEIGHT][WIDTH], int speed, int generation) {
  printf("\033[H");
  printf("..............................THE...GAME...OF...LIFE................."
         "............\n");
  printf("Alive: %d | Speed: %d | Ctrl+C to exit\n\n", generation,
         1000000 / speed / 2);

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      printf("%c", field[i][j]);
    }
    printf("\n");
  }
  printf("\033[J");
  usleep(speed);
}

void analysis(char field[HEIGHT][WIDTH]) {
  char field_new[HEIGHT][WIDTH];
  for (int x = 0; x < HEIGHT; x++) {
    for (int y = 0; y < WIDTH; y++) {
      int neighbours = 0;

      for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
          if (i == 0 && j == 0)
            continue;

          int nx = (x + i + HEIGHT) % HEIGHT;
          int ny = (y + j + WIDTH) % WIDTH;

          if (field[nx][ny] == '*') {
            neighbours++;
          }
        }
      }

      if (field[x][y] == '*') {
        field_new[x][y] = (neighbours == 2 || neighbours == 3) ? '*' : '.';
      } else {
        field_new[x][y] = (neighbours == 3) ? '*' : '.';
      }
    }
  }

  for (int x = 0; x < HEIGHT; x++) {
    for (int y = 0; y < WIDTH; y++) {
      field[x][y] = field_new[x][y];
    }
  }
}