//
//  naviebayes.cpp
//  TextClassifier
//
//  Created by Tuition.in on 9/28/19.
//  Copyright © 2019 Tuition.in. All rights reserved.
//

#include "naviebayes.hpp"
#include "regression.hpp"
#include <algorithm>
#include <math.h>
#include <unordered_set>

double test_accuracy(const std::string &foldername,std::map<std::string, pair<int,int>> *vBoW, unsigned int wordscnt[][2], bool bBoW)
{
    fstream infile;
    struct dirent * dp;
    string line;
    std::string hamspamfolder[2];
    map<string, pair<int,int>>::iterator itr;
    map<string, int> words;
    map<string, int>::iterator itrwords;
    short truepositive = 0, truenegative=0, falsenegative = 0, falsepositive = 0;
    hamspamfolder[0] = foldername + "//spam";
    hamspamfolder[1] = foldername + "//ham";
    double probability[2] = {0,0};
    short folderindex = 0;
    unordered_set <string> stringSet;
    if(!bBoW)
    {
        for(auto i=vBoW[0].begin();i!=vBoW[0].end();i++)
        {
            stringSet.insert(i->first);
        }
        for(auto i=vBoW[1].begin();i!=vBoW[1].end();i++)
        {
            stringSet.insert(i->first);
        }
    }
    //cout <<"0:"<<vBoW[0].size()<< "  1:"<<vBoW[1].size()<<endl;
    
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
                return 0;
            }

            // read file line by line
            // assumption is newlines signify document breaks
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
                    words[token]++;
                  token = strtok (NULL, delims);
                }
            }//for each line in file
            infile.close();
            if(bBoW)
            {
                for(map<string, int>::iterator i=words.begin(); i != words.end(); i++)
                {
                    itr = vBoW[0].find(i->first);
                    if(itr != vBoW[0].end())
                        probability[0] += (double)i->second * log10(double(itr->second.first+1)/double(wordscnt[0][0] + vBoW[0].size()));
                    else
                        probability[0] += (double)i->second * log10(double(1)/double(wordscnt[0][0] + vBoW[0].size()));
                    
                    itr = vBoW[1].find(i->first);
                    if(itr != vBoW[1].end())
                        probability[1] += (double)i->second * log10(double(itr->second.first+1)/double(wordscnt[0][1] + vBoW[1].size()));
                    else
                        probability[1] += (double)i->second * log10(double(1)/double(wordscnt[0][1] + vBoW[1].size()));
                }
                probability[0] += log10(double(wordscnt[1][0]/double(wordscnt[1][0] + wordscnt[1][1])));
                probability[1] += log10(double(wordscnt[1][1]/double(wordscnt[1][0] + wordscnt[1][1])));
            }
            else
            {
                for (const string& x : stringSet)
                {
                    itrwords=words.find(x);
                    if(itrwords != words.end())
                    {
                        
                        for(short i=0;i<2;i++)
                        {
                            itr = vBoW[i].find(x);
                            if(itr !=vBoW[i].end())
                            {
                                //cout << i<<"-"<<x << "-" << double(itr->second.second+1)/double(wordscnt[1][i] + 2)<< endl;
                                probability[i] += log10(double(itr->second.second+1)/double(wordscnt[1][i] + 2));
                            }
                            else
                            {
                                //cout << i<<"-"<<x << "-" << double(1)/double(wordscnt[1][i] + 2)<< endl;
                                probability[i] += log10(double(1)/double(wordscnt[1][i] + 2));
                            }
                        }
                        //itrwords->second = -1;
                    }
                    else
                    {
                        for(short i=0;i<2;i++)
                        {
                            itr = vBoW[i].find(x);
                            if(itr !=vBoW[i].end())
                            {
                                //cout << i<<"-"<<x << "-" << double(1) - double(itr->second.second+1)/double(wordscnt[1][i] + 2)<< endl;
                                probability[i] += log10(double(1) - double(itr->second.second+1)/double(wordscnt[1][i] + 2));
                            }
                            else
                            {
                                //cout << i<<"-"<<x << "-" << double(1) - double(1)/double(wordscnt[1][i] + 2)<< endl;
                                probability[i] += log10(double(1) - double(1)/double(wordscnt[1][i] + 2));
                            }
                        }
                    }
                }
                probability[0] += log10(double(wordscnt[1][0]/double(wordscnt[1][0] + wordscnt[1][1])));
                probability[1] += log10(double(wordscnt[1][1]/double(wordscnt[1][0] + wordscnt[1][1])));
            }
            
            if(folderindex == 0)
            {
                if(probability[0] >= probability[1])
                    falsepositive++;
                else
                    falsenegative++;
            }
            
            else if(folderindex == 1)
            {
                if(probability[1] >= probability[0])
                    truepositive++;
                else
                    truenegative++;
            }
            probability[0] = 0;
            probability[1] = 0;
            words.clear();
        }
        closedir(dirp);
        folderindex++;
    }
    
    double accuracy = double(truepositive+falsepositive)/(truepositive+falsepositive + truenegative+falsenegative);
    double precision = double(truepositive)/(truepositive+falsepositive);
    cout << "precision :" << precision << endl;
    double  recall = double(truepositive)/(truepositive+falsenegative);
    cout << "recall :" << recall << endl;
    cout << "F1 Score :" << 2*(recall * precision) / (recall + precision) << endl;
    return (double)accuracy;
}


double logistic_regression(std::map<string,int> words,unsigned short *arrBoW[2],unsigned short *arrtestBoW[2], unsigned int &filescount, unsigned int &testfilescount,short ber)
{
    Regression logR(arrBoW, (int)words.size(), filescount, ber);

    //logR.set_lamda(0);
    //logR.set_alpha(0.01);
    logR.gradientdescent(0.0001,1000,70);
    
    logR.gradientdescent(0.0001,1000,30);
    
    logR.gradientdescent(0.0001,1000,100);
    
    return logR.accuracy_LR(arrtestBoW,testfilescount);
    
    //cout << endl << "F1_Score: " << logR.f1Score(d.XTest(), d.Test_oneHotMatrix(), true) << endl;
}
