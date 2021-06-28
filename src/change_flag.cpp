#include "change_flag.h"

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