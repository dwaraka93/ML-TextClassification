//
//  naviebayes.hpp
//  TextClassifier
//
//  Created by Tuition.in on 9/28/19.
//  Copyright © 2019 Tuition.in. All rights reserved.
//

#ifndef naviebayes_hpp
#define naviebayes_hpp
#include "textsplit.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>

double test_accuracy(const std::string &filename,std::map<std::string, pair<int,int>> *vBow, unsigned int wordscnt[][2], bool bBoW);

double logistic_regression(std::map<string,int> words,unsigned short *arrBoW[2],unsigned short *arrtestBoW[2], unsigned int &filescount, unsigned int &testfilescount,short ber);

#endif /* naviebayes_hpp */
