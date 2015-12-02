/* Reference
 * http://www.cplusplus.com/reference/cstdio/fopen/
 * http://www.cplusplus.com/reference/cstdio/fread/
 * http://www.cplusplus.com/reference/cstdio/fseek/
 * http://stackoverflow.com/questions/10375932/printing-the-hexadecimal-representation-of-a-char-array
 * */
#include "../global.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "dircoms.h"

int LoadImage(FILE * img) {

	int status = 0;

	printf("Loading BPB\n");
	status = LoadBPB(img);
	printf("Setting root directory\n");
	status = SetRootDir(img);

	if(status != 0 && status != 1) printf("\n\nLoadImage error: %d\n", status);

	return status;
}

int LoadBPB(FILE * img) {
	fseek(img, 11, SEEK_SET); // Seek_set sets the offset from the beginning of the file
							  // Offset moves to NEXT Byte so offset of 10 would put you at 11

	fread(&BPB_BytesPerSec, 2, 1, img);
	fread(&BPB_SecPerClus, 1, 1, img);
	fread(&BPB_RsvdSecCnt, 2, 1, img);
	fread(&BPB_NumFATs, 1, 1, img);
	fread(&BPB_RootEntCnt, 2, 1, img);
	fread(&BPB_TotSec16, 2, 1, img);

	fseek(img, 28, SEEK_SET);
	fread(&BPB_HiddSec, 4, 1, img);
	fread(&BPB_TotSec32, 4, 1, img);

	// FAT& 32 Only structures start here at offset 36
	fseek(img, 36, SEEK_SET);
	fread(&BPB_FATSz32, 4, 1, img);
	fread(&BPB_ExtFlags, 2, 1, img);
	fread(&BPB_FSVer, 2, 1, img);
	fread(&BPB_RootClus, 4, 1, img);
	fread(&BPB_FSInfo, 2, 1, img);
	fread(&BPB_BkBootSec, 2, 1, img);
	fread(&BPB_Reserved, 4, 3, img);	// note 12 bytes long
	fread(&BS_DrvNum, 1, 1, img);
	fread(&BS_Reserved1, 1, 1, img);

	fread(&BS_BootSig, 1, 1, img);
	fread(&BS_VolID, 4, 1, img);	// note 12 bytes long
	fread(&BS_VolLab, 1, 11, img);
	fread(&BS_FilSysType, 1, 8, img);

#ifdef _DEBUGGING_BOOT_SECT
	PrintBootSectInfo();
#endif

	return 0;
}

int PrintBootSectInfo() {

	printf("\n\nBytes Per Sector: %d\n", BPB_BytesPerSec);

	printf("Sector Per Cluster: %d\n", BPB_SecPerClus);

	printf("Reserved Sector Count: %d\n", BPB_RsvdSecCnt);

	printf("Number Of FATs: %d\n", BPB_NumFATs);

	printf("Root Directory Entry Count: %d\n", BPB_RootEntCnt);

	printf("Total Sector Count in 16Bit: %d\n", BPB_TotSec16);

	printf("Total Hidden Sectors: %d\n", BPB_HiddSec);

	printf("Total Sector Count in 32Bit: %d\n", BPB_TotSec32);

	printf("Sectors Occupied (32-bit count): %d\n", BPB_FATSz32);

	printf("Various External Flags: %d\n", BPB_ExtFlags);

	printf("Version Number: %d\n", BPB_FSVer);

	printf("Cluster Number of Root Dir !: %d\n", BPB_RootClus);

	printf("Sector Number of Info struct: %d\n", BPB_FSInfo);

	printf("Sector number of reserved boot record copy: %d\n", BPB_BkBootSec);

	printf("Reserved for future expansion:");

	for (int i = 0; i < 3; i++)
		printf("%d -", BPB_Reserved[i]);

	printf("\nIndicates media type for drive(0x00 is floppy): %d\n", BS_DrvNum);

	printf("Used by windows NT lol: %d\n", BS_Reserved1);

	printf("Indicates if following values are present (ID, LAB): %d\n",
			BS_BootSig);

	printf("Volume Serial ID Number: %d\n", BS_VolID);

	printf("Volume Serial Label:");

	for (int i = 0; i < 11; i++)
		printf("%d", BS_VolLab[i]);

	printf("\n File System Type (indeterminate info only):");
	for (int i = 0; i < 8; i++)
		printf("%c", BS_FilSysType[i]);

	printf("\n");
	return 0;
}

/*
 * Note also that the CountofClusters value is exactly that—the count of data clusters starting at cluster
2. The maximum valid cluster number for the volume is CountofClusters + 1, and the “count of
clusters including the two reserved clusters” is CountofClusters + 2.
*/
int SetRootDir(FILE * img) {

	struct cluster temp;

	fatcat.rootDirSectors = ((BPB_RootEntCnt * 32) + (BPB_BytesPerSec - 1)) / BPB_BytesPerSec;

	fatcat.firstDataSector = BPB_RsvdSecCnt + (BPB_NumFATs * BPB_FATSz32) + fatcat.rootDirSectors;

	fatcat.dataSectors = BPB_TotSec32 -(BPB_RsvdSecCnt + (BPB_NumFATs * BPB_FATSz32) + fatcat.rootDirSectors);

	fatcat.dataClusters = fatcat.dataSectors / BPB_SecPerClus;

	if(fatcat.dataClusters < 65526) return -1;

	temp = FindClusterInfo(BPB_RootClus);

	memcpy(fatcat.curDir->cluster, &temp, sizeof(struct cluster));

	fatcat.dirName = "/";

	printf("\n\nRoot Directory Address: %d", fatcat.rootDirSectors);
	printf("\nFirst Data Sector: 0x%x", fatcat.firstDataSector);
	printf("\nTotal Data Sectors: %d", fatcat.dataSectors);
	printf("\nTotal Data Clusters: %d\n\n", fatcat.dataClusters);

	return 1;
}

unsigned int FindFirstSector(unsigned int cluster){
	unsigned int firstSector = 0;

	firstSector = ((cluster - 2) * BPB_SecPerClus) + fatcat.firstDataSector;

	return firstSector;
}

struct cluster FindClusterInfo(unsigned int cluster){
	struct cluster info;
	unsigned int fatOffset = 0;
	unsigned long next_cluster = 0;
	unsigned long byte_addr;
	int more = 1; 				// Indicates there are more clusters in chain
	info.clusterNum = 0;

	while(more){
		fatOffset = cluster * 4;
		// Finds first data sector for cluster / cluster chain node
		info.firstSectors[info.clusterNum] = FindFirstSector(cluster);
		info.sectorNums[info.clusterNum] = BPB_RsvdSecCnt + (fatOffset / BPB_BytesPerSec);	// sector of cluster chain info
		info.entryOffset[info.clusterNum] = fatOffset % BPB_BytesPerSec;					// offset of cluster chain info

		// address found using the sector and offset
		byte_addr = (info.sectorNums[info.clusterNum] * BPB_BytesPerSec) + info.entryOffset[info.clusterNum];

		// goes to address to see if another cluster contains information in a cluster chain
		fseek(fatcat.img, byte_addr, SEEK_SET);
		fread(&next_cluster, 4, 1, fatcat.img);

		// check to see if the End of Cluster Chain value was found
		more = next_cluster != EOC;
		if(more) cluster = next_cluster;

#ifdef _DEBUGGING_F
		printf("\nFatOffset: 0x%x", fatOffset);
		printf("\nThisFATSecNum: 0x%x", info.sectorNums[info.clusterNum]);
		printf("\nThisFATEntOff: 0x%x", info.entryOffset[info.clusterNum]);
		printf("\nThisFATSecNum Address: 0x%lx\n", byte_addr);
		printf("\nEOC hit, value == 0x%lx", next_cluster);
#endif
		info.clusterNum++;
	}

	return info;
}

void GetClusterData(unsigned int cluster){
/*	unsigned long DWORD = 0;
	char* data = 0;
	struct cluster info = FindClusterInfo(cluster);*/

}

void SetClusterData(unsigned int cluster){


}
