#include "entry.h"

#include <stdio.h>
#include <string.h>
#include "utils/utils.h"

struct Entry create_entry(int key, char *str, int str_size)
{
	struct Entry new_entry;
	char death_descr[MAX_DEATH_DESCR];

	new_entry.key = key;
	new_entry.str = (char *) malloc(str_size+1);
	if (new_entry.str == NULL)
		return new_entry;
	sprintf(death_descr, "%s malloc failed", "create_e");
	DIE(new_entry.str == 0, death_descr);
	memcpy(new_entry.str, str, str_size + 1);
	return new_entry;
}
