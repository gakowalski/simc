#include <stdio.h>
#include <stdlib.h>

void die() {
	perror(NULL);
	exit(EXIT_FAILURE);
}

int main(int argc, char * argv[]) {
	FILE * xml;
	long xmlFileSize;
	long i;
	int colCounter;
	char * data;
	char * current;
	char * tmp;
	
	if (argc == 1) {
		printf("Usage: %s [filename]\n", argv[0]);
		return EXIT_SUCCESS;
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
	
	//xmlFileSize = 200;
	
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
		
		if (colCounter % 10) {
			fputc(';', stdout);
		} else {
			fputs("\n", stdout);
		}
	}
	
	fclose(xml);
	free(data);
	
	return EXIT_SUCCESS;
}
