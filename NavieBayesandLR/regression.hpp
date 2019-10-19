//
//  regression.hpp
//  TextClassifier
//
//  Created by Tuition.in on 10/1/19.
//  Copyright © 2019 Tuition.in. All rights reserved.
//

#ifndef regression_hpp
#define regression_hpp

#include <stdio.h>
#include<iostream>
#include<fstream>
#include<math.h>
#include <vector>
#include <map>

using namespace std;

class Regression
{
public:
    enum RegressionType{Regres, Classif};

    Regression(unsigned short* arrBow[2], unsigned int wordscount, unsigned int filescount, short ber);
    ~Regression();

    double gradientdescent(const double, const unsigned int, short valid = 70);

    void init_theta(void);
    void printTheta(void) const;

    double alpha(void) const;
    void set_alpha(const double);

    double lamda(void) const;
    void set_lamda(const double);

    virtual double cost();
    virtual double derivative(unsigned int feature);
    double accuracy_LR(unsigned short* arrBoW[2], unsigned int filescnt);
    

protected:
    vector<double> d_Theta;
    double d_Theta0[2];
    double *expfx;
    unsigned short **d_dset;
    short validation;
    short nBoW;

    unsigned int filescount;
    unsigned int wordscount;
    double d_alpha;
    double d_lamda;

};

#endif /* regression_hpp */
