#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../global.h"
#include "UTILS/dircoms.h"
#include "UTILS/filecoms.h"
#include "UTILS/utils.h"

int openFile(char args[][ACOLS]) {
	// Parse through entries in directory
	// If exists && is file, add to open file table (OPENFILES, extern)
	// Else if directory, print error
	// Else print error
	char * fileName = args[1][0];
	// *** WARNING ***
	// Directory organization needs to be discussed before proceeding
	unsigned long byte_addr = fatcat.curDir->cluster->firstSectors[0] * BPB_BytesPerSec;
	unsigned char free = 0x01;
	int endofdir = 0;
	int linecount = 1;
	int i = 0;
	int found_match = 0;
        struct directory curdir;

        while(!endofdir){
                linecount %= 8;
                curdir = parsedir(byte_addr, 0);
                if(checkdirerr(curdir)) return -1;
                free = curdir.name[0];

                if(free == 0xE5 || free == 0x00){
                        endofdir = 1;
                } else if (strstr(curdir.name, fileName) != NULL) {
			if (curdir.Attr & ATTR_DIRECTORY) {
				printf("Error: '%s' is a directory\n", fileName);
			}
			else {
				found_match = 1;
			}
			break;
		} else {
                        if(linecount % 8 == 0) printf("\n");
                        byte_addr += 32;
                        linecount++;
                }
        }

	if (found_match) {
		for (i = 0; i < 1000; i++) {
			if (OPENFILES[i].name[0] == '\0') {
				OPENFILES[i] = curdir;
				break;
			}
		}
	}
	else {
		printf("Error: no file '%s' found\n", fileName);
	}

	return 0;
}

int sizeFile(char args[][ACOLS]){


	return -1;
}
