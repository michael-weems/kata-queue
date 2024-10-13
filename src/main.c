#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
	printf("Usage: %s <action>\n", argv[0]);
	printf("Usage: %s -f <database filepath>\n", argv[0]);
	printf("\t -f  - (required) path to database file\n");
	printf("\t -n  - Create new database file\n");
	printf("\t -a  - Text description of a new kata to add to the database\n");
	printf("\t -r  - Exact-match of text description of a kata to remove from the database\n");
	printf("\t -g  - Generate a new queue of katas\n");
	printf("\t -d  - Dequeue the next kata to perform\n");
	printf("\t -q  - Print the queue of katas\n");
	printf("\t -k  - List all katas in the database\n");
	printf("\t -e  - Empty the queue\n");
	printf("\t -w  - Wipe the all katas from the database\n");
	return;
}

int main(int argc, char *argv[]) { 

	// inputs
	char *filepath = NULL;
	bool newDatabase = false;
	char *addKataString = NULL;
	char *removeKataString = NULL;
	bool generateQueue = false;
	bool dequeueKata = false;
	bool printQueue = false;
	bool printKatas = false;
	bool emptyQueue = false;
	bool wipeKatas = false;

	int c;

	int dbfd = -1;
	struct dbheader_t *dbhdr = NULL;
	struct kata_t *katas = NULL;
	struct queued_t *queued = NULL;

	while ((c = getopt(argc, argv, "f:na:r:gdqkew")) != -1) {
		switch (c) {
			case 'f':
				filepath = optarg;	
				break;
			case 'n':
				newDatabase = true;	
				break;
			case 'a':
				addKataString = optarg;	
				break;
			case 'r':
				removeKataString = optarg;	
				break;
			case 'g':
				generateQueue = true;
				break;
			case 'd':
				dequeueKata = true;
				break;
			case 'q':
				printQueue = true;
				break;
			case 'k':
				printKatas = true;
				break;
			case 'e':
				emptyQueue = true;
				break;
			case 'w':
				wipeKatas = true;
				break;
			case '?':
				printf("Unknown option -%c\n", c);
				break;		
			default: 
				return -1;
		};
	}

	if (filepath == NULL) {
		print_usage(argv);
		printf("error: -f is a required parameter\n");
		return -1;

	}

	if (newDatabase){
		dbfd = create_db_file(filepath);	
		if (dbfd == -1) {
			printf("error: create database file (%s)\n", filepath);
			return -1;
		}

		if (db_create_header(dbfd, &dbhdr) == STATUS_ERROR) {
			printf("error: create database header\n");
			return -1;
		}
	} else {
		dbfd = open_db_file(filepath);	
		if (dbfd == -1) {
			printf("error: open database file (%s)\n", filepath);
			return -1;
		}

		if (db_validate_header(dbfd, &dbhdr) == STATUS_ERROR) {
			printf("error: validate database header\n");
			return -1;
		}
	}

	if (db_read_file(dbfd, dbhdr, &katas, &queued) != STATUS_SUCCESS) {
		printf("error: read database into memory\n");
		return 0;
	}

	if (emptyQueue) {
		dbhdr->queueSize = 0;
		queued = realloc(queued, dbhdr->queueSize*(sizeof(struct queued_t)));
	}
	if (wipeKatas) {
		dbhdr->kataCount = 0;
		katas = realloc(katas, dbhdr->kataCount*(sizeof(struct kata_t)));
	}

	if (addKataString != NULL) {
		dbhdr->kataCount++;

		katas = realloc(katas, dbhdr->kataCount*(sizeof(struct kata_t)));

		if (kata_add(dbhdr, katas, addKataString) == STATUS_ERROR) {
			printf("error: add kata to database (kata: %s)\n", addKataString);
			return -1;
		}
	}
	if (removeKataString != NULL) {
		int status = kata_remove(dbhdr, katas, removeKataString); 
		if (status == STATUS_ERROR) {
			printf("error: remove kata (%s)\n", removeKataString);
			return -1;
		} 
		if (status == STATUS_NOT_FOUND) {
			printf("error: remove kata (%s): not found\n", removeKataString);
		}
		if (status == STATUS_SUCCESS) {
			dbhdr->kataCount--;
			katas = realloc(katas, dbhdr->kataCount*(sizeof(struct kata_t)));
		}
	}

	if (generateQueue) {
		int newSize = 5;
		if (dbhdr->kataCount == 0) {
			printf("error: generate queue: no katas in database\n");
			return -1;
		}
		if (dbhdr->kataCount < 5) {
			newSize = dbhdr->kataCount;
		}

		dbhdr->queueSize = 0;
		queued = realloc(queued, dbhdr->queueSize*(sizeof(struct queued_t)));
		// TODO: realloc error handling

		// TODO: randomly select the katas for the queue, not just the first 5
		for (int i = 0; i < newSize; ++i) {
			dbhdr->queueSize++;
			queued = realloc(queued, dbhdr->queueSize*(sizeof(struct queued_t)));
			if (queue_enqueue(dbhdr, queued, katas[i].title) == STATUS_ERROR){
				printf("error: enqueue kata (%s)\n", katas[i].title);
				return -1;
			}
		}
	}

	if (printKatas) {
		katas_print(dbhdr, katas);
	}

	if (printQueue) {
		queue_print(dbhdr, queued);
	}

	if (dequeueKata) {
		char *kata;
		int status = queue_deque(dbhdr, queued, kata); 
		if (status == STATUS_ERROR) {
			printf("error: deque kata: queue empty\n");
			return -1;
		} 
		if (status == STATUS_SUCCESS) {
			dbhdr->queueSize--;
			queued = realloc(queued, dbhdr->queueSize*(sizeof(struct queued_t)));
			printf("you shall now: %s\n", kata);
		}
	}

	db_write_file(dbfd, dbhdr, katas, queued);
	return 0;
}
