#define MAIN
#include <prefix.h>
int status = 0;
int altm = 0;
int main(int argc, char **argv)
{
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	start_color();
	curs_set(0);
	
	char *algorithm[] = {"Алгоритм Хаффмана", "Алгоритм Шаннона-Фано", "ВЫХОД ИЗ ПРОГРАММЫ", NULL};
	char *mode[] = {"ИНТЕРАКТИВНЫЙ РЕЖИМ", "ВВОД/ВЫВОД В ФАЙЛ", "ВЫХОД ИЗ ПРОГРАММЫ", NULL};

	altm = show_menu(algorithm, ARRAY_SIZE(algorithm), ALGORITHM_REQ);

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

