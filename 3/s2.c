#include <stdio.h>
#include <stdlib.h>
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

int main() {
  FILE *fp = fopen("data.txt", "r");

  size_t lc = line_count(fp);
  size_t linecap = 0;
  size_t llen = 0;

  char *line = NULL; 

  int t_sum = 0;

  unsigned long long int dupes[3] = {0ULL, 0ULL, 0ULL};
  unsigned int g_ind = 0; 

  while ((llen = getline(&line, &linecap, fp)) != -1) {
    for (int i = 0; i < llen-1; i++) {
      if (line[i] < 0x5b ) {
        dupes[g_ind] |= 1ULL << (line[i] - 0x27ULL);
      } else {
        dupes[g_ind] |= 1ULL << (line[i] - 0x61ULL);
      }
    }

    if (g_ind == 2) {
      unsigned long long int all_dupes = dupes[0] & dupes[1] & dupes[2];
      for (int i = 0; i < 52; i++) {
        if (all_dupes & 0x1) t_sum += (i+1);
        all_dupes = all_dupes >> 1;
      }
      dupes[0] = 0ULL;
      dupes[1] = 0ULL;
      dupes[2] = 0ULL;

      g_ind = 0; 
    } else {
      g_ind++;
    }
  }
  printf("T = %d\n", t_sum);

  return 0;
}
