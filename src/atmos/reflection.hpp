#pragma once

#include <type_traits>

//---------------------------------------------------------------------------------------------------------------------

template <typename... Types>
struct TypeList
{
};

//---------------------------------------------------------------------------------------------------------------------

template <typename TypeList1, typename TypeList2>
struct JoinTypeListsImpl;

template <typename... Types1, typename... Types2>
struct JoinTypeListsImpl<TypeList<Types1...>, TypeList<Types2...>>
{
    using Result = TypeList<Types1..., Types2...>;
};

template <typename TypeList1, typename TypeList2>
using JoinTypeLists = typename JoinTypeListsImpl<TypeList1, TypeList2>::Result;

//---------------------------------------------------------------------------------------------------------------------

template <unsigned int Num>
using NumType = std::integral_constant<unsigned int, Num>;

//---------------------------------------------------------------------------------------------------------------------

template <unsigned int Begin, unsigned int End>
struct GenerateNumListImpl
{
    static_assert(Begin <= End, "GenerateNumListImpl: end is less than begin");
    using Result = JoinTypeLists<TypeList<NumType<Begin>>, typename GenerateNumListImpl<Begin + 1, End>::Result>;
};

template <unsigned int Begin>
struct GenerateNumListImpl<Begin, Begin>
{
    using Result = TypeList<>;
};

template <unsigned int Begin, unsigned int End>
using GenerateNumList = typename GenerateNumListImpl<Begin, End>::Result;


//---------------------------------------------------------------------------------------------------------------------

#define START_REFLECTABLE_MEMBERS()                         \
    template <typename Owner, unsigned int Line>            \
    struct MemberGetterForLine;                             \
    static constexpr unsigned int FirstLine = __LINE__

#define END_REFLECTABLE_MEMBERS()                           \
    static constexpr unsigned int EndLine = __LINE__


#define REFLECTABLE_MEMBER(type, name)                                                              \
    template <typename Owner>                                                                       \
    struct MemberGetterForLine<Owner, __LINE__>                                                     \
    {                                                                                               \
        using ReflectableMemberType = type;                                                         \
        static constexpr const char* ReflectableMemberName = #name;                                 \
        static std::add_lvalue_reference_t<type> getMember(Owner& owner)                            \
        {                                                                                           \
            return owner.name;                                                                      \
        }                                                                                           \
        static std::add_lvalue_reference_t<std::add_const_t<type>> getMember(const Owner& owner)    \
        {                                                                                           \
            return owner.name;                                                                      \
        }                                                                                           \
    };                                                                                              \
    type name

//---------------------------------------------------------------------------------------------------------------------

template <class T>
class IsReflectable {
    struct Char1 { char ch[1]; };
    struct Char2 { char ch[2]; };

    template <class U>
    static Char1 check(decltype(U::FirstLine)*, decltype(U::EndLine)*);

    template <class U>
    static Char2 check(...);

public:
    static const bool value = sizeof(check<T>(nullptr, nullptr)) == sizeof(Char1);
};
