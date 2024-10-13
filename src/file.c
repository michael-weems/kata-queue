#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "file.h"
#include "common.h"

bool exists_db_file(char *filename) {
	int fd = open(filename, O_RDWR);
	if (fd != -1) {
		close(fd);
		return true;
	}
	close(fd);
	return false;
}

int create_db_file(char *filename) {
	if (exists_db_file(filename)) {
		printf("File already exists\n");
		return STATUS_ERROR;
	}

	int fd = open(filename, O_RDWR | O_CREAT, 0644);
	if (fd == -1) {
		perror("open");
		return STATUS_ERROR;
	}

	return fd;
}

int open_db_file(char *filename) {
	int fd = open(filename, O_RDWR, 0644);
	if (fd == -1) {
		perror("open");
		return STATUS_ERROR;
	}

	return fd;
}

