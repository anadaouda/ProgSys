#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  PLUS = '+',
  MULT = '*',
  MIN  = '-',
  DIV  = '/'
} sign_t;

typedef struct op{
  sign_t  sign;
  float(*operator)(float,float);
} op_t;

typedef union tag{
  float value;
  op_t op;
} tag_t;

typedef struct tree{
  tag_t tag;
  struct tree *left;
  struct tree *right;
} node_t;


#define is_leaf(node) (((node)->left == NULL) && ((node)->right == NULL))?1:0
#define generic_op(fname,op) float __##fname##__(float arg1, float arg2){ return ( arg1 op arg2 );}
#define init_node(signe,fname)  do { current->tag.op.sign     = signe;		           \
                                     current->tag.op.operator = __##fname##__; } while(0)
generic_op(add,+);
generic_op(mult,*);
generic_op(sub,-);
generic_op(divide,/);

node_t *build_tree(char **arg,int *position)
{
  node_t *current  = (node_t *)malloc(sizeof(node_t));
#ifdef DEBUG
  fprintf(stdout,"BT for arg : %s (%i)\n",arg[*position],*position);
#endif
  if ((arg[*position][0] != PLUS) &&
      (arg[*position][0] != MULT) &&
      (arg[*position][0] != DIV) &&
      (strcmp(arg[*position],"-") != 0))
    {
      current->tag.value = atof(arg[*position]);
      current->left  = NULL;
      current->right = NULL;
      (*position)++;
#ifdef DEBUG
      fprintf(stdout,"Leaf @ : %p | pos is %i\n",current,*position);
#endif
    }
  else{
    if( arg[*position][0] == MULT) init_node(MULT,mult);
    else if( arg[*position][0] == PLUS) init_node(PLUS,add);
    else if( arg[*position][0] == DIV)  init_node(DIV,divide);
    else if( arg[*position][0] == MIN)  init_node(MIN,sub);
    else fprintf(stdout,"Input error ! \n");

    (*position)++;
#ifdef DEBUG
    fprintf(stdout,"Node @ : %p | pos is %i\n",current,*position);
#endif
    current->left  = build_tree(arg,position);
    current->right = build_tree(arg,position);
#ifdef DEBUG
    fprintf(stdout,"====== Node @ : %p  | Left  %p \n",current,current->left);
    fprintf(stdout,"====== Node @ : %p  | Right %p \n",current,current->right);
#endif
  }
  return current;
}

void display_tree(node_t *node)
{
  if(is_leaf(node))
    fprintf(stdout,"%f",node->tag.value);
  else{
    fprintf(stdout,"(");
    display_tree(node->left);
    fprintf(stdout," %c ",node->tag.op.sign);
    display_tree(node->right);
    fprintf(stdout,")");
  }
}

float compute_tree(node_t *node)
{
  if(is_leaf(node))
    return node->tag.value;
  else
  //creer deux threads qui vont prendre une branche en arguments
  //attendre que les deux envoie et faire return node->tag.value
    return node->tag.op.operator(compute_tree(node->left),compute_tree(node->right));
}

void cleanup_tree(node_t *node)
{
  if(!is_leaf(node)){
    cleanup_tree(node->left);
    cleanup_tree(node->right);
  }
  free(node);
}

int main(int argc, char **argv)
{
  if(argc > 1){
    node_t *root = NULL;
    int position = 1;    /* remove argv[0] */
    float res = 0.0;

    root = build_tree(argv,&position);
    display_tree(root);
    res = compute_tree(root);
    fprintf(stdout," =  %f\n",res);
    cleanup_tree(root);
  }

  exit(EXIT_SUCCESS);
}
