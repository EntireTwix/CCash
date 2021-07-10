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
    std::cout << "\nSaving on close...\n"
              << bank.Save();
    exit(1);
}

int main(int argc, char **argv)
{
    {

        if (argc != 3)
        {
            std::cerr << "Usage: sudo ./bank <admin account> <saving frequency in minutes>\n";
            return 0;
        }
        if (geteuid() != 0)
        {
            std::cerr << "ERROR: CCash MUST be ran as root\n";
            return 0;
        }
        std::cout
            << "\nAPI Version     : " << API_VERSION
            << "\n\nAVX             : " << (__builtin_cpu_supports("avx") ? "enabled" : "disabled")
            << "\nAVX 2           : " << (__builtin_cpu_supports("avx2") ? "enabled" : "disabled")
            << "\nSSE 2           : " << (__builtin_cpu_supports("sse2") ? "enabled" : "disabled")
            << "\nSSE 3           : " << (__builtin_cpu_supports("sse3") ? "enabled" : "disabled")
            << "\nSSE 4.1         : " << (__builtin_cpu_supports("sse4.1") ? "enabled" : "disabled")
            << "\nSSE 4.2         : " << (__builtin_cpu_supports("sse4.2") ? "enabled" : "disabled")
#if MULTI_THREADED
            << "\n\nThreads         : " << get_nprocs() + 1
            << "\nMulti threading : enabled";
#else
            << "\n\nThreads         : " << 2
            << "\nMulti threading : disabled";
#endif

        //Loading users from users.json
        bank.Load();
        std::cout << "\n\nLoaded " << bank.NumOfUsers() << " Users"
                  << "\nLoaded " << bank.NumOfLogs() << " Logs"
                  << "\nLoaded " << bank.SumBal() << " CSH"
                  << std::endl; //flushing before EventLoop

        //Sig handling
        struct sigaction sigIntHandler;

        sigIntHandler.sa_handler = SaveSig;
        sigemptyset(&sigIntHandler.sa_mask);
        sigIntHandler.sa_flags = 0;

        sigaction(SIGINT, &sigIntHandler, NULL);

        //Admin account
        bank.admin_account = argv[1];

        //Auto Saving
        const unsigned long saving_freq = std::stoul(std::string(argv[2]));
        if (saving_freq) //if saving frequency is 0 then auto saving is turned off
        {
            std::thread([saving_freq]() {
                while (1)
                {
                    std::this_thread::sleep_for(std::chrono::minutes(saving_freq));
                    std::cout << "Saving " << std::time(0) << "...\n"
                              << bank.Save();
                }
            })
                .detach();
        }
    } //destroying setup variables
    static auto API = std::make_shared<api>(bank);
    static auto user_filter_default = std::make_shared<UserFilter<true, false>>(bank);
    static auto user_filter_sparse = std::make_shared<UserFilter<false, false>>(bank);
    static auto admin_filter = std::make_shared<UserFilter<false, true>>(bank);
    static auto json_resp_and_req_filter = std::make_shared<JsonFilter<true>>();
    static auto json_resp_filter = std::make_shared<JsonFilter<false>>();

    app()
        .loadConfigFile(config_location)
        .registerFilter(user_filter_default)
        .registerFilter(user_filter_sparse)
        .registerFilter(admin_filter)
        .registerFilter(json_resp_and_req_filter)
        .registerFilter(json_resp_filter)
        .registerController(API)
#if MULTI_THREADED
        .setThreadNum(get_nprocs())
#endif
        .run();

    return 0;
}
