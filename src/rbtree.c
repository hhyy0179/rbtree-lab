#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  
  p->nil = calloc(1,sizeof(node_t));
  p->nil->color = RBTREE_BLACK;

  p->nil->parent = NULL;
  p->nil->left = NULL;
  p->nil->right = NULL;
  p->nil->key = 0;

  p->root = p->nil;

  return p;
}

void post_order_traverse(const rbtree* t, node_t *p) {

  if (p == t->nil) return;

  post_order_traverse(t, p->left);
  post_order_traverse(t, p->right);
  free(p);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  post_order_traverse(t, t->root);
  free(t->nil);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t* p_cur = t->nil;
  node_t* cur = t->root;

  node_t* new_node = (node_t *)malloc(sizeof(node_t));
  new_node->key = key;
  
  while(cur != t->nil)
  {
    p_cur = cur;
    if(new_node->key < cur->key)
      cur = cur->left;
    else cur = cur->right;
  }

  //부모 연결
  new_node->parent = p_cur; 
  //부모에서 자식 연결
  if(p_cur == t->nil) // 최초 삽입하는 경우
    t->root = new_node;
  else if (new_node->key < p_cur->key)
    p_cur->left = new_node;
  else p_cur->right = new_node;

  //초기화
  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->color = RBTREE_RED;

  //RB-Tree 속성을 만족시키기 위한 작업
  node_t* p_node = new_node->parent;
  while(p_node->color == RBTREE_RED)
  {
    //삽입노드가 오른쪽 서브트리에 있을 경우,
    if(p_node == p_node->parent->right)
    {
      //삼촌 노드 색을 확인한다.
      p_cur = p_node->parent->left;

      //[CASE 1]
      if(p_cur->color == RBTREE_RED)
      {
        p_cur->color = RBTREE_BLACK;
        p_node->color = RBTREE_BLACK;
        p_node->parent->color = RBTREE_RED;
      }
      else
      {
        //[CASE 2]
        //꺾인 상태일 경우 펴준다
        if(new_node == p_node->left)
        {
          right_rotation(t,p_node);
          new_node = p_node;
          p_node = new_node->parent;
        }
        //[CASE 3]
        //펴져 있는 경우
        p_node->color = RBTREE_BLACK;
        p_node->parent->color = RBTREE_RED;
        left_rotation(t,p_node->parent);
      }
    }

    //삽입노드가 왼쪽 서브트리에 있을 경우,
    else
    {
      //삼촌 노드 색을 확인한다.
      p_cur = p_node->parent->right;

      //[CASE 1]
      if(p_cur->color == RBTREE_RED)
      {
        p_cur->color = RBTREE_BLACK;
        p_node->color = RBTREE_BLACK;
        p_node->parent->color = RBTREE_RED;
      }
      else
      {
        //[CASE 2]
        //꺾인 상태일 경우
        if(new_node == p_node->right)
        {
          left_rotation(t,p_node);
          new_node = p_node;
          p_node = new_node->parent;
        }
        //[CASE 3]
        //펴져 있는 경우
        p_node->color = RBTREE_BLACK;
        p_node->parent->color = RBTREE_RED;
        right_rotation(t,p_node->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
  
  return t->root;
}

//p를 기준으로 왼쪽으로 회전(색 안바꾸고 그냥 회전)
void left_rotation(rbtree *t, node_t *p)
{ 
  node_t* newp = p->right;

  //[단계1] newp의 자식과 p를 연결한다. 
  p->right = newp->left;
  if(newp->left != t->nil)
    newp->left->parent = p;

  //[단계2] newp의 부모를 p의 부모로 설정한다.
  newp->parent = p->parent;
  //p가 루트일 때,
  if(p->parent == t->nil) 
    t->root = newp;

  //p가 누군가의 왼쪽 자식 노드일때,
  else if(p == p->parent->left) 
    p->parent->left = newp;

  //p가 오른쪽 자식일때,
  else
    p->parent->right = newp;
  
  //[단계3] newp의 자식으로 p를 설정하고 p의 부모를 newp로 설정한다. 
  newp->left = p;
  p->parent = newp;

}

//p를 기준으로 오른쪽으로 회전(색 안바꾸고 그냥 회전)
void right_rotation(rbtree *t, node_t *p)
{
  node_t* newp = p->left;

  //[단계1] newp의 자식과 p를 연결한다. 
  p->left = newp->right;
  if(newp->right != t->nil)
    newp->right->parent = p;
  
  //[단계2] newp의 부모를 p의 부모로 설정한다.
  newp->parent = p->parent;

  if(p->parent == t->nil)
    newp = t->root;
  else if(p == p->parent->left)
    p->parent->left = newp;
  else p->parent->right = newp;

  //[단계3] newp의 자식으로 p를 설정하고 p의 부모를 newp로 설정한다.
  newp->right = p;
  p->parent = newp;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t* cur = t->root;

  while(cur != t->nil)
  {
      if(cur->key == key) return cur;

      if(cur->key < key)
        cur = cur->left;
      else 
        cur = cur->right;
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;

  while(cur != t->nil)
  {
    cur = cur->left;
  }

  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;

  while(cur != t->nil)
  {
    cur = cur->right;
  }

  return cur;
}

//transplant:이식 -> old의 parent에서 old자리에 new를 연결한다.
void transplant(rbtree *t, node_t *old, node_t *new)
{
  if(old->parent == t->nil) //부모가 nil이면 root라는 뜻
    t->root = new;

  else if(old == old->parent->left) //기존 노드가 왼쪽에서 왔으면
    old->parent->left = new; //새로운 노드를 왼쪽에 연결 
    
  else //기존 노드가 오른쪽에서 왔으면
    old->parent->right = new; //새로운 노드를 오른쪽에 연결

  if(new != t->nil) //nil일 경우, parent 설정 X
    new->parent = old->parent;
}

node_t *find_minimum(rbtree *t, node_t *p)
{
  while(p->left != t->nil)
  {
    p = p->left;
  }
  return p;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *suc, *extra_black;
  suc = p; //후임자 초기화
  color_t delete_color = suc->color; //삭제될 색 초기화

  //[자식이 0개 또는 1개일 경우]
  if(p->left == t->nil) //오른쪽 자식만 존재,
  {
    extra_black = p->right;
    transplant(t,p,p->right); //p자리에 오른쪽 자식 연결
  }
  else if(p->right == t->nil) //왼쪽 자식만 존재,
  {
    extra_black = p->left;
    transplant(t,p,p->left); //p자리에 왼쪽 자식 연결
  }
  //[자식이 2개일 경우]
  else
  {
    suc = find_minimum(t,p->right); //Rmin을 찾아서 후임자로 설정.
    delete_color = suc->color; //삭제될 color는 Rmin color랑 동일.
    extra_black = suc->right; //후임자의 우측 노드
    if(suc->parent == p) //즉, 후임자가 리프노드 였다면,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              b
    {
      extra_black->parent = suc; //nil노드의 parent를 설정하는 것이 이해가 잘 안감... ㅠ
    }
    else //suc이 리프노드가 아니라면
    {
      transplant(t,suc,suc->right); //삭제될 후임자 자리에 후임자의 오른쪽과 연결
      suc->right = p->right; //후임자의 오른쪽을 기존 오른쪽 노드와 연결
      suc->right->parent = suc; //연결 후, parent를 후임자로 설정                                                                                                                                                                              
    }
    transplant(t,p,suc); //기존 노드 자리에 suc노드 삽입.
    suc->left = p->left; // 새로운 left연결 후
    suc->left->parent = suc; // left 부모도 설정
    suc->color = p->color; // 원래 존재하던 color로 변경
  }
  
  if(delete_color == RBTREE_BLACK)
    delete_fixup(t,extra_black);
  free(p);
  return 0;
}

void delete_fixup(rbtree *t, node_t *extra_black)
{
  node_t *bro; //형제노드 선언
  while(extra_black != t->root && extra_black->color == RBTREE_BLACK)
  {
    //왼쪽 서브트리에 위치할 경우
    if(extra_black == extra_black->parent->left)
    {
      bro = extra_black->parent->right;

      //[case 1]: 형제의 색깔이 red일 경우, 색깔 변경 후, rotation
      if(bro->color == RBTREE_RED)
      {
        bro->parent->color = RBTREE_RED;
        bro->color = RBTREE_BLACK;
        left_rotation(t,bro->parent);
        bro = extra_black->parent->right;
      }

      //[case 2]: 형제의 색깔이 black이고, 형제의 자녀가 모두 black일 경우
      if(bro->left->color == RBTREE_BLACK && bro->right->color == RBTREE_BLACK)
      {
        bro->color = RBTREE_RED;
        extra_black = extra_black->parent;
      }
      else
      {
        //[case 3]: 형제의 색깔이 black이고, 형제의 왼쪽 자녀가 red일 경우(오른쪽 자녀가 black),
        if(bro->right->color == RBTREE_BLACK)
        {
          bro->color = RBTREE_RED;
          bro->left->color = RBTREE_BLACK;
          right_rotation(t,bro);
          bro = extra_black->parent->right;
        }
        //[case 4]: 형제의 색깔이 black이고, 형제의 오른쪽 자녀가 red일 경우,
        bro->color = extra_black->parent->color;
        extra_black->parent->color = RBTREE_BLACK;
        bro->right->color = RBTREE_BLACK;
        left_rotation(t,extra_black->parent);
        extra_black = t->root; //종료조건
      }
    }

    //오른쪽 서브트리에 위치할 경우
    else
    {
      bro = extra_black->parent->left;

      //[case 1]: 형제의 색깔이 red일 경우, 색깔 변경 후, rotation
      if(bro->color == RBTREE_RED)
      {
        bro->parent->color = RBTREE_RED;
        bro->color = RBTREE_BLACK;
        right_rotation(t,bro->parent);
        bro = extra_black->parent->left;
      }
      //[case 2]: 형제의 색깔이 black이고, 형제의 자녀가 모두 black일 경우
      if(bro->left->color == RBTREE_BLACK && bro->right->color == RBTREE_BLACK)
      {
        bro->color = RBTREE_RED;
        extra_black = extra_black->parent;
      }
      else
      {
        //[case 3]: 형제의 색깔이 black이고, 형제의 오른쪽 자녀가 red일 경우(왼쪽 자녀가 black),
        if(bro->left->color == RBTREE_BLACK)
        {
          bro->color = RBTREE_RED;
          bro->right->color = RBTREE_BLACK;
          left_rotation(t,bro);
          bro = extra_black->parent->left;
        }
        //[case 4]: 형제의 색깔이 black이고, 형제의 왼쪽 자녀가 red일 경우,
        bro->color = extra_black->parent->color;
        extra_black->parent->color = RBTREE_BLACK;
        bro->left->color = RBTREE_BLACK;
        right_rotation(t,extra_black->parent);
        extra_black = t->root; //종료조건
      }
    }
  }
  extra_black->color = RBTREE_BLACK; //루트의 color를 black으로 설정. 
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array

  return 0;
}


// 트리를 출력하는 함수
void print_rbtree(rbtree *t, node_t *node, int space)
{
  if (node == t->nil)
    return;

  space += 10;
  print_rbtree(t, node->right, space);

  printf("\n");
  for (int i = 10; i < space; i++)
    printf(" ");
  printf("%d(%s)\n", node->key, node->color == RBTREE_RED ? "R" : "B");

  print_rbtree(t, node->left, space);
}

int main()
{
  rbtree *t = new_rbtree(); // 레드-블랙 트리 생성 함수
  int key;

  printf("노드를 삽입하려면 키 값을 입력하세요 (음수를 입력하면 종료):\n");
  while (scanf("%d", &key) && key >= 0)
  {
    rbtree_insert(t, key);
    print_rbtree(t, t->root, 0);
    printf("--------\n");
  }

  // 트리 메모리 해제
  delete_rbtree(t);

  return 0;
}