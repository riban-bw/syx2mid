#include "stdio.h"

/**  Write a variable length size to file */
int writeVarLen(unsigned int nLen, FILE *pFile) {
  unsigned char acBuffer[10];
  int nSize;
  acBuffer[0] = (nLen & 0x7F);
  nLen >>= 7;
  for(nSize = 1; nSize < 10; ++nSize) {
    acBuffer[nSize] = (nLen & 0x7F) | 0x80;
    nLen >>= 7;
    if(nLen == 0)
      break;
  }

  for(int i = nSize; i >= 0; --i)
    fputc(acBuffer[i], pFile);
  return nSize + 1;
}

/**  Write a 4 byte integer MSB first to file */
void writeChunkLen(unsigned int nLen, FILE *pFile) {
  for(int i=3; i >= 0; --i)
    fputc((nLen >> (i * 8)) & 0xFF, pFile);
}


int main(int argc, char** argv) {
  if(argc != 2) {
    printf("Usage: syx2midi filename.syx\n");
    return -1;
  }
  FILE *pSyx;
  FILE *pMid;
  pSyx = fopen(argv[1], "r");
  if(!pSyx) {
    printf("Failed to open file \"%s\"\n", argv[1]);
    return -1;
  }
  pMid = fopen("sysex.mid", "w");
  if(!pMid) {
    printf("Failed to open output file \"sysex.mid\"\n");
    fclose(pSyx);
    return -1;
  }
  fseek(pSyx, 0L, SEEK_END);
  unsigned int nSyxLen = ftell(pSyx);
  rewind(pSyx);
  printf("Sysex file is %d bytes long\n", nSyxLen);
  // Write file header
  fputs("MThd", pMid);
  writeChunkLen(6, pMid);
  fputc(0, pMid); // MIDI format = 0
  fputc(0, pMid);
  fputc(0, pMid); // Quantity of tracks = 1
  fputc(1, pMid);
  fputc(0, pMid); // Time division (don't care for just sysex)
  fputc(0, pMid);
  // Write track header
  fputs("MTrk", pMid);
  writeChunkLen(nSyxLen, pMid);
  fputc(0, pMid); // Variable length quantity for delta time = 0
  // Start of sysex data
  fputc(0xF7, pMid);
  int nQuant = writeVarLen(nSyxLen, pMid); // Variable length quantity - sysex data length

  // Check for start and end Sysex commands within file
  int bPrefixSOS = 0;
  int bAppendEOS = 0;
  char c = getc(pSyx);
  if(c != (char)0xF0) {
    // Sysex file does not include Start Of Sysex command
    ++nSyxLen;
    bPrefixSOS = 1;
  }
  fseek(pSyx, -1L, SEEK_END);
  c = getc(pSyx);
  if(c != (char)0xF7) {
    ++nSyxLen;
    bAppendEOS = 1;
  }
  rewind(pSyx);
  
  // Fix chunk header (length)
  fseek(pMid, 0x12, SEEK_SET);
  writeChunkLen(nSyxLen + 2 + nQuant, pMid);
  fseek(pMid, 0x18 + nQuant, SEEK_SET);

  // Copy data from syx file
  if(bPrefixSOS)
    fputc(0xF0, pMid);
  c = getc(pSyx);
  while(c != EOF) {
    fputc(c, pMid);
    c = fgetc(pSyx);
  }
  if(bAppendEOS)
    fputc(0xF7, pMid);

  
  fclose(pSyx);
  fclose(pMid);
  printf("MIDI file created\n");
  return 0;
}
