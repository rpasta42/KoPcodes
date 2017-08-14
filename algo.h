#ifndef ALGO_H_INCLUDE
#define ALGO_H_INCLUDE

/******LIST******/

//[len/num]_entries already defined
#define LIST_NEW_SIZE(entries, entry_type, num_entries, len_entries) \
do { \
   entries = malloc(sizeof(entry_type)*len_entries); \
} while (0)

#define LIST_NEW(entries, entry_type, num_entries, len_entries) \
do { \
   num_entries = 0; \
   len_entries = 5; \
   entries = malloc(sizeof(entry_type)*len_entries); \
} while (0)

#define LIST_ADD_ITEM_SPACE(entries, entry_type, num_entries, len_entries) \
do { \
   if (num_entries + 1 > len_entries) { \
      len_entries = num_entries + 3; \
      len_entries *= 2; \
      entries = realloc(entries, sizeof(entry_type)*len_entries); \
   } \
} while (0)

#define LIST_ADD_SPACE_EXACT(ents, ent_type, num_ents, len_ents) \
do { \
   if (num_ents + 1 > len_ents) { \
      len_ents = num_ents + 1; \
      ents = realloc(ents, sizeof(ent_type)*len_ents); \
   } \
} while (0)

#define LIST_SHRINK_TO_FIT(ents, ent_type, num_ents, len_ents) \
do { \
   if (len_ents != num_ents) { \
      len_ents = num_ents; \
      ents = realloc(ents, sizeof(ent_type)*len_ents); \
   } \
} while (0)


/*
TODO:
   pointer list
   list that stores copies of elements
*/

/*
#define LIST2_NEW_TYPE(list_type_name, elem_type) \
   TYPE(list_type_name, struct) { \
      size_t len; \
      size_t num; \
      elem_type* ptr; \
   } END_TYPE(list_type_name);



LIST2_NEW_TYPE(

TYPE(k_list_t, struct) {

}

#define NEW_LIST
*/

/******END LIST******/


/******BYTESTIRNG******/

TYPE(byte_string_t, struct) {
   byte_t* str;
   size_t len;
   size_t num;
} END_TYPE(byte_string_t);


static inline void
byte_string_init_no_copy(byte_string_t* bs,
                         byte_t* p, size_t p_len)
{
   bs->str = p;
   bs->len = p_len;
   bs->num = p_len;
}

static inline void
byte_string_init_empty(byte_string_t* bs)
{
   LIST_NEW(bs->str, byte_t, bs->num, bs->len);
}

static inline void
byte_string_init_from_bytes(byte_string_t* bs,
                            byte_t* str, size_t len_str)
{
   bs->num = len_str;
   bs->len = len_str;
   LIST_NEW_SIZE(bs->str, byte_t, bs->num, bs->len);

   memcpy(bs->str, str, bs->num);
}

static inline void
byte_string_init_from_str(byte_string_t* bs, c_string_t str)
{
   //printf("strlen('%s') is %i\n", str, strlen(str));
   byte_string_init_from_bytes(bs, (byte_t*)str, strlen(str));
}


static inline byte_string_t* /* new byte string from bytes */
byte_string_new_from_bytes(byte_t* str, size_t len_str)
{
   byte_string_t* bs = malloc(sizeof(byte_string_t));
   byte_string_init_from_bytes(bs, str, len_str);
   return bs;
}

static inline byte_string_t* /* returns new bs from str */
byte_string_new_from_str(c_string_t str)
{
   byte_string_t* bs = malloc(sizeof(byte_string_t));
   byte_string_init_from_str(bs, str);
   return bs;
}



static inline uint64_t
byte_string_hash(byte_string_t* bt)
{
   uint64_t hash = 5381;

   int i;
   for (i = 0; i < bt->num; i++) {
      uint32_t c = bt->str[i];
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
   }

   return hash;
}

static inline byte_string_t*
byte_string_copy(byte_string_t* bs)
{
   byte_string_t* ret = byte_string_new_from_bytes(bs->str, bs->num);
   return ret;
}


static inline void /* append b to end of a */
byte_string_append(byte_string_t* a, byte_string_t* b)
{

   int old_a_num = a->num;
   int new_a_num = a->num + b->num;
   a->num = new_a_num;
   LIST_ADD_SPACE_EXACT(a->str, byte_t,
                        a->num,
                        a->len);
   memcpy(a->str + old_a_num, b->str, b->num);

}

static inline byte_string_t* /* add a and b & return new byte_string */
byte_string_add(byte_string_t* a, byte_string_t* b)
{
   byte_string_t* c = malloc(sizeof(byte_string_t));
   c->num = a->num + b->num;
   c->len = a->len + b->len;
   LIST_NEW_SIZE(c->str, byte_t, c->num, c->len);

   memcpy(c->str, a->str, a->num);
   memcpy(c->str + a->num, b->str, b->num);

   return c;
}

static inline void //copy a into uninitialized b
byte_string_cp(byte_string_t* a, byte_string_t* b)
{
   b->len = a->len;
   b->num = a->num;

   b->str = malloc(sizeof(a->len));
   memcpy(b->str, a->str, a->num);
}

static inline void
byte_string_append_char(byte_string_t* bs, char c)
{
   LIST_ADD_ITEM_SPACE(bs->str, byte_t, bs->num, bs->len);
   bs->str[bs->num++] = c;
}

static inline byte_string_t*
byte_string_subset(byte_string_t* bs, size_t start, size_t end) {
   size_t len = end - start;
   if (len < 1 || (bs->len <= start) || (bs->len < end))
      return NULL;

   byte_string_t* ret =
      byte_string_new_from_bytes(bs->str + start,
                                 len);
   return ret;
}

static inline int
byte_string_cmp(byte_string_t* a, byte_string_t* b)
{
   return strncmp(a->str, b->str, MIN(a->num, b->num));
   //if (a->num != b->num)
}


static inline bool /* no null characters in [bs[0]..bs[bs->len-1]] */
byte_string_is_good_c_str(byte_string_t* bs)
{
   int i;
   for (i = 0; i < bs->num; i++) {
      if (bs->str[i] == '\0')
         return false;
   }
   return true;
}

//returns NULL if error
//NEED TO FREE RETURN VALUE YOURSELF
static c_string_t
byte_string_to_c_str(byte_string_t* bs)
{
   if (!byte_string_is_good_c_str(bs))
      return NULL;
   char* mem = malloc(bs->num + 1);
   memcpy(mem, bs->str, bs->num);

   mem[bs->num] = '\0';
   return mem;
}


static void
byte_string_free(byte_string_t* s, bool is_malloced)
{
   free(s->str);
   if (is_malloced)
      free(s);
}



/* STUFF FOR byte_string_get_index()
 * uint16_t ret = byte_string_get_index();
 * if (BYTE_STIRNG_IS_GOOD_INDEX(ret))
 *   char_at_index = BYTE_STRING_EXTRACT_INDEX(ret);
 */
#define BYTE_STRING_RET_ERR  ((uint16_t)(1 << 15))
#define BYTE_STRING_RET_SUCC(x) (x)
#define BYTE_STRING_IS_GOOD_INDEX(x) (!((x) & (1 << 15)))
#define BYTE_STRING_EXTRACT_INDEX(x) ((uint8_t)((x << 1) >> 1))

static inline uint16_t
byte_string_get_index(byte_string_t* bs, size_t index)
{
   uint16_t ret = 0;
   if (index < 0 || index >= bs->len-1)
      return BYTE_STRING_RET_ERR;
   else
      return BYTE_STRING_RET_SUCC(bs->str[index]);
}


/******END BYTESTIRNG*******/


/********RB TREE********/

TYPE(rb_tree_t, struct) {
   uint64_t key;
   void* data;

   char color; //'r'/'b'

   struct rb_tree_t** root;
   struct rb_tree_t* p; //parent
   struct rb_tree_t* left;
   struct rb_tree_t* right;

} END_TYPE(rb_tree_t);

void rb_tree_new(rb_tree_t* t);

rb_tree_t* rb_tree_insert(rb_tree_t* t, uint64_t key, void* entry);
void rb_tree_insert_str(rb_tree_t* t, c_string_t* key, void* entry);

void* rb_tree_search(rb_tree_t* t, uint64_t key);

void rb_tree_delete_data(rb_tree_t* t);
void rb_tree_free(rb_tree_t* t);


/******END RB TREE******/

#endif //ALGO_H_INCLUDE
