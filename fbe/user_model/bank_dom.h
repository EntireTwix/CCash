//------------------------------------------------------------------------------
// Automatically generated by the Fast Binary Encoding compiler, do not modify!
// https://github.com/chronoxor/FastBinaryEncoding
// Source: user_model.fbe
// FBE version: 1.14.1.0
//------------------------------------------------------------------------------

#pragma once

#if defined(__clang__)
#pragma clang system_header
#elif defined(__GNUC__)
#pragma GCC system_header
#elif defined(_MSC_VER)
#pragma system_header
#endif

#include "fbe.h"

namespace bank_dom {
using namespace FBE;
} // namespace bank_dom

namespace FBE {
using namespace ::bank_dom;
} // namespace FBE

namespace bank_dom {

struct Transaction
{
    std::string counterparty;
    bool receiving;
    uint32_t amount;
    uint64_t time;

    size_t fbe_type() const noexcept { return 1; }

    Transaction();
    Transaction(const std::string& arg_counterparty, bool arg_receiving, uint32_t arg_amount, uint64_t arg_time);
    Transaction(const Transaction& other) = default;
    Transaction(Transaction&& other) = default;
    ~Transaction() = default;

    Transaction& operator=(const Transaction& other) = default;
    Transaction& operator=(Transaction&& other) = default;

    bool operator==(const Transaction& other) const noexcept;
    bool operator!=(const Transaction& other) const noexcept { return !operator==(other); }
    bool operator<(const Transaction& other) const noexcept;
    bool operator<=(const Transaction& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const Transaction& other) const noexcept { return !operator<=(other); }
    bool operator>=(const Transaction& other) const noexcept { return !operator<(other); }

    std::string string() const { std::stringstream ss; ss << *this; return ss.str(); }

    friend std::ostream& operator<<(std::ostream& stream, const Transaction& value);

    void swap(Transaction& other) noexcept;
    friend void swap(Transaction& value1, Transaction& value2) noexcept { value1.swap(value2); }
};

} // namespace bank_dom

#if defined(FMT_VERSION) && (FMT_VERSION >= 90000)
template <> struct fmt::formatter<bank_dom::Transaction> : ostream_formatter {};
#endif

template<>
struct std::hash<bank_dom::Transaction>
{
    typedef bank_dom::Transaction argument_type;
    typedef size_t result_type;

    result_type operator() (const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace bank_dom {

struct Logs
{
    std::vector<::bank_dom::Transaction> data;

    size_t fbe_type() const noexcept { return 2; }

    Logs();
    explicit Logs(const std::vector<::bank_dom::Transaction>& arg_data);
    Logs(const Logs& other) = default;
    Logs(Logs&& other) = default;
    ~Logs() = default;

    Logs& operator=(const Logs& other) = default;
    Logs& operator=(Logs&& other) = default;

    bool operator==(const Logs& other) const noexcept;
    bool operator!=(const Logs& other) const noexcept { return !operator==(other); }
    bool operator<(const Logs& other) const noexcept;
    bool operator<=(const Logs& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const Logs& other) const noexcept { return !operator<=(other); }
    bool operator>=(const Logs& other) const noexcept { return !operator<(other); }

    std::string string() const { std::stringstream ss; ss << *this; return ss.str(); }

    friend std::ostream& operator<<(std::ostream& stream, const Logs& value);

    void swap(Logs& other) noexcept;
    friend void swap(Logs& value1, Logs& value2) noexcept { value1.swap(value2); }
};

} // namespace bank_dom

#if defined(FMT_VERSION) && (FMT_VERSION >= 90000)
template <> struct fmt::formatter<bank_dom::Logs> : ostream_formatter {};
#endif

template<>
struct std::hash<bank_dom::Logs>
{
    typedef bank_dom::Logs argument_type;
    typedef size_t result_type;

    result_type operator() (const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace bank_dom {

struct User
{
    uint32_t balance;
    uint64_t password;
    std::optional<::bank_dom::Logs> logs;

    size_t fbe_type() const noexcept { return 3; }

    User();
    User(uint32_t arg_balance, uint64_t arg_password, const std::optional<::bank_dom::Logs>& arg_logs);
    User(const User& other) = default;
    User(User&& other) = default;
    ~User() = default;

    User& operator=(const User& other) = default;
    User& operator=(User&& other) = default;

    bool operator==(const User& other) const noexcept;
    bool operator!=(const User& other) const noexcept { return !operator==(other); }
    bool operator<(const User& other) const noexcept;
    bool operator<=(const User& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const User& other) const noexcept { return !operator<=(other); }
    bool operator>=(const User& other) const noexcept { return !operator<(other); }

    std::string string() const { std::stringstream ss; ss << *this; return ss.str(); }

    friend std::ostream& operator<<(std::ostream& stream, const User& value);

    void swap(User& other) noexcept;
    friend void swap(User& value1, User& value2) noexcept { value1.swap(value2); }
};

} // namespace bank_dom

#if defined(FMT_VERSION) && (FMT_VERSION >= 90000)
template <> struct fmt::formatter<bank_dom::User> : ostream_formatter {};
#endif

template<>
struct std::hash<bank_dom::User>
{
    typedef bank_dom::User argument_type;
    typedef size_t result_type;

    result_type operator() (const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace bank_dom {

struct Global
{
    std::vector<std::string> keys;
    std::vector<::bank_dom::User> users;

    size_t fbe_type() const noexcept { return 4; }

    Global();
    Global(const std::vector<std::string>& arg_keys, const std::vector<::bank_dom::User>& arg_users);
    Global(const Global& other) = default;
    Global(Global&& other) = default;
    ~Global() = default;

    Global& operator=(const Global& other) = default;
    Global& operator=(Global&& other) = default;

    bool operator==(const Global& other) const noexcept;
    bool operator!=(const Global& other) const noexcept { return !operator==(other); }
    bool operator<(const Global& other) const noexcept;
    bool operator<=(const Global& other) const noexcept { return operator<(other) || operator==(other); }
    bool operator>(const Global& other) const noexcept { return !operator<=(other); }
    bool operator>=(const Global& other) const noexcept { return !operator<(other); }

    std::string string() const { std::stringstream ss; ss << *this; return ss.str(); }

    friend std::ostream& operator<<(std::ostream& stream, const Global& value);

    void swap(Global& other) noexcept;
    friend void swap(Global& value1, Global& value2) noexcept { value1.swap(value2); }
};

} // namespace bank_dom

#if defined(FMT_VERSION) && (FMT_VERSION >= 90000)
template <> struct fmt::formatter<bank_dom::Global> : ostream_formatter {};
#endif

template<>
struct std::hash<bank_dom::Global>
{
    typedef bank_dom::Global argument_type;
    typedef size_t result_type;

    result_type operator() (const argument_type& value) const
    {
        result_type result = 17;
        return result;
    }
};

namespace bank_dom {

} // namespace bank_dom
