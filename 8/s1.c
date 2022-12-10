#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define L_VIS 0
#define R_VIS 1
#define T_VIS 2
#define B_VIS 3

void print_bits(unsigned int n){
    unsigned int i;

    i = 1U << (sizeof(n)-1);

    while (i > 0) {
      if (n & i)
        printf("1");
      else
        printf("0");

      i >>= 1;
    }
}

int main(int argc, char **argv) {
  FILE *fp = NULL;

  if (argc == 2) {
    fp = fopen(argv[1], "r");
  } else {
    printf("Missing data file!\n");
    return -1;
  }

  size_t linecap = 0;
  size_t llen = 0;

  char *line = NULL;

  int **grid = 0;
  char **vis = 0;

  int y = 0;
  int x = 0;
  
  while ((llen = getline(&line, &linecap, fp)) != -1) {
    x = llen - 1;
    grid = (int **)realloc(grid, (y + 1) * sizeof(*grid));
    grid[y] = (int *)malloc((llen - 1) * sizeof(int *));

    vis = (char **)realloc(vis, (y + 1) * sizeof(*vis));
    vis[y] = (char *)malloc((llen - 1) * sizeof(char *));
    memset(vis[y], 0x0, (llen-1)*sizeof(char*));

    for (int i = 0; i < llen - 1; i++) {
      grid[y][i] = line[i] - '0';
    }

    y++;
  }
  
  for (int i = 1; i < y - 1; i++) {
    for (int j = 1; j < x - 1; j++) {

      //LEFT
      int v = 1; 
      for (int l = j - 1; l >= 0; l--) {
        if (grid[i][j] <= grid[i][l])
          v = 0;
      }
      vis[i][j] |= v << L_VIS;

      //RIGHT
      v = 1;
      for (int l = j + 1; l < x; l++){
        if (grid[i][j] <= grid[i][l])
          v = 0;
      }
      vis[i][j] |= v << R_VIS;
        
    }
  }

  for (int j = 1; j < x - 1; j++) {
    for (int i = 1; i < y - 1; i++) {

      //TOP
      int v = 1; 
      for (int l = i - 1; l >= 0; l--) {
        if (grid[i][j] <= grid[l][j])
          v = 0;
      }
      vis[i][j] |= v << T_VIS;

      //BOTTOM
      v = 1;
      for (int l = i + 1; l < y; l++){
        if (grid[i][j] <= grid[l][j])
          v = 0;
      }
      vis[i][j] |= v << B_VIS;
        
    }
  }
        



  /* PRINT */
  printf("x %d y %x\n", x, y);
  for (int i = 0; i < y; i++) {
    for (int j = 0; j < x; j++) {
      printf("%d ", grid[i][j]);
    }
    printf("\n");
  }
  printf("===========\n");
  int ccount = 0;
  for (int i = 0; i < y; i++) {
    for (int j = 0; j < x; j++) {
      print_bits(vis[i][j]);
      printf(" ");
      if (vis[i][j] & 0xF)
        ccount++;
    }
    printf("\n");
  }

  ccount += (2*x) + (2*y) - 4;
  printf("CCOUNT: %d\n", ccount);
}
