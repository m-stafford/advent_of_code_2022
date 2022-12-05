#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0


void print_bits(unsigned long long n){
  unsigned long long i; 

  i = 1ULL << (sizeof(n)*8-1);

  while (i > 0) {
    if (n & i)
      printf("1"); 
    else 
      printf("0"); 

    i >>= 1;
  }
}


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
  char *brkt;

  int ss_cnt = 0;


  while ((llen = getline(&line, &linecap, fp)) != -1) {
    unsigned long long elf_one[2] = {0ULL, 0ULL};
    unsigned long long elf_two[2] = {0ULL, 0ULL};

    char *e_cov = strtok_r(line, ",", &brkt);
    int bounds[4];
    int arr_sep = 0;

    while (e_cov != NULL) {
      char *a_bounds = strtok(e_cov, "-");
      for (int i = 0 + arr_sep; a_bounds; i++) {
        bounds[i] = atoi(a_bounds);
        a_bounds = strtok(NULL, "-");
      }
      arr_sep = 2;
      e_cov = strtok_r(NULL, ",", &brkt);
    }

    printf("START\n");
    for (int i = bounds[0]; i <= bounds[1]; i++) {
      if (i < 64) elf_one[0] |= 1ULL << i;
      else elf_one[1] |= 1ULL << i;
    }

    for (int i = bounds[2]; i <= bounds[3]; i++) {
      if (i < 64) elf_two[0] |= 1ULL << i;
      else elf_two[1] |= 1ULL << i;
    }

    printf("  ");
    print_bits(elf_one[1]);
    print_bits(elf_one[0]);
    printf("\n");
    printf("  ");
    print_bits(elf_two[1]);
    print_bits(elf_two[0]);
    printf("\n");

    if (((elf_one[0] & elf_two[0]) == elf_one[0] && (elf_one[1] & elf_two[1]) == elf_one[1]) 
      || ((elf_one[0] & elf_two[0]) == elf_two[0] && (elf_one[1] & elf_two[1]) == elf_two[1])) {
        printf("  *********************SUPERSET\n");
        ss_cnt++;
    }



  }
  printf("SS = %d\n", ss_cnt);

  return 0;
}
