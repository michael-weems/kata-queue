#ifndef PARSE_H
#define PARSE_H

#define HEADER_MAGIC 0x4c4c4144

struct dbheader_t {
	unsigned int magic;
	unsigned short version;
	unsigned short queueSize;
	unsigned short kataCount;
	unsigned int filesize;
};

struct kata_t {
	char title[1024];
};

struct queued_t {
	int position;
	char title[1024];
};

int db_create_header(int fd, struct dbheader_t **headerOut);
int db_validate_header(int fd, struct dbheader_t **headerOut);
int db_write_file(int fd, struct dbheader_t *dbhdr, struct kata_t *katas, struct queued_t *queued);
int db_read_file(int fd, struct dbheader_t *dbhdr, struct kata_t **katasOut, struct queued_t **queuedOut); 

int kata_add(struct dbheader_t *dbhdr, struct kata_t *katas, char *addstring);
int kata_remove(struct dbheader_t *dbhdr, struct kata_t *katas, char *removestring);
void katas_print(struct dbheader_t *dbhdr, struct kata_t *katas);

int queue_enqueue(struct dbheader_t *dbhdr, struct queued_t *queued, char *kata);
int queue_deque(struct dbheader_t *dbhdr, struct queued_t *queued, char *dequeued);
void queue_print(struct dbheader_t *dbhdr, struct queued_t *queued); 

#endif
