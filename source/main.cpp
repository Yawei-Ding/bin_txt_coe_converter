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
#include <getopt.h>

using namespace std;

bool ends_with(string const &a, string const &b);
int getFileCount(char* filePath, bool bin, uint64_t width);
int readFile(char* filePath, uint64_t * ptr, uint64_t count, bool bin, uint64_t width);
int saveFile(char* filePath, uint64_t * ptr, uint64_t count, bool bin, uint64_t width);

uint64_t Count = 0;
uint64_t in_w  = 0;
uint64_t out_w = 0;
char* input_path    = NULL;
char* output_path     = NULL;
bool input_isbin    = 0;
bool output_isbin     = 0;

int parse_args(int argc, char *argv[]) {
    const struct option table[] = {
        {"input"   , required_argument, NULL, 'i'},
        {"output"  , required_argument, NULL, 'o'},
        {"in_w"    , required_argument, NULL, 'a'},
        {"out_w"   , required_argument, NULL, 'b'},
        {0         , 0                , NULL,  0 },
    };
    int o;
    while ( (o = getopt_long(argc, argv, "-d:i:", table, NULL)) != -1) {
        switch (o) {
            case 'i': input_path  = optarg; input_isbin  = ends_with((string const)input_path , ".bin"); break;
            case 'o': output_path = optarg; output_isbin = ends_with((string const)output_path, ".bin"); break;
            case 'a': in_w  = atoi(optarg); break;
            case 'b': out_w = atoi(optarg); break;
        }
    }
    return 0;
}

int main(int argc, char** argv, char** env) {

  parse_args(argc,argv);

  Count = getFileCount(input_path, input_isbin, in_w);
  uint64_t *ptr = new uint64_t[Count];
  readFile(input_path, ptr, Count, input_isbin, in_w);
  saveFile(output_path, ptr, Count, output_isbin, out_w);

  delete ptr;

  return 0;
}
