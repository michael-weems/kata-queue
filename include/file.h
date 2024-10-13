#ifndef DB_FILE_H
#define DB_FILE_H

#include <stdbool.h>

bool exists_db_file(char *filename);
int create_db_file(char *filename);
int open_db_file(char *filename);

#endif
