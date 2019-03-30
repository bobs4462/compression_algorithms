#define CODIFY
#define MOD(X) X < 0 ? X * (-1) : X
#include <prefix.h>

void codify(CODETABLE table) //функция для вычисления префикс кодов
{
	int qsz = table->distcharc; //size of queue below
	int mintreedepth = 1;
	CODEVALUE sorted[table->distcharc];
	int min = 1;//breakpoint in queue to insert new element

	for (int i = 0; i < qsz; ) {
		for (int j = 0; j < qsz; ++j)
			if (table->codes[j].occurence == min) {
				sorted[i].value = table->codes[j].value;
				sorted[i++].occurence = table->codes[j].occurence;
			}
		++min;
	}
	CODENODE *queue = malloc(sizeof(CODENODE) * qsz);
	CODENODE baseline = malloc(sizeof(struct codenode) * qsz);
	CODENODE temp;

	for (int i = 0; i < table->distcharc; ++i) {
		baseline[i].weight = sorted[i].occurence;
		baseline[i].origin = &sorted[i];
		if (i < qsz - 1)
			baseline[i].next = &baseline[i + 1];
		queue[i] = &baseline[i];	
	}
	//TODO figure out wether to use to two queues or two separate tables
	
	if (altm) 
		build_prefix_tree_sp(queue[0], queue[qsz - 1]);
	else
		build_prefix_tree_hf(queue, qsz);

	char t[100] = {0};
	for (int i = 0, j = 0; i < table->distcharc; ++i) {
		temp = &baseline[i];
		for (j = 0; temp->parent; j++) {
			t[j] = temp->dirctn;
			temp = temp->parent;
		}
		t[j] = 0;
		mintreedepth = (mintreedepth < j) ? mintreedepth : j;	
		strreverse(t);
		baseline[i].origin->prefixc = malloc(strlen(t) + 1);
		baseline[i].origin->codelen = strlen(t);
		strcpy(baseline[i].origin->prefixc, t);
		memset(t, 0, 100);
	}
	for (int i = 0, j = 0; i < table->distcharc; ++i) {
		while (sorted[j++].value != table->codes[i].value);
		table->codes[i].prefixc = sorted[--j].prefixc;
		table->codes[i].codelen = sorted[j].codelen;
		j = 0;
	}
	table->mintreedepth = mintreedepth;
	free(queue);
	free(baseline);
}

void build_prefix_tree_hf(CODENODE *queue, int qsz)
{
	int bp = 0;
	CODENODE temp;
	while (qsz > 1) {
		temp = malloc(sizeof(struct codenode));	
		temp->weight = queue[0]->weight + queue[1]->weight;
		temp->parent = NULL;
		queue[0]->parent = queue[1]->parent = temp;	
		queue[0]->dirctn = '0';
		queue[1]->dirctn = '1';

		for (int i = 0; i < qsz - 2; ++i) {
			queue[i] = queue[i + 2];
		}

		queue = realloc(queue, sizeof(CODENODE) * (--qsz));
		for (bp = 0; (bp < qsz - 1) && (queue[bp]->weight < temp->weight); ++bp);
		for (int i = 0; i < (qsz - bp - 1); ++i)
			queue[qsz - 1 - i] = queue[qsz - 2 - i];
		queue[bp] = temp;
	}

}
CODENODE build_prefix_tree_sp(CODENODE start, CODENODE end)
{
	if (start == end)
		return start;

	CODENODE root, zero, one;
	int acc = 0, totalr = 0, delta_min = 0, delta_cur = 0;
	CODENODE tracer = start, old_tracer = NULL;

	while (tracer != end) {
		totalr += tracer->weight;
		tracer = tracer->next;
	}
	totalr += end->weight;
	tracer = start;
	acc += tracer->weight;
	totalr -= tracer->weight;
	delta_min = delta_cur = totalr - acc;

	while (delta_min >= delta_cur) {
		old_tracer = tracer;
		tracer = tracer->next;
		acc += tracer->weight;
		totalr -= tracer->weight;
		delta_cur = MOD(totalr - acc);
		delta_min = delta_min > delta_cur ? delta_cur : delta_min;
	}
	
 	root = malloc(sizeof(struct codenode));
	root->parent = NULL;
	zero = build_prefix_tree_sp(start, old_tracer);
	one = build_prefix_tree_sp(old_tracer->next, end);
	one->parent = zero->parent = root;
	one->dirctn = '1';
	zero->dirctn = '0';
	return root;
}

void strreverse(char *string) //doesn't work for multibyte strings
{
	int middle, t;
	int end = strlen(string) - 1;
	middle = strlen(string) / 2;
	
	for (int i = 0; i < middle; ++i) {
		t = string[i];
		string[i] = string[end - i]; 
		string[end - i] = t;
	}
}
