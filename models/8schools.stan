data {
    int<lower=0> N;          // number of observations
    array[N] real sigma;   // standard deviation of each school
    array[N] real y;       // estimated coaching effect
}

parameters {
    real mu;
    real<lower=0> tau; // standard deviation must be positive
    array[N] real theta;
}

model {
    mu ~ uniform(-10, 30);
    tau ~ uniform(5, 10);
    theta ~ normal(mu, tau);

    // likelihood
    y ~ normal(theta, sigma);
}



