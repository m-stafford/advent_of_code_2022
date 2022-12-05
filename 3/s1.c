#include <stdio.h>
#include <stdlib.h>

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

int main() {
  FILE *fp = fopen("data.txt", "r");

  size_t lc = line_count(fp);
  size_t linecap = 0;
  size_t llen = 0;

  char *line = NULL; 

  int t_sum = 0;

  while ((llen = getline(&line, &linecap, fp)) != -1) {
    int sack_size = (llen/sizeof(char)) / 2;
    unsigned long long int dupes = 0x0;
    int d_ind = 0;


    for (int i = 0; i < sack_size; i++) {
      for (int j = 0; j < sack_size; j++) {
        if (line[i] == line[j + sack_size]) {
          if (line[j + sack_size] < 0x5b ) {
            dupes |= 1ULL << (line[j + sack_size] - 0x0027);
          } else {
            dupes |= 1ULL << (line[j + sack_size] - 0x0061);
          }
        }
      }
    }

    for (int i = 0; i < 52; i++) {
      if (dupes & 0x1) t_sum += (i+1);
      dupes = dupes >> 1;
    }
  }
  printf("T = %d\n", t_sum);

  return 0;
}
