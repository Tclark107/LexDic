//===============================================================================
// Dictionary.c
// My dictionary ADT.
//
// Tristan Clark
//===============================================================================

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include "Dictionary.h"

//------------------------------------------------------------------------------
// Constructors-Destructors
//------------------------------------------------------------------------------

// private Constructors-Destuctors and helpers ---------------------------------

// NodeObj
typedef struct NodeObj{
  KEY_TYPE key;
  VAL_TYPE value;
  struct NodeObj* right;
  struct NodeObj* left;
  struct NodeObj* prev; 
}NodeObj;

typedef NodeObj* Node;

// NewNode()
// Node Constructor
Node NewNode(KEY_TYPE key, VAL_TYPE value) {
   Node N = malloc(sizeof(NodeObj));
   assert(N!=NULL);
   N->key = key;
   N->value = value;
   N->left = N->right = N->prev = NULL;
   return(N);
}

// freeNode()
// Frees Node pointed to by pN
void freeNode(Node* pN) {
   if(pN!=NULL && *pN!=NULL){
      free(*pN);
      *pN = NULL;
   }
}

// deleteAll()
// Empties the Dictionary rooted at N
void deleteAll(Node N) {
   if(N!=NULL) {
      deleteAll(N->left);
      deleteAll(N->right);
      freeNode(&N);
   }
}

// findKey()
// returns the Node containing key k in the subtree rooted at R
// or returns NULL if no such Node exists
Node findKey(Node R, KEY_TYPE k) {
   if(R==NULL|| KEY_CMP(k,R->key)==0)
      return R;
   if(KEY_CMP(k,R->key)<0)
      return findKey(R->left,k);
   else
      return findKey(R->right,k);
}

// Public Constructors-Destructors ----------------------------------------------

// Dictionary Object
typedef struct DictionaryObj{
   Node root;
   Node cursor;
   int numPairs;
   int isUnique; 
}DictionaryObj;

Dictionary newDictionary(int unique) {
   Dictionary D = malloc(sizeof(DictionaryObj));
   assert(D!=NULL);
   D->numPairs = 0;
   D->cursor = KEY_UNDEF;
   D->isUnique = unique;
   return D;
}

void freeDictionary(Dictionary* pD) {
   if(pD!=NULL && *pD!=NULL) {
      makeEmpty(*pD);
      free(*pD);
      *pD = NULL;
   }
}

//-------------------------------------------------------------------------------
// Access Functions
//-------------------------------------------------------------------------------

// size()
// Returns the number of (key,value) pairs in Dictionary D.
int size(Dictionary D) {
   if(D==NULL) {
      fprintf(stderr,
      "Dictionary Error: calling size() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   return D->numPairs;
}

// getUnique()
// Returns true (1) if D requires that all pairs have unique keys. Returns
// false (0) if D accepts distinct pairs with identical keys.
int getUnique(Dictionary D) {
   if(D==NULL) {
      fprintf(stderr,
      "Dictionary Error: calling size() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }

   if(D->isUnique == 0) {
      return 0;
   }
   else {
      return 1;
   }
}

// lookup()
// If Dictionary D contains a (key, value) pair whose key matches k (i.e. if
// KEY_CMP(key, k)==0), then returns value. If D contains no such pair, then
// returns VAL_UNDEF.
VAL_TYPE lookup(Dictionary D, KEY_TYPE k) {
   if(D==NULL) {
      fprintf(stderr,
      "Dictionary Error: calling size() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   Node N;
   N = findKey(D->root,k);
   return(N==NULL ? VAL_UNDEF : N->value);
}

// Manipulation procedures ----------------------------------------------------

// insert()
// Insert the pair (k,v) into Dictionary D. 
// If getUnique(D) is false (0), then there are no preconditions.
// If getUnique(D) is true (1), then the precondition lookup(D, k)==VAL_UNDEF
// is enforced. 
void insert(Dictionary D, KEY_TYPE k, VAL_TYPE v) {
   if(D==NULL) {
      fprintf(stderr,
      "Dictionary Error: calling insert() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   if(D->isUnique==1) { // if unique then check for repeat keys
      if(findKey(D->root, k)!=KEY_UNDEF) {
         fprintf(stderr, 
         "Dictionary Error: cannot insert() duplicate key: \"%s\"\n",k);
         exit(EXIT_FAILURE);
      }
   } // otherwise just insert
   Node N,A,B;
   N = NewNode(k,v);
   B = KEY_UNDEF;
   A = D->root;
   while(A!=KEY_UNDEF){
      B = A;
      if(KEY_CMP(k,A->key)<0) {
         A = A->left;
      }else {
         A = A->right;
      }
   }
   if(B==KEY_UNDEF) {
      D->root = N;
   }else if(KEY_CMP(k,B->key)<0) {
      B->left = N;
      N->prev = B;
   }else {
      B->right = N;
      N->prev = B;
   }
   D->numPairs++;
}

// makeEmpty();
// re-sets D to the empty state.
void makeEmpty(Dictionary D) {
   deleteAll(D->root);
   D->root = NULL;
   D->numPairs = 0;
}

// Other operations -----------------------------------------------------------

// inOrder
// Recursive helper function that prints a tree in order by strcmp
void inOrder(FILE* out,Node N){
   //while(N!=KEY_UNDEF) {
   if(N!=NULL) {
      inOrder(out,N->left);
      fprintf(out,"%s %d\n",N->key,N->value);
      inOrder(out,N->right);
   }
}

// printDictionary()
// Prints a text representation of D to the file pointed to by out. Each key-
// value pair is printed on a single line, with the two items separated by a
// single space.  The pairs are printed in the order defined by the operator
// KEY_CMP().
void printDictionary(FILE* out, Dictionary D) {
   if(D==NULL) {
      fprintf(stderr, 
      "Dictionary Error: calling printDictionary() on NULL Dictionary "\
      "reference\n");
      exit(EXIT_FAILURE);
   }
   inOrder(out,D->root);   
}
