#pragma once

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "data.hpp"
#include "reflection.hpp"

using PropTreeConstRef = const boost::property_tree::ptree&;

template <typename Owner>
struct Setter;

template <typename T>
static
typename std::enable_if<!IsReflectable<T>::value, void>::type
deserializeMemberImpl(PropTreeConstRef pt, T& member, const char* name)
{
    member = pt.get<T>(name);
}

template <typename T>
static
typename std::enable_if<!IsReflectable<T>::value, void>::type
deserializeMemberImpl(PropTreeConstRef pt, std::optional<T>& member, const char* name)
{
    const auto& opt = pt.get_optional<T>(name);
    member = opt.has_value() ? std::optional<T>(opt.get()) : std::optional<T>();
}

template <typename T>
static
typename std::enable_if<IsReflectable<T>::value, void>::type
deserializeMemberImpl(PropTreeConstRef pt, std::vector<T>& member, const char* name)
{
    for (const auto& elem : pt.get_child(name)) {
        member.emplace_back(Setter<T>::deserialize(elem.second));
    }
}

template <typename T>
static
typename std::enable_if<IsReflectable<T>::value, void>::type
deserializeMemberImpl(PropTreeConstRef pt, T& member, const char* name)
{
    member = Setter<T>::deserialize(pt.get_child(name));
}

template <typename Owner>
class Setter
{
    template <unsigned int Line>
    using MemberSetter = typename Owner::template MemberGetterForLine<Owner, Line>;
    
    using MemberLines = GenerateNumList<Owner::FirstLine+1, Owner::EndLine>;

    template <typename TypeListHead, typename... TypeListTail>
    static void setImpl(Owner& owner, PropTreeConstRef pt, TypeList<TypeListHead, TypeListTail...>)
    {
        auto& member = MemberSetter<TypeListHead::value>::getMember(owner);
        const char* name = MemberSetter<TypeListHead::value>::ReflectableMemberName;
        deserializeMemberImpl(pt, member, name);
        setImpl(owner, pt, TypeList<TypeListTail...>());
    }

    static void setImpl(Owner& owner, PropTreeConstRef pt, TypeList<>)
    {
    }
public:
    static Owner deserialize(PropTreeConstRef pt)
    {
        Owner ret;
        setImpl(ret, pt, MemberLines());
        return ret;
    }
};

template <typename T>
T deserialize(const std::string& data)
{
  std::stringstream ss(data);

  boost::property_tree::ptree pt;
  boost::property_tree::read_json(ss, pt);

  return Setter<T>::deserialize(pt);
}
