#include "rbtree.h"

int main(int argc, char *argv[]) {
  rbtree *t = new_rbtree();

  rbtree_insert(t, 10);
  rbtree_insert(t, 5);
  rbtree_insert(t, 8);
  rbtree_insert(t, 34);
  rbtree_insert(t, 67);
  rbtree_insert(t, 23);
  rbtree_insert(t, 156);
  rbtree_insert(t, 24);
  rbtree_insert(t, 2);
  rbtree_insert(t, 12);
  rbtree_insert(t, 24);
  rbtree_insert(t, 36);
  rbtree_insert(t, 999);
  rbtree_insert(t, 25);

  node_t *a = rbtree_find(t, 25);
  node_t *b = rbtree_find(t, 1);

  rbtree_erase(t, a);
  

  delete_rbtree(t);

  return 0;
}