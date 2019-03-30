#define NCURSES_WIDECHAR 1
#include <datastruct.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define CHOICE_REQ "РЕЖИМ РАБОТЫ:"
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#ifdef CODIFY
PATHNODE *collapse(PATHNODE root, int nodecount);
void strreverse(char *string); //doesn't work for multibyte strings
PATHNODE *collapse(PATHNODE root, int nodecount);
void build_prefix_tree_hf(CODENODE *queue, int qsz);
CODENODE build_prefix_tree_sp(CODENODE start, CODENODE end);
extern int altm;
#include <pthread.h>
#endif

#ifdef MAIN
#include <locale.h>
#include <ncurses.h>
#define ALGORITHM_REQ "АЛГОРТМ КОДИРОВАНИЯ:"
int show_menu(char **items, int sz, char *msg);
void interactive(void);
void file_io(void);
#endif

#ifdef GUI
#include <menu.h>
#include <time.h>
#include <form.h>
#include <panel.h>
char *get_fixed(char *string, int fixedcount);
#endif

#ifdef UTILITY
PATHNODE spop(PATHNODE **stack, int *stsz);
void spush(PATHNODE **stack, int *stsz, PATHNODE node);
#endif

#ifdef MODE
#include <pthread.h>
void treedelete(PATHNODE root);
PATHNODE restore(PATHNODE *treearray, PATHNODE *root, int nodecount);
int show_menu(char **items, int sz, char *msg);
wchar_t **get_input(char *msg, char **desc, int count, int height, int width); //intro message, field, descriptions, field count, field height, field width
void codify(CODETABLE table);
void print_msg(char *msg);
CODETABLE gettable(void *textsource, M mode);
void encode(CODETABLE table, FILE *in, FILE *out);
void decode(FILE *in, FILE *out);
void visualize(CODETABLE table, wchar_t *source);
void filedump(CODETABLE table, FILE *out);
CODETABLE fileextract(FILE *in);
void menuvisualize(CODETABLE table);
PATHNODE build_search_tree(CODETABLE table);
void *writechar(void *ptr);
#endif

void *loading(void *msg);
