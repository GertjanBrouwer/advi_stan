import os
import random
import numpy as np
from cmdstanpy import CmdStanModel

stan_file = os.path.join(os.getcwd(), 'linear_regression.stan')
model = CmdStanModel(stan_file=stan_file)

randdata = np.random.randint(0, 100, size=1000)
average = np.mean(randdata)
randdata = randdata.tolist()
print("average: ", average)

fit = model.sample(data={"N": len(randdata), "y": randdata})
print(fit.summary())

