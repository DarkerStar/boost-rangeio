//
// Copyright (c) Mark A. Gibbs, 2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// 

// This test covers the deferred version of write_iterator_range, with
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
#include "extras/smart_delimiters.hpp"

namespace write_iterator_range_delimiter_tests {

// Confirm that empty ranges produce no output, and that they leave everything
// in the expected state. Delimiters should not be written.
namespace empty_range_lval_delim {

template <typename CharT>
void do_test()
{
  ::std::vector<double> r;
  int delim = 0;
  
  // As lvalue
  {
    ::std::basic_ostringstream<CharT> out;
    out.width(7);
    
    ::boost::rangeio::write_iterator_range_t<
        ::std::vector<double>::iterator,
        ::std::vector<double>::iterator,
        int&>
      res = ::boost::rangeio::write_iterator_range(r.begin(), r.end(), delim);
    
    // Make sure everything is sane before trying the write
    BOOST_TEST(bool(out));
    
    BOOST_TEST_EQ(0, delim);
    
    BOOST_TEST(r.begin() == res.next);
    BOOST_TEST_EQ(::std::size_t(0), res.count);
    
    // Do the write and check the resutls
    out << res;
    
    BOOST_TEST(bool(out));
    BOOST_TEST(out.str().empty());
    BOOST_TEST_EQ(::std::streamsize(0), out.width());
    
    BOOST_TEST_EQ(0, delim);
    
    BOOST_TEST(r.begin() == res.next);
    BOOST_TEST_EQ(::std::size_t(0), res.count);
  }
  
  // As rvalue
  {
    // Try it as an rvalue
    ::std::basic_ostringstream<CharT> out;
    out.width(7);
    out << ::boost::rangeio::write_iterator_range(r.begin(), r.end(), delim);
    
    BOOST_TEST(bool(out));
    BOOST_TEST(out.str().empty());
    BOOST_TEST_EQ(::std::streamsize(0), out.width());
  }
}

void test()
{
  do_test<char>();
  do_test<wchar_t>();
}

} // namespace empty_range_lval_delim

// Confirm that empty ranges produce no output, and that they leave everything
// in the expected state. Delimiters should not be written.
namespace empty_range_const_lval_delim {

template <typename CharT>
void do_test()
{
  ::std::vector<double> r;
  double const delim = 0.0;
  
  // As lvalue
  {
    ::std::basic_ostringstream<CharT> out;
    out.width(7);
    
    ::boost::rangeio::write_iterator_range_t<
        ::std::vector<double>::iterator,
        ::std::vector<double>::iterator,
        double const&>
      res = ::boost::rangeio::write_iterator_range(r.begin(), r.end(), delim);
    
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
  }
  
  // As rvalue
  {
    // Try it as an rvalue
    ::std::basic_ostringstream<CharT> out;
    out.width(7);
    out << ::boost::rangeio::write_iterator_range(r.begin(), r.end(), delim);
    
    BOOST_TEST(bool(out));
    BOOST_TEST(out.str().empty());
    BOOST_TEST_EQ(::std::streamsize(0), out.width());
  }
}

void test()
{
  do_test<char>();
  do_test<wchar_t>();
}

} // namespace empty_range_const_lval_delim

// Confirm that empty ranges produce no output, and that they leave everything
// in the expected state. Delimiters should not be written.
namespace empty_range_rval_delim {

template <typename CharT>
void do_test()
{
  ::std::vector<double> r;
  
  // Const value delim
  {
    ::std::basic_ostringstream<CharT> out;
    out.width(7);
    
    ::boost::rangeio::write_iterator_range_t<
        ::std::vector<double>::iterator,
        ::std::vector<double>::iterator,
        unsigned long>
      res = ::boost::rangeio::write_iterator_range(r.begin(), r.end(), 0uL);
    
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
  }
  
  {
    // Try it as an rvalue
    ::std::basic_ostringstream<CharT> out;
    out.width(7);
    out << ::boost::rangeio::write_iterator_range(r.begin(), r.end(), 0uL);
    
    BOOST_TEST(bool(out));
    BOOST_TEST(out.str().empty());
    BOOST_TEST_EQ(::std::streamsize(0), out.width());
  }
}

void test()
{
  do_test<char>();
  do_test<wchar_t>();
}

} // namespace empty_range_rval_delim

// Confirm that empty ranges produce no output, and that they leave everything
// in the expected state. Delimiters should not be written.
namespace empty_const_range_lval_delim {

template <typename CharT>
void do_test()
{
  ::std::vector<double> const r;
  int delim = 0;
  
  // As lvalue
  {
    ::std::basic_ostringstream<CharT> out;
    out.width(7);
    
    ::boost::rangeio::write_iterator_range_t<
        ::std::vector<double>::const_iterator,
        ::std::vector<double>::const_iterator,
        int&>
      res = ::boost::rangeio::write_iterator_range(r.begin(), r.end(), delim);
    
    // Make sure everything is sane before trying the write
    BOOST_TEST(bool(out));
    
    BOOST_TEST_EQ(0, delim);
    
    BOOST_TEST(r.begin() == res.next);
    BOOST_TEST_EQ(::std::size_t(0), res.count);
    
    // Do the write and check the resutls
    out << res;
    
    BOOST_TEST(bool(out));
    BOOST_TEST(out.str().empty());
    BOOST_TEST_EQ(::std::streamsize(0), out.width());
    
    BOOST_TEST_EQ(0, delim);
    
    BOOST_TEST(r.begin() == res.next);
    BOOST_TEST_EQ(::std::size_t(0), res.count);
  }
  
  // As rvalue
  {
    // Try it as an rvalue
    ::std::basic_ostringstream<CharT> out;
    out.width(7);
    out << ::boost::rangeio::write_iterator_range(r.begin(), r.end(), delim);
    
    BOOST_TEST(bool(out));
    BOOST_TEST(out.str().empty());
    BOOST_TEST_EQ(::std::streamsize(0), out.width());
  }
}

void test()
{
  do_test<char>();
  do_test<wchar_t>();
}

} // namespace empty_const_range_lval_delim

// Confirm that empty ranges produce no output, and that they leave everything
// in the expected state. Delimiters should not be written.
namespace empty_const_range_const_lval_delim {

template <typename CharT>
void do_test()
{
  ::std::vector<double> const r;
  double const delim = 0.0;
  
  // As lvalue
  {
    ::std::basic_ostringstream<CharT> out;
    out.width(7);
    
    ::boost::rangeio::write_iterator_range_t<
        ::std::vector<double>::const_iterator,
        ::std::vector<double>::const_iterator,
        double const&>
      res = ::boost::rangeio::write_iterator_range(r.begin(), r.end(), delim);
    
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
  }
  
  // As rvalue
  {
    // Try it as an rvalue
    ::std::basic_ostringstream<CharT> out;
    out.width(7);
    out << ::boost::rangeio::write_iterator_range(r.begin(), r.end(), delim);
    
    BOOST_TEST(bool(out));
    BOOST_TEST(out.str().empty());
    BOOST_TEST_EQ(::std::streamsize(0), out.width());
  }
}

void test()
{
  do_test<char>();
  do_test<wchar_t>();
}

} // namespace empty_const_range_const_lval_delim

// Confirm that empty ranges produce no output, and that they leave everything
// in the expected state. Delimiters should not be written.
namespace empty_const_range_rval_delim {

template <typename CharT>
void do_test()
{
  ::std::vector<double> const r;
  
  // Const value delim
  {
    ::std::basic_ostringstream<CharT> out;
    out.width(7);
    
    ::boost::rangeio::write_iterator_range_t<
        ::std::vector<double>::const_iterator,
        ::std::vector<double>::const_iterator,
        unsigned long>
      res = ::boost::rangeio::write_iterator_range(r.begin(), r.end(), 0uL);
    
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
  }
  
  {
    // Try it as an rvalue
    ::std::basic_ostringstream<CharT> out;
    out.width(7);
    out << ::boost::rangeio::write_iterator_range(r.begin(), r.end(), 0uL);
    
    BOOST_TEST(bool(out));
    BOOST_TEST(out.str().empty());
    BOOST_TEST_EQ(::std::streamsize(0), out.width());
  }
}

void test()
{
  do_test<char>();
  do_test<wchar_t>();
}

} // namespace empty_const_range_rval_delim

// Confirm that ranges are written properly, and that they leave everything
// in the expected state. The delimiter should be written between each pair of
// elements, but not at the beginning or end.
namespace normal_range {

template <typename CharT>
void do_test()
{
  // Prepare the range to be written
  int r[] = { 1, 1, 2, 3, 5, 8 };
  ::std::size_t const r_size = sizeof(r) / sizeof(r[0]);
  
  // As lval
  {
    // Prepare the output stream
    ::std::basic_ostringstream<CharT> out;
    out.imbue(::std::locale::classic());
    out << 'a';
    
    // Prepare the write object
    ::boost::rangeio::write_iterator_range_t<int*, int*, char const(&)[3]>
      w = ::boost::rangeio::write_iterator_range(r, r + r_size, "::");
    
    BOOST_TEST(r == w.next);
    BOOST_TEST_EQ(::std::size_t(0), w.count);
    
    // Do the write
    out << w;
    
    BOOST_TEST(bool(out));
    BOOST_RANGEIO_TEST_STR_EQ("a1::1::2::3::5::8", out.str());
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
    BOOST_RANGEIO_TEST_STR_EQ("a1::1::2::3::5::8x", out.str());
  }
  
  // As rval
  {
    ::std::basic_ostringstream<CharT> out;
    out.imbue(::std::locale::classic());
    out << 'a' << ::boost::rangeio::write_iterator_range(r, r + r_size, "::") << 'x';
    
    BOOST_TEST(bool(out));
    BOOST_TEST_EQ(::std::streamsize(0), out.width());
    BOOST_RANGEIO_TEST_STR_EQ("a1::1::2::3::5::8x", out.str());
  }
}

void test()
{
  do_test<char>();
  do_test<wchar_t>();
}

} // namespace normal_range

// Confirm that ranges are written properly, and that they leave everything
// in the expected state. The delimiter should be written between each pair of
// elements, but not at the beginning or end.
namespace normal_const_range {

template <typename CharT>
void do_test()
{
  // Prepare the range to be written
  unsigned long const r[] = { 867, 5309, 555, 2368 };
  ::std::size_t const r_size = sizeof(r) / sizeof(r[0]);
  
  // As lval
  {
    // Prepare the output stream
    ::std::basic_ostringstream<CharT> out;
    out.imbue(::std::locale::classic());
    out << 'y';
    
    // Do the write
    ::boost::rangeio::write_iterator_range_t<
        unsigned long const*,
        unsigned long const*,
        char>
      w = ::boost::rangeio::write_iterator_range(r, r + r_size, '-');
    
    BOOST_TEST(r == w.next);
    BOOST_TEST_EQ(::std::size_t(0), w.count);
    
    // Do the write
    out << w;
    
    BOOST_TEST(bool(out));
    BOOST_RANGEIO_TEST_STR_EQ("y867-5309-555-2368", out.str());
    BOOST_TEST_EQ(::std::streamsize(0), out.width());
    
    BOOST_TEST(r + r_size == w.next);
    BOOST_TEST_EQ(r_size, w.count);
    
    // Make sure the stream is still good for future writes
    BOOST_TEST(out << 'q');
    BOOST_RANGEIO_TEST_STR_EQ("y867-5309-555-2368q", out.str());
  }
  
  // As rval
  {
    ::std::basic_ostringstream<CharT> out;
    out.imbue(::std::locale::classic());
    
    out << 'y' << ::boost::rangeio::write_iterator_range(r, r + r_size, '-') << 'q';
    
    BOOST_TEST(bool(out));
    BOOST_RANGEIO_TEST_STR_EQ("y867-5309-555-2368q", out.str());
    BOOST_TEST_EQ(::std::streamsize(0), out.width());
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
  ::std::istringstream in("2 4 6 8");
  in.imbue(::std::locale::classic());
  
  ::std::ostringstream out;
  out.imbue(::std::locale::classic());
  out << "{ " <<
    ::boost::rangeio::write_iterator_range(
      ::std::istream_iterator<int>(in),
      (::std::istream_iterator<int>()),
      ", ") <<
    " }";
  
  BOOST_TEST_EQ("{ 2, 4, 6, 8 }", out.str());
}

} // namespace input_iterator_range

// Confirm that ranges are written properly, and that formatting is preserved
// across elements. Preserved formatting should not apply to delimiters.
namespace formatting {

void test()
{
  double const r[] = { 1.0, -2.3, 6.66666, -0.12345, -1.2345 };
  ::std::size_t const r_size = sizeof(r) / sizeof(r[0]);
  
  ::std::ostringstream out;
  out.imbue(::std::locale::classic());
  out << "{ ";
  
  out.imbue(::std::locale::classic());
  out.width(8);
  out.precision(3);
  out.fill('_');
  out.setf(::std::ios_base::internal, ::std::ios_base::adjustfield);
  out.setf(::std::ios_base::showpoint);
  
  out << ::boost::rangeio::write_iterator_range(r, r + r_size, " | ");
  out << " }";
  
  BOOST_TEST_EQ("{ ____1.00 | -___2.30 | ____6.67 | -__0.123 | -___1.23 }", out.str());
}

} // namespace formatting

// Verify that "smart delimiters" work.
namespace smart_delimiter_lval {

void test()
{
  // Prepare the range to be written
  ::std::string const r = "abcdef";
  
  // Prepare the output stream
  ::std::ostringstream out;
  out.imbue(::std::locale::classic());
  
  // Prepare the write object
  ::boost::rangeio::test_extras::incrementing_integer_delimiter iid;
  ::boost::rangeio::write_iterator_range_t<
      ::std::string::const_iterator,
      ::std::string::const_iterator,
      ::boost::rangeio::test_extras::incrementing_integer_delimiter&>
    w = ::boost::rangeio::write_iterator_range(r.begin(), r.end(), iid);
  
  BOOST_TEST_EQ(::std::size_t(0), iid.i);
  
  // Do write
  out << w;
  
  BOOST_TEST(r.end() == w.next);
  BOOST_TEST_EQ(r.size(), w.count);
  BOOST_TEST_EQ(r.size() - 1, iid.i);
  
  BOOST_TEST(bool(out));
  BOOST_TEST_EQ("a0b1c2d3e4f", out.str());
}

} // namespace smart_delimiter_lval

// Verify that "smart delimiters" work.
namespace smart_delimiter_rval {

void test()
{
  // Prepare the range to be written
  ::std::string const r = "wxyz";
  
  // Prepare the output stream
  ::std::ostringstream out;
  out.imbue(::std::locale::classic());
  
  // Prepare the write object
  ::boost::rangeio::write_iterator_range_t<
      ::std::string::const_iterator,
      ::std::string::const_iterator,
      ::boost::rangeio::test_extras::incrementing_integer_delimiter>
    w = ::boost::rangeio::write_iterator_range(r.begin(), r.end(),
      ::boost::rangeio::test_extras::incrementing_integer_delimiter());
  
  // Do the write
  out << w;
  
  BOOST_TEST(bool(out));
  BOOST_TEST_EQ("w0x1y2z", out.str());
}

} // namespace smart_delimiter_rval

} // namespace write_iterator_range_delimiter_tests

int main()
{
  using namespace write_iterator_range_delimiter_tests;
  
  empty_range_lval_delim::test();
  empty_range_const_lval_delim::test();
  empty_range_rval_delim::test();
  
  empty_const_range_lval_delim::test();
  empty_const_range_const_lval_delim::test();
  empty_const_range_rval_delim::test();
  
  normal_range::test();
  normal_const_range::test();
  
  input_iterator_range::test();
  
  formatting::test();
  
  smart_delimiter_lval::test();
  smart_delimiter_rval::test();
  
  return boost::report_errors();
}

#endif  // BOOST_NO_CXX11_RVALUE_REFERENCES
