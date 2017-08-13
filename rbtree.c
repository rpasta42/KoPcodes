#include "utils.h"




void _rb_tree_init_node(rb_tree_t* t, rb_tree_t** root,
                        uint64_t key, void* data, char color)
{
   t->p = NULL;
   t->left = NULL;
   t->right = NULL;
   t->key = key;
   t->data = data;
   t->color = color;

   t->root = root;
}


void rb_tree_new(rb_tree_t* t) {
   _rb_tree_init_node(t, NULL, NULL, NULL, 'b');
   t->p = NULL;
   t->left = NULL;
   t->right = NULL;

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
   if (y->left)
      y->left->p = x;
   y->p = x->p;

   if (x->p == NULL)
      *x->root = y;
   else if (x == x->p->left)
      x->p->left = y;
   else //if (x == x->p->right)
      x->p->right = y;

   y->left = x;
   x->p = y;
}

void _rb_tree_right_rot(rb_tree_t* y)
{
   rb_tree_t* x = y->left;
   if (x->right)
      x->right->p = y;

   x->p = y->p;

   if (y->p == NULL)
      *y->root = x;
   else if (y == y->p->left)
      y->p->left = x;
   else //if (y == y-p->right)
      y->p->right = x;

   x->right = y;
   y->p = x;
}


rb_tree_t* rb_tree_insert(rb_tree_t* t, uint64_t key, void* entry)
{
   rb_tree_t *root, *x, *ret_parent, *ret;

   root = *t->root;
   x = root;
   ret_parent = NULL;

   ret = malloc(sizeof(rb_tree_t));
   _rb_tree_init_node(ret, root->root, key, entry, 'r');


   while (x != NULL) {
      ret_parent = x;
      if (key < x->key) {
         x = x->left;
      }
      else {
         x = x->right;
      }
   }

   ret->p = ret_parent;
   if (ret_parent == NULL)
      *t->root = ret;
   else if (key < ret_parent->key)
      ret_parent->left = ret;
   else
      ret_parent->right = ret;


   _rb_tree_insert_fixup(ret);
   return ret;
}

void _rb_tree_insert_fixup(rb_tree_t* z)
{
   rb_tree_t* y;

   while (z->p->color == 'r') {
      if (z->p == z->p->p->left) {
         y = z->p->p->right;
         if (y->color == 'r') {
            z->p->color = 'b';
            y->color = 'b';
            z->p->p->color = 'r';
         }
         else if (z == z->p->right) {
            z = z->p;
            _rb_tree_left_rot(z);
            z->p->color = 'b';
            z->p->p->color = 'r';
            _rb_tree_right_rot(z->p->p);
         }
      }
      else { //if (z->p == z->p->p->right)
         y = z->p->p->right;
         if (y->color == 'r') {
            z->p->color = 'b';
            y->color = 'b';
            z->p->p->color = 'r';
         }
         else if (z == z->p->left) {
            z = z->p;
            _rb_tree_right_rot(z);
            z->p->color = 'b';
            z->p->p->color = 'r';
            _rb_tree_left_rot(z->p->p);
         }
      }

   }
   (*z->root)->color = 'b';
}

void rb_tree_insert_str(rb_tree_t* t, c_string_t* key, void* entry)
{

}


void* rb_tree_search(rb_tree_t* t, uint64_t key) {
   bool gt = key > t->key;
   bool lt = key < t->key;
   bool eq = key == t->key;


   if (eq)
      return t->data;

   if (lt && t->left == NULL)
      return NULL;
   if (gt && t->right == NULL)
      return NULL;

   if (lt)
      return rb_tree_search(t->left, key);
   else if (gt)
      return rb_tree_search(t->right, key);
}


