data {
  //int<lower=0> N;
  array[10000] real y;
}

parameters {
  real mu;
}

model {
  y ~ normal(mu, 0.0000001);
}