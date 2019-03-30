#define GUI
#include <prefix.h>
#include <errno.h>

extern int thr_done;
wchar_t **get_input(char *msg, char **desc, int count, int height, int width) //intro message, field, descriptions, field count, field height, field width
{
	curs_set(0);
	int x, y, d = 0, bc[count];
	wchar_t buffer[count][10000], **retval, c;
	WINDOW *text_win, *sub;
	FORM *text_form;
	FIELD *fields[count + 1];

	if (desc != NULL)
		d = 15;

	for (int i = 0; i < count; ++i) {
		fields[i] = new_field(height, width, 2 + i * (height + 2), 1, 0, 0);	
		set_field_opts(fields[i], ~(O_AUTOSKIP | O_BLANK | O_STATIC));
		bc[i] = 0;
	}
	fields[count] = NULL;
	text_form = new_form(fields);
	form_opts_off(text_form, O_BS_OVERLOAD);

	scale_form(text_form, &y, &x);	
	text_win = newwin(y += 6, (x += 3) + d , (LINES - y) / 2, (COLS - x -d) / 2);
	set_form_win(text_form, text_win);
	set_form_sub(text_form, sub = derwin(text_win, y - 6, x - 2, 3, 1 + d));
	post_form(text_form);
	refresh();

	if (desc != NULL) {
		for (int i = 0; i < count; ++i) {
			mvwprintw(text_win, 5 + i * (height + 2), 1, desc[i]);
		}
		for (int i = 0; i < count; ++i) {
			mvwaddch(text_win, 4 + i * (height + 2), d, ACS_ULCORNER);
			mvwhline(text_win, 4 + i * (height + 2), d + 1, ACS_HLINE, d + width + 1);
			mvwaddch(text_win, 4 + i * (height + 2), d + width + 2, ACS_URCORNER);
			mvwaddch(text_win, 5 + i * (height + 2) + height, d, ACS_LLCORNER);
			mvwhline(text_win, 5 + i * (height + 2) + height, d + 1, ACS_HLINE, d + width + 1);
			mvwaddch(text_win, 5 + i * (height + 2) + height, d + width + 2, ACS_LRCORNER);
			mvwvline(text_win, 5 + i * (height + 2), d, ACS_VLINE, height);
			mvwvline(text_win, 5 + i * (height + 2), d + width + 2, ACS_VLINE, height);
		}
	}

	mvwprintw(text_win, 1, 2, msg);
	box(text_win, 0, 0);

	mvwaddch(text_win, 2, 0, ACS_LTEE);
	mvwhline(text_win, 2, 1, ACS_HLINE, x + d - 2);
	mvwaddch(text_win, 2, x - 1, ACS_RTEE);

	keypad(text_win, TRUE);
	wrefresh(text_win);
	x = 0; y = 0;

	while ((wget_wch(text_win, &c)) != KEY_F(1)) {
		switch(c) {
			case 9:
			case KEY_DOWN:
				if (y < count - 1) {
					++y;
					form_driver_w(text_form, KEY_CODE_YES,  REQ_NEXT_FIELD);
					form_driver_w(text_form, KEY_CODE_YES, REQ_END_LINE);
				}
				break;
			case KEY_UP:
				if (y) {
					--y;
				form_driver_w(text_form, KEY_CODE_YES, REQ_PREV_FIELD);
				form_driver_w(text_form, KEY_CODE_YES, REQ_END_LINE);
				}
				break;
			case KEY_DC:
			case KEY_BACKSPACE:
				form_driver_w(text_form, KEY_CODE_YES, REQ_DEL_PREV);
				if (bc[y])
					buffer[y][--bc[y]] = L'\0';
				break;
			case 10:
				buffer[y][bc[y]] = L'\0';
				goto CLEANUP;
				break;
			default:
				if (bc[y] < 9999) {
					form_driver_w(text_form, OK, c);
					buffer[y][bc[y]++] = c;
				}
		}
	}
CLEANUP:
	retval = malloc(count * sizeof(wchar_t *));	
	for (int i = 0; i < count; ++i) {
		retval[i] = malloc(sizeof(wchar_t) * ++bc[i]);
		wcsncpy(retval[i], buffer[i], bc[i]);
	}

	for (int i = 0; i < count; ++i) {
		free_field(fields[i]);
	}
	unpost_form(text_form);
	free_form(text_form);
	werase(text_win);
	wrefresh(text_win);
	delwin(sub);
	delwin(text_win);
	return retval;
}

int show_menu(char **items, int sz, char *msg)
{
	int x, y;
	MENU *menu;
	WINDOW *wmenu, *sub;
	ITEM *itptr[sz];
	for (int i = 0, *uptr; items[i]; i++) {
		itptr[i] = new_item(items[i], NULL);
		uptr = malloc(sizeof(int));	
		*uptr = i;
		set_item_userptr(itptr[i], uptr);
	}
	itptr[sz - 1] = NULL;
	menu = new_menu(itptr);
	scale_menu(menu, &y, &x);
	if (y > 35) y = 35;
	if (x > 120) x = 120;
	wmenu = newwin(y + 9, x + 16, (LINES - y - 8) / 2, (COLS - x - 16) / 2);
	keypad(wmenu, TRUE);
	set_menu_win(menu, wmenu);	
	set_menu_sub(menu, sub = derwin(wmenu, y, x , 6, 7));
	post_menu(menu);
	box(wmenu, 0, 0);
	mvwaddch(wmenu, 2, 0, ACS_LTEE);
	mvwhline(wmenu, 2, 1, ACS_HLINE, x + 14);
	mvwaddch(wmenu, 2, x + 15, ACS_RTEE);
	mvwprintw(wmenu, 1, 7, msg);
	wrefresh(wmenu);

	while((y = wgetch(wmenu)) != KEY_F(1))
	{
		switch(y)
		{
			case KEY_DOWN:
				menu_driver(menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
				break;
			case 10:
				x = *(int*)item_userptr(current_item(menu));
				goto CLEANUP;
		}
	}

CLEANUP:
	for (int i = 0; i < sz; i++) {
		free(item_userptr(itptr[i]));
		free_item(itptr[i]);
	}
	unpost_menu(menu);
	free_menu(menu);
	werase(wmenu);
	wrefresh(wmenu);
	delwin(sub);
	delwin(wmenu);
	clear();
	return x;
}

void visualize(CODETABLE table, wchar_t *source)
{
	WINDOW *table_win, *text;
	table_win = newwin(table->distcharc + 10, 47, 4, 10);
	box(table_win, 0, 0);
	mvwaddch(table_win, 6, 0, ACS_LTEE);
	mvwhline(table_win, 6, 1, ACS_HLINE, 45);
	mvwaddch(table_win, 6, 46, ACS_RTEE);
	mvwaddch(table_win, 8, 0, ACS_LTEE);
	mvwhline(table_win, 8, 1, ACS_HLINE, 45);
	mvwaddch(table_win, 8, 46, ACS_RTEE);
	mvwvline(table_win, 7, 10, ACS_VLINE, table->distcharc + 2);
	mvwvline(table_win, 7, 20, ACS_VLINE, table->distcharc + 2);
	mvwvline(table_win, 7, 30, ACS_VLINE, table->distcharc + 2);
	mvwprintw(table_win, 2, 3, "Таблица префиксных кодов (Хаффман)");
	mvwprintw(table_win, 3, 3, "Общее количество символов: %u", table->totcharc);
	mvwprintw(table_win, 4, 3, "Количество различных символов: %u", table->distcharc);
	mvwprintw(table_win, 7, 2, "10-ное");
	mvwprintw(table_win, 7, 12, "Символ");
	mvwprintw(table_win, 7, 22, "Частота");
	mvwprintw(table_win, 7, 32, "Префикс код");
	for (int i = 0; i < table->distcharc; ++i) {
		mvwprintw(table_win, 9 + i, 2, "%6d", table->codes[i].value);
		mvwprintw(table_win, 9 + i, 12, "'%lc'", table->codes[i].value > 31 ? table->codes[i].value : L' ');
		mvwprintw(table_win, 9 + i, 22, "%7.5f", table->codes[i].distribution);
		mvwprintw(table_win, 9 + i, 32, "%9s", table->codes[i].prefixc);
	}
	if (source) {
		text = newwin(20, 50, 4, 70);
		mvwprintw(text, 1, 1, "Введенный вами текст следующий:");
		mvwprintw(text, 2, 1, "%ls", source);
		wrefresh(text);
	}
	wrefresh(table_win);
	getch();
	werase(table_win);
	wrefresh(table_win);
	if (source)  {
		werase(text);
		wrefresh(text);
		delwin(text);
	}
		
	delwin(table_win);
}

void menuvisualize(CODETABLE table)
{
	
	char *t, buf[180];
	char **strings = calloc(table->distcharc + 1, sizeof(char *));
	for (int i = 0; i < table->distcharc; ++i) {
		snprintf(buf, 178, "%6d - %1lc - %8.7f - %25s\0", 
				table->codes[i].value,
				table->codes[i].value > 31 ? table->codes[i].value : L'#',
				table->codes[i].distribution,
				get_fixed(table->codes[i].prefixc, 20));
		strings[i] = calloc(strlen(buf) + 1, sizeof(char));
		strcpy(strings[i], buf);
	}
	strings[table->distcharc] = NULL;
	mvprintw((LINES - 30) / 2, (COLS / 2) - 15,  "Общее количество символов: %u", table->totcharc);	
	mvprintw((LINES - 30) / 2 + 1, (COLS / 2) - 15, "Количество различных символов: %u", table->distcharc);
	refresh();
	show_menu(strings, table->distcharc + 1, "Таблица префиксных кодов:");
	for (int i = 0; i < table->distcharc; ++i)
		free(strings[i]);                           
	free(strings);
}
	
void *loading(void *msg)
{
	struct timespec t;
	t.tv_sec = 0;
	t.tv_nsec = 400000000;
	CIRCULAR animation[5];
	CIRCULAR tracer;
	curs_set(0);
	char *dots[] = {".", "..", "...", "....", "....."};
	int x, y;
	for (int i = 0; i < 5; ++i)
		animation[i] = malloc(sizeof(struct cicular));
	for (int i = 0; i < 4; ++i) {
		animation[i]->value = dots[i];
		animation[i]->next = animation[i + 1];
	}
	animation[4]->value = dots[4];
	animation[4]->next = animation[0];
		
	char *cmsg = (char*)msg;
	mvprintw(LINES / 2, (COLS - 28) / 2, cmsg);
	refresh();
	tracer = animation[0];
	getyx(stdscr, y, x);
	while (!thr_done) {
		nanosleep(&t, NULL);
		move(y, x);
		clrtoeol();
		mvprintw(y, x, tracer->value);
		refresh();
		tracer = tracer->next;
	}
	clear();
	return msg;
}

void print_msg(char *msg)
{
	clear();
	mvprintw(LINES / 2, (COLS - 10) / 2, "ГОТОВО !!!");
	mvprintw(LINES / 2 + 1, (COLS - 30) / 2, "Код успешно сохранён в файл '%s'", msg);
	refresh();
	getch();
}
