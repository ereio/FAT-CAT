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

	// Temporary Allocation. Don't want to declare malloc yet be need to!
	unsigned short secsize[1];
	unsigned char secperclus[1];
	unsigned short rsvdseccnt[1];
	unsigned char numfats[1];
	unsigned short BPB_RootEntCnt[1];
	unsigned short BPB_TotSec16[1];
	unsigned int BPB_HiddSec[1];
	unsigned int BPB_TotSec32[1];
	unsigned int BPB_FATSz32[1];

	img = fopen(args[1], "r");

	fseek(img, 11, SEEK_SET);	// Seek_set sets the offset from the beginning of the file
								// Offset moves to NEXT Byte so offset of 10 would put you at 11
	fread(secsize,2,1,img);
	fread(secperclus,1,1,img);
	fread(rsvdseccnt,2,1,img);
	fread(numfats,1,1,img);
	fread(BPB_RootEntCnt,2,1,img);
	fread(BPB_TotSec16,2,1,img);

	fseek(img, 28, SEEK_SET);
	fread(BPB_HiddSec,4,1,img);
	fread(BPB_TotSec32,4,1,img);

	// FAT 32 Only structures start here at offset 36
	fseek(img, 28, SEEK_SET);

	printf("\n\nBytes Per Sector: %d\n", secsize[0]);

	printf("Sector Per Cluster: %d\n", secperclus[0]);

	printf("Reserved Sector Count: %d\n", rsvdseccnt[0]);

	printf("Number Of FATs: %d\n", numfats[0]);

	printf("Root Directory Entry Count: %d\n", BPB_RootEntCnt[0]);

	printf("Total Sector Count in 16Bit: %d\n", BPB_TotSec16[0]);

	printf("Total Hidden Sectors: %d\n", BPB_HiddSec[0]);

	printf("Total Sector Count in 32Bit: %d\n", BPB_TotSec32[0]);




	return 0;
}

int _covert(unsigned char * swap){



	return 0;
}
