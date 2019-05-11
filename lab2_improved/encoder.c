#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHIFT 0x7
#define OCTET 0x8
#define ZERO  0x30U
#define ONE  0x31U
#define KILO 0x4B0
#define WORDLEN 0xC

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
	char word[WORDLEN + 1];
	char *superarray = malloc(KILO);
	word[WORDLEN] = 0;
	int wordcount = 0;
	int controls = 4;
	FILE *out = fopen("original_code.txt", "w");
	int i = 0, kc = 1; 
	word[0] = word[1] = ZERO;


	for (int i = 0; text[i]; ++i) {
		for (int j = 2; j < WORDLEN; ++j) {
			if (is_power_of_two(j + 1)) {
				word[j++] = ZERO;
			}
			word[j] = ((unsigned char)text[i] >> SHIFT) + ZERO;
			text[i] = text[i] << 1;
		}
		if (wordcount * WORDLEN >= kc * KILO)
			superarray = realloc(superarray, ++kc * KILO);
		strcpy(superarray + wordcount * WORDLEN, word);
		wordcount += 1;
	}

	int p = 0, jump = 0, controlled = 0, block = 0;
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
			if (controlled % 2)
				*(superarray + i * WORDLEN + p - 1) = ONE;
			controlled = 0;
		}
	}
	fprintf(out, "%s", superarray);
}

