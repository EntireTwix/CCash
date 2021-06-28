#include "admin_filter.h"

static char DecodeChar2(const char ch)
{
    if (ch >= 'A' && ch <= 'Z')
    {
        return ch - 'A';
    }
    if (ch >= 'a' && ch <= 'z')
    {
        return ch - 'a' + 26;
    }
    if (ch >= '0' && ch <= '9')
    {
        return ch - '0' + 52;
    }
    return 63 - (ch == '-');
}

char *DecodeBase642(const char *string)
{
    char *output;
    size_t length = strlen(string);
    if (!(output = (char *)malloc(1 + (length >> 2) * 3 - (string[length - 1] == '=') - (string[length - 2] == '='))))
    {
        return (char *)0;
    }

    size_t index = 0;
    uint32_t storage = 0;
    while (string[4])
    {
        storage |= DecodeChar2(*string++) << 18;
        storage |= DecodeChar2(*string++) << 12;
        storage |= DecodeChar2(*string++) << 6;
        storage |= DecodeChar2(*string++);

        output[index++] = storage >> 16;
        output[index++] = (char)(storage >> 8);
        output[index++] = (char)storage;

        storage = 0;
    }

    storage |= DecodeChar2(*string++) << 18;
    storage |= DecodeChar2(*string++) << 12;
    output[index++] = storage >> 16;

    if (*string == '=')
    {
        output[index] = '\0';
        return output;
    }
    storage |= DecodeChar2(*string++) << 6;
    output[index++] = (char)(storage >> 8);

    if (*string == '=')
    {
        output[index] = '\0';
        return output;
    }
    storage |= DecodeChar2(*string);
    output[index++] = (char)storage;

    output[index] = '\0';
    return output;
}

AdminFilter::AdminFilter(Bank &b) : bank(b) {}

void AdminFilter::doFilter(const HttpRequestPtr &req,
                           FilterCallback &&fcb,
                           FilterChainCallback &&fccb)
{
    const std::string &auth_header = req->getHeader("Authorization");
    if (auth_header.size() > 6)
    {
        if (auth_header.substr(0, 6) == "Basic ")
        {
            std::stringstream ss(DecodeBase642(auth_header.substr(6).c_str()));
            std::string username, password;
            std::getline(ss, username, ':');
            std::getline(ss, password);
            if (bank.AdminVerifyPass(password)) //is admin
            {
                if (bank.VerifyPassword(username, password)) //is valid pair
                {
                    fccb();
                    return;
                }
            }
        }
    }
    auto resp = HttpResponse::newHttpJsonResponse("Invalid Credentials");
    resp->setStatusCode(k401Unauthorized);
    fcb(resp);
}