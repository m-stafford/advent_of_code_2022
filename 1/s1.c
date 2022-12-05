#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main() {
  FILE *fp = fopen("data.txt", "r");

  char *data = (char *)malloc(sizeof(char) * 20 * 2264);
  char *line = NULL; 
  ssize_t linecap = 0;

  int max_sum = 0;
  int curr_sum = 0;

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
      if (curr_sum > max_sum) max_sum = curr_sum;
      curr_sum = 0;
    }

    
  }

  printf("%d\n", max_sum);
  return 0;
}
