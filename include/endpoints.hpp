#pragma once
enum Endpoint
{
    UserNotFound = -1,
    WrongPassword = -2,
    Refunded = -3,
    InvalidRequest = -4,
    WrongAdminPassword = -5,
    NameTooLong = -6,
    UserAlreadyExists = -7,
    InsufficientFunds = -8,
};