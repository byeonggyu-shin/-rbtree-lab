#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *NIL = (node_t *)calloc(1, sizeof(node_t));
  // root 초기값 설정
  // black , parent nil , child nil => sentinel node
  NIL->color = RBTREE_BLACK; 
  p->nil = NIL;  
  p->root = NIL;
  return p;
}

void delete_rbtree_nodes(rbtree *t, node_t *x) {

  if (x != t->nil) {
    //DFS로 트리를 순회 
    delete_rbtree_nodes(t, x->left);
    delete_rbtree_nodes(t, x->right);
    //각 노드를 해제
    free(x);
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  node_t *x;
  x = t->root;

  delete_rbtree_nodes(t, x);
  free(t->nil);
  free(t);
}

void rotate_left(rbtree *t, node_t *x) {
  node_t *y;

  y = x->right;  // y 선언하고 x의 right 
  x->right = y->left;   // y의 left 서브트리 x의 right로 옮김
  // y의 왼쪽이 nil이 아닌경우  , 즉 x의 오른쪽 자식이 nil이 아닌경우 => 꺽여있는 경우
  if (y->left != t->nil){
    y->left->parent = x;  // y의 왼쪽 서브 트리 부모를 x로 설정 => 펴준다
  }
  y->parent = x->parent;  // y의 부모 노드를 x의 부모 노드로 설정
  // x의 부모노드가 nil이 이먄
  if (x->parent == t->nil){
    t->root = y;   // y는 루트
  } // x의 부모 노드가 왼쪽이면 
  else if (x == x->parent->left)
  {  // x의 부모 노드의 왼쪽 ,x 가 같으면 부모노드 왼쪽을 y값으로
    x->parent->left = y;
  } // x의 부모 노드의 오른쪽 ,x 가 같으면 부모노드 오른쪽을 y값으로
  else {
    x->parent->right = y;
  }
  y->left = x;          // x를 왼쪽으로 
  x->parent = y;        // y를 x의 부모로 설정
}

void rotate_right(rbtree *t, node_t *x) {
  node_t *y;

  y = x->left;
  x->left = y->right;

  if (y->right != t->nil){
    y->right->parent = x;
  }
  y->parent = x->parent;

  if (x->parent == t->nil){
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }
  y->right = x;
  x->parent = y;
}

// case 1 : 노드 z의 삼촌 y가 적색인 경우
// case 2 : 노드 z의 삼촌 y가 흑색이며 노드 z가 오른쪽 자식인 경우
// case 3 : 노드 z의 삼촌 y가 흑색이며 노드 z가 왼쪽 자식인 경우

void rbtree_insert_fixup(rbtree *t, node_t *z) {
  node_t *y;
  // 부모의 색이 레드일 경우 => 규칙 위반
  while (z->parent->color == RBTREE_RED){
    // z 부모가 조부모 왼쪽 서브 트리
    if (z->parent == z->parent->parent->left){
      y = z->parent->parent->right;  
      // 노드 z의 삼촌 y가 red
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;             // case 1
        y->color = RBTREE_BLACK;                     // case 1
        z->parent->parent->color = RBTREE_RED;       // case 1
        z = z->parent->parent;                       // case 1
      }
      else{          // z의 삼촌 y가 black , z가 오른쪽 자식
        if (z == z->parent->right){
          z = z->parent;                              // case 2
          rotate_left(t, z);                          // case 2
        }
      // z 삼촌 y black, z가 오른쪽 자식
      z->parent->color = RBTREE_BLACK;                // case 3
      z->parent->parent->color = RBTREE_RED;          // case 3
      rotate_right(t ,z->parent->parent);             // case 3
      }
    } // z의 부모가 조부모 오른쪽 서브트리   <=  반대로 하면됨 
    else {
      y = z->parent->parent->left;
      // z 삼촌 y가 red 인 경우    <-  case 4
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // z 삼촌 y가 black , z가 오른쪽  <- case 5
      else {
        if (z == z->parent->left) {
          z = z->parent;
          rotate_right(t, z);
        }
      // z의 삼촌 y가 black , z가 오른쪽 자식   <-  case 6
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      rotate_left(t ,z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  // y  x루트의 주소 사이즈 
  node_t *y = t->nil;
  node_t *x = t->root;
  // 노드 크기 만큼의 동적 메모리 할당
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  // 입력받은 key 값 할당
  z->key= key;
  
  while (x != t->nil) {
      y = x; // y => nil 주소도 업데이트
      // 크면 왼쪽으로 한 칸 내려감
      if (z->key < x->key) {
          x = x->left;
      }
      // 작으면 오른쪽으로 한칸 내려감
      else {
          x = x->right;
      }
  };
  // nil까지 내려가서 z의 부모값으로 y 할당
  z->parent = y;

  if (y == t->nil){  // x가 값이 없어서 while문 패스시 , 
    t->root = z;  // root 주소에 할당
  }  
  else if (z->key < y->key){   //입력값 z가 부모값 y보다 크면
    y->left = z;   // 왼쪽 
  }
  else {
    y->right = z;  //오른쪽
  };

  // 삽입된 z  child nil , color red 
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  // rbtree 특성 위반 체크 
  rbtree_insert_fixup(t, z);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *x;
  
  x = t->root;
  while (x != t->nil) {
     if (key == x->key) {
        return x;
    } else if (key < x->key) {
        x = x->left;
    } else if (key > x->key) {
        x = x->right;
    } 
  }
  return NULL;
}


node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  if (t->root == t->nil){
    return NULL;
  }
  
  node_t *x;

  x = t->root;
  while (x->left != t->nil) {
      x = x->left;
  }  
  return x; 
  // return t->root; 
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  if (t->root == t->nil){
    return NULL;
  }

  node_t *x;

  x = t->root;
  while (x->right != t->nil) {
      x = x->right;
  }  
  return x; 

  // return t->root;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  if(u->parent == t->nil){
    t->root = v;
  }
  else if (u == u->parent->left){
    u->parent->left = v;
  }
  else{
    u->parent->right = v;
  }

  v->parent = u->parent;
}

// 삭제시 규칙 위반 하는 경우
// case 1: x의 형제 w가 적색
// case 2: x의 형제 w는 흑색이고 w의 두 자식이 모두 흑색
// case 3: x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자식은 흑색
// case 4: x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색

void rbtree_delete_fixup(rbtree *t, node_t *x){
   while (x != t->root && x->color == RBTREE_BLACK) {
    // CASE 1 ~ 4 : LEFT CASE
    if (x == x->parent->left){
      node_t *w = x->parent->right;
      
      // CASE 1 : x의 형제 w가 적색인 경우
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rotate_left(t, x->parent);
        w = x->parent->right;
      }

      // CASE 2 : x의 형제 w는 흑색이고 w의 두 지식이 모두 흑색인 경우
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }

      // CASE 3 : x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
      else{ 
        if (w->right->color == RBTREE_BLACK) {
            w->left->color = RBTREE_BLACK;
            w->color = RBTREE_RED;
            rotate_right(t, w);
            w = x->parent->right;
        }

        // CASE 4 : x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색인 경우
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        rotate_left(t, x->parent);
        x = t->root;
      }
    }
    // CASE 5 ~ 8 : RIGHT CASE
    else {
      node_t *w = x->parent->left;

      // CASE 5 : x의 형제 w가 적색인 경우
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rotate_right(t, x->parent);
        w = x->parent->left;
      }

      // CASE 6 : x의 형제 w는 흑색이고 w의 두 지식이 모두 흑색인 경우
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }

      // CASE 7 : x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
      else 
      {
          if (w->left->color == RBTREE_BLACK) {
            w->right->color = RBTREE_BLACK;
            w->color = RBTREE_RED;
            rotate_left(t, w);
            w = x->parent->left;
            }

        // CASE 8 : x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색인 경우
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        rotate_right(t, x->parent);
        x = t->root;
      }
    }
  }

  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y;
  node_t *x;
  color_t y_original_color;

  y = p;
  y_original_color = y->color;

  if(p->left == t->nil){
    x = p->right;
    rbtree_transplant(t, p, p->right);
  }
  else if (p->right == t->nil){
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else{

    y = p->right;
    while(y->left != t->nil){
      y = y->left;
    }
    // y = rbtree_min((const rbtree *)p->right);
    y_original_color = y->color;
    x = y->right;


    if (y->parent == p){
      x->parent = y;
    }
    else{
      rbtree_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if (y_original_color == RBTREE_BLACK){
    rbtree_delete_fixup(t, x);
  }

  free(p);
  return 0;
}

void inorder_traversal(const node_t *x, key_t *arr, size_t *idx, const rbtree *t) {
  if (x != t->nil) {
    inorder_traversal(x->left, arr, idx, t);
    arr[*idx] = x->key;
    (*idx)++;
    inorder_traversal(x->right, arr, idx, t);
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  size_t idx = 0;
  inorder_traversal(t->root, arr, &idx, t);
  return 0;
}


