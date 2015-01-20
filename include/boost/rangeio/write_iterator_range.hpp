//
// Copyright (c) Mark A. Gibbs, 2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// 

#ifndef BOOST_RANGEIO_Inc_write_iterator_range_2015_01_01_
#define BOOST_RANGEIO_Inc_write_iterator_range_2015_01_01_

#include <boost/config.hpp>

#include <cstddef>
#include <iosfwd>
#include <utility>

#include <boost/rangeio/detail/write_all.hpp>

namespace boost {
namespace rangeio {

// 
// Result type from the immediate version of write_iterator_range().
// 
// Has two public data members:
//   next:  an iterator to the next element in the range that would be written
//   count: the number of elements written
// 
template <typename InputIterator>
struct write_iterator_range_result_t
{
  //BOOST_CONCEPT_ASSERT(( boost_concepts::InputIterator<InputIterator> ));
  //BOOST_CONCEPT_ASSERT(( boost_concepts::SinglePassIterator<InputIterator> ));
  //BOOST_CONCEPT_ASSERT(( boost_concepts::ReadableIterator<InputIterator> ));
  
  typedef InputIterator iterator;
  
  explicit write_iterator_range_result_t(iterator p) :
    next(p),
    count(0)
  {}
  
  iterator    next;
  std::size_t count;
};

#ifdef BOOST_NO_CXX11_RVALUE_REFERENCES

// 
// Immediate version of write_iterator_range() with delimiter - non-const ref version.
// 
// Available only in C++98 mode.
// 
template <typename InputIterator, typename Sentinel, typename Delimiter, typename CharT, typename Traits>
write_iterator_range_result_t<InputIterator>
write_iterator_range(std::basic_ostream<CharT, Traits>& out, InputIterator first, Sentinel last, Delimiter& delimiter)
{
  write_iterator_range_result_t<InputIterator> result(first);
  ::boost::rangeio::detail::write_all(out, result.next, last, delimiter, result.count);
  return result;
}

// 
// Immediate version of write_iterator_range() with delimiter - const ref version.
// 
// Available only in C++98 mode.
// 
template <typename InputIterator, typename Sentinel, typename Delimiter, typename CharT, typename Traits>
write_iterator_range_result_t<InputIterator>
write_iterator_range(std::basic_ostream<CharT, Traits>& out, InputIterator first, Sentinel last, Delimiter const& delimiter)
{
  write_iterator_range_result_t<InputIterator> result(first);
  ::boost::rangeio::detail::write_all(out, result.next, last, delimiter, result.count);
  return result;
}

#else // BOOST_NO_CXX11_RVALUE_REFERENCES

// 
// Immediate version of write_iterator_range() with delimiter.
// 
// Available only in C++11 or better mode.
// 
template <typename InputIterator, typename Sentinel, typename Delimiter, typename CharT, typename Traits>
write_iterator_range_result_t<InputIterator>
write_iterator_range(std::basic_ostream<CharT, Traits>& out, InputIterator first, Sentinel last, Delimiter&& delimiter)
{
  write_iterator_range_result_t<InputIterator> result(first);
  ::boost::rangeio::detail::write_all(out, result.next, last, std::forward<Delimiter>(delimiter), result.count);
  return result;
}

#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

// 
// Immediate version of write_iterator_range() without delimiter.
// 
template <typename InputIterator, typename Sentinel, typename CharT, typename Traits>
write_iterator_range_result_t<InputIterator>
write_iterator_range(std::basic_ostream<CharT, Traits>& out, InputIterator first, Sentinel last)
{
  write_iterator_range_result_t<InputIterator> result(first);
  ::boost::rangeio::detail::write_all(out, result.next, last, result.count);
  return result;
}

} // namespace rangeio
} // namespace boost

#endif  // include guard
