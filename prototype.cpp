//#include <stan/model/model_header.hpp>
#include "build/mean_model.hpp"
#include <stan/io/empty_var_context.hpp>
#include <stan/io/json/json_data.hpp>
#include <stan/model/gradient.hpp>
#include <stan/services/util/create_rng.hpp>

#include<iostream>
#include<filesystem>
#include<vector>
#include<variant>
#include<time.h>

namespace mm = mean_model_model_namespace;

int main() {
    // TODO: Vergelijken met advi.hpp, daar worden modellen anders gebruikt.
    //stan::model::model_base* model; unrelated to above TODO.
    mm::mean_model_model* model;
    std::ifstream in("data/data.json");
    stan::json::json_data data_context(in);
    unsigned int seed = 1234;
    model = new mm::mean_model_model(data_context, seed, &std::cout);

    
    // working gradient, mean should be 0.3, thus with 0.2 it returns 1
    // input 1.3 returns -1000000000000000
    // input 0.3 returns 0
    // input 0.2 returns +100000000000000
    // input 0.30000000001 returns -10000.0234375

    // Adam optimizer
    double step_size = 0.01;
    double b1 = 0.9;
    double b2 = 0.999;
    double eps = 1e-8;
    double m_mu = 0;
    double v_mu = 0;
    double m_hat_mu = 0;
    double v_hat_mu = 0;
    double m_omega = 0;
    double v_omega = 0;
    double m_hat_omega = 0;
    double v_hat_omega = 0;
    double mu_step = 0;
    double omega_step = 0;

    Eigen::VectorXd working_gradient;
    Eigen::VectorXd mu = Eigen::VectorXd::Zero(1);
    mu[0] = 0;
    Eigen::VectorXd omega = Eigen::VectorXd::Zero(1);
    omega[0] = 1;
    Eigen::VectorXd mu_grad = Eigen::VectorXd::Zero(1);
    Eigen::VectorXd omega_grad = Eigen::VectorXd::Zero(1);
    Eigen::VectorXd gradient = Eigen::VectorXd::Zero(1);
    double temp_lp = 0.0;
    Eigen::VectorXd eta = Eigen::VectorXd::Zero(1);
    Eigen::VectorXd zeta = Eigen::VectorXd::Zero(1);
    Eigen::Matrix<stan::math::var, -1, 1> eigen_x(1);
    eigen_x[0] = 0; // TODO: Stan inits uniformly [-2,2]
    // This is not used bit still required as an input.
    Eigen::Matrix<int, -1, 1> eigen_int(1);
    eigen_int[0] = 1;
    auto rng = stan::services::util::create_rng(1234, 0);


    // Parameter updates iteration
    for (int t = 0; t < 1000; t++)
    {
        // data above from function log_prob_jacobian
        // false, false, but advi.hpp uses false, true. Have to check this with a reparameterization model
        //stan::math::var lp = model->log_prob<false, false, stan::math::var>(eigen_x, &std::cout);
        int m = 5; // monte carlo samples
        for (int i = 0; i < m; i++) {
            eta[0] = stan::math::normal_rng(0, 1, rng);
            //zeta = omega squared * eta + mu 
            zeta = eta.array().cwiseProduct(omega.array().exp()) + mu.array();
            //std::cout << "zeta: " << zeta[0] << std::endl;
            eigen_x[0] = zeta[0];
            // clock_t cbegin = clock();
            auto lp = model->log_prob<false, false, stan::math::var>(eigen_x, &std::cout);
            stan::math::grad(lp.vi_);
            // clock_t cend = clock();
            // std::cout << (double)(cend - cbegin) / CLOCKS_PER_SEC << '\n';
            //stan::model::gradient();
            //stan::model::gradient(lp, eigen_x, temp_lp, gradient, &std::cout);
            working_gradient = eigen_x.adj();
            mu_grad += working_gradient;
            omega_grad.array() += working_gradient.array().cwiseProduct(eta.array());
        }

        mu_grad /= static_cast<double>(m);
        omega_grad /= static_cast<double>(m);
        omega_grad.array() = omega_grad.array().cwiseProduct(omega.array().exp()) + 1;  //entropy

        m_mu = b1 * m_mu + (1 - b1) * mu_grad[0];
        v_mu = b2 * v_mu + (1 - b2) * pow(mu_grad[0], 2);
        m_hat_mu = m_mu / (1 - pow(b1, t + 1));
        v_hat_mu = v_mu / (1 - pow(b2, t + 1));
        mu_step = step_size * m_hat_mu / (sqrt(v_hat_mu) + eps);
        mu[0] += step_size * m_hat_mu / (sqrt(v_hat_mu) + eps);

        m_omega = b1 * m_omega + (1 - b1) * omega_grad[0];
        v_omega = b2 * v_omega + (1 - b2) * pow(omega_grad[0], 2);
        m_hat_omega = m_omega / (1 - pow(b1, t + 1));
        v_hat_omega = v_omega / (1 - pow(b2, t + 1));
        omega_step = step_size * m_hat_omega / (sqrt(v_hat_omega) + eps);
        omega[0] += step_size * m_hat_omega / (sqrt(v_hat_omega) + eps);

    }
    std::cout << "mu: " << mu[0] << " omega: " << omega[0] << " eigen_x" << eigen_x[0]<< std::endl;
    std::cout << "mu_step: " << mu_step << " omega_step: " << omega_step << std::endl;


    // TODO: dimension (1) is now hardcoded
    // Eigen::VectorXd mu_grad = Eigen::VectorXd::Zero(1);
    // Eigen::VectorXd omega_grad = Eigen::VectorXd::Zero(1);
    // Eigen::VectorXd tmp_mu_grad = Eigen::VectorXd::Zero(1);
    // double tmp_lp = 0.0;

    // stan::model::gradient(lp, eigen_x, tmp_lp, tmp_mu_grad, &std::cout);

    



    // step size 1/(n+1) n is run 
    //auto grd = model->log_prob_impl<false, false>(eigen_x, eigen_int, &std::cout);

    // Code copied from generated header file mean_model.hpp
    // stan::math::accumulator<double> lp_accum__;
    // double lpdf = stan::math::normal_lpdf<false>(y, 0.4, 0.0000001);
    // lp_accum__.add(stan::math::normal_lpdf<false>(y, 0.4, 0.0000001));
    // lp_accum__.add(0.0);
    // auto lpsum = lp_accum__.sum();



    //std::cout << "Num params r: " << model->num_params_r() << std::endl;
    std::cout << "Hello world" << std::endl;
    return 0;
}

void troep_to_save() {

    std::vector<double> x = std::vector<double>{0.2};
    std::vector<int> y = std::vector<int>{1, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    Eigen::Matrix<double, -1, 1> eigen_double(1);
    eigen_double[0] = 0.2;

}