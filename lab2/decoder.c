#include <encoder.c>

int vf = 0;

int verify (char *code, int controls, int codelen)
{
	int error_position = 0;
	int error = 0;
	int p = 0, jump = 0, block = 0, controlled = 0;
	for (int i = 0; i <= controls; ++i) {
		block = p = raise_to_power_of_two(i);
		jump = p - 1;
		while (jump < codelen) {
			if (code[jump] - ZERO)
				controlled += 1;
			if (--block) 
				jump += 1;
			else {
				jump += (p + 1);
				block = p;
			}
		} 
		if (controlled % 2) {
			error = 1;
			error_position += p;
			printf("ERROR POSTION %d\n", p);
		}
		controlled = 0;
	}
	if (error) { 
		printf("ERROR OCCURRED\n");
		printf("FLIPPING BIT AT POSTION %d\n", error_position);
		code[error_position - 1] = (code[error_position - 1] - ZERO) ? '0' : '1';
	}

}

int decoder(FILE *file_stream)
{
	FILE *message = fopen("message.txt", "w");
	char *code = malloc(KILO);
	unsigned char character;
	int kc = 2, i = 0, c;
	for (i = 0; (c = fgetc(file_stream)) != EOF; ++i)
		if (i < KILO * (kc - 1))
			code[i] = c;
		else {
			code = realloc(code, kc * KILO);
			code[i] = c;
			kc += 1;
		}

	code[--i] = 0;
	
	
	int controls = 0, t = i - 1;
	
	while (t / 2) {
		++controls;
		t /= 2;
	}
	printf("CONTROLS %d\n", controls);
	if (vf)
		verify(code, controls, i);

	FILE *test = fopen("testwhole.txt", "w");
	fprintf(test, "%s", code);
	
    for (int j = 0, k = 0; code[j]; ++j) {
		if (is_power_of_two(j + 1)) {
			continue;
		}
		if (k < OCTET) {	
			character <<= 1;
			character += code[j] - ZERO;
			k += 1;
		}
		else {
			fputc(character, message);
			character = 0;
			k = 0;
			--j;
		}
	}
	fclose(message);
	fclose(file_stream);
}

