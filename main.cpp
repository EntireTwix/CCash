#include <iostream>
#include <chrono>
#include <thread>
#include <sys/types.h>
#include <unistd.h>
#include "bank_api.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std::chrono;
using namespace drogon;

static Bank bank;

void SaveSig(int s)
{
    bank.Save();
    std::cout << "\nSaving on close...\n";
    exit(1);
}

int main(int argc, char **argv)
{
    static_assert(bool(max_log_size) == bool(pre_log_size), "You must either utilize both or neither logging variables.\n");
    static_assert(max_log_size >= pre_log_size, "The maximum log size must be larger than or equal to the amount preallocated.\n");
    static_assert(!max_log_size || !(max_log_size % pre_log_size), "The maximum log size must be divisible by the preallocation size.\n");

    if (argc != 4)
    {
        std::cerr << "Usage: sudo ./bank <admin password> <saving frequency in minutes> <threads>\n";
        return 0;
    }
    if (geteuid() != 0)
    {
        std::cerr << "ERROR: CCash MUST be ran as root\n";
        return 0;
    }

    //Loading users from users.json
    bank.Load();

    //Sig handling
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = SaveSig;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    //Admin Password
    bank.admin_pass = argv[1];

    //Auto Saving
    const unsigned long saving_freq = std::stoul(std::string(argv[2]));
    if (saving_freq) //if saving frequency is 0 then auto saving is turned off
    {
        std::thread([saving_freq]() {
            while (1)
            {
                std::this_thread::sleep_for(std::chrono::minutes(saving_freq));
                if (bank.GetChangeState())
                {
                    bank.Save();
                }
                std::cout << "Saving " << std::time(0) << '\n';
            }
        }).detach();
    }

    //endpoints
    auto APIv1 = std::make_shared<v1::api>(bank); //v1

    app().registerPostHandlingAdvice(
        [](const drogon::HttpRequestPtr &req, const drogon::HttpResponsePtr &resp) {
            resp->addHeader("Access-Control-Allow-Origin", "*"); //CORS
        });
    app().loadConfigFile(config_location).registerController(APIv1).setThreadNum(std::stoul(std::string(argv[3]))).run();

    return 0;
}
