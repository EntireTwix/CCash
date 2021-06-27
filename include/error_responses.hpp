#pragma once
enum ErrorResponse
{
    UserNotFound = -1,      //404
    WrongPassword = -2,     //401
    InvalidRequest = -3,    //400
    NameTooLong = -4,       //400?
    UserAlreadyExists = -5, //409
    InsufficientFunds = -6, //402?
};