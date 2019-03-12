#pragma once

#include <stdint.h>

struct Entry {
	int64_t key;
	char *str;
};

struct Entry create_entry(int key, char *str, int str_size);
char *entry_to_str(struct Entry entry);
