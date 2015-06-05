//
// Copyright (c) Mark A. Gibbs, 2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// 

// This test covers the deferred version of write_iterator_range, without
// delimiters.
// 
// The tests must confirm that writes are done correctly and that formatting is
// preserved between elements.
// 
// This test requires C++11.

#include <boost/config.hpp>

#ifdef BOOST_NO_CXX11_RVALUE_REFERENCES
#   include <iostream>
int main() { ::std::cout << "Not supported in C++98.\n"; }
#else

#include <locale>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <boost/core/lightweight_test.hpp>

#include <boost/rangeio/write_iterator_range.hpp>

#include "extras/more_tests.hpp"

namespace write_iterator_range_tests {

// Confirm that empty ranges produce no output, and that they leave everything
// in the expected state.
namespace empty_range {

template <typename CharT>
void do_test()
{
  ::std::vector<double> r;
  ::std::basic_ostringstream<CharT> out;
  
  // Set the width, just to test it later.
  out.width(7);
  
  ::boost::rangeio::write_iterator_range_t<
      ::std::vector<double>::iterator,
      ::std::vector<double>::iterator,
      void>
    res = ::boost::rangeio::write_iterator_range(r.begin(), r.end());
  
  // Make sure everything is sane before trying the write
  BOOST_TEST(bool(out));
  
  BOOST_TEST(r.begin() == res.next);
  BOOST_TEST_EQ(::std::size_t(0), res.count);
  
  // Do the write and check the resutls
  out << res;
  
  BOOST_TEST(bool(out));
  BOOST_TEST(out.str().empty());
  BOOST_TEST_EQ(::std::streamsize(0), out.width());
  
  BOOST_TEST(r.begin() == res.next);
  BOOST_TEST_EQ(::std::size_t(0), res.count);
  
  // Try it as an rvalue
  out.width(7);
  out << ::boost::rangeio::write_iterator_range(r.begin(), r.end());
  
  BOOST_TEST(bool(out));
  BOOST_TEST(out.str().empty());
  BOOST_TEST_EQ(::std::streamsize(0), out.width());
  
  BOOST_TEST(r.begin() == res.next);
  BOOST_TEST_EQ(::std::size_t(0), res.count);
}

void test()
{
  do_test<char>();
  do_test<wchar_t>();
}

} // namespace empty_range

// Confirm that empty ranges produce no output, and that they leave everything
// in the expected state.
namespace empty_const_range {

template <typename CharT>
void do_test()
{
  ::std::vector<double> const r;
  ::std::basic_ostringstream<CharT> out;
  
  // Set the width, just to test it later.
  out.width(7);
  
  ::boost::rangeio::write_iterator_range_t<
      ::std::vector<double>::const_iterator,
      ::std::vector<double>::const_iterator,
      void>
    res = ::boost::rangeio::write_iterator_range(r.begin(), r.end());
  
  // Make sure everything is sane before trying the write
  BOOST_TEST(bool(out));
  
  BOOST_TEST(r.begin() == res.next);
  BOOST_TEST_EQ(::std::size_t(0), res.count);
  
  // Do the write and check the resutls
  out << res;
  
  BOOST_TEST(bool(out));
  BOOST_TEST(out.str().empty());
  BOOST_TEST_EQ(::std::streamsize(0), out.width());
  
  BOOST_TEST(r.begin() == res.next);
  BOOST_TEST_EQ(::std::size_t(0), res.count);
  
  // Try it as an rvalue
  out.width(7);
  out << ::boost::rangeio::write_iterator_range(r.begin(), r.end());
  
  BOOST_TEST(bool(out));
  BOOST_TEST(out.str().empty());
  BOOST_TEST_EQ(::std::streamsize(0), out.width());
  
  BOOST_TEST(r.begin() == res.next);
  BOOST_TEST_EQ(::std::size_t(0), res.count);
}

void test()
{
  do_test<char>();
  do_test<wchar_t>();
}

} // namespace empty_const_range

// Confirm that ranges are written properly, and that they leave everything
// in the expected state.
namespace normal_range {

template <typename CharT>
void do_test()
{
  // Prepare the range to be written
  int r[] = { 1, 1, 2, 3, 5, 8 };
  ::std::size_t const r_size = sizeof(r) / sizeof(r[0]);
  
  // Lval version
  {
    // Prepare the output stream
    ::std::basic_ostringstream<CharT> out;
    out.imbue(::std::locale::classic());
    out << 'a';
    
    // Set up the write object
    ::boost::rangeio::write_iterator_range_t<int*, int*>
      w = ::boost::rangeio::write_iterator_range(r, r + r_size);
    
    // Make sure the write object is sane before the write
    BOOST_TEST(&r[0] == w.next);
    BOOST_TEST_EQ(std::size_t(0), w.count);
    
    // Do the write, and check the results
    out << w;
    
    BOOST_TEST(bool(out));
    BOOST_RANGEIO_TEST_STR_EQ("a112358", out.str());
    BOOST_TEST_EQ(::std::streamsize(0), out.width());
    
    BOOST_TEST(r + r_size == w.next);
    BOOST_TEST_EQ(r_size, w.count);
    
    // Make sure the input range wasn't changed
    BOOST_TEST_EQ(1, r[0]);
    BOOST_TEST_EQ(1, r[1]);
    BOOST_TEST_EQ(2, r[2]);
    BOOST_TEST_EQ(3, r[3]);
    BOOST_TEST_EQ(5, r[4]);
    BOOST_TEST_EQ(8, r[5]);
    
    // Make sure the stream is still good for future writes
    BOOST_TEST(out << 'x');
    BOOST_RANGEIO_TEST_STR_EQ("a112358x", out.str());
  }
  
  // Rval version
  {
    // Prepare the output stream
    ::std::basic_ostringstream<CharT> out;
    out.imbue(::std::locale::classic());
    out << 'a';
    
    // Do the write, and check the results
    out << ::boost::rangeio::write_iterator_range(r, r + r_size);
    
    BOOST_TEST(bool(out));
    BOOST_RANGEIO_TEST_STR_EQ("a112358", out.str());
    BOOST_TEST_EQ(::std::streamsize(0), out.width());
    
    // Make sure the input range wasn't changed
    BOOST_TEST_EQ(1, r[0]);
    BOOST_TEST_EQ(1, r[1]);
    BOOST_TEST_EQ(2, r[2]);
    BOOST_TEST_EQ(3, r[3]);
    BOOST_TEST_EQ(5, r[4]);
    BOOST_TEST_EQ(8, r[5]);
    
    // Make sure the stream is still good for future writes
    BOOST_TEST(out << 'x');
    BOOST_RANGEIO_TEST_STR_EQ("a112358x", out.str());
  }
}

void test()
{
  do_test<char>();
  do_test<wchar_t>();
}

} // namespace normal_range

// Confirm that ranges are written properly, and that they leave everything
// in the expected state.
namespace normal_const_range {

template <typename CharT>
void do_test()
{
  // Prepare the range to be written
  unsigned long const r[] = { 867, 5309, 555, 2368 };
  ::std::size_t const r_size = sizeof(r) / sizeof(r[0]);
  
  // Lval version
  {
    // Prepare the output stream
    ::std::basic_ostringstream<CharT> out;
    out.imbue(::std::locale::classic());
    out << 'y';
    
    // Set up the write object
    ::boost::rangeio::write_iterator_range_t<unsigned long const*, unsigned long const*>
      w = ::boost::rangeio::write_iterator_range(r, r + r_size);
    
    // Do the write, and check the results
    out << w;
    
    BOOST_TEST(bool(out));
    BOOST_RANGEIO_TEST_STR_EQ("y86753095552368", out.str());
    BOOST_TEST_EQ(::std::streamsize(0), out.width());
    
    BOOST_TEST(r + r_size == w.next);
    BOOST_TEST_EQ(r_size, w.count);
    
    // Make sure the stream is still good for future writes
    BOOST_TEST(out << 'q');
    BOOST_RANGEIO_TEST_STR_EQ("y86753095552368q", out.str());
  }
  
  // Rval version
  {
    // Prepare the output stream
    ::std::basic_ostringstream<CharT> out;
    out.imbue(::std::locale::classic());
    out << 'y';
    
    // Do the write, and check the results
    out << ::boost::rangeio::write_iterator_range(r, r + r_size);
    
    BOOST_TEST(bool(out));
    BOOST_RANGEIO_TEST_STR_EQ("y86753095552368", out.str());
    BOOST_TEST_EQ(::std::streamsize(0), out.width());
    
    // Make sure the stream is still good for future writes
    BOOST_TEST(out << 'q');
    BOOST_RANGEIO_TEST_STR_EQ("y86753095552368q", out.str());
  }
}

void test()
{
  do_test<char>();
  do_test<wchar_t>();
}

} // namespace normal_const_range

// Confirm that everything works properly, even when the source range uses
// the most restricted class of iterators (InputIterators).
namespace input_iterator_range {

void test()
{
  // Prepare the range to be written
  ::std::istringstream in("2 4 6 8");
  in.imbue(::std::locale::classic());
  
  // Prepare the output stream
  ::std::ostringstream out;
  out.imbue(::std::locale::classic());
  
  ::std::istream_iterator<int> const e;
  
  // Do the write
  out << "pre" <<
    ::boost::rangeio::write_iterator_range(
      ::std::istream_iterator<int>(in),
      (::std::istream_iterator<int>())) <<
    "post";
  
  BOOST_TEST_EQ("pre2468post", out.str());
}

} // namespace input_iterator_range

// Confirm that ranges are written properly, and that formatting is preserved
// across elements.
namespace formatting {

void test()
{
  // Prepare the range to be written
  int const r[] = { 0x0287, 0x071A, 0x00E6, 0x001A, 0x029E };
  ::std::size_t const r_size = sizeof(r) / sizeof(r[0]);
  
  // Prepare the output stream
  ::std::ostringstream out;
  out.imbue(::std::locale::classic());
  out << "{ ";
  
  out.width(7);
  out.fill('.');
  out.setf(::std::ios_base::hex, ::std::ios_base::basefield);
  out.setf(::std::ios_base::left, ::std::ios_base::adjustfield);
  out.setf(::std::ios_base::uppercase);
  out.setf(::std::ios_base::showbase);
  
  // Do the write
  out << ::boost::rangeio::write_iterator_range(r, r + r_size) << " }";
  
  BOOST_TEST_EQ("{ " "0X287.." "0X71A.." "0XE6..." "0X1A..." "0X29E.." " }", out.str());
}

} // namespace formatting

} // namespace write_iterator_range_tests

int main()
{
  using namespace write_iterator_range_tests;
  
  empty_range::test();
  empty_const_range::test();
  
  normal_range::test();
  normal_const_range::test();
  
  input_iterator_range::test();
  
  formatting::test();
  
  return boost::report_errors();
}

#endif  // BOOST_NO_CXX11_RVALUE_REFERENCES
