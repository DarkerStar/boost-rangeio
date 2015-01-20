//
// Copyright (c) Mark A. Gibbs, 2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// 

#include <iterator>
#include <sstream>
#include <vector>

#include <boost/core/lightweight_test.hpp>

#include <boost/rangeio/write_iterator_range.hpp>

#include "extras/more_tests.hpp"

namespace write_iterator_range_imm_tests {

namespace empty_range {

template <typename CharT>
void do_test()
{
  std::vector<double> r;
  std::basic_ostringstream<CharT> out;
  
  // Set the width, just to test it later.
  out.width(7);
  
  ::boost::rangeio::write_iterator_range_result_t<std::vector<double>::iterator> res =
    ::boost::rangeio::write_iterator_range(out, r.begin(), r.end());
  
  BOOST_TEST(r.end() == res.next);
  BOOST_TEST_EQ(std::size_t(0), res.count);
  
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

namespace empty_const_range {

template <typename CharT>
void do_test()
{
  std::vector<double> const r;
  std::basic_ostringstream<CharT> out;
  
  // Set the width, just to test it later.
  out.width(7);
  
  ::boost::rangeio::write_iterator_range_result_t<std::vector<double>::const_iterator> res =
    ::boost::rangeio::write_iterator_range(out, r.begin(), r.end());
  
  BOOST_TEST(r.end() == res.next);
  BOOST_TEST_EQ(std::size_t(0), res.count);
  
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
  ::boost::rangeio::write_iterator_range_result_t<int*> res =
    ::boost::rangeio::write_iterator_range(out, r, r + r_size);
  
  // Make sure the "out" arguments were properly set
  BOOST_TEST(r + r_size == res.next);
  BOOST_TEST_EQ(r_size, res.count);
  
  // Make sure the write happened as it should have
  BOOST_TEST(bool(out));
  BOOST_RANGEIO_TEST_STR_EQ("a112358", out.str());
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
  BOOST_RANGEIO_TEST_STR_EQ("a112358x", out.str());
}

void test()
{
  do_test<char>();
  do_test<wchar_t>();
}

} // namespace normal_range

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
  ::boost::rangeio::write_iterator_range_result_t<unsigned long const*> res =
    ::boost::rangeio::write_iterator_range(out, r, r + r_size);
  
  // Make sure the "out" arguments were properly set
  BOOST_TEST(r + r_size == res.next);
  BOOST_TEST_EQ(r_size, res.count);
  
  // Make sure the write happened as it should have
  BOOST_TEST(bool(out));
  BOOST_RANGEIO_TEST_STR_EQ("y86753095552368", out.str());
  BOOST_TEST_EQ(std::streamsize(0), out.width());
  
  // Make sure the stream is still good for future writes
  BOOST_TEST(out << 'q');
  BOOST_RANGEIO_TEST_STR_EQ("y86753095552368q", out.str());
}

void test()
{
  do_test<char>();
  do_test<wchar_t>();
}

} // namespace normal_const_range

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
  out << "pre";
  
  std::istream_iterator<int> const e;
  
  // Do the write
  ::boost::rangeio::write_iterator_range_result_t<std::istream_iterator<int> > res =
    ::boost::rangeio::write_iterator_range(out, std::istream_iterator<int>(in), e);
  
  // Make sure the "out" arguments were properly set
  BOOST_TEST(e == res.next);
  BOOST_TEST_EQ(in_size, res.count);
  BOOST_TEST(in.eof());
  
  // Make sure the write happened as it should have
  BOOST_TEST(bool(out));
  BOOST_TEST_EQ("pre2468", out.str());
  BOOST_TEST_EQ(std::streamsize(0), out.width());
  
  // Make sure the stream is still good for future writes
  BOOST_TEST(out << "post");
  BOOST_TEST_EQ("pre2468post", out.str());
}

} // namespace input_iterator_range

namespace formatting {

void test()
{
  // Prepare the range to be written
  int const r[] = { 0x0287, 0x071A, 0x00E6, 0x001A, 0x029E };
  std::size_t const r_size = sizeof(r) / sizeof(r[0]);
  
  // Prepare the output stream
  std::ostringstream out;
  out.imbue(std::locale::classic());
  out << "{ ";
  
  out.width(7);
  out.fill('.');
  out.setf(std::ios_base::hex, std::ios_base::basefield);
  out.setf(std::ios_base::left, std::ios_base::adjustfield);
  out.setf(std::ios_base::uppercase);
  out.setf(std::ios_base::showbase);
  
  // Do the write
  ::boost::rangeio::write_iterator_range_result_t<int const*> res =
    ::boost::rangeio::write_iterator_range(out, r, r + r_size);
  
  // Make sure the "out" arguments were properly set
  BOOST_TEST(r + r_size == res.next);
  BOOST_TEST_EQ(r_size, res.count);
  
  // Make sure the write happened as it should have
  BOOST_TEST(bool(out));
  BOOST_TEST_EQ(std::streamsize(0), out.width());
  BOOST_TEST(out << " }");
  BOOST_TEST_EQ("{ " "0X287.." "0X71A.." "0XE6..." "0X1A..." "0X29E.." " }", out.str());
}

} // namespace formatting

} // namespace write_iterator_range_imm_tests

int main()
{
  using namespace write_iterator_range_imm_tests;
  
  empty_range::test();
  empty_const_range::test();
  
  normal_range::test();
  normal_const_range::test();
  
  input_iterator_range::test();
  
  formatting::test();
  
  return boost::report_errors();
}
