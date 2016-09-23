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


#include <type_traits>

namespace std {

template< class T >
constexpr bool is_enum_v = is_enum<T>::value;

}

template<typename T>
struct is_bitmask : std::false_type {};

template<typename T>
using is_bitmask_t = typename is_bitmask<T>::type;
template<typename T>
constexpr bool is_bitmask_v = is_bitmask<T>::value;

template<typename Enum>
class mask_of {
private:
  static_assert(std::is_enum_v<Enum>, "T should be an enumeration type");
  using u_type = std::underlying_type_t<Enum>;

public:
  using value_type = decltype(std::declval<u_type>() | std::declval<u_type>());

  constexpr explicit mask_of(Enum v) noexcept : value_{static_cast<u_type>(v)} {}
  constexpr explicit mask_of(value_type v) noexcept : value_{v} {}
  constexpr operator value_type () const noexcept { return value_; }

  friend constexpr mask_of operator&(mask_of lhs, Enum rhs)
  {
    return mask_of{static_cast<value_type>(lhs) & static_cast<u_type>(rhs)};
  }
  friend constexpr mask_of operator&(Enum lhs, mask_of rhs)
  {
    return mask_of{static_cast<u_type>(lhs) & static_cast<value_type>(rhs)};
  }

private:
  value_type value_;
};

template<typename Enum>
std::enable_if_t<is_bitmask_v<Enum>, mask_of<Enum>>
constexpr operator|(Enum lhs, Enum rhs)
{
  using u_type = std::underlying_type_t<Enum>;
  return mask_of<Enum>{static_cast<u_type>(lhs) | static_cast<u_type>(rhs)};
}

template<typename Enum>
std::enable_if_t<is_bitmask_v<Enum>, mask_of<Enum>>
constexpr operator&(Enum lhs, Enum rhs)
{
  using u_type = std::underlying_type_t<Enum>;
  return mask_of<Enum>{static_cast<u_type>(lhs) & static_cast<u_type>(rhs)};
}
