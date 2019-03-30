#define UTILITY
#include <haffman.h>
/*gettable берёт источник символов (файл или строка)
 * читает её заполняет codetable всей информацией кроме
 * кроме кодов*/
CODETABLE gettable(void *textsource, M mode)	
{
	CODETABLE table;
	unsigned int dcount = 0, tcount = 0, tsz;
	int val, occs[256] = {0}; //occurrences
	wchar_t temp[256] = {0};
	wchar_t *hit;
	switch (mode) {
		case FILE_S:
			;
			FILE *fptr = (FILE *)textsource;
			while ((val = fgetwc(fptr)) != WEOF) {
				if (!(hit = wcschr(temp, val))) {
					temp[dcount] = val;
					++occs[dcount];
					++dcount;
				}
				else
					++(occs[hit - temp]);
				++tcount;
			}
			break;
		case STRING_S:
			;
			wchar_t *cptr = (wchar_t *)textsource;
			for (int i = 0; cptr[i] != '\0'; ++i) {
				if (!(hit = wcschr(temp, cptr[i]))) {
					temp[dcount] = cptr[i];
					++occs[dcount];
					++dcount;
					}
				else
					++(occs[hit - temp]);
				++tcount;
			}
			break;
	}

	table = malloc(tsz = (sizeof(*table) + sizeof(struct codevalue) * wcslen(temp)));
	table->totcharc = tcount; 
	table->distcharc = wcslen(temp);
	table->tablesize = tsz;

	for (int i = 0; i < table->distcharc; ++i) {
		table->codes[i].value = temp[i];
		table->codes[i].occurence = occs[i];
		table->codes[i].distribution = occs[i] / (double)tcount;
	}
		
	return table;
}

/*spush, spop, treedelete функции для поддержания чистоты*/
void spush(PATHNODE **stack, int *stsz, PATHNODE node)
{
    *stack = realloc(*stack, sizeof(PATHNODE) * (++(*stsz)));
    (*stack)[*stsz - 1] = node;
}

PATHNODE spop(PATHNODE **stack, int *stsz)
{
    PATHNODE temp = (*stack)[*stsz - 1];
    *stack = realloc(*stack, sizeof(PATHNODE) * (--(*stsz)));
    return temp;
}

void treedelete(PATHNODE root)
{
    unsigned int stsz = 1;
    PATHNODE *stack = malloc(sizeof(PATHNODE));
    PATHNODE node = *stack = root;


    while (stsz) {
        if (node->child[0])
            spush(&stack, &stsz, node->child[0]);
        if (node->child[1])
            spush(&stack, &stsz, node->child[1]);
        free(node);
        node = spop(&stack, &stsz);
    }
}


/*build_search_tree построение поискового дерева
 * используя префикс коды в таблице*/
PATHNODE build_search_tree(CODETABLE table)
{
	char buf[50] = {0};
	PATHNODE root = malloc(sizeof(struct pathnode));
	root->child[0] = root->child[1] = NULL;
	PATHNODE allocator = NULL;
	PATHNODE tracer = NULL;
	for (int i = 0; i < table->distcharc; ++i) {
		strcpy(buf, table->codes[i].prefixc);	
		tracer = root;
		for (int j = 0; buf[j] != 0; ++j) {
			if (tracer->child[buf[j] - 48] == NULL) {
				allocator = malloc(sizeof(struct pathnode));	
				allocator->child[0] = allocator->child[1] = NULL;
				tracer->child[buf[j] - 48] = allocator;
			}

			tracer = tracer->child[buf[j] - 48];
		}
		tracer->value = table->codes[i].value;
	}
	return root;
}

