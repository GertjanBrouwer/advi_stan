data {
  int<lower=0> N;
  array[N] real y;
}

parameters {
  real mu;
}

model {
  y ~ normal(mu, 0.0000001);
}