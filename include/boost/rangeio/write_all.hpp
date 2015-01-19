//
// Copyright (c) Mark A. Gibbs, 2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// 

#ifndef BOOST_RANGEIO_Inc_write_all_2015_01_01_
#define BOOST_RANGEIO_Inc_write_all_2015_01_01_

#include <boost/config.hpp>

#include <cstddef>
#include <iosfwd>
#include <utility>

#include <boost/rangeio/detail/write_all.hpp>

namespace boost {
namespace rangeio {

// 
// Result type from do_write_iterator_range().
// 
// Has two public data members:
//   next:  an iterator to the next element in the range that would be written
//   count: the number of elements written
// 
template <typename OutputIterator>
struct write_iterator_range_result_t
{
  typedef OutputIterator iterator;
  
  explicit write_iterator_range_result_t(iterator p) :
    next(p),
    count(0)
  {}
  
  iterator    next;
  std::size_t count;
};

#ifdef BOOST_NO_CXX11_RVALUE_REFERENCES

template <typename OutputIterator,
          typename Sentinel,
          typename Delimiter,
          typename CharT,
          typename Traits>
write_iterator_range_result_t<OutputIterator>
do_write_iterator_range(
  std::basic_ostream<CharT, Traits>& out,
  OutputIterator first,
  Sentinel last,
  Delimiter& delimiter)
{
  write_iterator_range_result_t<OutputIterator> result(first);
  
  ::boost::rangeio::detail::write_all(out, result.next, last, delimiter, result.count);
  
  return result;
}

template <typename OutputIterator,
          typename Sentinel,
          typename Delimiter,
          typename CharT,
          typename Traits>
write_iterator_range_result_t<OutputIterator>
do_write_iterator_range(
  std::basic_ostream<CharT, Traits>& out,
  OutputIterator first,
  Sentinel last,
  Delimiter const& delimiter)
{
  write_iterator_range_result_t<OutputIterator> result(first);
  
  ::boost::rangeio::detail::write_all(out, result.next, last, delimiter, result.count);
  
  return result;
}

#else // BOOST_NO_CXX11_RVALUE_REFERENCES

template <typename OutputIterator,
          typename Sentinel,
          typename Delimiter,
          typename CharT,
          typename Traits>
write_iterator_range_result_t<OutputIterator>
do_write_iterator_range(
  std::basic_ostream<CharT, Traits>& out,
  OutputIterator first,
  Sentinel last,
  Delimiter&& delimiter)
{
  write_iterator_range_result_t<OutputIterator> result(first);
  
  ::boost::rangeio::detail::write_all(out, result.next, last, std::forward<Delimiter>(delimiter), result.count);
  
  return result;
}

#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

template <typename OutputIterator,
          typename Sentinel,
          typename CharT,
          typename Traits>
write_iterator_range_result_t<OutputIterator>
do_write_iterator_range(
  std::basic_ostream<CharT, Traits>& out,
  OutputIterator first,
  Sentinel last)
{
  write_iterator_range_result_t<OutputIterator> result(first);
  
  ::boost::rangeio::detail::write_all(out, result.next, last, result.count);
  
  return result;
}

// 
// Result type of write_iterator_range().
// 
/*
template <typename OutputIterator, typename Sentinel = OutputIterator, typename Delimiter = void>
struct write_iterator_range_t
{
  typedef OutputIterator iterator;
  
  explicit write_iterator_range_t(iterator p, Sentinel e) :
    next(p),
    end_(e),
    count(0)
  {}
  
  iterator    next;
  std::size_t count;
  
private:
  Sentinel end_;
};
*/

} // namespace rangeio
} // namespace boost

#endif  // include guard
