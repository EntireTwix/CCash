#include "bank.h"

#if MULTI_THREADED
phmap::parallel_flat_hash_map<
    std::string, User,
    xxHashStringGen,
    phmap::priv::hash_default_eq<std::string>,
    phmap::priv::Allocator<phmap::priv::Pair<const std::string, User>>,
    4UL,
    std::mutex>
    Bank::users;
#else
phmap::parallel_flat_hash_map<std::string, User, xxHashStringGen> Bank::users;
#endif

#if CONSERVATIVE_DISK_SAVE
ChangeFlag<false> Bank::save_flag;
#endif

std::shared_mutex Bank::iter_lock;
std::string Bank::admin_account;

using namespace drogon;

#if CONSERVATIVE_DISK_SAVE
#define SET_CHANGES_ON save_flag.SetChangesOn()
#else
#define SET_CHANGES_ON
#endif

inline bool ValidUsername(const std::string &name) noexcept
{
    if (name.size() < min_name_size || name.size() > max_name_size)
    {
        return false;
    }
    for (char c : name)
    {
        if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_'))
        {
            return false;
        }
    }
    return true;
}

//NOT THREAD SAFE
size_t Bank::NumOfUsers() noexcept { return Bank::users.size(); }

//NOT THREAD SAFE
size_t Bank::NumOfLogs() noexcept
{
#if MAX_LOG_SIZE > 0
    size_t res = 0;
    for (const auto &u : users)
    {
        res += u.second.log.data.size();
    }
    return res;
#else
    return 0;
#endif
}

//NOT THREAD SAFE
size_t Bank::SumBal() noexcept
{
    size_t res = 0;
    for (const auto &u : users)
    {
        res += u.second.balance;
    }
    return res;
}

BankResponse Bank::GetBal(const std::string &name) noexcept
{
    uint32_t res = 0;
    if (!ValidUsername(name) || !Bank::users.if_contains(name, [&res](const User &u) { res = u.balance; }))
    {
        return {k404NotFound, "\"User not found\""};
    }
    else
    {
        return {k200OK, std::to_string(res)};
    }
}
#if MAX_LOG_SIZE > 0
BankResponse Bank::GetLogs(const std::string &name) noexcept
{
    BankResponse res;
    if (!Bank::users.modify_if(name, [&name, &res](User &u) { res = {k200OK, u.log.GetLogs(name)}; }))
    {
        return {k404NotFound, "\"User not found\""};
    }
    else
    {
        return res;
    }
}
BankResponse Bank::GetLogsV2(const std::string &name) noexcept
{
    BankResponse res;
    if (!Bank::users.modify_if(name, [&name, &res](User &u) { res = {k200OK, u.log.GetLogsV2()}; }))
    {
        return {k404NotFound, "\"User not found\""};
    }
    else
    {
        return res;
    }
}
#endif
BankResponse Bank::SendFunds(const std::string &a_name, const std::string &b_name, uint32_t amount) noexcept
{
    if (!amount)
    {
        return {k400BadRequest, "\"Amount cannot be 0\""};
    }
    if (a_name == b_name)
    {
        return {k400BadRequest, "\"Names cannot match\""};
    }
    if (!Contains(b_name))
    {
        return {k404NotFound, "\"Reciever does not exist\""};
    }

    BankResponse res;
    std::shared_lock<std::shared_mutex> lock{iter_lock};
#if MAX_LOG_SIZE > 0
    time_t current_time = time(NULL);
    if (!Bank::users.modify_if(a_name, [current_time, &a_name, &b_name, &res, amount](User &a)
#else
    if (!Bank::users.modify_if(a_name, [&a_name, &b_name, &res, amount](User &a)
#endif
                               {
                                   //if "A" can afford it
                                   if (a.balance < amount)
                                   {
                                       res = {k400BadRequest, "\"Insufficient funds\""};
                                   }
                                   else
                                   {
                                       a.balance -= amount;
#if MAX_LOG_SIZE > 0
                                       a.log.AddTrans(b_name, false, amount, current_time);
#endif
                                       res = {k200OK, std::to_string(a.balance)};
                                   }
                               }))
    {
        return {k404NotFound, "\"Sender does not exist\""};
    }
    if (res.first == k200OK)
    {
#if MAX_LOG_SIZE > 0
        Bank::users.modify_if(b_name, [current_time, &a_name, &b_name, amount](User &b) {
            b.balance += amount;
            b.log.AddTrans(a_name, true, amount, current_time);
        });
#else
        Bank::users.modify_if(b_name, [amount](User &b) { b.balance += amount; });
#endif
        SET_CHANGES_ON;
    }
    return res;
}
bool Bank::VerifyPassword(const std::string &name, const std::string_view &attempt) noexcept
{
    bool res = false;
    Bank::users.if_contains(name, [&res, &attempt](const User &u) { res = (u.password == xxHashStringGen{}(attempt)); });
    return res;
}

void Bank::ChangePassword(const std::string &name, const std::string &new_pass) noexcept
{
    SET_CHANGES_ON;
    Bank::users.modify_if(name, [&new_pass](User &u) { u.password = xxHashStringGen{}(new_pass); });
}
BankResponse Bank::SetBal(const std::string &name, uint32_t amount) noexcept
{
    if (ValidUsername(name) && Bank::users.modify_if(name, [amount](User &u) { u.balance = amount; }))
    {
        SET_CHANGES_ON;
        return {k204NoContent, std::nullopt}; //returns new balance
    }
    else
    {
        return {k404NotFound, "\"User not found\""};
    }
}
BankResponse Bank::ImpactBal(const std::string &name, int64_t amount) noexcept
{
    if (amount == 0)
    {
        return {k400BadRequest, "\"Amount cannot be 0\""};
    }
    uint32_t bal;
    if (ValidUsername(name) && Bank::users.modify_if(name, [&bal, amount](User &u) { bal = (u.balance < (amount * -1) ? u.balance = 0 : u.balance += amount); }))
    {
        SET_CHANGES_ON;
        return {k200OK, std::to_string(bal)}; //may return new balance
    }
    else
    {
        return {k404NotFound, "\"User not found\""};
    }
}
bool Bank::Contains(const std::string &name) noexcept
{
    return ValidUsername(name) && Bank::users.contains(name);
}
#if MAX_LOG_SIZE > 0
BankResponse Bank::PruneUsers(time_t threshold_time, uint32_t threshold_bal) noexcept
#else
BankResponse Bank::PruneUsers(uint32_t threshold_bal) noexcept
#endif
{
    std::unique_lock<std::shared_mutex> lock{iter_lock};
    size_t deleted_count = 0;
#if RETURN_ON_DEL
    uint32_t bal = 0;
#endif
    for (const auto &u : users)
    {
#if RETURN_ON_DEL
        if (Bank::users.erase_if(u.first, [threshold_time, threshold_bal, &bal, &deleted_count](User &u) {
                bal += u.balance;
#else
        if (Bank::users.erase_if(u.first, [threshold_time, threshold_bal, &deleted_count](User &u) {
#endif
#if MAX_LOG_SIZE > 0
                return ((!u.log.data.size() || u.log.data.back().time < threshold_time) && u.balance < threshold_bal);
#else
                return (u.balance < threshold_bal);
#endif
            }))
        {

            SET_CHANGES_ON;
            ++deleted_count;
        }
    }
#if RETURN_ON_DEL
    if (bal)
    {
        Bank::users.modify_if(return_account, [bal](User &u) { u.balance += bal; });
    }
#endif
    return {k200OK, std::to_string(deleted_count)};
}

BankResponse Bank::AddUser(const std::string &name, uint32_t init_bal, const std::string &init_pass) noexcept
{
    if (!ValidUsername(name))
    {
        return {k400BadRequest, "\"Invalid Username\""};
    }
    std::shared_lock<std::shared_mutex> lock{iter_lock};
    if (Bank::users.try_emplace_l(
            name, [](User &) {}, init_bal, init_pass))
    {
        SET_CHANGES_ON;
        return {k204NoContent, std::nullopt};
    }
    else
    {
        return {k409Conflict, "\"User already exists\""};
    }
}
BankResponse Bank::DelUser(const std::string &name) noexcept
{
    std::shared_lock<std::shared_mutex> lock{iter_lock};
#if RETURN_ON_DEL
    uint32_t bal;
    if (Bank::users.if_contains(name, [&bal](const User &u) { bal = u.balance; }) &&
        bal)
    {
        Bank::users.modify_if(return_account, [bal](User &u) { u.balance += bal; });
    }
#endif
    if (ValidUsername(name) && Bank::users.erase(name))
    {
        SET_CHANGES_ON;
        return {k204NoContent, std::nullopt};
    }
    else
    {
        return {k404NotFound, "\"User not found\""};
    }
}
//assumes we know name exists, unlike DelUser
void Bank::DelSelf(const std::string &name) noexcept
{
    std::shared_lock<std::shared_mutex> lock{iter_lock};
#if RETURN_ON_DEL
    uint32_t bal;
    if (Bank::users.if_contains(name, [&bal](const User &u) { bal = u.balance; }) &&
        bal)
    {
        Bank::users.modify_if(return_account, [bal](User &u) { u.balance += bal; });
    }
#endif
    SET_CHANGES_ON;
    Bank::users.erase(name);
}
//ONLY EVER BEING CALLED BY SAVE THREAD OR C-INTERUPT
const char *Bank::Save()
{
#if CONSERVATIVE_DISK_SAVE
    if (save_flag.GetChangeState())
    {
#endif
        std::ofstream users_save(users_location, std::ios::out | std::ios::binary);
        if (!users_save.is_open())
        {
            throw std::invalid_argument("Cannot access saving file\n");
        }
        bank_dom::Global users_copy;
        users_copy.users.reserve(Bank::users.size());
        users_copy.keys.reserve(Bank::users.size());
        {
            std::unique_lock<std::shared_mutex> lock{iter_lock};
            for (const auto &u : users)
            {
                Bank::users.if_contains(u.first, [&users_copy, &u](const User &u_val) {
                    users_copy.users.emplace_back(u_val.Encode());
                    users_copy.keys.emplace_back(u.first);
                });
            }
        }
        FBE::bank_dom::GlobalFinalModel writer;
        writer.serialize(users_copy);
        assert(writer.verify() && "Data is corrupted!");
        const FBE::FBEBuffer &write_buffer = writer.buffer();
        users_save.write((char *)write_buffer.data(), write_buffer.size());
        users_save.close();
        if (!users_save.good())
        {
            throw std::invalid_argument("Error occurred at writing to save file\n");
        }
#if CONSERVATIVE_DISK_SAVE
        save_flag.SetChangesOff();
        return "        to disk...\n";
    }
    else
    {
        return "     no changes...\n";
    }
#else
    return "        to disk...\n";
#endif
}
//NOT THREAD SAFE, BY NO MEANS SHOULD THIS BE CALLED WHILE RECEIEVING REQUESTS
void Bank::Load()
{
    std::ifstream users_load(users_location, std::ios::out | std::ios::binary);
    if (!users_load.is_open())
    {
        throw std::invalid_argument("Cannot find save file, to generate a new one run \"sudo ./bank\" (warning: will override file if it already exists)\n");
    }

    uint32_t buffer_size;
    users_load.read((char *)&buffer_size, 4);                             //reading first 32 bits for size
    FBE::bank_dom::GlobalFinalModel reader;                               //declaring model
    reader.resize(buffer_size);                                           //allocating new memory
    users_load.read((char *)reader.buffer().data() + 4, buffer_size - 4); //reading rest of file
    memcpy((char *)reader.buffer().data(), &buffer_size, 4);              //copying first 32 bits back

    if (!reader.verify())
    {
        throw std::invalid_argument("Data is corrupted\n");
    }
    bank_dom::Global users_global;
    reader.deserialize(users_global);

    for (size_t i = 0; i < users_global.users.size(); ++i)
    {
        Bank::users.try_emplace(users_global.keys[i], users_global.users[i]);
    }
}
