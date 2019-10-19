This repositry has following algorithms implemented in c++

– Multinomial Naive Bayes on the Bag of words model

– Discrete Naive Bayes on the Bernoulli model

– Logistic Regression on both Bag of words and Bernoulli models

– SGDClassifier on both Bag of words and Bernoulli models

Implemented the multinomial and Discrete Naive Bayes algorithm for text classification described here: http://nlp.stanford.edu/IR-book/pdf/13bayes.pdf

Implemented the MCAP Logistic Regression algorithm with L2 regularization. Tried different values of λ. Divided the given training set into two sets using a 70/30 split (namely the first split has 70% of the examples and the second split has the remaining 30%). Learned parameters using the 70% split, treated the 30% data as validation data and use it to select a value for λ. Then, used the chosen value of λ to learn the parameters using the full training set and report accuracy on the test set. Used gradient ascent for learning the weights for a maxiumum iterations of 1000 and learning rate of 0.001.

SGDClassifier using GridSearchCV in scikit-learn

The data set used is ham and spam mails for classification.
http://www.hlt.utdallas.edu/~vgogate/ml/2019f/homeworks/hw2_train.zip
http://www.hlt.utdallas.edu/~vgogate/ml/2019f/homeworks/hw2_test.zip
