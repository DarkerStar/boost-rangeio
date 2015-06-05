//
// Copyright (c) Mark A. Gibbs, 2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// 

// Contains low-level implementation functions for the range write operations
// used throughout the library.
// 
// This file is written to be C++98-safe.

#ifndef BOOST_RANGEIO_Inc_detail_X_write_all_2015_01_01_
#define BOOST_RANGEIO_Inc_detail_X_write_all_2015_01_01_

#include <boost/config.hpp>

#include <cstddef>

#include <boost/rangeio/detail/formatting_saver.hpp>

namespace boost {
namespace rangeio {
namespace detail {

// Underlying implementation function for all versions of write without
// delimiters.
// 
// Thus function is ultimately used under the hood by all of the write
// functions in the RangeIO library that do not write delimiters between the
// elements. The interface is designed so that higher level structures can
// have their members updated via references as the write proceeds, so if there
// is any kind of error, those members will be left in the last good state.
// 
// While i is not equal to e and bool(out) is true, performs "out << *i". If
// boo(out) is still true, performs "++i" and "++n".
// 
// Between each write, the stream's formatting state is restored to what it
// was before the first write.
// 
// At the end of the function, whether there have been any writes or not, the
// stream width is set to zero.
template <
  typename InputIterator,
  typename Sentinel,
  typename CharT,
  typename Traits>
void
write_impl(
  ::std::basic_ostream<CharT, Traits>& out,
  InputIterator& i,
  Sentinel const e,
  ::std::size_t& n)
{
  // Only bother to attempt writing if the range is empty (i == e) or the
  // output stream is good (bool(out) is true).
  if (!(i == e) && bool(out))
  {
    // Save the formatting state prior to writing the first element.
    detail::formatting_saver<CharT, Traits> formatting(out);
    
    if ((out << *i))
    {
      // If the first write succeeds, increment:
      ++n; // ... the write count (do first because it will never throw)
      ++i; // ... the iterator
      
      while ((i != e) && bool(out))
      {
        // If there are still more elements to write (and the output stream is
        // still good), restore the formatting state to what it was before the
        // first write.
        formatting.restore();
        
        if ((out << *i))
        {
          // If the next write succeeds, increment:
          ++n; // ... the write count (do first because it will never throw)
          ++i; // ... the iterator
        }
      }
    }
  }
  
  // Regardless of anything else, reset the stream's width to zero.
  out.width(0);
}

// Underlying implementation function for all versions of write with delimiters.
// 
// Thus function is ultimately used under the hood by all of the write
// functions in the RangeIO library that write delimiters between the elements.
// The interface is designed so that higher level structures can have their
// members updated via references as the write proceeds, so if there is any
// kind of error, those members will be left in the last good state.
// 
// While i is not equal to e and bool(out) is true, performs "out << *i". If
// boo(out) is still true, performs "++i" and "++n".
// 
// Between each write, "out << delim" is performed. If bool(out) is still true
// after that, the stream's formatting state is restored to what it was before
// the first write.
// 
// At the end of the function, whether there have been any writes or not, the
// stream width is set to zero.
template <
  typename InputIterator,
  typename Sentinel,
  typename Delimiter,
  typename CharT,
  typename Traits>
void
write_impl(
  ::std::basic_ostream<CharT, Traits>& out,
  InputIterator& i,
  Sentinel const e,
  Delimiter& delim,
  ::std::size_t& n)
{
  // Only bother to attempt writing if the range is empty (i == e) or the
  // output stream is good (bool(out) is true).
  if (!(i == e) && bool(out))
  {
    // Save the formatting state prior to writing the first element.
    detail::formatting_saver<CharT, Traits> formatting(out);
    
    if ((out << *i))
    {
      // If the first write succeeds, increment:
      ++n; // ... the write count (do first because it will never throw)
      ++i; // ... the iterator
      
      while ((i != e) && bool(out) && (out << delim))
      {
        // If there are still more elements to write (and the output stream is
        // still good), restore the formatting state to what it was before the
        // first write.
        formatting.restore();
        
        if ((out << *i))
        {
          // If the next write succeeds, increment:
          ++n; // ... the write count (do first because it will never throw)
          ++i; // ... the iterator
        }
      }
    }
  }
  
  // Regardless of anything else, reset the stream's width to zero.
  out.width(0);
}

} // namespace detail
} // namespace rangeio
} // namespace boost

#endif  // include guard
