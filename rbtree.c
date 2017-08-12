#include "utils.h"


void rb_tree_new(rb_tree_t* t) {
   t->p = NULL;
   t->left = NULL;
   t->right = NULL;
   t->key = NULL;
   t->data = NULL;
   t->color = 'b';

   t->root = malloc(sizeof(rb_tree_t*)); //set every t to t->root
   *t->root = t;
}



void rb_tree_delete_data(rb_tree_t* t) {
   if (t->p && t->p->data)
      K_FREE(t->p->data);

   if (t->data)
      K_FREE(t->p->data);

   rb_tree_delete_data(t->left);
   rb_tree_delete_data(t->right);
}


//void rb_tree_map(rb_tree_t* t, void (*f)(rb_tree_t*))

void rb_tree_free_keys(rb_tree_t* t) {

}

void rb_tree_free(rb_tree_t* t) {
   rb_tree_t* root = *t->root;

}



//TODO: don't need because root is double pointer now
void _rb_tree_root_fix_childs_root(rb_tree_t* root)
{
   //go through each child and set their root to root
}

bool _rb_tree_left_rot(rb_tree_t* x)
{
   rb_tree_t* y = x->right; //TODO: check that there is x->right?

   x->right = y->left;
   if (y->left) {
      y->left->p = x;
   }
   y->p = x->p;

   if (x->p == NULL) {

   }

}

void _rb_tree_right_rot(rb_tree_t* t)
{

}

void rb_tree_insert(rb_tree_t* t, c_string_t* key, void* entry)
{

}

