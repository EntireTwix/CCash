#include <iostream>
#include <chrono>
#include <thread>
#include <sys/types.h>
#include <unistd.h>
#include "bank_f.hpp"

using namespace drogon;

int main(int argc, char **argv)
{
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

    //Admin Password
    bank.admin_pass = argv[1];

    //Auto Saving
    const unsigned long saving_freq = std::stoul(argv[2]);
    if (saving_freq) //if saving frequency is 0 then auto saving is turned off
    {
        std::thread([&argv, saving_freq]() {
            while (1)
            {
                std::this_thread::sleep_for(std::chrono::minutes(saving_freq));
                bank.Save();
            }
        }).detach();
    }

    auto API = std::make_shared<BankF>();
    app().registerPostHandlingAdvice(
        [](const drogon::HttpRequestPtr &req, const drogon::HttpResponsePtr &resp) {
            resp->addHeader("Access-Control-Allow-Origin", "*");
        });
    app().addListener("0.0.0.0", 80).registerController(API).setThreadNum(std::stoul(argv[3])).enableRunAsDaemon().setSSLFiles("/etc/letsencrypt/live/ccash.ryzerth.com/fullchain.pem", "/etc/letsencrypt/live/ccash.ryzerth.com/privkey.pem").run();

    return 0;
}