import os
import numpy as np
import pandas as pd
from cmdstanpy import CmdStanModel

stan_file = os.path.join(os.getcwd(), 'models/8schools.stan')
model = CmdStanModel(stan_file=stan_file)

y = [28.39, 7.94, -2.75, 6.82, -0.64, 0.63, 18.01, 12.16]
sigma = [14.9, 10.2, 16.3, 11.0, 9.4, 11.4, 10.4, 17.6]

fit = model.sample(data={"N": len(y), "y": y, "sigma": sigma}, show_console=True)
print(fit.summary(percentiles=(5, 25, 50, 75, 95)))

