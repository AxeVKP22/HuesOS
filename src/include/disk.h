struct FSHeader {
    char name[4];
    unsigned short tableStart;
    unsigned short tableSize;
};


extern void getDrive(char* drive);
extern int readSectors(int sectorsToRead, int startingSector, char drive, unsigned short segment, unsigned short offset); // return value is 0 if err, 1 if all good
extern int writeSectors(int sectorsToWrite, int startingSector,char drive, unsigned short segmentB, unsigned short offsetB);// return value is 0 if err, 1 if all good