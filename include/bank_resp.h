#pragma once
#include <string>
#include <optional>
#include <drogon/HttpTypes.h>
#include <drogon/HttpResponse.h>
#include <../src/HttpResponseImpl.h>
#include <../src/HttpAppFrameworkImpl.h>

using BankResponse = std::pair<drogon::HttpStatusCode, std::optional<std::string>>;

template <>
drogon::HttpResponsePtr drogon::toResponse(BankResponse &&data);