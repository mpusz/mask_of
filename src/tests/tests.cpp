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
#include <gtest/gtest.h>

enum class MarketState : std::uint8_t {
  A = 1 << 0,
  B = 1 << 1,
  C = 1 << 2,
  D = 1 << 3,
};
template<> struct is_bitmask<MarketState> : std::true_type {};

// explicit instantiation needed to make code coverage metrics work correctly
template class mask_of<MarketState>;

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

using mask = mask_of<MarketState>;

TEST(maskOf, CompileTime)
{
  static_assert(0 == mask{}, "");

  static_assert(MarketState::A == mask{MarketState::A}, "");

  static_assert(mask{MarketState::A} == mask{MarketState::A}, "");
  static_assert(mask{MarketState::B} == mask{MarketState::B}, "");
  static_assert(mask{MarketState::C} == mask{MarketState::C}, "");
  static_assert(MarketState::A == mask{MarketState::A}, "");
  static_assert(MarketState::B == mask{MarketState::B}, "");
  static_assert(MarketState::C == mask{MarketState::C}, "");
  static_assert(mask{MarketState::A} == MarketState::A, "");
  static_assert(mask{MarketState::B} == MarketState::B, "");
  static_assert(mask{MarketState::C} == MarketState::C, "");

  static_assert(mask{MarketState::A} != mask{}, "");
  static_assert(mask{MarketState::B} != mask{MarketState::A}, "");
  static_assert(MarketState::A != mask{}, "");
  static_assert(MarketState::B != mask{MarketState::A}, "");
  static_assert(mask{} != MarketState::A, "");
  static_assert(mask{MarketState::A} != MarketState::B, "");

  static_assert(MarketState::A == (mask{MarketState::A} & mask{MarketState::A}), "");
  static_assert(MarketState::A != (mask{MarketState::A} & mask{MarketState::B}), "");
  static_assert(MarketState::A == (mask{MarketState::A} & MarketState::A), "");
  static_assert(MarketState::A != (mask{MarketState::A} & MarketState::B), "");
  static_assert(MarketState::A == (MarketState::A & mask{MarketState::A}), "");
  static_assert(MarketState::A != (MarketState::A & mask{MarketState::B}), "");
  static_assert(MarketState::A == (MarketState::A & MarketState::A), "");
  static_assert(MarketState::A != (MarketState::A & MarketState::B), "");

  static_assert(MarketState::A == (mask{MarketState::A} | mask{MarketState::A}), "");
  static_assert(MarketState::A != (mask{MarketState::A} | mask{MarketState::B}), "");
  static_assert(MarketState::A == (mask{MarketState::A} | MarketState::A), "");
  static_assert(MarketState::A != (mask{MarketState::A} | MarketState::B), "");
  static_assert(MarketState::A == (MarketState::A | mask{MarketState::A}), "");
  static_assert(MarketState::A != (MarketState::A | mask{MarketState::B}), "");
  static_assert(MarketState::A == (MarketState::A | MarketState::A), "");
  static_assert(MarketState::A != (MarketState::A | MarketState::B), "");

  static_assert(mask{}                                  == (mask{MarketState::A} ^ mask{MarketState::A}), "");
  static_assert((MarketState::A | mask{MarketState::B}) == (mask{MarketState::A} ^ mask{MarketState::B}), "");
  static_assert(mask{}                                  == (mask{MarketState::A} ^ MarketState::A), "");
  static_assert((MarketState::A | mask{MarketState::B}) == (mask{MarketState::A} ^ MarketState::B), "");
  static_assert(mask{}                                  == (MarketState::A ^ mask{MarketState::A}), "");
  static_assert((MarketState::A | mask{MarketState::B}) == (MarketState::A ^ mask{MarketState::B}), "");
  static_assert(mask{}                                  == (MarketState::A ^ MarketState::A), "");
  static_assert((MarketState::A | mask{MarketState::B}) == (MarketState::A ^ MarketState::B), "");

  static_assert(mask{} != ~mask{}, "");
  static_assert(~MarketState::A == ~mask{MarketState::A}, "");
}

TEST(maskOf, DefaultConstructor)
{
  EXPECT_EQ(0, mask{});
}

TEST(maskOf, UserConstructor)
{
  EXPECT_EQ(MarketState::A, mask{MarketState::A});
}

TEST(maskOf, eqOp)
{
  EXPECT_EQ(mask{MarketState::A}, mask{MarketState::A});
  EXPECT_EQ(mask{MarketState::B}, mask{MarketState::B});
  EXPECT_EQ(mask{MarketState::C}, mask{MarketState::C});
  EXPECT_EQ(MarketState::A, mask{MarketState::A});
  EXPECT_EQ(MarketState::B, mask{MarketState::B});
  EXPECT_EQ(MarketState::C, mask{MarketState::C});
  EXPECT_EQ(mask{MarketState::A}, MarketState::A);
  EXPECT_EQ(mask{MarketState::B}, MarketState::B);
  EXPECT_EQ(mask{MarketState::C}, MarketState::C);
}

TEST(maskOf, neOp)
{
  EXPECT_NE(mask{MarketState::A}, mask{});
  EXPECT_NE(mask{MarketState::B}, mask{MarketState::A});
  EXPECT_NE(MarketState::A, mask{});
  EXPECT_NE(MarketState::B, mask{MarketState::A});
  EXPECT_NE(mask{}, MarketState::A);
  EXPECT_NE(mask{MarketState::A}, MarketState::B);
}

TEST(maskOf, and)
{
  EXPECT_EQ(MarketState::A, mask{MarketState::A} & mask{MarketState::A});
  EXPECT_NE(MarketState::A, mask{MarketState::A} & mask{MarketState::B});
  EXPECT_EQ(MarketState::A, mask{MarketState::A} & MarketState::A);
  EXPECT_NE(MarketState::A, mask{MarketState::A} & MarketState::B);
  EXPECT_EQ(MarketState::A, MarketState::A & mask{MarketState::A});
  EXPECT_NE(MarketState::A, MarketState::A & mask{MarketState::B});
  EXPECT_EQ(MarketState::A, MarketState::A & MarketState::A);
  EXPECT_NE(MarketState::A, MarketState::A & MarketState::B);
}

TEST(maskOf, or)
{
  EXPECT_EQ(MarketState::A, mask{MarketState::A} | mask{MarketState::A});
  EXPECT_NE(MarketState::A, mask{MarketState::A} | mask{MarketState::B});
  EXPECT_EQ(MarketState::A, mask{MarketState::A} | MarketState::A);
  EXPECT_NE(MarketState::A, mask{MarketState::A} | MarketState::B);
  EXPECT_EQ(MarketState::A, MarketState::A | mask{MarketState::A});
  EXPECT_NE(MarketState::A, MarketState::A | mask{MarketState::B});
  EXPECT_EQ(MarketState::A, MarketState::A | MarketState::A);
  EXPECT_NE(MarketState::A, MarketState::A | MarketState::B);
}

TEST(maskOf, xor)
{
  EXPECT_EQ(mask{},                                mask{MarketState::A} ^ mask{MarketState::A});
  EXPECT_EQ(MarketState::A | mask{MarketState::B}, mask{MarketState::A} ^ mask{MarketState::B});
  EXPECT_EQ(mask{},                                mask{MarketState::A} ^ MarketState::A);
  EXPECT_EQ(MarketState::A | mask{MarketState::B}, mask{MarketState::A} ^ MarketState::B);
  EXPECT_EQ(mask{},                                MarketState::A ^ mask{MarketState::A});
  EXPECT_EQ(MarketState::A | mask{MarketState::B}, MarketState::A ^ mask{MarketState::B});
  EXPECT_EQ(mask{},                                MarketState::A ^ MarketState::A);
  EXPECT_EQ(MarketState::A | mask{MarketState::B}, MarketState::A ^ MarketState::B);
}

TEST(maskOf, not)
{
  EXPECT_NE(mask{}, ~mask{});
  EXPECT_EQ(~MarketState::A, ~mask{MarketState::A});
}
