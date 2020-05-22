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
      fprintf(stderr,"Dictionary Error: calling size() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   return D->numPairs;
}

// getUnique()
// Returns true (1) if D requires that all pairs have unique keys. Returns
// false (0) if D accepts distinct pairs with identical keys.
int getUnique(Dictionary D) {
   if(D==NULL) {
      fprintf(stderr,"Dictionary Error: calling size() on NULL Dictionary reference\n");
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
      fprintf(stderr,"Dictionary Error: calling size() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   Node N;
   N = findKey(D->root,k);
   return(N==NULL ? VAL_UNDEF : N->value);
}

// makeEmpty();
// re-sets D to the empty state.
void makeEmpty(Dictionary D) {
   deleteAll(D->root);
   D->root = NULL;
   D->numPairs = 0;
}
