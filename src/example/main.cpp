#include "mask_of.h"
#include <cstdint>
#include <string>
#include <cassert>
#include <iostream>


enum class MarketState : std::uint8_t {
  A = 1 << 0,
  B = 1 << 1,
  C = 1 << 2,
  D = 1 << 3,
};
template<> struct is_bitmask<MarketState> : std::true_type {};

std::string to_string(MarketState s)
{
  switch(s) {
    case MarketState::A: return "STATE_A";
    case MarketState::B: return "STATE_B";
    case MarketState::C: return "STATE_C";
    case MarketState::D: return "STATE_D";
  }
  assert(false);
  return {};
}

constexpr bool is_in_state(MarketState current, mask_of<MarketState> expected_mask)
{
  return (expected_mask & current) != 0;
}

auto get_state() { return MarketState::B; }

void foo()
{
  MarketState state = get_state();
  const bool valid = is_in_state(state, MarketState::A | MarketState::B);
  std::cout << to_string(state) << ", valid: " << (valid ? "true" : "false") << "\n";
}

int main()
{
  constexpr mask_of<MarketState> s1{MarketState::A};
  constexpr mask_of<MarketState> s2{MarketState::A | MarketState::B};
  static_assert(is_in_state(MarketState::A, s1), "Wrong state");
  static_assert(is_in_state(MarketState::A, s2), "Wrong state");

  foo();
}
