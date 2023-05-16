#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <termios.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int itoa_bin(uint64_t data, char *bin){
    if(bin == NULL)
        return -1;
    
    char *start = bin;

    int i = 64;
    while(i > 0){
        if(data & 0x1)
            *bin++ = 0x31;
        else
            *bin++ = 0x30;
 
        data >>= 1;
        i--;
    }
 
    *bin = 0;
    //reverse the order
    char *low, *high, temp;
    low = start, high = bin - 1;
    
    while(low < high){
        temp = *low;
        *low = *high;
        *high = temp;
 
        ++low; 
        --high;
    }
    return 0;
}

bool ends_with(string const &a, string const &b) {
    auto len = b.length();
    auto pos = a.length() - len;
    if (pos < 0)
        return false;
    auto pos_a = &a[pos];
    auto pos_b = &b[0];
    while (*pos_a)
        if (*pos_a++ != *pos_b++)
            return false;
    return true;
}

int getFileCount(char* filePath, bool bin, uint64_t width)
{
  uint64_t fileSize = -1;
  struct stat statbuff;
  if(stat(filePath, &statbuff) < 0){  
    return -1;
  }else{
    return bin ? statbuff.st_size/(width/8) : statbuff.st_size/(width+1);
  }
}

int readBin(char* filePath, uint64_t * ptr, uint64_t count, uint64_t width)
{
  ifstream in(filePath, ios::in | ios::binary);
  for(int idx = 0; idx < count; idx++){
    in.read((char *) (ptr+idx), width/8);
  }

  printf("reading bin data, count == %ld, width == %ldbits \n", in.gcount()/8, width);
  return 0;
}

int readTxt(char* filePath, uint64_t * ptr, uint64_t count, uint64_t width)
{
  // 1. get open fd:
  int fd = open(filePath, O_RDONLY);
  if(fd == -1)
  {
    printf("open %s error.\n", filePath);
    return -1;
  }

  // 2. read frame:
  char lineBuffer[width+1];

  for(int i = 0; i < count; i++){
    read(fd, lineBuffer, width+1);
    if((i == 0) && ((int)lineBuffer[width] != 10)) {
      printf("Please make sure your input txt file width is right!\n");
      return -1;
    }
    lineBuffer[width] = '\0';
    ptr[i] = strtoul(lineBuffer, NULL, 2);
  }

  printf("reading txt data, count == %ld, width == %ldbits \n", count, width);
  close(fd);
  return 0;
}

int readFile(char* filePath, uint64_t * ptr, uint64_t count, bool bin, uint64_t width)
{
  return bin ? readBin(filePath, ptr, count, width) : readTxt(filePath, ptr, count, width);
}

int saveBin(char* filePath, uint64_t * ptr, uint64_t count, uint64_t width)
{
  ofstream out(filePath, ios::out | ios::binary);
  for(int idx = 0; idx < count; idx++){
    out.write((char *) (ptr+idx), width/8);
  }  
  printf("saving bin data, count == %ld, width == %ldbits \n", count, width);
  return 0;
}

int saveTxt(char* filePath, uint64_t * ptr, uint64_t count, uint64_t width)
{
  int fd = open(filePath, O_RDWR | O_CREAT | O_TRUNC, 0777);
  char frameChar64bit[65];
  char frameChar[width+1];
  char endl[] = "\n";

  for(int idx = 0; idx < count; idx++){
    itoa_bin(*(ptr+idx),frameChar64bit);
    strncpy(frameChar, frameChar64bit+64-width, width+1);
    write(fd, frameChar, width);
    write(fd, endl, 1);
  }

  printf("saving txt data, count == %ld, width == %ldbits \n", count, width);

  close(fd);
  return 0;
}

int saveFile(char* filePath, uint64_t * ptr, uint64_t count, bool bin, uint64_t width)
{
  return bin ? saveBin(filePath, ptr, count, width) : saveTxt(filePath, ptr, count, width);
}