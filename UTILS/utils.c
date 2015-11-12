/* Reference
 * http://www.cplusplus.com/reference/cstdio/fopen/
 * http://www.cplusplus.com/reference/cstdio/fread/
 * http://www.cplusplus.com/reference/cstdio/fseek/
 *
 * */
#include "../global.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

int LoadImage(char* args[]){

	int status = 0;

	status = LoadBPB(args);

	return status;

}

int LoadBPB(char* args[]){

	FILE * img;
	int i = 0;
	unsigned char b[4];

	img = fopen(args[1], "r");
	fseek(img, 11, SEEK_SET);	// Seek_set sets the offset from the beginning of the file
	fread(b,2,1,img);

	for(i=0; i < 2; i++)
		printf("%d", b[i]);

	return 0;
}
