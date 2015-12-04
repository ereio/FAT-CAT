#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../global.h"
#include "UTILS/dircoms.h"
#include "UTILS/filecoms.h"
#include "UTILS/utils.h"

int num_open_files = 0;

int openFile(char args[][ACOLS]) {
	char * fileName = malloc(sizeof(char) * strlen(args[1]));
	strcpy(fileName, args[1]);
	struct directory found_dir;
	found_dir = finddir(*fatcat.curDir, ATTR_ALL, fileName);

	if (found_dir.name[0] == 0x00 || (found_dir.Attr & ATTR_LONG_NAME)) {
		printf("%s does not exist in this directory\n", fileName);
		return 0;
	}

	if (!(found_dir.Attr & ATTR_DIRECTORY)) {
		char * mode = malloc(sizeof(char) * strlen(args[2]));
		strcpy(mode, args[2]);

		if (!strcmp(mode, "r"))
			found_dir.Mode = R;
		else if (!strcmp(mode, "rw") || !strcmp(mode, "wr"))
			found_dir.Mode = RW;
		else if (!strcmp(mode, "w"))
			found_dir.Mode = W;
		else {
			printf("Incorrect mode or missing argument\n");
			return 0;
		}

		if (checkFile(found_dir) != -1) {
			printf("File %s already open\n", fileName);
			return 0;
		}

		OPENFILES[num_open_files] = found_dir;
		num_open_files++;
	}
	else {
		printf("%s is a directory\n", fileName);
	}

	return 0;
}

// returns -1 if not open
// returns R for open - read
// returns RW for open - read/write
// returns W for open - write
int checkFile(struct directory dir) {
	int i;
	for (i = 0; i < num_open_files; i++) {
		if (OPENFILES[i].FstClusHi == dir.FstClusHi &&
				OPENFILES[i].FstClusLO == dir.FstClusLO) {
			return OPENFILES[i].Mode;
		}
	}

	return -1;
}

void removeOpenFile(struct directory dir) {
	int i;
	int j;
	int k;
	struct directory tmp;

	for (i = 0; i < num_open_files; i++) {
		if (OPENFILES[i].FstClusHi == dir.FstClusHi &&
				OPENFILES[i].FstClusLO == dir.FstClusLO) {

			for (j = i; j < num_open_files; j++) {
				for (k = j + 1; k < num_open_files; k++) {
					OPENFILES[j] = OPENFILES[k];
				}
			}

			OPENFILES[num_open_files] = tmp;

			num_open_files--;
			return;
		}
	}

	return;
}

void printFiles() {
	int i;
	for (i = 0; i < num_open_files; i++) {
		printf("%s, ", OPENFILES[i].name);
	}

	printf("\n");
}

void closeFile(char args[][ACOLS]) {
	char * fileName = malloc(sizeof(char) * strlen(args[1]));
	struct directory found_dir;

	found_dir = finddir(*fatcat.curDir, ATTR_ALL, fileName);
	strcpy(fileName, args[1]);

	if (found_dir.name[0] == 0x00 || (found_dir.Attr & ATTR_LONG_NAME)) {
		printf("%s does not exist in this directory\n", fileName);
		return;
	}

	if (!(found_dir.Attr & ATTR_DIRECTORY)) {
		if (checkFile(found_dir) != -1) {
			removeOpenFile(found_dir);
			return;
		}

		printf("File %s is not open\n", fileName);
	}
	else {
		printf("%s is a directory\n", fileName);
	}

	return;
}

int sizeFile(char args[][ACOLS]){
	char * fileName = malloc(sizeof(char) * strlen(args[1]));
	strcpy(fileName, args[1]);
	struct directory found_dir;
	found_dir = finddir(*fatcat.curDir, ATTR_ALL, fileName);
	if (found_dir.name[0] == 0x00 || (found_dir.Attr & ATTR_LONG_NAME)) {
		printf("%s does not exist in this directory\n", fileName);
		return 0;
	}

	if (!(found_dir.Attr & ATTR_DIRECTORY)) {
		printf("%d\n", found_dir.FileSize);
	}
	else {
		printf("%s is a directory\n", fileName);
	}

	return -1;
}

int rmFile(char args[][ACOLS]){
	struct directory found_dir;

	char * fileName = malloc(sizeof(char) * strlen(args[1]));

	strcpy(fileName, args[1]);
	found_dir = finddir(*fatcat.curDir, ATTR_ALL, fileName);

	if (found_dir.name[0] == 0x00 || (found_dir.Attr & ATTR_LONG_NAME)) {
		printf("%s does not exist in this directory\n", fileName);
		return 0;
	}

	if (!(found_dir.Attr ^ ATTR_DIRECTORY)) {
		printf("%s is a directory\n", fileName);
		return 0;
	}

	if(found_dir.name[0] != ' '){
		freecluschain(found_dir);

	}

	return 0;
}







