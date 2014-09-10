#ifndef _CMATH_HH
#define _CMATH_HH

#include <cmath>
#include <type_traits>

namespace std {

template <typename T>
  constexpr auto ispow2(T x) noexcept -> typename std::enable_if<std::is_integral<T>::value,bool>::type;
template <typename T>
  constexpr auto ceilp2(T x) noexcept -> typename std::enable_if<std::is_integral<T>::value,T>::type;
template <typename T>
  constexpr auto floorp2(T x) noexcept -> typename std::enable_if<std::is_integral<T>::value,T>::type;

} //namespace std

#endif
