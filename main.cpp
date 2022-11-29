#include <iostream>
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

using namespace drogon;

void SaveSig(int s)
{
    std::cout << "\nSaving on close...\n"
              << Bank::Save();
    exit(1);
}

int main(int argc, char **argv)
{
    {
        static_assert(MAX_LOG_SIZE >= 0);
        if (argc == 1)
        {
            std::ofstream users_save(users_location, std::ios::out | std::ios::binary);
            if (users_save.is_open())
            {
                uint8_t temp[16]{16, 0, 0, 0, 4};
                users_save.write((char *)temp, 16);
                users_save.close();
                std::cout << "User save file generated\nUsage: sudo ./bank <admin account> <saving frequency in minutes>\n";
            }
            else
            {
                std::cerr << "File cannot be created (may already exist)\n";
            }
            return 0;
        }
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
        Bank::Load();
        size_t num_of_logs = Bank::NumOfLogs();
        size_t num_of_users = Bank::NumOfUsers();
        std::cout << "\n\nLoaded " << num_of_users << " Users ~" << (float)(sizeof(User) * num_of_users) / 1048576 << "Mb"
                  << "\nLoaded " << num_of_logs << " Logs ~" << (float)(num_of_logs * (93 + sizeof(Transaction) + max_name_size)) / 1048576 << "Mb" //93:cached response per log(heap), sizeof(Transaction), max_name_size:counterparty(heap)
                  << "\nLoaded " << Bank::SumBal() << " C$H"
                  << std::endl; //flushing before EventLoop

        //Sig handling
        struct sigaction sigIntHandler;
        sigIntHandler.sa_handler = SaveSig;
        sigemptyset(&sigIntHandler.sa_mask);
        sigIntHandler.sa_flags = 0;
        sigaction(SIGINT, &sigIntHandler, NULL);

        //Admin account
        Bank::admin_account = argv[1];

        //Auto Saving
        const unsigned long saving_freq = std::stoul(std::string(argv[2]));
        if (saving_freq) //if saving frequency is 0 then auto saving is turned off
        {
            std::thread([saving_freq]()
                        {
                            while (1)
                            {
                                std::this_thread::sleep_for(std::chrono::minutes(saving_freq));
                                std::cout << "Saving " << std::time(0) << "...\n"
                                          << Bank::Save();
                            }
                        })
                .detach();
        }
    } //destroying setup variables

    app()
        .loadConfigFile(config_location)
#if MULTI_THREADED
        .setThreadNum(get_nprocs())
#endif
        .run();

    SaveSig(0);

    return 0;
}
