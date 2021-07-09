#include "str_intrusion.h"

//this function is horribly jank
template <typename Tag>
struct result
{
    typedef typename Tag::type type;
    static type ptr;
};
template <typename Tag>
typename result<Tag>::type result<Tag>::ptr;

template <typename Tag, typename Tag::type p>
struct rob : result<Tag>
{
    struct filler
    {
        filler() { result<Tag>::ptr = p; }
    };
    static filler filler_obj;
};
template <typename Tag, typename Tag::type p>
typename rob<Tag, p>::filler rob<Tag, p>::filler_obj;
struct string_length
{
    typedef void (std::string::*type)(size_t);
};
template class rob<string_length, &std::string::_M_length>;
struct string_data
{
    typedef void (std::string::*type)(char *);
};
template class rob<string_data, &std::string::_M_data>;

StrFromSV_Wrapper::StrFromSV_Wrapper(std::string_view sv) noexcept
{
    (str.*result<string_data>::ptr)((char *)sv.data());
    (str.*result<string_length>::ptr)(sv.size());
}
StrFromSV_Wrapper::~StrFromSV_Wrapper() noexcept
{
    (str.*result<string_data>::ptr)(nullptr);
    (str.*result<string_length>::ptr)(0);
}