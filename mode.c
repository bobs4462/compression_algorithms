#define MODE
#include <prefix.h>
extern int status;
int thr_done = 0;
void interactive(void)
{
	unsigned int c;
	char *welcome = "Введите текст для кодирования";
	wchar_t **textc = get_input(welcome, NULL, 1, 15, 50);
	CODETABLE table = gettable(textc[0], STRING_S);	
	codify(table);
	
	visualize(table, textc[0]);
	for (int i = 0; i < table->distcharc; ++i)
		free(table->codes[i].prefixc);
	free(table);
	return;
}

void file_io(void)
{
	pthread_t load_screen;
	int c, x, y;
	wchar_t **filesw;
	char *filesc[2];
	char *mode[] = {"РЕЖИМ ДЕКОДИРОВАНИЯ", "РЕЖИМ КОДИРОВАНИЯ", "ВЫХОД ИЗ ПРОГРАММЫ", NULL};
	CODETABLE table;	

	c = show_menu(mode, ARRAY_SIZE(mode), CHOICE_REQ);
	filesw = get_input(mode[c], (char *[]){"Файл ввода:", "Файл вывода:"}, 2, 1, 30);
	filesc[0] = malloc(x = wcstombs(NULL, filesw[0],0) + 1);
	filesc[1] = malloc(y = wcstombs(NULL, filesw[1],0) + 1);
	wcstombs(filesc[0], filesw[0], x);
	wcstombs(filesc[1], filesw[1], y);
	FILE *in = fopen(filesc[0], "r+");
	FILE *out = fopen(filesc[1], "w+");

	switch (c) {
		case 0:
			decode(in, out);
			break;
		case 1:
			pthread_create(&load_screen, NULL, loading, "ИДЁТ СОЗДАНИЕ ТАБЛИЦЫ ");
			table = gettable(in, FILE_S);
			codify(table);
			thr_done = 1;
			pthread_join(load_screen, NULL);
			encode(table, in, out);
	}
	print_msg(filesc[1]);

	return;
}


void encode(CODETABLE table, FILE *in, FILE *out)
{
	rewind(in);
	pthread_t load_screen;

	int k = 0;
	wchar_t c;
	menuvisualize(table);
	thr_done = 0;
	pthread_create(&load_screen, NULL, loading, "ИДЁТ КОДИРОВАНИЕ СООБЩЕНИЯ ");
	filedump(table, out);	
	while ((c = fgetwc(in)) != WEOF) {
		for (int i = 0; i < table->distcharc; ++i) {
			if (c == table->codes[i].value) {
				fputs(table->codes[i].prefixc, out);
				break;
			}
		}
	}
	for (int i = 0; i < table->distcharc; ++i)
		free(table->codes[i].prefixc);
	thr_done = 1;
	pthread_join(load_screen, NULL);
	free(table);
	fclose(out);
	fclose(in);
	return;
}

void decode(FILE *in, FILE *out) 
{
	thr_done = 0;
	pthread_t load_screen;
	int charcount = 0, z = 0;
	rewind(in);
	char buf[100];
	wchar_t flush[10000];
	memset(buf, 0, 100);
	int c, k = 0, flushcounter = 0;
	CODETABLE table = fileextract(in);
	unsigned int mintreedepth = table->mintreedepth;
	unsigned int totcharc = table->totcharc;
	PATHNODE root, tracker;
	menuvisualize(table);
	pthread_create(&load_screen, NULL, loading, "ИДЁТ ДЕКОДИРОВАНИЕ СООБЩЕНИЯ ");
	if (status)
		while ((c = fgetc(in)) != EOF) {
			buf[k++] = c;	
			for (int i = 0; i < table->distcharc; ++i) {
				if (!strcmp(table->codes[i].prefixc, buf)) {	
					k = 0;
					++charcount;
					memset(buf, 0, 100);
					fputwc(table->codes[i].value, out);
					break;
				}
			}
		}
	else {
		root = build_search_tree(table);
		while (totcharc) {
			tracker = root;
			while (k < mintreedepth)	
				buf[k++] = fgetc(in);	
			for (int i = 0; (i < k); ++i)
				tracker = tracker->child[buf[i] - 48];
				
			while (tracker->child[0]) {
				buf[k++] = fgetc(in);
				tracker = tracker->child[buf[k - 1] - 48];
			}
			memset(buf, 0, 100);
			flush[flushcounter++] = tracker->value;	
			if (flushcounter == 9999) {
				flush[9999] = flushcounter = 0;
				fputws(flush, out);
			}
			--totcharc;
			k = 0;
		}
	}
	flush[flushcounter] = 0;
	fputws(flush, out);
	thr_done = 1;
	pthread_join(load_screen, NULL);
	treedelete(root);
	free(table);
	fclose(out);
	fclose(in);
}
