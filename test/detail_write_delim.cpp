//
// Copyright (c) Mark A. Gibbs, 2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// 

// This test covers the internal write implementation used by all public write
// operations in the library - specifically the version that uses delimiters.
// 
// The tests must confirm that writes are done correctly and that formatting is
// preserved between elements.
// 
// This test must work even in C++98 mode.

#include <locale>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <boost/core/lightweight_test.hpp>

#include <boost/rangeio/detail/write.hpp>

#include "extras/more_tests.hpp"
#include "extras/smart_delimiters.hpp"

namespace write_impl_delim_tests {

// Confirm that empty ranges produce no output, and that they leave everything
// in the expected state. Delimiters should not be written.
namespace empty_range {

template <typename CharT>
void do_test()
{
  std::vector<double> r;
  std::basic_ostringstream<CharT> out;
  
  // Set the width, just to test it later.
  out.width(7);
  
  std::vector<double>::iterator i = r.begin();
  std::size_t const n_init = 69;
  std::size_t n = n_init;
  ::boost::rangeio::detail::write_impl(out, i, r.end(), ",", n);
  
  BOOST_TEST_EQ(n_init, n);
  BOOST_TEST(r.begin() == i);
  BOOST_TEST(r.end() == i);
  
  BOOST_TEST(bool(out));
  BOOST_TEST(out.str().empty());
  BOOST_TEST_EQ(std::streamsize(0), out.width());
}

void test()
{
  do_test<char>();
  do_test<wchar_t>();
}

} // namespace empty_range

// Confirm that empty ranges produce no output, and that they leave everything
// in the expected state. Delimiters should not be written.
namespace empty_const_range {

template <typename CharT>
void do_test()
{
  std::vector<int> const r;
  std::basic_ostringstream<CharT> out;
  
  // Set the width, just to test it later.
  out.width(7);
  
  std::vector<int>::const_iterator i = r.begin();
  std::size_t const n_init = 69;
  std::size_t n = n_init;
  ::boost::rangeio::detail::write_impl(out, i, r.end(), ",", n);
  
  BOOST_TEST_EQ(n_init, n);
  BOOST_TEST(r.begin() == i);
  BOOST_TEST(r.end() == i);
  
  BOOST_TEST(bool(out));
  BOOST_TEST(out.str().empty());
  BOOST_TEST_EQ(std::streamsize(0), out.width());
}

void test()
{
  do_test<char>();
  do_test<wchar_t>();
}

} // namespace empty_const_range

// Confirm that ranges are written properly, and that they leave everything
// in the expected state. The delimiter should be written between each pair of
// elements, but not at the beginning or end.
namespace normal_range {

template <typename CharT>
void do_test()
{
  // Prepare the range to be written
  int r[] = { 1, 1, 2, 3, 5, 8 };
  std::size_t const r_size = sizeof(r) / sizeof(r[0]);
  
  // Prepare the output stream
  std::basic_ostringstream<CharT> out;
  out.imbue(std::locale::classic());
  out << 'a';
  
  // Do the write
  int* i = r;
  std::size_t const n_init = 42;
  std::size_t n = n_init;
  ::boost::rangeio::detail::write_impl(out, i, i + r_size, "::", n);
  
  // Make sure the "out" arguments were properly set
  BOOST_TEST_EQ(r_size, n - n_init);
  BOOST_TEST(r + r_size == i);
  
  // Make sure the write happened as it should have
  BOOST_TEST(bool(out));
  BOOST_RANGEIO_TEST_STR_EQ("a1::1::2::3::5::8", out.str());
  BOOST_TEST_EQ(std::streamsize(0), out.width());
  
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
  std::size_t const r_size = sizeof(r) / sizeof(r[0]);
  
  // Prepare the output stream
  std::basic_ostringstream<CharT> out;
  out.imbue(std::locale::classic());
  out << 'y';
  
  // Do the write
  unsigned long const* i = r;
  std::size_t const n_init = 42;
  std::size_t n = n_init;
  char const delim = '-';
  ::boost::rangeio::detail::write_impl(out, i, i + r_size, delim, n);
  
  // Make sure the "out" arguments were properly set
  BOOST_TEST_EQ(r_size, n - n_init);
  BOOST_TEST(r + r_size == i);
  
  // Make sure the write happened as it should have
  BOOST_TEST(bool(out));
  BOOST_RANGEIO_TEST_STR_EQ("y867-5309-555-2368", out.str());
  BOOST_TEST_EQ(std::streamsize(0), out.width());
  
  // Make sure the stream is still good for future writes
  BOOST_TEST(out << 'q');
  BOOST_RANGEIO_TEST_STR_EQ("y867-5309-555-2368q", out.str());
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
  std::istringstream in("2 4 6 8");
  in.imbue(std::locale::classic());
  int const in_data[] = { 2, 4, 8, 8 };
  std::size_t const in_size = sizeof(in_data) / sizeof(in_data[0]);
  
  // Prepare the output stream
  std::ostringstream out;
  out.imbue(std::locale::classic());
  out << "{ ";
  
  std::istream_iterator<int> i(in);
  std::istream_iterator<int> const e;
  std::size_t const n_init = 32;
  std::size_t n = n_init;
  ::boost::rangeio::detail::write_impl(out, i, e, ", ", n);
  
  // Make sure the "out" arguments were properly set
  BOOST_TEST_EQ(in_size, n - n_init);
  BOOST_TEST(i == e);
  BOOST_TEST(in.eof());
  
  // Make sure the write happened as it should have
  BOOST_TEST_EQ("{ 2, 4, 6, 8", out.str());
  BOOST_TEST_EQ(std::streamsize(0), out.width());
  
  // Make sure the stream is still good for future writes
  BOOST_TEST(bool(out));
  BOOST_TEST(out << " }");
  BOOST_TEST_EQ("{ 2, 4, 6, 8 }", out.str());
}

} // namespace input_iterator_range

// Confirm that ranges are written properly, and that formatting is preserved
// across elements. Preserved formatting should not apply to delimiters.
namespace formatting {

void test()
{
  // Prepare the range to be written
  double const r[] = { 1.0, -2.3, 6.66666, -0.12345, -1.2345 };
  std::size_t const r_size = sizeof(r) / sizeof(r[0]);
  
  // Prepare the output stream
  std::ostringstream out;
  out.imbue(std::locale::classic());
  out << "{ ";
  
  out.imbue(std::locale::classic());
  out.width(8);
  out.precision(3);
  out.fill('_');
  out.setf(std::ios_base::internal, std::ios_base::adjustfield);
  out.setf(std::ios_base::showpoint);
  
  // Do the write
  double const* i = r;
  std::size_t const n_init = 13;
  std::size_t n = n_init;
  ::boost::rangeio::detail::write_impl(out, i, i + r_size, " | ", n);
  
  // Make sure the "out" arguments were properly set
  BOOST_TEST_EQ(r_size, n - n_init);
  BOOST_TEST(r + r_size == i);
  
  // Make sure the write happened as it should have
  BOOST_TEST(bool(out));
  BOOST_TEST_EQ(std::streamsize(0), out.width());
  BOOST_TEST(out << " }");
  BOOST_TEST_EQ("{ ____1.00 | -___2.30 | ____6.67 | -__0.123 | -___1.23 }", out.str());
}

} // namespace formatting

// Verify that "smart delimiters" work.
namespace smart_delimiter {

void test()
{
  // Prepare the range to be written
  std::string const r = "abcdef";
  
  // Prepare the output stream
  std::ostringstream out;
  out.imbue(std::locale::classic());
  
  // Do the write
  std::string::const_iterator i = r.begin();
  std::size_t n = 0;
  ::boost::rangeio::test_extras::incrementing_integer_delimiter iid;
  ::boost::rangeio::detail::write_impl(out, i, r.end(), iid, n);
  
  // Make sure the "out" arguments were properly set
  BOOST_TEST_EQ(r.size(), n);
  BOOST_TEST(r.end() == i);
  BOOST_TEST_EQ(r.size() - 1, iid.i);
  
  // Make sure the write happened as it should have
  BOOST_TEST(bool(out));
  BOOST_TEST_EQ("a0b1c2d3e4f", out.str());
}

} // namespace smart_delimiter

} // namespace write_impl_delim_tests

int main()
{
  using namespace write_impl_delim_tests;
  
  empty_range::test();
  empty_const_range::test();
  
  normal_range::test();
  normal_const_range::test();
  
  input_iterator_range::test();
  
  formatting::test();
  
  smart_delimiter::test();
  
  return boost::report_errors();
}
