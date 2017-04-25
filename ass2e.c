/* ========================================================================= *
 | The University of Melbourne                                               |
 | COMP10002 Foundations of Algorithms, Semester 2, 2015                     |
 | Project 2 written by Geoffrey Law (Student ID: 759218), October 2015      |
 * ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "treeops.h"
/* treeops module (Moffat, 2012) */

#define INITIAL 100

/* define the data structure 
 */
typedef struct {
    int index;
    char *phrase;
} data_t;

/* compare phrases function (for constructing binary search tree)
 */
int cmpPhrase(void *x1, void *x2) {
    data_t *p1=x1, *p2=x2;
    return strcmp(p1->phrase, p2->phrase);
}

/* traverse tree action
 */
void freeNode(void *x) {
    data_t *p=x;
    free(p->phrase);
    free(p);
}

/* function prototypes
 */
char *getText(char *text);
int encode(char *text);

/*****************************************************************************/

/* my dear main function
 */
int main(int argc, char *argv[]) {
    int nfactors;
    char *text=NULL;
    text = getText(text);       /* get the entire text */
    nfactors = encode(text);    /* encode process returns no. of factors */
    fprintf(stderr, "encode: %6lu bytes input\n", strlen(text));
    fprintf(stderr, "encode: %6d factors generated\n", nfactors);
    
    /* ta daaa! */
    return 0;
}

/*****************************************************************************/

/* get the entire text from stdin and store into a string
 */
char *getText(char *text) {
    int ch, len=0;
    size_t currentSize=INITIAL;
    text = (char*)malloc(INITIAL*sizeof(char));
    assert(text!=NULL);
    while ((ch=getchar())!=EOF) {
        text[len++] = ch;
        if (len == currentSize) {
            currentSize *= 2;
            text = (char*)realloc(text, currentSize*sizeof(char));
            assert(text!=NULL);
            /* reallocating memory with step size 100 */
        }
    }
    text[len] = '\0';
    
    return text;
}

/*****************************************************************************/

/* construct the binary search tree
 * search while constructing the tree and print the output factor (c,k) pair
 */
int encode(char *text) {
    tree_t *tree;
    data_t *new, *found;
    int n=1, index=1, k=0;
    char *phrase;
    
    tree = make_empty_tree(cmpPhrase); /* treeops module (Moffat, 2012) */
    
    /* print the output while generating the dictionary */
    while (*text) {
        
        /* generate a phrase;
         * initialize the phrase with memset n+1 null bytes to ensure that
         * there will be a null byte in the last element for closing string
         * after strncpy action is done */
        phrase = (char*)malloc((n+1)*sizeof(char));
        assert(phrase!=NULL);
        memset(phrase, '\0', (n+1)*sizeof(char));
        strncpy(phrase, text, n);
        
        /* create a new data and start searching */
        new = (data_t*)malloc(sizeof(*new));
        assert(new!=NULL);
        new->index = index;
        new->phrase = phrase;
        found = search_tree(tree, new); /* treeops module (Moffat, 2012) */
        
        if (!found) {
            /* insert data into tree */
            new->phrase = (char*)malloc((strlen(phrase)+1)*sizeof(char));
            assert(new->phrase!=NULL);
            strcpy(new->phrase, phrase);
            tree = insert_in_order(tree, new); /* treeops module (Moffat, 2012) */
            /* print the output (c,k) pair 
             * where c is the last character currently read (text[n-1])
             * and k is index of the prefix found
             */
            printf("%c%d\n", *(text+(n-1)), k);
            /* shift the text n position and start next entry */
            text += n;
            n = 1;
            k = 0;
            index++;
        } else {
            /* extend n; slicing n characters of the text on next iteration
             * and record index of the phrase found and store into k
             */
            n++;
            k = found->index;
        }
        
        if (*(text+n)=='\0') {
            /* approaching the end of the text
             * print the last output
             */
            printf("%c%d\n", *(text+(n-1)), k);
            text += n;
        }
        
        free(phrase);
        phrase = NULL;
    }
    
    /* free all nodes in the tree */
    traverse_tree(tree, freeNode); /* treeops module (Moffat, 2012) */
    /* and destroy the tree itself */
    free_tree(tree); /* treeops module (Moffat, 2012) */
    tree = NULL;
    
    return index;
}

/*****************************************************************************/

/* ALGORITHMS ARE FUN! */

/* Reference
 * Moffat, A. (2012). Programming, Problem Solving, and Abstraction with C.
 * Sydney, Australia: Pearson Custom Books. 
 */
