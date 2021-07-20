#include "bank.h"

using namespace drogon;

__attribute__((always_inline)) inline bool ValidUsername(const std::string &name) noexcept
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
size_t Bank::NumOfUsers() const noexcept { return users.size(); }

//NOT THREAD SAFE
size_t Bank::NumOfLogs() const noexcept
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
size_t Bank::SumBal() const noexcept
{
    size_t res = 0;
    for (const auto &u : users)
    {
        res += u.second.balance;
    }
    return res;
}

BankResponse Bank::GetBal(const std::string &name) const noexcept
{
    uint32_t res = 0;
    if (!ValidUsername(name) || !users.if_contains(name, [&res](const User &u)
                                                   { res = u.balance; }))
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
    if (!users.modify_if(name, [&res](User &u)
                         { res = {k200OK, u.log.GetLogs()}; }))
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
    std::shared_lock<std::shared_mutex> lock{save_lock};
#if MAX_LOG_SIZE > 0
    time_t current_time = time(NULL);
#endif
    if (!users.modify_if(a_name, [current_time, &a_name, &b_name, &res, amount](User &a)
                         {
                             //if A can afford it
                             if (a.balance < amount)
                             {
                                 res = {k400BadRequest, "\"Insufficient funds\""};
                             }
                             else
                             {
                                 a.balance -= amount;
#if MAX_LOG_SIZE > 0
                                 a.log.AddTrans(a_name, b_name, amount, current_time);
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
        users.modify_if(b_name, [current_time, &a_name, &b_name, amount](User &b)
                        {
                            b.balance += amount;
                            b.log.AddTrans(a_name, b_name, amount, current_time);
                        });
#else
        users.modify_if(b_name, [amount](User &b)
                        { b.balance += amount; });
#endif
#if CONSERVATIVE_DISK_SAVE
#if MULTI_THREADED
        save_flag.SetChangesOn();
#else
        save_flag = true;
#endif
#endif
    }
    return res;
}
bool Bank::VerifyPassword(const std::string &name, const std::string_view &attempt) const noexcept
{
    bool res = false;
    users.if_contains(name, [&res, &attempt](const User &u)
                      { res = (u.password == xxHashStringGen{}(attempt)); });
    return res;
}

void Bank::ChangePassword(const std::string &name, const std::string &new_pass) noexcept
{
#if CONSERVATIVE_DISK_SAVE
#if MULTI_THREADED
    save_flag.SetChangesOn();
#else
    save_flag = true;
#endif
#endif
    users.modify_if(name, [&new_pass](User &u)
                    { u.password = xxHashStringGen{}(new_pass); });
}
BankResponse Bank::SetBal(const std::string &name, uint32_t amount) noexcept
{
    if (ValidUsername(name) && users.modify_if(name, [amount](User &u)
                                               { u.balance = amount; }))
    {
#if CONSERVATIVE_DISK_SAVE
#if MULTI_THREADED
        save_flag.SetChangesOn();
#else
        save_flag = true;
#endif
#endif
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
    uint32_t balance;
    if (ValidUsername(name) && users.modify_if(name, [&balance, amount](User &u)
                                               { balance = (u.balance < (amount * -1) ? u.balance = 0 : u.balance += amount); }))
    {
#if CONSERVATIVE_DISK_SAVE
#if MULTI_THREADED
        save_flag.SetChangesOn();
#else
        save_flag = true;
#endif
#endif
        return {k200OK, std::to_string(balance)}; //may return new balance
    }
    else
    {
        return {k404NotFound, "\"User not found\""};
    }
}
bool Bank::Contains(const std::string &name) const noexcept
{
    return ValidUsername(name) && users.contains(name);
}
bool Bank::AdminVerifyAccount(const std::string &name) noexcept
{
    return (name == admin_account);
}
BankResponse Bank::AddUser(const std::string &name, uint32_t init_bal, const std::string &init_pass) noexcept
{
    if (!ValidUsername(name))
    {
        return {k400BadRequest, "\"Invalid Username\""};
    }
    std::shared_lock<std::shared_mutex> lock{save_lock};
    if (users.try_emplace_l(
            name, [](User &) {}, init_bal, init_pass))
    {
#if CONSERVATIVE_DISK_SAVE
#if MULTI_THREADED
        save_flag.SetChangesOn();
#else
        save_flag = true;
#endif
#endif
        return {k204NoContent, std::nullopt};
    }
    else
    {
        return {k409Conflict, "\"User already exists\""};
    }
}
BankResponse Bank::DelUser(const std::string &name) noexcept
{
#if RETURN_ON_DEL
    uint32_t bal;
    if (users.if_contains(name, [&bal](const User &u)
                          { bal = u.balance; }) &&
        bal)
    {
        users.modify_if(return_account, [bal](User & u))
        {
            u.balance += bal;
        }
    }
#endif
    std::shared_lock<std::shared_mutex> lock{save_lock};
    if (ValidUsername(name) && users.erase(name))
    {
#if CONSERVATIVE_DISK_SAVE
#if MULTI_THREADED
        save_flag.SetChangesOn();
#else
        save_flag = true;
#endif
#endif
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
#if RETURN_ON_DEL
    uint32_t bal;
    if (users.if_contains(name, [&bal](const User &u)
                          { bal = u.balance; }) &&
        bal)
    {
        users.modify_if(return_account, [bal](User & u))
        {
            u.balance += bal;
        }
    }
#endif
#if CONSERVATIVE_DISK_SAVE
#if MULTI_THREADED
    save_flag.SetChangesOn();
#else
    save_flag = true;
#endif
#endif
    std::shared_lock<std::shared_mutex> lock{save_lock};
    users.erase(name);
}
//ONLY EVER BEING CALLED BY SAVE THREAD OR C-INTERUPT
const char *Bank::Save()
{
#if CONSERVATIVE_DISK_SAVE
    if (
#if MULTI_THREADED
        save_flag.GetChangeState()
#else
        save_flag
#endif
    )
    {
#endif
        std::ofstream users_save(users_location, std::ios::out | std::ios::binary);
        if (!users_save.is_open())
        {
            throw std::invalid_argument("Cannot access saving file\n");
        }
        bank_dom::Global users_copy;
        users_copy.users.reserve(users.size());
        users_copy.keys.reserve(users.size());
        {
            std::unique_lock<std::shared_mutex> lock{save_lock};
            for (const auto &u : users)
            {
                //we know it contains this key but we call this func to grab mutex
                users.if_contains(u.first, [&users_copy, &u](const User &u_val)
                                  {
                                      users_copy.users.emplace_back(u_val.Encode());
                                      users_copy.keys.emplace_back(u.first);
                                  });
            }
        }
        FBE::bank_dom::GlobalFinalModel writer;
        writer.serialize(users_copy);
        if (!writer.verify())
        {
            throw std::invalid_argument("Data is corrupted\n");
        }
        const FBE::FBEBuffer &write_buffer = writer.buffer();
        users_save.write((char *)write_buffer.data(), write_buffer.size());
        users_save.close();
        if (!users_save.good())
        {
            throw std::invalid_argument("Error occurred at writing\n");
        }
#if CONSERVATIVE_DISK_SAVE
#if MULTI_THREADED
        save_flag.SetChangesOff();
#else
        save_flag = true;
#endif
        return "        to disk...\n";
    }
    else
    {
        return "     no changes...\n";
    }
#endif
}
//NOT THREAD SAFE, BY NO MEANS SHOULD THIS BE CALLED WHILE RECEIEVING REQUESTS
void Bank::Load()
{
    std::ifstream users_load(users_location, std::ios::out | std::ios::binary);
    if (!users_load.is_open())
    {
        throw std::invalid_argument("Cannot find save file, to generate a new one run ./bank\n");
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
        users.try_emplace(users_global.keys[i], users_global.users[i]);
    }
}
