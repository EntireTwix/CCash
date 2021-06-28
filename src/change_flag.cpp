#include "change_flag.h"

ChangeFlag::ChangeFlag() noexcept {}

ChangeFlag::ChangeFlag(ChangeFlag &&f) noexcept
{
    change_flag.store(f.GetChangeState(), std::memory_order_release); //is this safe?
}

void ChangeFlag::SetChangesOn() noexcept
{
    return change_flag.store(1, std::memory_order_release);
}
void ChangeFlag::SetChangesOff() noexcept
{
    return change_flag.store(0, std::memory_order_release);
}
bool ChangeFlag::GetChangeState() const noexcept
{
    return change_flag.load(std::memory_order_acquire);
}