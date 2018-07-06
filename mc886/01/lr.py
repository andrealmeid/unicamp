import pandas
from sklearn import linear_model

regr = linear_model.LinearRegression()
data = pandas.read_csv("year-prediction-msd-train.txt")

x = data['year'].values.reshape(-1, 1)
y = data.loc[0:, '1ta':'71tc']

regr.fit(x, y)

print('Coefficients: \n', regr.coef_)
