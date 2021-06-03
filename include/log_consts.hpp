#pragma once

// `max_log_size` must be divisible by `pre_log_size`
constexpr auto max_log_size = 100; // Setting to 0 does not compile logging
constexpr auto pre_log_size = 10;
