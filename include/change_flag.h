#pragma once
#include <atomic>

template <bool init>
class ChangeFlag
{
private:
    std::atomic<bool> change_flag = init; // if true changes have been made

public:
    ChangeFlag() noexcept;
    ChangeFlag(ChangeFlag &&) noexcept;

    void SetChangesOn() noexcept;
    void SetChangesOff() noexcept;
    bool GetChangeState() const noexcept;
};