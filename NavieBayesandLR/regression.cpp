//
//  regression.cpp
//  TextClassifier
//
//  Created by Tuition.in on 10/1/19.
//  Copyright © 2019 Tuition.in. All rights reserved.
//

#include "regression.hpp"

Regression::Regression(unsigned short *arrBoW[2], unsigned int wordcount,  unsigned int filescnt, short ber)
{
    
    d_Theta0[0] = 0;
    d_Theta0[1] = 0;
    if(!filescnt)
    {
        cerr << "Regression: Regression class." << endl
             << "Regression(const DataSet&, const char*) constructor" << endl
             << "Dataset size: " << filescount << " cannot be 0." << endl;

        exit(1);
    }
    filescount = filescnt;
        if(!wordcount)
        {
            cerr << "Regression: Regression class." << endl
                 << "Regression(const DataSet&, const char*) constructor" << endl
                 << "Feature size: " << wordcount << " of the dataset cannot be 0." << endl;

            exit(1);
        }
    wordscount = wordcount;
    d_dset = arrBoW;
    
    //--Initialize theta matrix with uniform random--//
    d_Theta.reserve(wordcount);
    expfx = new double [filescnt];
    //--Default 𝛼 and λ parameter values--//
    d_alpha = 0.001;
    d_lamda = 0.0;
    validation = 100;
    nBoW = ber;
}


Regression::~Regression()
{
    d_Theta.clear();
    delete [] expfx;
}


double Regression::cost()
{
    double allcost = 0;
    double tempcost = 0;
    /*if(d_Theta.size()<=0)
    {
        cerr << "Regression: LogisticRegression class." << endl
             << "double cost() method" << endl
        << "weights vector is empty"<< endl;
        exit(1);
    }*/


    if(d_dset == NULL)
    {
        cerr << "Regression: LogisticRegression class." << endl
             << "double cost(mat&, const mat&) const method" << endl
        << "the dataset is null"<< endl;
        exit(1);
    }
    double d_thetasq = 0;
    for(unsigned int words=0;words<wordscount;words++)
    {
        d_thetasq += d_Theta[words] * d_Theta[words];
    }
    
    if(validation == 100)
    {
        for(unsigned int files=0;files<filescount;files++)
        {
            for(unsigned int words=0;words<wordscount;words++)
            {
                tempcost += double(*(d_dset[nBoW] + (wordscount+1)*files + words)) * d_Theta[words];
            }
            tempcost += d_Theta0[0];
            
            double e = 2.718281828;
           /* if (tempcost >= 10){
                expfx[files] =  pow(e, -10);
            }else if (tempcost <= -10){
                expfx[files] =  pow(e, 10);
            }
            else*/
                expfx[files] = pow(e, tempcost);
            
            //expfx[files] = exp(tempcost);
            allcost +=  tempcost * double(*(d_dset[nBoW] + (wordscount+1)*files + wordscount)) - log(1.0 + expfx[files]) - d_lamda*d_thetasq/2;
            
            tempcost = 0;
        }
    
    }
    else if(validation == 30)
    {
        for(unsigned int files=0;files<filescount;files = files + 3)
               {
                       for(unsigned int words=0;words<wordscount;words++)
                       {
                           tempcost += double(*(d_dset[nBoW] + (wordscount+1)*files + words)) * d_Theta[words];
                       }
                       tempcost += d_Theta0[0];
                       
                       double e = 2.718281828;
                      /* if (tempcost >= 10){
                           expfx[files] =  pow(e, -10);
                       }else if (tempcost <= -10){
                           expfx[files] =  pow(e, 10);
                       }
                       else*/
                           expfx[files] = pow(e, tempcost);
                       
                       //expfx[files] = exp(tempcost);
                       allcost +=  tempcost * double(*(d_dset[nBoW] + (wordscount+1)*files + wordscount)) - log(1.0 + expfx[files]) - d_lamda*d_thetasq/2;
                       
                       tempcost = 0;
                   
               }
            
    }
    else{
        for(unsigned int files=0;files<filescount;files++)
                     {
                         if(files%3 != 0)
                         {
                             for(unsigned int words=0;words<wordscount;words++)
                             {
                                 tempcost += double(*(d_dset[nBoW] + (wordscount+1)*files + words)) * d_Theta[words];
                             }
                             tempcost += d_Theta0[0];
                             
                             double e = 2.718281828;
                            /* if (tempcost >= 10){
                                 expfx[files] =  pow(e, -10);
                             }else if (tempcost <= -10){
                                 expfx[files] =  pow(e, 10);
                             }
                             else*/
                                 expfx[files] = pow(e, tempcost);
                             
                             //expfx[files] = exp(tempcost);
                             allcost +=  tempcost * double(*(d_dset[nBoW] + (wordscount+1)*files + wordscount)) - log(1.0 + expfx[files]) - d_lamda*d_thetasq/2;
                             
                             tempcost = 0;
                         }
                     }
    }
    
    
    
    //--        1  m                                 --//
    //--J(Ө) = --- ∑ [-y'⁽i⁾ log(h_Ө(x⁽i⁾))], ∀ j = 0--//
    //--        m  i                                 --//

    //--        1  m                            λ   n                 --//
    //--J(Ө) = --- ∑ [-y'⁽i⁾ log(h_Ө(x⁽i⁾))] + ---- ∑(Ө_j)^2, ∀ j >= 1--//
    //--        m  i                            2m  j                 --//

    //cost = ((-1.0/m) * accu(Y.t() % log(h_theta))) + ((d_lamda / (2.0*m)) * (accu(theta % theta)));

    return allcost;
}


double Regression::derivative(unsigned int feature)
{
    double derviate= 0;
    
    if(validation == 100)
    {
        for(int files = 0;files<filescount;files++)
        {
            derviate += double(*(d_dset[nBoW] + (wordscount+1)*files + feature)) *
            (double(*(d_dset[nBoW] + (wordscount+1)*files + wordscount)) - (expfx[files]/(1+expfx[files]))) -
            d_lamda*d_Theta[feature];
        }
    }
    else if(validation == 30)
    {
        for(int files = 0;files<filescount; files = files + 3)
        {
                derviate += double(*(d_dset[nBoW] + (wordscount+1)*files + feature)) *
                           (double(*(d_dset[nBoW] + (wordscount+1)*files + wordscount)) - (expfx[files]/(1+expfx[files]))) -
                           d_lamda*d_Theta[feature];
        }
    }
    else{
        for(int files = 0;files<filescount;files++)
        {
            if(files%3 != 0)
            {
                derviate += double(*(d_dset[nBoW] + (wordscount+1)*files + feature)) *
                (double(*(d_dset[nBoW] + (wordscount+1)*files + wordscount)) - (expfx[files]/(1+expfx[files]))) -
                d_lamda*d_Theta[feature];
            }
            
        }
    }

    

   // mat h_theta;

   // h_theta = sigmoid(X * d_Theta);

    //--            _                              _          --//
    //--  ∂J(Ө)    |  m                             |         --//
    //-- ------- = |  ∑ [h_Ө(x⁽i⁾) - y⁽i⁾] (x_j)⁽i⁾ |, ∀ j = 0--//
    //--   ∂Θ_j    |_ i                            _|         --//

    //--            _                                     _           --//
    //--  ∂J(Ө)    |  m                                    |          --//
    //-- ------- = |  ∑ [h_Ө(x⁽i⁾) - y⁽i⁾] (x_j)⁽i⁾ + λӨ_j |, ∀ j >= 1--//
    //--   ∂Θ_j    |_ i                                   _|          --//

    //DeltaTheta = (X.t() * (h_theta - Y.t())) + (d_lamda * theta);

    return derviate;
}


double Regression::gradientdescent(const double delta, const unsigned int max_iter = 0, short valid)
{
    vector<double> temp_theta;
    double c = 0;
    double c_prev= 0;
    unsigned int it=0;
    double derivate0 = 0;
    validation = valid;
    temp_theta.resize(wordscount);
    
    //--Calculating pretrained cost of the dataset--//
    c = cost();
    
        if(validation == 100)
        {
            do
            {
            for(int files=0;files<filescount;files++)
            {
                derivate0 += (double(*(d_dset[nBoW] + (wordscount+1)*files + wordscount)) - (expfx[files]/(1+expfx[files])));
            }
            d_Theta0[1] = d_Theta0[0] + (d_alpha * derivate0);
            derivate0 = 0;
            
            //--               𝛼   ∂J(Ө)  --//
            //-- Θ_j := Θ_j - --- ------- --//
            //--               m   ∂Θ_j   --//
            for(unsigned int weights = 0;weights<wordscount; weights++)
                temp_theta[weights] = d_Theta[weights] + (d_alpha * derivative(weights));

            d_Theta = temp_theta;
            d_Theta0[0] = d_Theta0[1];
            c_prev = c;
            c = cost();
            it++;
            //cout << it++ << " " << c << endl;
            }while(fabs(c_prev - c) > delta && (max_iter ? ((it <= max_iter) ? true : false) : true));
        }
        else if(validation == 30)
        {
            double max_cost = -10000000000;
            double final_lam = 0;
            double arrlam[] = {0.001, 0.003, 0.005,0.007,0.009, 0.1, 0.3, 0.5, 0.7, 0.9, 1,3,5,7,9};
            for(int lam = 0; lam < 15;lam++)
            {
                d_lamda = arrlam[lam];
                it = 0;
            /*do
            {
            for(int files=0;files<filescount;files = files + 3)
            {
                    derivate0 += (double(*(d_dset[nBoW] + (wordscount+1)*files + wordscount)) - (expfx[files]/(1+expfx[files])));
            }
            d_Theta0[1] = d_Theta0[0] + (d_alpha * derivate0);
            derivate0 = 0;
            
            //--               𝛼   ∂J(Ө)  --//
            //-- Θ_j := Θ_j - --- ------- --//
            //--               m   ∂Θ_j   --//
            for(unsigned int weights = 0;weights<wordscount; weights++)
                temp_theta[weights] = d_Theta[weights] + (d_alpha * derivative(weights));

            d_Theta = temp_theta;
            d_Theta0[0] = d_Theta0[1];*/
            c_prev = c;
            c = cost();
            //it++;
            //cout << it++ << " " << c << endl;
           // }while(fabs(c_prev - c) > delta && (max_iter ? ((it <= max_iter) ? true : false) : true));
                if(c > max_cost)
                {
                    max_cost= c;
                    final_lam = d_lamda;
                }
            }
            d_lamda = final_lam;
        }
        else{
            do
            {
            for(int files=0;files<filescount;files++)
            {
                if(files%3 != 0)
                {
                    derivate0 += (double(*(d_dset[nBoW] + (wordscount+1)*files + wordscount)) - (expfx[files]/(1+expfx[files])));
                }
            }
            d_Theta0[1] = d_Theta0[0] + (d_alpha * derivate0);
            derivate0 = 0;
            
            //--               𝛼   ∂J(Ө)  --//
            //-- Θ_j := Θ_j - --- ------- --//
            //--               m   ∂Θ_j   --//
            for(unsigned int weights = 0;weights<wordscount; weights++)
                temp_theta[weights] = d_Theta[weights] + (d_alpha * derivative(weights));

            d_Theta = temp_theta;
            d_Theta0[0] = d_Theta0[1];
            c_prev = c;
            c = cost();
            it++;
            //cout << it++ << " " << c << endl;
            }while(fabs(c_prev - c) > delta && (max_iter ? ((it <= max_iter) ? true : false) : true));
        }

    /*cout << endl << "Finished training. Training details:"
         << endl << "Iterations: " << it
         << endl << "Delta_J(Theta): " << fabs(c_prev - c)
         << endl << "J(Theta): " << c << endl;*/

    return c;
}


void Regression::printTheta(void) const
{
    cout << endl << "Theta:" << endl;
    for(unsigned int i=0; i<wordscount; i++)
    {
        cout << "theta["<< i << "]:" << d_Theta[i] << endl;
        //for(unsigned int j=0; j<d_Theta.n_cols; j++)
        //{
           // cout << "theta[" << i << "," << j << "]:" << d_Theta(i,j) << endl;
        //}
    }
}


double Regression::alpha(void) const
{
    return d_alpha;
}


void Regression::set_alpha(const double alpha)
{
    if(alpha <= 0.0)
    {
        cerr << "Regression: Regression class." << endl
             << "void set_alpha(const double) method" << endl
             << "Alpha: "<< alpha  << " must be > 0." << endl;

        exit(1);
    }
    else
    {
        d_alpha = alpha;
    }
}


double Regression::lamda(void) const
{
    return d_lamda;
}


void Regression::set_lamda(const double lamda)
{
    if(lamda < 0.0)
    {
        cerr << "Regression: Regression class." << endl
             << "void set_lamda(const double) method" << endl
             << "Lamda: "<< lamda  << " must be >= 0." << endl;

        exit(1);
    }
    else
    {
        d_lamda = lamda;
    }
}

double Regression::accuracy_LR(unsigned short* arrBoW[2], unsigned int filescnt)
{
    double  sum = 0;
    short truepositive = 0, truenegative=0, falsenegative = 0, falsepositive = 0;
    for(unsigned int i=0; i < filescnt;i++)
    {
        for(int j=0;j<wordscount;j++)
        {
            sum += d_Theta[j]*double(*(arrBoW[nBoW]+ (wordscount+1)*i + j));
        }
        sum +=d_Theta0[0];
        
        if (1.0/(1.0+exp(sum)) > 0.5)
        {
            if(*(arrBoW[nBoW]+ (wordscount+1)*i + wordscount) == 0)
                falsepositive++;
            else
                falsenegative++;
        }
        else
        {
            if(*(arrBoW[nBoW]+ (wordscount+1)*i + wordscount) == 1)
                truepositive++;
            else
                truenegative++;
        }
        sum = 0;
    }
    double accuracy = double(truepositive+falsepositive)/(truepositive+falsepositive + truenegative+falsenegative);
    double precision = double(truepositive)/(truepositive+falsepositive);
    cout << "precision :" << precision << endl;
    double  recall = double(truepositive)/(truepositive+falsenegative);
    cout << "recall :" << recall << endl;
    cout << "F1 Score :" << 2*(recall * precision) / (recall + precision) << endl;
    return (double)accuracy;
}
