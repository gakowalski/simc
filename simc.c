#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void die() {
	perror(NULL);
	exit(EXIT_FAILURE);
}

int main(int argc, char * argv[]) {
	FILE * xml;
	long xmlFileSize;
	long i;
	int colNumber;
	int colCounter;
	char * data;
	char * current;
	char * tmp;
	char * closed_tag;
	int empty_tags;
	
	if (argc == 1) {
		printf("SIMC.XML to CSV converter (C) grzegorz.kowalski@wit.edu.pl\n");
		printf("Usage: %s [filename] [options] > [output_filename]\n", argv[0]);
		printf("Options: \n\t--empty-tags\tEnable processing of empty tags\n");
		return EXIT_SUCCESS;
	}
	
	empty_tags = 0;
	
	if (argc == 3) {
		if (strcmp(argv[2], "--empty-tags") == 0) {
			empty_tags = 1;
		}
	}
	
	xml = fopen(argv[1], "rb");
	
	if (xml == NULL) die();
	if (fseek(xml, 0L, SEEK_END) != 0) die();
	if ((xmlFileSize = ftell(xml)) == -1) die();

	rewind(xml);
	
	if ((data = (char *) malloc(xmlFileSize + 1)) == NULL) die();
	data[xmlFileSize] = 0x00;
	if (fread(data, xmlFileSize, 1, xml) != 1) {
		printf("Error reading file: couldn't read it in one big chunk.\n");
		return EXIT_FAILURE;
	};
	
	colNumber	= 0;
	current		= strstr(data, "<row");
	tmp 		= strstr(current + 5, "</row>");
	while (current) {
		current = strstr(current + 4, "<col");
		if (current < tmp) {
			colNumber++;
		} else {
			break;
		}
	}
	
	if (empty_tags) {
		current = strstr(data, "\"/>");
		for (i = current - data; current && i < xmlFileSize; i = current - data) {
			memcpy(current, "\"></col>", 8);
			current = strstr(current, "\"/>");
		}
	}
	
	if (!colNumber) {
		printf("Error reading file: couldn't find <col> tags in first <row> tag!");
		return EXIT_FAILURE;
	}
	
	current = strstr(data, "<row>");
	if (!current) {
		printf("Error reading file: couldn't find <row> tag.\n");
		return EXIT_FAILURE;
	};
	
	for (i = current - data, colCounter = 1; i < xmlFileSize; colCounter++) {
		current = strstr(current, "\">");
		if (!current) break;
		current += 2;
		tmp = strstr(current, "</");
		if (!current) break;
		fwrite(current, tmp - current, 1, stdout);
		i += tmp - current;
		current = tmp;
		
		if (colCounter % colNumber) {
			fputc(';', stdout);
		} else {
			fputs("\n", stdout);
		}
	}
	
	fclose(xml);
	free(data);
	
	return EXIT_SUCCESS;
}
