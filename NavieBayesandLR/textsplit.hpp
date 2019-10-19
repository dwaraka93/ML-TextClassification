#ifndef CSV_HPP_INCLUDE
#define CSV_HPP_INCLUDE

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>

using namespace std;

void read_file(const std::string &filename,std::map<std::string, pair<int,int>> *vBow, unsigned int wordscnt[][2]);
void read_file_LR(const string &foldername, map<string,int> &words, unsigned short *arrBoW[2], unsigned int &filecnt);

void read_testfile_LR(const string &foldername, map<string,int> &words, unsigned short *arrBoW[2], unsigned int &FileCnt);

bool validate_token(char* str);
bool validate_token_LR(char* str);

class splitstring : public std::string {
    std::vector<std::string> flds;
public:
    splitstring(std::string s) : std::string(s) { };
    std::vector<std::string>& split(char delim, int rep=0);
};

#endif /* end of include guard: CSV_HPP_INCLUDE */

