#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main() {
  FILE *fp = fopen("data.txt", "r");

  int data[1000];
  char *line = NULL; 
  ssize_t linecap = 0;

  int curr_sum = 0;
  int c = 0;

  memset(data, 0, sizeof(data));

  while (getline(&line, &linecap, fp) != -1) {

    char clean_line[20];

    int is_num = 1;


    for (int i = 0; i < strlen(line) - 1; i++) {
      if (!isdigit(line[i])) is_num = 0;
    }

    if (strlen(line)-1 <= 0) is_num = 0;

    if (is_num) {
      int val = atoi(line);
      curr_sum += val;
    } else {
      data[c] = curr_sum;
      curr_sum = 0;
      c++;
    }
  }

  int top_val_ind[3] = {-1, -1, -1};
  int top_val[3] = {0, 0, 0};
  int max_val = 0;
  int total_sum = 0;

  for (int i = 0; i < sizeof(top_val_ind)/sizeof(int); i++) {
    for (int j = 0; j < sizeof(data)/sizeof(int); j++) {
      if (data[j] > max_val) {

        int prev_max = 0;
        for (int k = 0; k < sizeof(top_val_ind)/sizeof(int); k++) {
          if (j == top_val_ind[k]) prev_max = 1;
        }

        if (!prev_max) {
          max_val = data[j];
          top_val_ind[i] = j;
        }
      }
    }
    top_val[i] = max_val;
    printf("%d, %d\n", i, max_val);
    max_val = 0;
    total_sum += top_val[i];
  }
  printf("%d\n", total_sum);
  return 0;
}
