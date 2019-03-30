#define MAIN
#include <prefix.h>
int status = 0;
int main(int argc, char **argv)
{
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	start_color();
	curs_set(0);
	
	char *mode[] = {"ИНТЕРАКТИВНЫЙ РЕЖИМ", "ВВОД/ВЫВОД В ФАЙЛ", "ВЫХОД ИЗ ПРОГРАММЫ", NULL};
//	while (getch())
	switch (show_menu(mode, ARRAY_SIZE(mode), CHOICE_REQ)) {
		case 0:
			interactive();
			break;
		case 1:
			file_io();
			break;
	}

	return 0;	
	move(0, 0);
	erase();
	endwin();
}

