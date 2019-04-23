#include <decoder.c>

int main(int argc, char **argv)
{
	if (argv[1][1] == 'e') {
		FILE *input = fopen("test.txt", "r");

		char *text = malloc(KILO);
		int kc = 2, i, c;  // KILO counter

		for (i = 0; (c = fgetc(input)) != EOF; ++i)
			if (i < KILO * (kc - 1))
				text[i] = c;
			else {
				text = realloc(text, kc * KILO);
				text[i] = c;
				kc += 1;
			}
		text[i] = 0;
		encoder(text);
		fclose(input);
	}
	else {
		FILE *file_stream = fopen("output.txt", "r");	
		decoder(file_stream);
	}
}

