import os
import random
import numpy as np
import pandas as pd
from cmdstanpy import CmdStanModel

stan_file = os.path.join(os.getcwd(), 'mean_model.stan')
model = CmdStanModel(stan_file=stan_file)

# randdata = np.random.randint(0, 100, size=1000)
# average = np.mean(randdata)
# randdata = randdata.tolist()
# print("average: ", average)

#df = pd.DataFrame({"y": [8.2, 1.4, 2.5, 6.1, 8.9, 9.7, 5.6, 9, 3, 2.1]})
randints = np.random.randint(0, 100, size=10000)
df = pd.DataFrame({"y": randints})



fit = model.sample(data=df, show_console=True)

print(fit.summary())
print("average: ", np.mean(randints))

