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

template <typename InputIterator, typename Sentinel, typename CharT, typename Traits>
void
write_impl(
  ::std::basic_ostream<CharT, Traits>& out,
  InputIterator& i,
  Sentinel e,
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
      ++n; // ... the iterator
      ++i; // ... the write count
      
      while ((i != e) && bool(out))
      {
        // If there are still more elements to write (and the output stream is
        // still good), restore the formatting state to what it was before the
        // first write.
        formatting.restore();
        
        if ((out << *i))
        {
          // If the next write succeeds, increment:
          ++n; // ... the iterator
          ++i; // ... the write count
        }
      }
    }
  }
  
  // Regardless of anything else, reset the stream's width to zero.
  out.width(0);
}

template <typename InputIterator, typename Sentinel, typename Delimiter, typename CharT, typename Traits>
void
write_impl(
  ::std::basic_ostream<CharT, Traits>& out,
  InputIterator& i,
  Sentinel e,
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
      ++n; // ... the iterator
      ++i; // ... the write count
      
      while ((i != e) && bool(out) && (out << delim))
      {
        // If there are still more elements to write (and the output stream is
        // still good), restore the formatting state to what it was before the
        // first write.
        formatting.restore();
        
        if ((out << *i))
        {
          // If the next write succeeds, increment:
          ++n; // ... the iterator
          ++i; // ... the write count
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
