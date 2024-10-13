#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "common.h"
#include "parse.h"

int db_create_header(int fd, struct dbheader_t **headerOut) {
	struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
	if (header == -1) {
		printf("Malloc failed to create dbheader_t\n");
		return STATUS_ERROR;
	}
	header->magic = HEADER_MAGIC;
	header->version = 0x1;
	header->queueSize = 0;
	header->kataCount = 0;
	header->filesize = sizeof(struct dbheader_t);

	*headerOut = header;

	return STATUS_SUCCESS;
}
int db_validate_header(int fd, struct dbheader_t **headerOut) {
	if (fd < 0) {
		printf("Got a bad FD from the user\n");
		return STATUS_ERROR;
	}

	struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
	if (header == -1) {
		printf("Malloc failed to create a db header\n");
		return STATUS_ERROR;
	}

	// read the header out of the file, then ensure it is the correct size
	if(read(fd, header, sizeof(struct dbheader_t)) != sizeof(struct dbheader_t)) {
		perror("read");
		free(header);
		return STATUS_ERROR;
	}

	// unpack the header and convert it from "network-endian" to "host-endian"
	// Here: take the value stored at header->version and convert it to the host-endian type so our computer can actually use it correctly
	header->version = ntohs(header->version);
	header->queueSize = ntohs(header->queueSize);
	header->kataCount = ntohs(header->kataCount);
	header->magic = ntohl(header->magic);
	header->filesize = ntohl(header->filesize);

	if (header->magic != HEADER_MAGIC) {
		printf("Improper header magic\n");
		free(header);
		return STATUS_ERROR;
	}
	if (header->version != 1) {
		printf("Improper header version\n");
		free(header);
		return STATUS_ERROR;
	}

	struct stat dbstat = {0};
	fstat(fd, &dbstat);
	if (header->filesize != dbstat.st_size) {
		printf("Corrupted database\n");
		free(header);
		return STATUS_ERROR;
	}

	*headerOut = header;
}

int db_write_file(int fd, struct dbheader_t *dbhdr, struct kata_t *katas, struct queued_t *queued) {
	if (fd < 0) {
		printf("Got a bad FD from the user\n");
		return STATUS_ERROR;
	}

	// preserve count since we're transforming it below
	int realkatacount = dbhdr->kataCount;
	int realqueuesize = dbhdr->queueSize;
	int realfilesize = sizeof(struct dbheader_t) + (sizeof(struct kata_t) * realkatacount) + (sizeof(struct queued_t) * realqueuesize);

	dbhdr->magic = htonl(dbhdr->magic);
	dbhdr->version = htons(dbhdr->version);
	dbhdr->kataCount = htons(dbhdr->kataCount);
	dbhdr->queueSize = htons(dbhdr->queueSize);
	dbhdr->filesize = htonl(realfilesize);

	lseek(fd, 0, SEEK_SET);

	write(fd, dbhdr, sizeof(struct dbheader_t));

	int i = 0;
	for (; i < realkatacount; i++) {
		write(fd, &katas[i], sizeof(struct kata_t));
	}

	for (i = 0; i < realqueuesize; i++) {
		queued[i].position = htons(queued[i].position);
		write(fd, &queued[i], sizeof(struct queued_t));
	}

	// Removed entries should also reduce the filesize
	if (ftruncate(fd, realfilesize) < 0) {
		perror("truncate");
		return STATUS_ERROR;
	}

	return STATUS_SUCCESS;
}	

int db_read_file(int fd, struct dbheader_t *dbhdr, struct kata_t **katasOut, struct queued_t **queuedOut) {
	if (fd < 0) {
		printf("Got a bad FD from the user\n");
		return STATUS_ERROR;
	}

	// katas
	int kataCount = dbhdr->kataCount;
	struct kata_t *katas = calloc(kataCount, sizeof(struct kata_t));
	if (katas == -1) {
		printf("Calloc failed\n");
		return STATUS_ERROR;
	}

	// read the katas into the pointer
	read(fd, katas, kataCount*sizeof(struct kata_t));

	// queue
	int queueSize = dbhdr->queueSize;

	// read the queue into the pointer
	struct queued_t *queued = calloc(queueSize, sizeof(struct queued_t));
	if (queued == -1) {
		printf("Calloc failed\n");
		return STATUS_ERROR;
	}

	read(fd, queued, queueSize*sizeof(struct queued_t));

	for (int i = 0; i < queueSize; i++) {
		queued[i].position = ntohs(queued[i].position);
	}

	// TODO: sort the queue

	*katasOut = katas;
	*queuedOut = queued;
	return STATUS_SUCCESS;
}

int kata_add(struct dbheader_t *dbhdr, struct kata_t *katas, char *addstring){

	strncpy(katas[dbhdr->kataCount-1].title, addstring, sizeof(katas[dbhdr->kataCount-1].title));
	return STATUS_SUCCESS;
}
int kata_remove(struct dbheader_t *dbhdr, struct kata_t *katas, char *removestring){
	int i = 0;
	bool found = false;
	for (; i < dbhdr->kataCount; i++) {
		if (found == true) {
			// shift the array over one
			katas[i-1] = katas[i];
			continue;
		}

		if (strcmp(katas[i].title, removestring) != 0) {
			continue;
		}

		found = true;
	}
	if (found == false) {
		return STATUS_NOT_FOUND;
	}

	return STATUS_SUCCESS;
}
void katas_print(struct dbheader_t *dbhdr, struct kata_t *katas) {
	int i = 0;
	printf("katas:\n");
	for (; i < dbhdr->kataCount; i++) {
		printf("\t- %s\n", katas[i].title);
	} 
}

int queue_enqueue(struct dbheader_t *dbhdr, struct queued_t *queued, char *kata) {

	strncpy(queued[dbhdr->queueSize-1].title, kata, sizeof(queued[dbhdr->queueSize-1].title));
	queued[dbhdr->queueSize-1].position = dbhdr->queueSize;

	return STATUS_SUCCESS;
}

int queue_deque(struct dbheader_t *dbhdr, struct queued_t *queued, char *dequeued) {
	if (dbhdr->queueSize < 1) {
		return STATUS_ERROR;
	}

	strncpy(dequeued, queued[0].title, sizeof(queued[0].title));

	int i = 1;
	for (; i < dbhdr->queueSize; ++i) {
		queued[i-1] = queued[i];
		queued[i-1].position = i;
	}

	return STATUS_SUCCESS;
}

void queue_print(struct dbheader_t *dbhdr, struct queued_t *queued) {
	int i = 0;
	printf("queue:\n");
	for (; i < dbhdr->queueSize; i++) {
		printf("\t%d: %s\n", queued[i].position, queued[i].title);
	} 
}
