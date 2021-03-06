#include <stdio.h>
#include <ctype.h>

#include "utils/utils.h"
#include "priority_queue.h"

#define MAX_LINE_LEN 20000
#define MAX_TOKENS 100

int tokenize(char *line, char **tokens, int *tokens_len)
{
	char delim[] = " \t\n\r";
	char *tok;
	int i;

	for (i = 0, tok = strtok(line, delim);
			i < MAX_TOKENS && tok != NULL;
			i++, tok = strtok(NULL, delim))
		tokens[i] = tok;
	*tokens_len = i;

	return 0;
}

int is_integer(char *str)
{
	if (*str == '-')
		str++;
	while (*str != '\0') {
		if (!isdigit(*str))
			return 0;
		str++;
	}
	return 1;
}

int is_word(char *str)
{
	while (*str != '\0') {
		if (!isalpha(*str))
			return 0;
		str++;
	}
	return 1;
}

int interpret_and_execute(char **toks, int toks_len, PriorityQueue *pqueue,
		char **result)
{
	int ret_code;

	*result = NULL;
	if (strcmp(toks[0], "insert") == 0 && toks_len == 3) {
		if (is_word(toks[1]) && is_integer(toks[2])) {
			ret_code = insert(pqueue, toks[1], atoi(toks[2]));
			if (ret_code != 0)
				return ret_code;
		}
	} else if (strcmp(toks[0], "top") == 0 && toks_len == 1) {
		*result = top(pqueue);
	} else if (strcmp(toks[0], "pop") == 0 && toks_len == 1) {
		pop(pqueue);
	} else if (strcmp(toks[0], "_print") == 0)
		println(pqueue);

	return 0;
}

int main(int argc, char **argv)
{
	char **fnv = NULL;
	int fnc = 0;
	FILE *in = NULL;

	char line[MAX_LINE_LEN];
	char *fgets_ret;
	int line_no = 0;

	char *toks[MAX_TOKENS];
	int toks_len;
	char *res;
	int ret_code;

	char ddescrfgets[MAX_DEATH_DESCR];

	PriorityQueue *pq = create_priority_queue();

	if (pq == NULL)
		return ALLOC_FAIL;

	sprintf(ddescrfgets, "%s:file reading fgets", "mf");

	if (argc == 1) {
		in = stdin;
		fnc = 1;
	} else {
		fnv = argv + 1;
		fnc = argc - 1;
	}

	for (; fnc > 0; fnc--) {
		if (in != stdin) {
			in = fopen(*fnv++, "r");
			if (in == NULL)
				continue;
		}
		do {
			fgets_ret = fgets(line, MAX_LINE_LEN, in);
			line_no++;
			pq->line_no = line_no;
			if (!feof(in)) {
				DIE(fgets_ret == NULL, ddescrfgets);
				tokenize(line, toks, &toks_len);
				ret_code = interpret_and_execute(toks,
						toks_len, pq, &res);
				if (ret_code != 0)
					return ret_code;
				if (res != NULL)
					printf("%s\n", res);
			}
		} while (!feof(in));
		if (in != stdin)
			fclose(in);
	}

	delete_priority_queue(pq);

	return 0;
}
