//
// Copyright (c) Mark A. Gibbs, 2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// 

#ifndef BOOST_RANGEIO_TestInc_extras_X_smart_delimiters_2015_01_01_
#define BOOST_RANGEIO_TestInc_extras_X_smart_delimiters_2015_01_01_

#include <boost/config.hpp>

#include <cstddef>
#include <iosfwd>

namespace boost {
namespace rangeio {
namespace test_extras {

// 
// A simple smart delimiter that prints an int value between each item, that
// starts at 0 and increments by 1 after each write.
// 
struct incrementing_integer_delimiter
{
  incrementing_integer_delimiter() : i(0) {}
  
  std::size_t i;
};

template <typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& out, incrementing_integer_delimiter& iid)
{
  if (out << iid.i)
    ++iid.i;
  
  return out;
}

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES

template <typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& out, incrementing_integer_delimiter&& iid)
{
  return out << iid;
}

#endif // #ifndef BOOST_NO_CXX11_RVALUE_REFERENCES

} // namespace test_extras
} // namespace rangeio
} // namespace boost

#endif  // include guard
