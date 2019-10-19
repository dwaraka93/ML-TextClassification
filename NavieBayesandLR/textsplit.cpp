#include "textsplit.hpp"

void read_file(const string &foldername, map<string,pair<int,int>>* words, unsigned int arr[][2]) {
  fstream infile;
  string line;
  vector<vector<int>> csv;
    short folderindex = 0;
    int   wordcount = 0;
    int   filecount = 0;
    struct dirent * dp;
    string hamspamfolder[2];
    map<string,pair<int,int>>::iterator itr;
    map<string,bool> vocablary;
    hamspamfolder[1] = foldername + "//ham";
    hamspamfolder[0] = foldername + "//spam";
    while(folderindex < 2)
    {
        DIR* dirp = opendir(hamspamfolder[folderindex].c_str());
        if(dirp == NULL)
        {
            cout << "Unable to open the directory:"<<hamspamfolder[folderindex]<< endl;
            return;
        }
        while ((dp = readdir(dirp)) != NULL) {
            if(!strncmp(dp->d_name,".",1) || !strncmp(dp->d_name, "..",2))
                continue;
            infile.open(hamspamfolder[folderindex] +"//" +dp->d_name,ios::in);
            if(!infile.is_open())
            {
                cout << "error opening file: "<< hamspamfolder[folderindex] <<"/"<< dp->d_name<< endl;
                return;
            }
            
            while(getline(infile, line)) {
                // read all the words into a vector
                char *token;
                char delims[] = " \t\r"; // space and tab
                token = strtok(&line[0], delims);
                while (token != NULL)
                {
                    if(validate_token(token))
                    {
                        token = strtok (NULL, delims);
                        continue;
                    }
                    itr = words[folderindex].find(token);
                    if(itr != words[folderindex].end())
                    {
                        itr->second.first++;
                        if(vocablary.find(token) == vocablary.end())
                        {
                            vocablary[token] = true;
                            itr->second.second++;
                        }
                    }
                    else
                    {
                        words[folderindex][token].first = 1;
                        words[folderindex][token].second = 1;
                        vocablary[token] = true;
                    }
                  wordcount++;
                  token = strtok (NULL, delims);
                }
            }//for each line in file
            infile.close();
            //cout << "file:" << dp->d_name<< "words:"<<words[folderindex].size()<<endl;
            vocablary.clear();
            filecount++;
        }
        arr[0][folderindex] = wordcount;
        arr[1][folderindex] = filecount;
        wordcount = 0;
        filecount = 0;
        closedir(dirp);
        folderindex++;
    }
}

void read_file_LR(const string &foldername, map<string,int> &words, unsigned short *arrBoW[2], unsigned int &FileCnt) {
  fstream infile;
  string line;
    short folderindex = 0;
    int   wordcount = 0;
    struct dirent * dp;
    string hamspamfolder[2];
    vector<int> classvar;
    map<int,pair<int,int>> documentstats;
    vector<map<int,pair<int,int>>> multidocuments;
    map<string,int>::iterator itr;
    map<string,bool> vocablary;
    hamspamfolder[1] = foldername + "//ham";
    hamspamfolder[0] = foldername + "//spam";
    while(folderindex < 2)
    {
        DIR* dirp = opendir(hamspamfolder[folderindex].c_str());
        while ((dp = readdir(dirp)) != NULL) {
            if(!strncmp(dp->d_name,".",1) || !strncmp(dp->d_name, "..",2))
                continue;
            infile.open(hamspamfolder[folderindex] +"//" +dp->d_name,ios::in);
            if(!infile.is_open())
            {
                cout << "error opening file: "<<dp->d_name<< endl;
                return;
            }
            
            while(getline(infile, line)) {
                // read all the words into a vector
                char *token;
                char delims[] = " \t\r"; // space and tab
                token = strtok(&line[0], delims);
                while (token != NULL)
                {
                    if(validate_token(token))
                    {
                        token = strtok (NULL, delims);
                        continue;
                    }
                    itr = words.find(token);
                    if(itr != words.end())
                    {
                        if(vocablary.find(token) != vocablary.end())
                            documentstats[itr->second].first++;
                        else
                        {
                            vocablary[token] = true;
                            documentstats[itr->second].first = 1;
                            documentstats[itr->second].second = 1;
                        }
                    }
                    else
                    {
                        words[token] = wordcount;
                        documentstats[wordcount].first = 1;
                        documentstats[wordcount].second = 1;
                        wordcount++;
                        vocablary[token] = true;
                    }
                  token = strtok (NULL, delims);
                }
            }//for each line in file
            infile.close();
            vocablary.clear();
            classvar.push_back(folderindex);
            multidocuments.push_back(documentstats);
            documentstats.clear();
        }
        closedir(dirp);
        folderindex++;
    }
    
    //unsigned short *arrBoW[2];
    arrBoW[0] = NULL;
    arrBoW[1] = NULL;
    arrBoW[0] = new unsigned short[(multidocuments.size()*(words.size()+1))*sizeof(short)];
    arrBoW[1] = new unsigned short[(multidocuments.size()*(words.size()+1))*sizeof(short)];
    if(arrBoW[0] == NULL)
        cout << "Memory allocation failed in storing bag of words matrix"<< endl;
    if(arrBoW[1] == NULL)
        cout << "Memory allocation failed in storing  matrix"<< endl;
    
    memset (arrBoW[0],0,(multidocuments.size()*(words.size() + 1))*sizeof(short));
    memset (arrBoW[1],0,(multidocuments.size()*(words.size() + 1))*sizeof(short));
    int index = 0;
    for (auto itr=multidocuments.begin(); itr != multidocuments.end(); itr++)
    {
        for(auto itr1= itr->begin(); itr1 !=itr->end();itr1++)
        {
            *(arrBoW[0] + index * (words.size()+1) + itr1->first) = itr1->second.first;
            *(arrBoW[1] + index * (words.size()+1) + itr1->first) = itr1->second.second;
            
        }
        *(arrBoW[0] + index*( words.size()+1) + words.size()) = classvar[index];
        *(arrBoW[1] + index*( words.size()+1) + words.size()) = classvar[index];
        index++;
    }
    FileCnt = (int)multidocuments.size();
    for(auto itr=multidocuments.begin(); itr != multidocuments.end(); itr++)
        itr->clear();
    multidocuments.clear();
    classvar.clear();
}


bool validate_token(char* token)
{
    bool isvalid = false;
    int i=0;
    char arry[4] = {'t','h','e','\0'};
    while(token[i] != '\0')
    {
        if(token[i]>= 'A' && token[i] <= 'z' ){
            isvalid = true;
            token[i] = tolower(token[i]);
        }
        else if(token[i] >= '0' && token[i] <= '9')
            isvalid = true;
        i++;
    }
    if(strncmp(arry,token,4) == 0) return true;
    if(isvalid && i>1) return false;
    return true;
}


bool validate_token_LR(char* token)
{
    bool isvalid = false;
    
    int i=0;
    
    while(token[i] != '\0')
    {
        if(token[i]>= 'A' && token[i] <= 'z' ){
            isvalid = true;
            token[i] = tolower(token[i]);
        }
        i++;
    }
    
    if(isvalid && i>3) return false;
    return true;
}


void read_testfile_LR(const string &foldername, map<string,int> &words, unsigned short *arrBoW[2], unsigned int &FileCnt) {
  fstream infile;
  string line;
    short folderindex = 0;
    struct dirent * dp;
    string hamspamfolder[2];
    vector<int> classvar;
    map<int,pair<int,int>> documentstats;
    vector<map<int,pair<int,int>>> multidocuments;
    map<string,int>::iterator itr;
    map<string,bool> vocablary;
    hamspamfolder[1] = foldername + "//ham";
    hamspamfolder[0] = foldername + "//spam";
    while(folderindex < 2)
    {
        DIR* dirp = opendir(hamspamfolder[folderindex].c_str());
        while ((dp = readdir(dirp)) != NULL) {
            if(!strncmp(dp->d_name,".",1) || !strncmp(dp->d_name, "..",2))
                continue;
            infile.open(hamspamfolder[folderindex] +"//" +dp->d_name,ios::in);
            if(!infile.is_open())
            {
                cout << "error opening file: "<<dp->d_name<< endl;
                return;
            }
            
            while(getline(infile, line)) {
                // read all the words into a vector
                char *token;
                char delims[] = " \t\r"; // space and tab
                token = strtok(&line[0], delims);
                while (token != NULL)
                {
                    if(validate_token(token))
                    {
                        token = strtok (NULL, delims);
                        continue;
                    }
                    itr = words.find(token);
                    if(itr != words.end())
                    {
                        if(vocablary.find(token) != vocablary.end())
                            documentstats[itr->second].first++;
                        else
                        {
                            vocablary[token] = true;
                            documentstats[itr->second].first = 1;
                            documentstats[itr->second].second = 1;
                        }
                    }
                  token = strtok (NULL, delims);
                }
            }//for each line in file
            infile.close();
            vocablary.clear();
            classvar.push_back(folderindex);
            multidocuments.push_back(documentstats);
            documentstats.clear();
        }
        closedir(dirp);
        folderindex++;
    }
    
    //unsigned short *arrBoW[2];
    arrBoW[0] = NULL;
    arrBoW[1] = NULL;
    arrBoW[0] = new unsigned short[(multidocuments.size()*(words.size()+1))*sizeof(short)];
    arrBoW[1] = new unsigned short[(multidocuments.size()*(words.size()+1))*sizeof(short)];
    if(arrBoW[0] == NULL)
        cout << "Memory allocation failed in storing bag of words matrix"<< endl;
    if(arrBoW[1] == NULL)
        cout << "Memory allocation failed in storing  matrix"<< endl;
    
    memset (arrBoW[0],0,(multidocuments.size()*(words.size() + 1))*sizeof(short));
    memset (arrBoW[1],0,(multidocuments.size()*(words.size() + 1))*sizeof(short));
    int index = 0;
    for (auto itr=multidocuments.begin(); itr != multidocuments.end(); itr++)
    {
        for(auto itr1= itr->begin(); itr1 !=itr->end();itr1++)
        {
            *(arrBoW[0] + index * (words.size()+1) + itr1->first) = itr1->second.first;
            *(arrBoW[1] + index * (words.size()+1) + itr1->first) = itr1->second.second;
            
        }
        *(arrBoW[0] +  index*(words.size() + 1) + words.size()) = classvar[index];
        *(arrBoW[1] + index*(words.size() + 1) + words.size()) = classvar[index];
        index++;
    }
    FileCnt = (int)multidocuments.size();
    for(auto itr=multidocuments.begin(); itr != multidocuments.end(); itr++)
        itr->clear();
    multidocuments.clear();
    classvar.clear();
}
