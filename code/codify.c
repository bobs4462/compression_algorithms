#define CODIFY
#include <haffman.h>

void codify(CODETABLE table) //функция для вычисления префикс кодов 
{
	int qsz = table->distcharc; //размер очереди (используется в дальнейшем)
	int mintreedepth = 1;
	CODEVALUE sorted[table->distcharc];
	int min = 1, bp; //bp - для вставки в очередь нового эл-та

/*после окончания цикла получим полностью отсортированную очередь*/
	for (int i = 0; i < qsz; ) { 
		for (int j = 0; j < qsz; ++j)
			if (table->codes[j].occurence == min) {
				sorted[i].value = table->codes[j].value;
				sorted[i++].occurence = table->codes[j].occurence;
			}
		++min;
	}
	CODENODE *queue = malloc(sizeof(CODENODE) * qsz); //очередь
/*baseline имеет точную копию очереди до начала работы алгоритма, 
 * также каждый элемент baseline имееет ссылку на элемент из массива
 * codes в таблице кодов*/
	CODENODE baseline = malloc(sizeof(struct codenode) * qsz);
	CODENODE temp; //указатель на побегушках
	for (int i = 0; i < table->distcharc; ++i) {
		baseline[i].weight = sorted[i].occurence;
		baseline[i].origin = &sorted[i];
		queue[i] = &baseline[i];	
	}
/*начало работы алгоритма, вся суть которого получить бинарное дерево
 * алгоритм использует сортируемую очередь для получения оптимальных 
 * префикс кодов*/
	while (qsz > 1) {
		temp = malloc(sizeof(struct codenode));	
		temp->weight = queue[0]->weight + queue[1]->weight;
		temp->parent = NULL;
		queue[0]->parent = queue[1]->parent = temp;	
		queue[0]->dirctn = '0';
		queue[1]->dirctn = '1';

		for (int i = 0; i < qsz - 2; ++i) {
			queue[i] = queue[i + 2]; //двигаем элементы на 2 влево
		}

		queue = realloc(queue, sizeof(CODENODE) * (--qsz)); //сжимаем очередь
		//находим место для вставки нового элемента
		for (bp = 0; (bp < qsz - 1) && (queue[bp]->weight < temp->weight); ++bp); 
		for (int i = 0; i < (qsz - bp - 1); ++i)
			queue[qsz - 1 - i] = queue[qsz - 2 - i];//подвиньтесь!
		queue[bp] = temp; //вставляем
	}
	/*сборка кодов, начинаем снизу с каждого элемента baseline,
	 * оттуда движемся вверх используя указатели до корня,
	 * в результате получим перевернутый префикс код*/
	char t[100] = {0}; //времменный буффер для сборки кода
	for (int i = 0, j = 0; i < table->distcharc; ++i) {
		temp = &baseline[i];
		for (j = 0; temp->parent; j++) {
			t[j] = temp->dirctn; 
			temp = temp->parent;
		}
		t[j] = 0;
		/*mintreedepth мета информация касательно минимальной глубины дерева,
		 * данная информация существенно ускоряет процесс декодирования в 
		 * дальнейшем*/
		mintreedepth = (mintreedepth < j) ? mintreedepth : j;	
		strreverse(t); //переворачиваем строку
		baseline[i].origin->prefixc = malloc(strlen(t) + 1);
		//codelen мета-информация для записи таблицы в файл
		baseline[i].origin->codelen = strlen(t);
		/*кладем полученный код в таблицу */
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
