#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define T_DIR 0x0
#define T_FILE 0x1

typedef struct node node;

struct node {
  char type;
  size_t file_size;
  char name[16];

  int c_count;
  node *parent;
  node *children; 
};

node *init_node(char type, size_t file_size, char *name) {
  node *n_node = (node *)malloc(sizeof(node));

  n_node->type = type;
  n_node->file_size = file_size;
  n_node->c_count = 0;

  n_node->parent = NULL;
  n_node->children = NULL;

  strcpy(n_node->name, name);

  return n_node;
}

int add_child(node **tree, node* leaf) {
  if ((*tree) != NULL) {
    int c_count = (*tree)->c_count;

    if (((*tree)->children = realloc((*tree)->children, sizeof(node)))) {
      node *ptr;
      leaf->parent = (*tree);
      (*tree)->children[c_count] = *leaf;
      (*tree)->file_size += (*tree)->children[c_count].file_size;
      (*tree)->c_count++;
      if ((*tree)->parent != NULL) {
        ptr = (*tree)->parent;
        while(ptr->parent != NULL) {
          ptr->file_size += (*tree)->children[c_count].file_size;
          ptr = ptr->parent;
        }
      }
    } else {
      printf("ERROR: %s\n", strerror(errno));
      return -1;
    }
  } else {
    (*tree) = leaf;
  }

  return 0;
}

int del_child(node **tree, node** leaf) {
  if ((*tree) != NULL && (*leaf) != NULL) {
    node *tmp = malloc(((*tree)->c_count-1) * sizeof(node));

    for (int i = 0; i < (*tree)->c_count; i++)
      if (*leaf != &((*tree)->children[i]))
        tmp[i] = (*tree)->children[i];

    (*leaf)->children = tmp;
    free((*leaf)->children);

    (*leaf)->c_count--;
  }
  return 0;

}

void print_tree(node *tree, int level) {
  for (int i = 0; i < level; i++)
    printf("  ");

  printf("%s %lu\n", tree->name, tree->file_size);
  for (int i = 0; i < tree->c_count; i++) 
    print_tree(&(tree->children[i]), level + 1);
}

void find_dir(node *tree, size_t max_size, size_t *sum) {
  if (tree->file_size <= max_size && tree->type == T_DIR)
    *sum += tree->file_size;

  for (int i = 0; i < tree->c_count; i++) 
    find_dir(&(tree->children[i]), max_size, sum);
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
  char *col;
  char *brkt;

  node *root = NULL;
  node *level;
  
  while ((llen = getline(&line, &linecap, fp)) != -1) {
    char *buf[3];
    int cnt = 0;

    col = strtok_r(line, " ", &brkt);
    while (col != NULL) {
      if (col[strlen(col)-1] == '\n') {
        buf[cnt] = strndup(col, strlen(col)-1);
      } else
        buf[cnt] = strdup(col);

      cnt++;
      col = strtok_r(NULL, " ", &brkt);
    }

    if (strcmp(buf[0], "$") == 0) {
      if (strcmp(buf[1], "cd") == 0) { 
        if (strcmp(buf[2], "..") == 0) {
          level = level->parent;
        } else if (strcmp(buf[2], "/") == 0) {
          root = init_node(T_DIR, 0, "/");
          level = root;
        } else {
          for(int i = 0; i < level->c_count; i++) {
            if (level->children[i].type == T_DIR) {
              if (strcmp(level->children[i].name, buf[2]) == 0) {
                level = &(level->children[i]);
                break; 
              }
            }
          }
        }
      }
    } else if (strcmp(buf[0], "ls") != 0) {
      node *child;

      if (strcmp(buf[0], "dir") == 0) {
        child = init_node(T_DIR, 0, buf[1]);
      } else {
        child = init_node(T_FILE, atoi(buf[0]), buf[1]);
      }
      add_child(&level, child);
    }
  }
  printf("Tree: \n");
  print_tree(root, 0);

  size_t s = 0;
  find_dir(root, 100000, &s);
  printf("size: %lu\n", s);

  return 0;

}
