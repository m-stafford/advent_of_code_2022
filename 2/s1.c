#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


size_t line_count(FILE *fp) {
  char *line = NULL; 
  size_t linecap = 0;
  size_t lines = 0;

  while (getline(&line, &linecap, fp) != -1) {
    lines++;
  }
  rewind(fp);
  return lines;
}

#define ROCK 1
#define PAPER 2
#define SCISSORS 3
#define WIN 6
#define LOSE 0
#define DRAW 3

int main() {
  FILE *fp = fopen("data.txt", "r");
  size_t lc = line_count(fp);

  char *data = (char *)malloc(sizeof(char) * 10 * lc);
  char *line = NULL; 
  size_t linecap = 0;

  short int total_score = 0;

  while (getline(&line, &linecap, fp) != -1) {
    char opp = line[0];
    char self = line[2];
    short int score = 0;
    printf("OPP = %c, SELF = %c", opp,self);

    if ((opp == 'A') && (self == 'X')) score = DRAW + ROCK;
    else if ((opp == 'A') && (self == 'Y')) score = WIN + PAPER;
    else if ((opp == 'A') && (self == 'Z')) score = LOSE + SCISSORS;
    else if ((opp == 'B') && (self == 'X')) score = LOSE + ROCK;
    else if ((opp == 'B') && (self == 'Y')) score = DRAW + PAPER;
    else if ((opp == 'B') && (self == 'Z')) score = WIN + SCISSORS;
    else if ((opp == 'C') && (self == 'X')) score = WIN + ROCK;
    else if ((opp == 'C') && (self == 'Y')) score = LOSE + PAPER;
    else if ((opp == 'C') && (self == 'Z')) score = DRAW + SCISSORS;

    printf("MATCH SCORE = %d\n", score);
    total_score += score;
    
  }

  printf("TOTAL SCORE = %d\n", total_score);
  return 0;
}
