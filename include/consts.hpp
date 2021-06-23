#pragma once

// Setting both values to 0 does not compile logging
constexpr unsigned max_log_size = 100;
constexpr unsigned pre_log_size = 10;

constexpr unsigned max_name_size = 50;

constexpr const char *users_location = "../users.json";
constexpr const char *config_location = "../config.json";

//returns money to an account on deletion
constexpr bool return_on_del = false;
constexpr const char *return_account = "";