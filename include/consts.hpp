#pragma once

// Setting both values to 0 does not compile logging (useful for if disk/memory is very valuable)
constexpr unsigned max_log_size = 100;
constexpr unsigned pre_log_size = 10;

constexpr unsigned max_name_size = 50;

constexpr const char *users_location = "../users.json";
constexpr const char *config_location = "../config.json";

// Returns money to an account on deletion (useful if you dont want any money to leave the economy)
constexpr bool return_on_del = false;
constexpr const char *return_account = "";

/*
if true, when frequency is hit after changes have happened then save 
    pros
        LOW disk usage
    cons
        LOW atomic overhead
if false, when frequency is hit save
    pros
        ZERO atomic overhead
    cons
        HIGH disk usage
*/
constexpr bool conservative_disk_save = true;

//constexpr unsigned max_users = 1000;