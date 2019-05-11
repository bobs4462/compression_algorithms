#include <encoder.c>

int vf = 0;

int verify (char *superarray, int controls, int codelen)
{
	int error_position = 0;
	int error = 0;
	int wordcount = codelen / WORDLEN;
	int p = 0, jump = 0, block = 0, controlled = 0;
	for (int i = 0; i < wordcount; ++i) {
		for (int j = 0; j < controls; ++j) {
			block = p = raise_to_power_of_two(j);
			jump = p - 1;
			while (jump < WORDLEN) {
				if (*(superarray + i * WORDLEN + jump) - ZERO)
					++controlled;
				if (--block)
					++jump;
				else {
					jump += (p + 1);
					block = p;
				}
			}
			if (controlled % 2) {
				error += 1;
				error_position += p;
				printf("ERROR POSTION %d IN WORD %d\n", p, i);
				controlled = 0;
			}
		}
		if (error) {
			printf("FLIPPING BIT AT POSTION %d in word %d\n", error_position, i);
			*(superarray + i * WORDLEN + error_position - 1) = (*(superarray + i * WORDLEN + error_position - 1) - ZERO) ? ZERO : ONE;
		}
		error_position = error = 0;
	}
}

int decoder(FILE *file_stream)
{
	FILE *message = fopen("decoded.txt", "w");
	char *code = malloc(KILO);
	unsigned char character;
	int kc = 2, i = 0, c, controls = 4;
	for (i = 0; (c = fgetc(file_stream)) != EOF; ++i)
		if (i < KILO * (kc - 1))
			code[i] = c;
		else {
			code = realloc(code, kc * KILO);
			code[i] = c;
			kc += 1;
		}

	code[--i] = 0;
	
	if (vf)
		verify(code, controls, i);

	int wordcount = i / WORDLEN;

	FILE *test = fopen("fixed_code.txt", "w");
	fprintf(test, "%s", code);

	for (int j = 0; j < wordcount; ++j) {
		for (int k = 0; k < WORDLEN; ++k) {
			if (is_power_of_two(k + 1)) {
				continue;
			}
			character <<= 1;
			character += *(code + j * WORDLEN + k) - ZERO;
		}
		fputc(character, message);
		character = 0;
	}

	fclose(message);
	fclose(file_stream);
}

