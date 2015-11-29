
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../global.h"
#include "UTILS/dircoms.h"
#include "UTILS/utils.h"

#define DIR_Name 11
#define DIR_Attr 1
#define DIR_NTRes 1
#define DIR_CrtTimeTenth 1
#define DIR_CrtTime 2
#define DIR_CrtDate 2
#define DIR_LstAccDate 2
#define DIR_FstClusHI 2
#define DIR_WrtTime 2
#define DIR_WrtDate 2
#define DIR_FstClusLO 2
#define DIR_FileSize 4

int printDirectory(char args[][ACOLS]){
	int status = 0;

	if(args[1][0] != 0){
		printf("Args[1] == %d\n", args[1][0]);
		return 0;
	} else {
		status = parseContents(fatcat.currentSector);
	}

	return status;
}

int parseContents(unsigned long sector){
	unsigned long byte_addr = sector * BPB_BytesPerSec;
	unsigned char free = 0x01;
	int endofdir = 0;
	int linecount = 1;
	struct directory curdir;

	while(!endofdir){
		linecount %= 8;
		curdir = parseDirectory(byte_addr);
		if(ErrorCheckDirectory(curdir)) return -1;
		free = curdir.name[0];

		if(free == 0xE5 || free == 0x00){
			endofdir = 1;
		} else {
			if(linecount % 8 == 0) printf("\n");
			byte_addr += 32;
			linecount++;
		}
	}

	printf("\n");
	return 0;
}

struct directory parseDirectory(unsigned long byte_addr){
	struct directory dir;

	fseek(fatcat.img, byte_addr, SEEK_SET);

	fread(&dir.name, DIR_Name, 1, fatcat.img);
	fread(&dir.Attr, DIR_Attr, 1, fatcat.img);
	fread(&dir.NTRes, DIR_NTRes, 1, fatcat.img);
	fread(&dir.CrtTimeTenth, DIR_CrtTimeTenth, 1, fatcat.img);
	fread(&dir.CrtTime, DIR_CrtTime, 1, fatcat.img);
	fread(&dir.CrtDate, DIR_CrtDate, 1, fatcat.img);
	fread(&dir.LstAccDate, DIR_LstAccDate, 1, fatcat.img);
	fread(&dir.FstClusHi, DIR_FstClusHI, 1, fatcat.img);
	fread(&dir.WrtTime, DIR_WrtTime, 1, fatcat.img);
	fread(&dir.WrtDate, DIR_WrtDate, 1, fatcat.img);
	fread(&dir.FstClusLO, DIR_FstClusLO, 1, fatcat.img);
	fread(&dir.FileSize, DIR_FileSize, 1, fatcat.img);

#ifdef  _DEBUGGING
	printf("\nByte Address Read: 0x%07lx", byte_addr);
	PrintDirVerbose(dir);
#else
	PrintDirStandard(dir);
#endif

	return dir;
}

int ErrorCheckDirectory(struct directory dir){

	if(dir.name[0] == 0x20) return -1;
	for(int i=0; i < 11; i++){
		if( dir.name[i] == 0x22 ||
			dir.name[i] == 0x2A ||
			dir.name[i] == 0x2B ||
			dir.name[i] == 0x2C ||
			dir.name[i] == 0x2E ||
			dir.name[i] == 0x2F ||
			dir.name[i] == 0x3A ||
			dir.name[i] == 0x3B ||
			dir.name[i] == 0x3C ||
			dir.name[i] == 0x3D ||
			dir.name[i] == 0x3E ||
			dir.name[i] == 0x3F ||
			dir.name[i] == 0x5B ||
			dir.name[i] == 0x5C ||
			dir.name[i] == 0x5D ||
			dir.name[i] == 0x7C ){
			printf("\nInvalid Char: %x", dir.name[i]);
			return -2;
		}
	}
	return 0;
}

void PrintDirVerbose(struct directory dir){
	printf("\nDIR_Name:%s", dir.name);
	printf("\nDIR_Attr: 0x%x", dir.Attr);
	printf("\nDIR_NTRes: 0x%x", dir.NTRes);
	printf("\nDIR_CrtTimeTenth:0x%x", dir.CrtTimeTenth);
	printf("\nDIR_CrtTime: 0x%x", dir.CrtTime);
	printf("\nDIR_CrtDate: 0x%x", dir.CrtDate);
	printf("\nDIR_LstAccDate: 0x%x", dir.LstAccDate);
	printf("\nDIR_FstClusHI: 0x%x", dir.FstClusHi);
	printf("\nDIR_WrtTime: 0x%x", dir.WrtTime);
	printf("\nDIR_WrtDate 0x:%x", dir.WrtDate);
	printf("\nDIR_FstClusLO: 0x%x", dir.FstClusLO);
	printf("\nDIR_FileSize:0x%x\n", dir.FileSize);
}

void PrintDirStandard(struct directory dir){
	printf("%-15.15s",dir.name);
}
