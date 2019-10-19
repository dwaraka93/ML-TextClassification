//
//  main.cpp
//  TextClassifier
//
//  Created by Tuition.in on 9/26/19.
//  Copyright © 2019 Tuition.in. All rights reserved.
//

#include <iostream>
#include "textsplit.hpp"
#include "naviebayes.hpp"
#include "regression.hpp"

using namespace std;


int main(int argc, char**argv)
{
  if (argc < 4) {
    cout <<"Invalid number of command line arguments\n"
             <<"Expected format ./TextClassifier.out -options trainingdata testdata";
    return -1;
  }
    std::map<string,pair<int,int>> mBoW[2];
    unsigned int wordcount[2][2];
    std::map<string,int> words;
    unsigned short *arrBoW[2];
    unsigned int filescount = 0;
    unsigned int testfilescount = 0;
    unsigned short *arrtestBoW[2];
    double accuracy;
    
    if(!strcmp(argv[1],"-mn"))
    {
        
        read_file(argv[2], mBoW, wordcount);
        accuracy = test_accuracy(argv[3], mBoW, wordcount, true);
        cout << "Multinomial Navie Bayes accuracy:" <<accuracy << endl;
    }
    else if(!strcmp(argv[1],"-bn"))
    {
        read_file(argv[2], mBoW, wordcount);
        accuracy = test_accuracy(argv[3], mBoW, wordcount, false);
        cout << "Discerete Navie Bayes accuracy:" << accuracy <<endl;
    }
    else if(!strcmp(argv[1],"-mlr"))
    {
        cout << "started logistic regression"<< endl;
        read_file_LR(argv[2], words, arrBoW, filescount);
        read_testfile_LR(argv[3], words, arrtestBoW, testfilescount);
        accuracy = logistic_regression(words, arrBoW, arrtestBoW, filescount, testfilescount,0);
        cout << "Logistic regression accuracy using Bag of Words Model:" << accuracy << endl;
    }
    else if(!strcmp(argv[1],"-blr"))
    {
        cout << "started logistic regression"<< endl;
        read_file_LR(argv[2], words, arrBoW, filescount);
        read_testfile_LR(argv[3], words, arrtestBoW, testfilescount);
        accuracy = logistic_regression(words, arrBoW, arrtestBoW, filescount, testfilescount,1);
        cout << "Logistic regression accuracy using Bernouli Model:" << accuracy << endl;
    }
    else if(!strcmp(argv[1],"-sdg"))
    {
        read_file_LR(argv[2], words, arrBoW, filescount);
        read_testfile_LR(argv[3], words, arrtestBoW, testfilescount);
        fstream foutBoW;
        fstream foutBer;
        string strBoWfile = string(argv[2]) + "_BoW.csv";
        foutBoW.open(strBoWfile, ios::out);
        if(!foutBoW.is_open())
        {
            cout << "error creating file: "<<strBoWfile<< endl;
            return 0;
        }
        string strBerfile = string(argv[2]) + "_Bernouli.csv";
        foutBer.open(strBerfile, ios::out);
        if(!foutBer.is_open())
        {
            cout << "error creating file: "<<strBerfile<< endl;
            return 0;
        }
        for(int i=0;i<filescount;i++)
        {
            string bufferBoW;
            string bufferBer;
            for(int word = 0; word< words.size() ;word++)
            {
                bufferBoW.append(to_string(*(arrBoW[0] + i*(words.size()+1) + word)) + ",");
                bufferBer.append(to_string(*(arrBoW[1] + i*(words.size()+1) + word)) + ",");
                
            }
            bufferBoW.append(to_string(*(arrBoW[0] + i*(words.size()+1) + words.size())));
            bufferBer.append(to_string(*(arrBoW[1] + i*(words.size()+1) + words.size())));
            foutBoW << bufferBoW<< endl;
            foutBer << bufferBer<< endl;
        }
        foutBoW.close();
        foutBer.close();
        
        strBoWfile = string(argv[3]) + "_BoW.csv";
        foutBoW.open(strBoWfile, ios::out);
        if(!foutBoW.is_open())
        {
            cout << "error creating file: "<<strBoWfile<< endl;
            return 0;
        }
        strBerfile = string(argv[3]) + "_Bernouli.csv";
        foutBer.open(strBerfile, ios::out);
        if(!foutBer.is_open())
        {
            cout << "error creating file: "<<strBerfile<< endl;
            return 0;
        }
        for(int i=0;i<testfilescount;i++)
        {
            string bufferBoW;
            string bufferBer;
            for(int word = 0; word< words.size() ;word++)
            {
                bufferBoW.append(to_string(*(arrtestBoW[0] + i*(words.size()+1) + word)) + ",");
                bufferBer.append(to_string(*(arrtestBoW[1] + i*(words.size()+1) + word)) + ",");
                
            }
            bufferBoW.append(to_string(*(arrtestBoW[0] + i*(words.size()+1) + words.size())));
            bufferBer.append(to_string(*(arrtestBoW[1] + i*(words.size()+1) + words.size())));
            foutBoW << bufferBoW<< endl;
            foutBer << bufferBer<< endl;
        }
        foutBoW.close();
        foutBer.close();
        cout << "Bag fo Words and Bernouli files generation is completed. Use this files for in SDG classifer"<< endl;
    }
    else{
        cout <<"Invalid -options in command line arguments\n";
    }
    
  return 0;
}
