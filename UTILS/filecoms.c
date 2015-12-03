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

		printf("mode: %s\n", mode);

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

		OPENFILES[num_open_files] = found_dir;
		num_open_files++;
	}
	else {
		printf("%s is a directory\n", fileName);
	}

	return 0;
}

int sizeFile(char args[][ACOLS]){


	return -1;
}
