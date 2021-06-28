#include <iostream>
#include <chrono>
#include <thread>
#include <sys/types.h>
#include <unistd.h>
#include "bank_api.h"

//sig handling headers
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//threads of cpu
#include <sys/sysinfo.h>

using namespace std::chrono;
using namespace drogon;

static Bank bank;

void SaveSig(int s)
{
    std::cout << "\nSaving on close...\n";
    bank.Save();
    exit(1);
}

int main(int argc, char **argv)
{
    std::cout << "\nSSE3    : " << (__builtin_cpu_supports("sse3") ? "enabled" : "disabled")
              << "\nThreads : " << get_nprocs() + 1
              << std::endl; //flushing before EventLoop

    static_assert(bool(MAX_LOG_SIZE) == bool(PRE_LOG_SIZE), "You must either utilize both or neither logging variables.\n");
    static_assert(MAX_LOG_SIZE >= PRE_LOG_SIZE, "The maximum log size must be larger than or equal to the amount preallocated.\n");
    static_assert(!MAX_LOG_SIZE || !(MAX_LOG_SIZE % PRE_LOG_SIZE), "The maximum log size must be divisible by the preallocation size.\n");

    if (argc != 3)
    {
        std::cerr << "Usage: sudo ./bank <admin password> <saving frequency in minutes>\n";
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
                std::cout << "Saving " << std::time(0) << '\n';
                if (bank.GetChangeState())
                {
                    std::cout << "    to disk...\n";
                    bank.Save();
                }
                else
                {
                    std::cout << "    no changes...\n";
                }
            }
        }).detach();
    }

    auto API = std::make_shared<api>(bank);
    auto user_filter = std::make_shared<UserFilter>(bank);
    auto admin_filter = std::make_shared<AdminFilter>(bank);

    app().registerPostHandlingAdvice(
        [](const drogon::HttpRequestPtr &req, const drogon::HttpResponsePtr &resp) {
            resp->addHeader("Access-Control-Allow-Origin", "*"); //CORS
        });
    app()
        .loadConfigFile(config_location)
        .registerFilter(user_filter)
        .registerFilter(admin_filter)
        .registerController(API)
        .setThreadNum(get_nprocs())
        .run();

    return 0;
}
