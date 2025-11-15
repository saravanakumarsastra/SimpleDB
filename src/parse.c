#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "common.h"
#include "parse.h"

void output_file(int fd, struct dbheader_t *dbhdr){
	printf("Success until here");
	if(fd < 0){
		printf("Got a bad FD from the user");
		return STATUS_ERROR;
	}
	
	dbhdr->version = htons(dbhdr->version);
	dbhdr->count = htons(dbhdr->version);
	dbhdr->magic = htonl(dbhdr->magic);
	dbhdr->filesize = htonl(dbhdr->filesize); 

	
	fseek(fd, 0, SEEK_SET);
	write(fd, dbhdr, sizeof(struct dbheader_t));

	return;
}

int validate_db_header(int fd, struct dbheader_t **headerOut) {
	if(fd < 0){
		printf("Got a bad FD from the user");
		return STATUS_ERROR;
	}

	struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
	if (header == -1){
		printf("Malloc failed to create a db header");
		return STATUS_ERROR;
	}

	if (read(fd, header, sizeof(struct dbheader_t)) != sizeof(struct dbheader_t)) {
		printf("This line got executed\n");
		perror("read");
		free(header);
		return STATUS_ERROR;
	}

	header->version = ntohs(header->version);
	header->count = ntohs(header->version);
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
		printf("Corrupted database \n");
		free(header);
		return STATUS_ERROR;
	}

	*headerOut = header;
}

int create_db_header(int fd, struct dbheader_t **headerOut){
	struct dbheader_t *db_header = calloc(1, sizeof(struct dbheader_t));
	if (db_header == -1){
		printf("Malloc failed to create DB header\n");
		return STATUS_ERROR;
	}

	db_header->version = 0x01;
	db_header->count = 0;
	db_header->magic = HEADER_MAGIC;
	db_header->filesize = sizeof(struct dbheader_t);

	*headerOut = db_header;
	
	return STATUS_SUCCESS;

}

