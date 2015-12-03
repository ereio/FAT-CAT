#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../global.h"
#include "UTILS/dircoms.h"
#include "UTILS/filecoms.h"
#include "UTILS/utils.h"

int openFile(char args[][ACOLS]) {
	char * fileName = malloc(sizeof(char) * strlen(args[1]));
	strcpy(fileName, args[1]);
	struct directory found_dir;
	found_dir = finddir_noattr(*fatcat.curDir, fileName);

	if (found_dir.name[0] == 0x00 || (found_dir.Attr & ATTR_LONG_NAME)) {
		printf("%s does not exist in this directory\n", fileName);
		return 0;
	}

	if (!(found_dir.Attr & ATTR_DIRECTORY)) {
		printf("found file %s\n", fileName);
	}
	else {
		printf("%s is a directory\n", fileName);
	}

	return 0;
}

int sizeFile(char args[][ACOLS]){


	return -1;
}
