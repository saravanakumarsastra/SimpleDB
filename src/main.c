#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "file.h"
#include "parse.h"
#include "common.h"

void print_usage(char *cmd){
	printf("Usage %s -n -f <database file>\n", cmd);
	printf("\t -n - create new database file\n");
	printf("\t -f - (required) path to database\n");
	return;
}

int main(int argc, char *argv[]){

	bool newfile = false;
	char *filepath = NULL;
	int c;
	int dbfd = -1;

	struct dbheader_t *header = NULL;

	while ((c = getopt(argc, argv, "nf:")) != -1) {
		switch (c) {
			case 'n':
				newfile = true;
				break;
			case 'f':
				filepath = optarg;
				break;
			case '?':
				printf("Unknown option -%c\n", c);
				break;
			default:
				return -1;

		}
	}

	if(filepath == NULL){
		printf("File path is a required argument\n");
		print_usage(argv[0]);
		return 0;
	}

	if (newfile) {
		dbfd = create_db_file(filepath);
		if (dbfd == STATUS_ERROR) {
			printf("Unable to create database\n");
			return -1;
		}

		if(create_db_header(dbfd, &header) == STATUS_ERROR){
			printf("Failed to create database header \n");
			return -1;
		}
	} else {
		dbfd = open_db_file(filepath);
		if(dbfd == STATUS_ERROR) {
			printf("Unable to open database file\n");
			return -1;
		}

		if (validate_db_header(dbfd, &header) == STATUS_ERROR) {
			printf("Failed to validate database header\n");
			return -1;
		}
	}

	output_file(dbfd, header);


	return 0;
}
