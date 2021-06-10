#pragma once
enum ErrorResponse
{
    UserNotFound = -1,
    WrongPassword = -2,
    InvalidRequest = -3,
    WrongAdminPassword = -4,
    NameTooLong = -5,
    UserAlreadyExists = -6,
    InsufficientFunds = -7,
};