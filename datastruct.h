#include <wchar.h>

typedef struct pathnode {
	wchar_t value;
	struct pathnode *child[2];
} *PATHNODE;

typedef struct codevalue {
	wchar_t value;
	unsigned int occurence;
	double distribution;
	short codelen;
	char *prefixc;
	char *shannon;
} CODEVALUE;

typedef struct codetable { //master table to hold them all
	unsigned int totcharc; //total charcount
	unsigned int distcharc; //distinct charcount
	unsigned int tablesize;
	unsigned int mintreedepth; //min depth of binary tree pointed by
	CODEVALUE codes[];
} *CODETABLE;

typedef struct codenode {
	unsigned int weight;
	unsigned char dirctn;
	struct codenode *parent;
	struct codenode *next;
	CODEVALUE *origin;
} *CODENODE;


typedef struct chartofile {
	wchar_t value;
	FILE *output;
} *CHARTOFILE;

typedef struct cicular {
	char *value;
	struct cicular *next;
} *CIRCULAR;

typedef enum work_mode{FILE_S, STRING_S} M;




