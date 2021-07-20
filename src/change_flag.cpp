#include "change_flag.h"

template <bool init>
ChangeFlag<init>::ChangeFlag() noexcept {}
template <bool init>
ChangeFlag<init>::ChangeFlag(ChangeFlag &&f) noexcept
{
    change_flag.store(f.GetChangeState(), std::memory_order_release);
}

template <bool init>
void ChangeFlag<init>::SetChangesOn() noexcept
{
    return change_flag.store(1, std::memory_order_release);
}
template <bool init>
void ChangeFlag<init>::SetChangesOff() noexcept
{
    return change_flag.store(0, std::memory_order_release);
}
template <bool init>
bool ChangeFlag<init>::GetChangeState() const noexcept
{
    return change_flag.load(std::memory_order_acquire);
}

template class ChangeFlag<true>;
template class ChangeFlag<false>;