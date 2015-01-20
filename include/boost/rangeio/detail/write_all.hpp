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

namespace boost {
namespace rangeio {
namespace detail {

// 
// In order to provide even limited support for smart delimiters in C++98, it is
// necessary to have THREE different versions of detail::write_all() with
// delimiter.
// 
// In C++98 mode, you need 2 overloads - one that takes the delimiter by ref,
// and one that takes it by const-ref.
// 
// In C++11 (or better), you just need a single function: the delimiter is taken
// as a universal reference. This not only covers both the C++98 overloads, it
// allows for non-const rvalue delimiters.
// 
// All three of these functions have the exact same body. The only difference is
// how the arguments are taken. Unfortunately, without some grodiness like
// casting away const-ness, there's no way that all three can be deferred to a
// single fourth function. The only option to avoid code duplication is...
// ugh... a macro.
// 
// This is that macro. It is the function body of the delimiter version of
// detail::write_all(). It is used in all three of those functions - the two for
// C++98 (that take the delimiter by ref and const-ref), and the one for C++11
// (that takes the delimiter as a forwarding-ref).
// 
#define BOOST_RANGEIO_WriteAllImpl \
  if ((i != e) && bool(out)) \
  { \
    ::boost::rangeio::detail::formatting_saver<CharT, Traits> formatting(out); \
    \
    if ((out << *i)) \
    { \
      ++n; \
      ++i; \
      \
      while ((i != e) && bool(out) && (out << delim)) \
      { \
        formatting.restore(); \
        \
        if ((out << *i)) \
        { \
          ++n; \
          ++i; \
        } \
      } \
    } \
  } \
  \
  out.width(0);

// 
// Underlying write_all() implementation - delimiter version.
// 

#ifdef BOOST_NO_CXX11_RVALUE_REFERENCES

// 
// C++98 version of detail::write_all() with delimiter, non-const delimiter version.
// 
// Available only in C++98 mode.
// 
template <typename InputIterator, typename Sentinel, typename Delimiter, typename CharT, typename Traits>
void write_all(std::basic_ostream<CharT, Traits>& out, InputIterator& i, Sentinel e, Delimiter& delim, std::size_t& n)
{
  BOOST_RANGEIO_WriteAllImpl
}

// 
// C++98 version of detail::write_all() with delimiter, const delimiter version.
// 
// Available only in C++98 mode.
// 
template <typename InputIterator, typename Sentinel, typename Delimiter, typename CharT, typename Traits>
void write_all(std::basic_ostream<CharT, Traits>& out, InputIterator& i, Sentinel e, Delimiter const& delim, std::size_t& n)
{
  BOOST_RANGEIO_WriteAllImpl
}

#else // BOOST_NO_CXX11_RVALUE_REFERENCES

// 
// C++11 version of detail::write_all() with delimiter.
// 
// Available only in C++11 or better mode.
// 
template <typename InputIterator, typename Sentinel, typename Delimiter, typename CharT, typename Traits>
void write_all(std::basic_ostream<CharT, Traits>& out, InputIterator& i, Sentinel e, Delimiter&& delim, std::size_t& n)
{
  BOOST_RANGEIO_WriteAllImpl
}

#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

#undef BOOST_RANGEIO_WriteAllImpl

// 
// Underlying write_all() implementation - no delimiter version.
// 
template <typename InputIterator, typename Sentinel, typename CharT, typename Traits>
void write_all(std::basic_ostream<CharT, Traits>& out, InputIterator& i, Sentinel e, std::size_t& n)
{
  if ((i != e) && bool(out))
  {
    ::boost::rangeio::detail::formatting_saver<CharT, Traits> formatting(out);
    
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
