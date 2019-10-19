import pandas as pd
from sklearn.linear_model import SGDClassifier
from sklearn.model_selection import GridSearchCV
from sklearn.metrics import accuracy_score
from sklearn.metrics import precision_recall_fscore_support
import sys

# Load the csv file into pandas dataframe
training_data = pd.read_csv(sys.argv[1], header=None)
test_data = pd.read_csv(sys.argv[2], header = None)

Xtst = test_data.iloc[:, 0:len(test_data.columns) -1]
ytst = test_data.iloc[:,-1]
    
Xtra = training_data.iloc[:, 0:len(training_data.columns)-1]
ytra = training_data.iloc[:,-1]
 
params = {
    "loss" : ["hinge", "log", "squared_hinge", "modified_huber"],
    "alpha" : [0.0001, 0.001, 0.01, 0.1],
    "penalty" : ["l2", "l1", "none"],
}
 
model = SGDClassifier(max_iter=1000)
clf = GridSearchCV(model, param_grid=params)
clf.fit(Xtra, ytra)
print(clf.best_score_)
print(clf.best_estimator_)

predictions = clf.predict(Xtst)
print("Test Accuracy:")
print(accuracy_score(ytst, predictions))
print("precision_recall_fscore:")
print(precision_recall_fscore_support(ytst, predictions, average='macro'))