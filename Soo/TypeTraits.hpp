#pragma once
#include <map>
#include <set>
#include <type_traits>
#include <vector>

namespace Soo{

template <typename Container>
struct IsVectorLike : std::false_type
{};

template <typename T, typename Alloc>
struct IsVectorLike<std::vector<T, Alloc>> : std::true_type
{};

template <typename Container>
struct IsSetLike : std::false_type
{};

template <typename T, typename Compare, typename Alloc>
struct IsSetLike<std::set<T, Compare, Alloc>> : std::true_type
{};

template <typename Container>
struct IsMapLike : std::false_type
{};

template <typename Key, typename T, typename Compare, typename Alloc>
struct IsMapLike<std::map<Key, T, Compare, Alloc>> : std::true_type
{};

} //namespace Soo
