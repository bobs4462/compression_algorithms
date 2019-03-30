#include <prefix.h>

void filedump(CODETABLE table, FILE *out)
{
	fwrite(table, table->tablesize, 1, out);
	for (int i = 0; i < table->distcharc; ++i)
		fwrite(table->codes[i].prefixc, table->codes[i].codelen, 1, out);
}


CODETABLE fileextract(FILE *in)
{
	unsigned int tsz;
	CODETABLE table = malloc(sizeof(struct codetable));
	fread(table, sizeof(struct codetable), 1, in);
	tsz = table->tablesize;
	free(table);
	rewind(in);
	table = malloc(tsz);	
	fread(table, tsz, 1, in);
	for (int i = 0; i < table->distcharc; ++i) {
		table->codes[i].prefixc = malloc(table->codes[i].codelen + 1);
		fread(table->codes[i].prefixc, table->codes[i].codelen, 1, in);
		table->codes[i].prefixc[table->codes[i].codelen] = 0;
	}
	return table;
}
