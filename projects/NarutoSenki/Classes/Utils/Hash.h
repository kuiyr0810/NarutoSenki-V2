#pragma once
#include <string>
#include "Data/Config.h"

namespace HashUtils {

DETAIL_NS_BEGIN

constexpr uint32_t hash32(const char *str, size_t n, uint32_t basis = 2166136261U)
{
	return n == 0 ? basis : hash32(str + 1, n - 1, (basis ^ str[0]) * 16777619U);
}

constexpr uint64_t hash64(const char *str, size_t n, uint64_t basis = 14695981039346656037UL)
{
	return n == 0 ? basis : hash64(str + 1, n - 1, (basis ^ str[0]) * 1099511628211UL);
}

static inline uint32_t runtime_hash32(const char *str, size_t n, uint32_t basis = 2166136261U)
{
	return n == 0 ? basis : runtime_hash32(str + 1, n - 1, (basis ^ str[0]) * 16777619U);
}

static inline uint64_t runtime_hash64(const char *str, size_t n, uint64_t basis = 14695981039346656037UL)
{
	return n == 0 ? basis : runtime_hash64(str + 1, n - 1, (basis ^ str[0]) * 1099511628211UL);
}

DETAIL_NS_END

template <size_t N>
constexpr uint32_t hash32(const char (&s)[N]) { return detail::hash32(s, N - 1); }

template <size_t N>
constexpr uint64_t hash64(const char (&s)[N]) { return detail::hash64(s, N - 1); }

static inline uint32_t hash32(const std::string &s) { return detail::runtime_hash32(s.c_str(), s.length()); }

static inline uint64_t hash64(const std::string &s) { return detail::runtime_hash64(s.c_str(), s.length()); }

}
