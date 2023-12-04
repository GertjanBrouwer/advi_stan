#include<unistd.h> // fork() and exec()
#include<sys/wait.h> // wait()

// RapidJSON
#include "include/rapidjson/document.h" 
#include "include/rapidjson/filewritestream.h" 
#include "include/rapidjson/ostreamwrapper.h"
#include "include/rapidjson/writer.h"
#include<fstream>

// stan test
#include <stan/model/model_header.hpp>
// end stan test

#include<iostream>
#include<filesystem>
#include<vector>
#include<variant>

int main() {
    std::cout << "Hello world" << std::endl;
    int x = 10;
    std::cout << "Not up to date" << std::endl;
    return 0;
}

// wrapper for calls to lpdf and lpdf_grad
// int wrapper(std::string stan_model_executable) {
//     int x = 0;
//     if (fork() == 0)
//     {
//         std::cout << "Child process" << std::endl;
//         std::filesystem::path cur_path = std::filesystem::current_path();
//         cur_path /= stan_model_executable;
//         std::cout << cur_path << std::endl;
        
//         const char* args[] = {
//             "bernoulli",
//             "log_prob", 
//             "jacobian=0", 
//             "constrained_params=constrained.json",
//             "data",
//             "file=bernoulli.data.json",
//             NULL};

//         int exec_status = execvp(cur_path.c_str(), const_cast<char**>(args));
//         std::cout << "Exec status: " << exec_status << " errno: " << errno << std::endl;
//         exit(exec_status);
//     }

//     int child_status = -1;
//     wait(&child_status);

//     // TODO: check child_status
    
//     std::cout << "Parent process, child status: " << child_status << std::endl;
//     std::cout << std::filesystem::current_path() << std::endl;

//     return 0;
// }

// class StanModel {
//     private:
//         std::string stan_model;
//         std::string stan_model_name;
//     public:
//         StanModel(std::string stan_model_name) {
//             std::filesystem::path cur_path = std::filesystem::current_path();
//             cur_path /= stan_model_name;
//             std::cout << cur_path << std::endl;
//             stan_model = cur_path;
//             stan_model_name = stan_model_name;
//         }

//         std::vector<double> log_prob(
//             std::vector<std::pair<std::string, double>> constrained_params,
//             std::vector<std::pair<std::string, std::variant<int, double, std::vector<int>, std::vector<double>>>> data) 
//         {
//             constrained_to_json(constrained_params);
//             // Write data to file
//             data_to_json(data);
//             // Run stan model
//             std::cout << "Run model: " << run_model();
//             // Read output from CSV file
//             // Return output

//             return {0.0};
//         }

//         void data_to_json(std::vector<std::pair<std::string, std::variant<int, double, std::vector<int>, std::vector<double>>>> data) {
//             rapidjson::Document document;
//             document.SetObject();
//             rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

//             for (auto d : data) {
//                 rapidjson::Value key(d.first.c_str(), allocator);
//                 if (std::holds_alternative<int>(d.second)) {
//                     document.AddMember(key, std::get<int>(d.second), allocator);
//                 } else if (std::holds_alternative<double>(d.second)) {
//                     document.AddMember(key, std::get<double>(d.second), allocator);
//                 } else if (std::holds_alternative<std::vector<int>>(d.second)) {
//                     rapidjson::Value arr(rapidjson::kArrayType);
//                     for (auto i : std::get<std::vector<int>>(d.second)) {
//                         arr.PushBack(i, allocator);
//                     }
//                     document.AddMember(key, arr, allocator);
//                 } else if (std::holds_alternative<std::vector<double>>(d.second)) {
//                     rapidjson::Value arr(rapidjson::kArrayType);
//                     for (auto i : std::get<std::vector<double>>(d.second)) {
//                         arr.PushBack(i, allocator);
//                     }
//                     document.AddMember(key, arr, allocator);
//                 }
//             }

//             std::ofstream stream("data.json");
//             rapidjson::OStreamWrapper osw(stream);
//             rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
//             document.Accept(writer);
//         }

//         void constrained_to_json(std::vector<std::pair<std::string, double>> constrained_params) {
//             rapidjson::Document document;
//             document.SetObject();
//             rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

//             for (auto cparams : constrained_params) {
//                 rapidjson::Value key(cparams.first.c_str(), allocator);
//                 document.AddMember(key, cparams.second, allocator);
//             }

        
//             std::ofstream stream("constrained.json");
//             rapidjson::OStreamWrapper osw(stream);
//             rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
//             document.Accept(writer);
//         }

//         int run_model() {
//             if (fork() == 0)
//             { // Inside if is child process
                 
//                 const char* args[] = {
//                     stan_model_name.c_str(),
//                     "log_prob", // This also returns gradients 
//                     "jacobian=0", // Gradient without jacobian adjustments i.e. constrained 
//                     "constrained_params=constrained.json",
//                     "data",
//                     "file=bernoulli.data.json",
//                     NULL};

//                 int exec_status = execvp(stan_model.c_str(), const_cast<char**>(args));
//                 std::cout << "Exec status: " << exec_status << " errno: " << errno << std::endl;
//                 exit(exec_status);
//             }

//             int child_status = -1;
//             wait(&child_status);
//             if (child_status != 0) {
//                 std::cout << "Child process failed | Stan model not executed" << std::endl;
//                 return child_status;
//             }

//             return 0;
//         }
// };

// int main() {

//     // Relative to the current working directory
//     std::string stan_model_executable = "bernoulli";

//     StanModel model(stan_model_executable);
//     double tht = 0.2;
//     std::vector<std::pair<std::string, double>> constrained_params = {{"theta", tht}};
//     std::vector<std::pair<std::string, std::variant<int, double, std::vector<int>, std::vector<double>>>> data = {{"N", 10}, {"y", std::vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 1}}}; 

//     model.log_prob(constrained_params, data);
//     // model.constrained_to_json(constrained_params);
//     // model.data_to_json(data);
//     //int status = wrapper(stan_model_executable);

//     return 0;
// }