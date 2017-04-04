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
  const MarketState state = get_state();
  const bool valid = is_in_state(state, MarketState::A | MarketState::B);
  std::cout << to_string(state) << ", valid: " << (valid ? "true" : "false") << "\n";
}

int main()
{
  foo();
}
