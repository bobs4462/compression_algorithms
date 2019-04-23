#include <stdio.h>
#include <stdlib.h>

#define SHIFT 0x7
#define OCTET 0x8
#define ZERO  0x30U
#define ONE  0x31U
#define KILO 0x3E8

int raise_to_power_of_two(int power)
{
	int result = 1;
	while (power--)
		result *= 2;
	return result;
}

int is_power_of_two(int num)
{
	int original = num;
	int power = 0;
	for (power = 0; num / 2; num /= 2, ++power);
	while (power--)
		num *= 2;
	if (original == num)
		return 1;
	else return 0;
}

int encoder(char *text)
{
	int bitcount = 2;
	int controls = 0;
	FILE *out = fopen("output.txt", "w");
	int i = 0; fputc(ZERO, out); fputc(ZERO, out);

	for (int i = 0; text[i]; ++i) {
		for (int j = 0; j < OCTET; ++j) {
			if (is_power_of_two(bitcount + 1)) {
				fputc(ZERO, out);
				++bitcount;
			}
			fputc(((unsigned char)text[i] >> SHIFT) + ZERO, out);
			text[i] = text[i] << 1;
			++bitcount;
		}
	}

	fflush(out);
	
	while (bitcount / 2) {
		++controls;
		bitcount /= 2;
	}

	int p = 0, jump = 0, controlled = 0, block = 0, c;

	for (int i = 0; i <= controls; ++i) {
		block = p = raise_to_power_of_two(i);
		jump = p - 1;
		while (fseek(out, jump, SEEK_SET)) {
			if (c = (fgetc(out) - ZERO))
				if (feof(out)) {
					break;
				}
				else if (c)
					controlled += 1;
			if (--block) 
				jump += 1;
			else {
				jump += (p + 1);
				block = p;
			}
		}
		clearerr(out);

		fseek(out, p - 1, SEEK_SET);

		if (controlled % 2)
			fputc(ONE, out);
		controlled = 0;
	}
	fclose(out);
}

int encoder_v2(char *text)
{
	char *array = malloc(KILO);
	int bitcount = 2;
	int controls = 0;
	FILE *out = fopen("output.txt", "w");
	int i = 0, kc = 1; 
	array[0] = array[1] = ZERO;


	for (int i = 0; text[i]; ++i) {
		for (int j = 0; j < OCTET; ++j) {
			if (bitcount >= kc * KILO)
				array = realloc(array, ++kc * KILO);
			if (is_power_of_two(bitcount + 1)) {
				array[bitcount] = ZERO;
				++bitcount;
			}
			array[bitcount] = ((unsigned char)text[i] >> SHIFT) + ZERO;
			text[i] = text[i] << 1;
			++bitcount;
		}
	}
	array[bitcount] = 0;

	int t = bitcount;	
	while (t / 2) {
		++controls;
		t /= 2;
	}

	int p = 0, jump = 0, controlled = 0, block = 0, c;

	for (int i = 0; i <= controls; ++i) {
		block = p = raise_to_power_of_two(i);
		jump = p - 1;
		while (jump < bitcount) {
			if (array[jump] - ZERO)
				controlled += 1;
			if (--block) 
				jump += 1;
			else {
				jump += (p + 1);
				block = p;
			}
		}

		if (controlled % 2)
			array[p - 1] = ONE;
		controlled = 0;
	}
	fprintf(out, "%s", array);
}

