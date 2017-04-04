// The MIT License (MIT)
//
// Copyright (c) 2016 Mateusz Pusz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <type_traits>

template<typename T>
struct is_bitmask : std::false_type {};

template<typename T>
using is_bitmask_t = typename is_bitmask<T>::type;
template<typename T>
constexpr bool is_bitmask_v = is_bitmask<T>::value;

template<typename Enum>
class mask_of {
public:
  static_assert(std::is_enum<Enum>::value, "T should be an enumeration type");
  static_assert(is_bitmask_v<Enum>, "T should be a bitmask type");

  using value_type = std::underlying_type_t<Enum>;

  mask_of() = default;
  constexpr explicit mask_of(Enum e) noexcept : value_{static_cast<value_type>(e)} {}
  constexpr /* explicit */ operator value_type () const noexcept { return value_; }

  constexpr bool operator ==(mask_of<Enum> other) const noexcept { return value_ == other.value_; }

  constexpr mask_of& operator &=(mask_of other) noexcept         { value_ &= other.value_; return *this; }
  constexpr mask_of& operator |=(mask_of other) noexcept         { value_ |= other.value_; return *this; }
  constexpr mask_of& operator ^=(mask_of other) noexcept         { value_ ^= other.value_; return *this; }
  constexpr mask_of operator ~() const noexcept                  { return mask_of{static_cast<value_type>(~value_)}; }
  constexpr mask_of& operator &=(Enum v) noexcept                { return *this &= mask_of<Enum>{v}; }
  constexpr mask_of& operator |=(Enum v) noexcept                { return *this |= mask_of<Enum>{v}; }
  constexpr mask_of& operator ^=(Enum v) noexcept                { return *this ^= mask_of<Enum>{v}; }

private:
  value_type value_;
  constexpr explicit mask_of(value_type v) noexcept : value_{v} {}
};

template<typename Enum> constexpr bool operator != (mask_of<Enum> lhs, mask_of<Enum> rhs) noexcept        { return !(lhs == rhs); }
template<typename Enum> constexpr bool operator == (mask_of<Enum> lhs, Enum rhs) noexcept                 { return lhs == mask_of<Enum>{rhs}; }
template<typename Enum> constexpr bool operator != (mask_of<Enum> lhs, Enum rhs) noexcept                 { return lhs != mask_of<Enum>{rhs}; }
template<typename Enum> constexpr bool operator == (Enum lhs, mask_of<Enum> rhs) noexcept                 { return mask_of<Enum>{lhs} == rhs; }
template<typename Enum> constexpr bool operator != (Enum lhs, mask_of<Enum> rhs) noexcept                 { return mask_of<Enum>{lhs} != rhs; }

template<typename Enum> constexpr mask_of<Enum> operator &(mask_of<Enum> lhs, mask_of<Enum> rhs) noexcept { return lhs &= rhs; }
template<typename Enum> constexpr mask_of<Enum> operator |(mask_of<Enum> lhs, mask_of<Enum> rhs) noexcept { return lhs |= rhs; }
template<typename Enum> constexpr mask_of<Enum> operator ^(mask_of<Enum> lhs, mask_of<Enum> rhs) noexcept { return lhs ^= rhs; }

template<typename Enum> constexpr mask_of<Enum> operator &(mask_of<Enum> lhs, Enum rhs) noexcept          { return lhs & mask_of<Enum>{rhs}; }
template<typename Enum> constexpr mask_of<Enum> operator |(mask_of<Enum> lhs, Enum rhs) noexcept          { return lhs | mask_of<Enum>{rhs}; }
template<typename Enum> constexpr mask_of<Enum> operator ^(mask_of<Enum> lhs, Enum rhs) noexcept          { return lhs ^ mask_of<Enum>{rhs}; }
template<typename Enum> constexpr mask_of<Enum> operator &(Enum lhs, mask_of<Enum> rhs) noexcept          { return mask_of<Enum>{lhs} & rhs; }
template<typename Enum> constexpr mask_of<Enum> operator |(Enum lhs, mask_of<Enum> rhs) noexcept          { return mask_of<Enum>{lhs} | rhs; }
template<typename Enum> constexpr mask_of<Enum> operator ^(Enum lhs, mask_of<Enum> rhs) noexcept          { return mask_of<Enum>{lhs} ^ rhs; }

template<typename Enum> constexpr std::enable_if_t<is_bitmask_v<Enum>, mask_of<Enum>> operator &(Enum lhs, Enum rhs) { return mask_of<Enum>{lhs} & mask_of<Enum>{rhs}; }
template<typename Enum> constexpr std::enable_if_t<is_bitmask_v<Enum>, mask_of<Enum>> operator |(Enum lhs, Enum rhs) { return mask_of<Enum>{lhs} | mask_of<Enum>{rhs}; }
template<typename Enum> constexpr std::enable_if_t<is_bitmask_v<Enum>, mask_of<Enum>> operator ^(Enum lhs, Enum rhs) { return mask_of<Enum>{lhs} ^ mask_of<Enum>{rhs}; }
template<typename Enum> constexpr std::enable_if_t<is_bitmask_v<Enum>, mask_of<Enum>> operator ~(Enum v)             { return ~mask_of<Enum>{v}; }
