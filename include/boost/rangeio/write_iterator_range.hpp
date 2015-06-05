//
// Copyright (c) Mark A. Gibbs, 2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// 

// Defines the write_iterator_range() family of functions, and associated types.
// 
// Requires at least C++11.

#ifndef BOOST_RANGEIO_Inc_write_iterator_range_2015_01_01_
#define BOOST_RANGEIO_Inc_write_iterator_range_2015_01_01_

#include <boost/config.hpp>

#ifdef BOOST_NO_CXX11_RVALUE_REFERENCES
#   error "C++98 is not supported"
#else

#include <cstddef>
#include <iosfwd>
#include <utility>

#include <boost/core/enable_if.hpp>

#include <boost/rangeio/detail/write.hpp>

#include <boost/type_traits/is_base_of.hpp>

namespace boost {
namespace rangeio {

// Result type from the immediate version of write_iterator_range().
// 
// Has two public data members:
//   next:  an iterator to the next element in the range that would be written,
//          or one-past-the-end if all were written.
//   count: the number of elements written
// 
template <typename InputIterator>
struct write_iterator_range_result_t
{
  //BOOST_CONCEPT_ASSERT(( boost_concepts::InputIterator<InputIterator> ));
  //BOOST_CONCEPT_ASSERT(( boost_concepts::SinglePassIterator<InputIterator> ));
  //BOOST_CONCEPT_ASSERT(( boost_concepts::ReadableIterator<InputIterator> ));
  
  typedef InputIterator iterator;
  
  iterator       next;
  ::std::size_t  count;
  
//protected:  
  explicit write_iterator_range_result_t(iterator p) :
    next(::std::move(p)),
    count(0)
  {}
};

// Result type from the deferred version of write_iterator_range().
// 
// Comes in three different flavours (via partial specialization):
//   write_iterator_range_t<I, S, D>:
//     Delimiter version, with rvalue delimiter. Takes ownership of the
//     delimiter via moving.
//   write_iterator_range_t<I, S, D&>:
//     Delimiter version, with (const or non-const) lvalue delimiter. Does not
//     take ownership of delimiter - uses it via reference.
//   write_iterator_range_t<I, S> (using D = void):
//     Non-delimiter version.
// 
// Has two public data members:
//   next:  an iterator to the next element in the range that would be written,
//          or one-past-the-end if all were written.
//   count: the number of elements written
// 
// All versions support ostream insertion both as lval and rval references (the
// latter is necessary to support using write_iterator_range() inline in a
// stream of write operations).
// 
// Internally stores the range end marker - so it knows when to stop writing -
// and the delimiter (if necessary, and if so by val or by ref depending).
// 
template <typename InputIterator, typename Sentinel, typename Delimiter = void>
struct write_iterator_range_t :
  protected write_iterator_range_result_t<InputIterator>
{
public:
  using typename write_iterator_range_result_t<InputIterator>::iterator;
  
  using write_iterator_range_result_t<InputIterator>::next;
  using write_iterator_range_result_t<InputIterator>::count;
  
//protected:
  explicit write_iterator_range_t(iterator p, Sentinel e, Delimiter d) :
    write_iterator_range_result_t<InputIterator>(::std::move(p)),
    last_(::std::move(e)),
    delim_(::std::move(d))
  {}
  
  Sentinel   last_;
  Delimiter  delim_;
};

template <typename InputIterator, typename Sentinel, typename Delimiter>
struct write_iterator_range_t<InputIterator, Sentinel, Delimiter&> :
  protected write_iterator_range_result_t<InputIterator>
{
public:
  using typename write_iterator_range_result_t<InputIterator>::iterator;
  
  using write_iterator_range_result_t<InputIterator>::next;
  using write_iterator_range_result_t<InputIterator>::count;
  
//protected:
  explicit write_iterator_range_t(iterator p, Sentinel e, Delimiter& d) :
    write_iterator_range_result_t<InputIterator>(::std::move(p)),
    last_(::std::move(e)),
    delim_(d)
  {}
  
  Sentinel    last_;
  Delimiter&  delim_;
};

template <typename InputIterator, typename Sentinel>
class write_iterator_range_t<InputIterator, Sentinel, void> :
  protected write_iterator_range_result_t<InputIterator>
{
public:
  using typename write_iterator_range_result_t<InputIterator>::iterator;
  
  using write_iterator_range_result_t<InputIterator>::next;
  using write_iterator_range_result_t<InputIterator>::count;
  
//protected:
  explicit write_iterator_range_t(iterator p, Sentinel e) :
    write_iterator_range_result_t<InputIterator>(::std::move(p)),
    last_(::std::move(e))
  {}
  
  Sentinel  last_;
};

template <typename InputIterator, typename Sentinel, typename Delimiter, typename CharT, typename Traits>
::std::basic_ostream<CharT, Traits>&
operator<<(::std::basic_ostream<CharT, Traits>& o, write_iterator_range_t<InputIterator, Sentinel, Delimiter>& w)
{
  detail::write_impl(o, w.next, w.last_, w.delim_, w.count);
  return o;
}

template <typename InputIterator, typename Sentinel, typename Delimiter, typename CharT, typename Traits>
::std::basic_ostream<CharT, Traits>&
operator<<(::std::basic_ostream<CharT, Traits>& o, write_iterator_range_t<InputIterator, Sentinel, Delimiter>&& w)
{
  return o << w;
}

template <typename InputIterator, typename Sentinel, typename CharT, typename Traits>
::std::basic_ostream<CharT, Traits>&
operator<<(::std::basic_ostream<CharT, Traits>& o, write_iterator_range_t<InputIterator, Sentinel, void>& w)
{
  detail::write_impl(o, w.next, w.last_, w.count);
  return o;
}

template <typename InputIterator, typename Sentinel, typename CharT, typename Traits>
::std::basic_ostream<CharT, Traits>&
operator<<(::std::basic_ostream<CharT, Traits>& o, write_iterator_range_t<InputIterator, Sentinel, void>&& w)
{
  return o << w;
}

// Immediate write_iterator_range().
// 
// These versions of write_iterator_range take an ostream& as their first
// argument, and perform the write immediately, returning a struct with info
// about how it went.
// 
// There are two versions - one with a delimiter, and one without.
// 
template <typename InputIterator, typename Sentinel, typename Delimiter, typename CharT, typename Traits>
write_iterator_range_result_t<InputIterator>
write_iterator_range(::std::basic_ostream<CharT, Traits>& o, InputIterator i, Sentinel const e, Delimiter&& d)
{
  write_iterator_range_result_t<InputIterator> w(i);
  detail::write_impl(o, w.next, e, d, w.count);
  return w;
}

template <typename InputIterator, typename Sentinel, typename CharT, typename Traits>
write_iterator_range_result_t<InputIterator>
write_iterator_range(::std::basic_ostream<CharT, Traits>& o, InputIterator i, Sentinel const e)
{
  write_iterator_range_result_t<InputIterator> w(i);
  detail::write_impl(o, w.next, e, w.count);
  return w;
}

// Deferred write_iterator_range().
// 
// These versions of write_iterator_range do not accept a stream as the first
// argument. They either take a pair of iterators (describing a range) and a
// delimiter, or just a pair of iterators.
// 
// There are technically just two versions - delimiter and no-delimiter - but
// two versions of the delimiter version are needed: one for when the delimiter
// is an lvalue (in which case it's taken by ref) and one for when its an
// rvalue (in which case it's moved into the return structure).
// 
// All three return a structure which can be used in an ostream insert
// expression, and queried to see how the last write operation went.
// 
template <typename InputIterator, typename Sentinel, typename Delimiter>
typename ::boost::disable_if_c<
  ::boost::is_base_of<std::ios_base, InputIterator>::value,
  write_iterator_range_t<InputIterator, Sentinel, Delimiter&>>::type
write_iterator_range(InputIterator i, Sentinel e, Delimiter& d)
{
  return write_iterator_range_t<InputIterator, Sentinel, Delimiter&>(::std::move(i), ::std::move(e), d);
}

template <typename InputIterator, typename Sentinel, typename Delimiter>
typename ::boost::disable_if_c<
  ::boost::is_base_of<std::ios_base, InputIterator>::value,
  write_iterator_range_t<InputIterator, Sentinel, Delimiter>>::type
write_iterator_range(InputIterator i, Sentinel e, Delimiter&& d)
{
  return write_iterator_range_t<InputIterator, Sentinel, Delimiter>(::std::move(i), ::std::move(e), ::std::move(d));
}

template <typename InputIterator, typename Sentinel>
write_iterator_range_t<InputIterator, Sentinel>
write_iterator_range(InputIterator i, Sentinel e)
{
  return write_iterator_range_t<InputIterator, Sentinel>(::std::move(i), ::std::move(e));
}

} // namespace rangeio
} // namespace boost

#endif  // BOOST_NO_CXX11_RVALUE_REFERENCES
#endif  // include guard
