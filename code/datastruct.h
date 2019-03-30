/*pathnode используется при декодирования 
 * является узлом древа поиска, которое
 * формируется используя таблицу кодов
 * данный подход снизил время декодирования
 * в 5 раз*/

typedef struct pathnode {
	wchar_t value;
	struct pathnode *child[2];
} *PATHNODE;

/*Основная структура содержащие данные о кодах 
 * и символах*/
typedef struct codevalue {
	wchar_t value;
	unsigned int occurence;
	double distribution;
	short codelen;
	char *prefixc;
} CODEVALUE;

/*Мастер структура*/
typedef struct codetable { 
	unsigned int totcharc;
	unsigned int distcharc;
	unsigned int tablesize;
	unsigned int mintreedepth; 
	CODEVALUE codes[];
} *CODETABLE;

/*Используется при формировании префиксного дерева*/
typedef struct codenode {
	unsigned int weight;
	unsigned char dirctn;
	struct codenode *parent;
	CODEVALUE *origin;
} *CODENODE;






