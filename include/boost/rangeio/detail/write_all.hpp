//
// Copyright (c) Mark A. Gibbs, 2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// 

#ifndef BOOST_RANGEIO_Inc_detail_X_write_all_2015_01_01_
#define BOOST_RANGEIO_Inc_detail_X_write_all_2015_01_01_

#include <boost/config.hpp>

#include <cstddef>

#include <boost/rangeio/detail/formatting_saver.hpp>

// 
// This little macro allows C++11 forwarding-reference arguments for the
// delimiter in C++11 but falls back on const ref in C++98, so that one function
// can be used in both modes.
// 
#ifdef BOOST_NO_CXX11_RVALUE_REFERENCES
#   define BOOST_RANGEIO_Delimiter_Arg(x) x const&
#else
#   define BOOST_RANGEIO_Delimiter_Arg(x) x&&
#endif

namespace boost {
namespace rangeio {
namespace detail {

// 
// Underlying write_all() implementation - delimiter version.
// 
template <typename InputIterator,
          typename Sentinel,
          typename Delimiter,
          typename CharT,
          typename Traits>
void
write_all(
  std::basic_ostream<CharT, Traits>& out,
  InputIterator& i,
  Sentinel e,
  BOOST_RANGEIO_Delimiter_Arg(Delimiter) delim,
  std::size_t& n)
{
  if ((i != e) && bool(out))
  {
    boost::rangeio::detail::formatting_saver<CharT, Traits> formatting(out);
    
    if ((out << *i))
    {
      ++n;
      ++i;
      
      while ((i != e) && bool(out) && (out << delim))
      {
        formatting.restore();
        
        if ((out << *i))
        {
          ++n;
          ++i;
        }
      }
    }
  }
  
  out.width(0);
}

// 
// Underlying write_all() implementation - no delimiter version.
// 
template <typename InputIterator,
          typename Sentinel,
          typename CharT,
          typename Traits>
void
write_all(
  std::basic_ostream<CharT, Traits>& out,
  InputIterator& i,
  Sentinel e,
  std::size_t& n)
{
  if ((i != e) && bool(out))
  {
    boost::rangeio::detail::formatting_saver<CharT, Traits> formatting(out);
    
    if ((out << *i))
    {
      ++n;
      ++i;
      
      while ((i != e) && bool(out))
      {
        formatting.restore();
        
        if ((out << *i))
        {
          ++n;
          ++i;
        }
      }
    }
  }
  
  out.width(0);
}

} // namespace detail
} // namespace rangeio
} // namespace boost

#endif  // include guard
