#pragma once
#include <atomic>

class ChangeFlag
{
private:
    std::atomic<bool> change_flag = false; //if true changes have been made

public:
    ChangeFlag() noexcept;
    ChangeFlag(ChangeFlag &&) noexcept;

    void SetChangesOn() noexcept;
    void SetChangesOff() noexcept;
    bool GetChangeState() const noexcept;
};