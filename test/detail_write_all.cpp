//
// Copyright (c) Mark A. Gibbs, 2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// 

#include <algorithm>
#include <locale>
#include <functional>
#include <iterator>
#include <sstream>
#include <vector>

#include <boost/core/lightweight_test.hpp>

#include <boost/rangeio/detail/write_all.hpp>

namespace write_all_tests {

// 
// Function object that narrows non-char characters using the default locale.
// 
template <typename CharT>
class narrow_char :
  std::unary_function<CharT, char>
{
public:
  narrow_char() :
    fac_(std::use_facet<std::ctype<CharT> >(std::locale()))
  {}
  
  explicit narrow_char(std::locale const& loc) :
    fac_(std::use_facet<std::ctype<CharT> >(loc))
  {}
  
  char operator()(CharT c) const { return fac_.narrow(c, '?'); }
  
private:
  std::ctype<CharT> const& fac_;
};

// 
// Function object that widens char characters using the default locale.
// 
template <typename CharT>
class widen_char :
  std::unary_function<char, CharT>
{
public:
  widen_char() :
    fac_(std::use_facet<std::ctype<CharT> >(std::locale()))
  {}
  
  explicit widen_char(std::locale const& loc) :
    fac_(std::use_facet<std::ctype<CharT> >(loc))
  {}
  
  CharT operator()(char c) const { return fac_.widen(c); }
  
private:
  std::ctype<CharT> const& fac_;
};

// 
// Converts a narrow string (std::string) to a wide-character string.
// 
template <typename CharT>
std::basic_string<CharT> widen_string(std::string const& s)
{
  std::basic_string<CharT> r(s.size(), CharT());
  std::transform(s.begin(), s.end(), r.begin(), widen_char<CharT>(std::locale()));
  return r;
}

// 
// Function to test two strings to verify that they're equal in a way that makes
// the contents of both strings print out if they're not equal.
// 
// The generic version just does a regular equality test, and if it fails, it
// narrows both strings then uses BOOST_TEST_EQ() (which prints both strings if
// they're not equal). This is a hack that isn't guaranteed to work (two strings
// may compare unequal when wide but then narrow to identical strings), but it's
// better than nothing.
// 
template <typename CharT>
void test_equal_string(std::basic_string<CharT> const& a, std::basic_string<CharT> const& b)
{
  if (a != b)
  {
    std::string a_str(a.size(), CharT());
    std::string b_str(b.size(), CharT());
    
    std::transform(a.begin(), a.end(), a_str.begin(), narrow_char<CharT>(std::locale()));
    std::transform(b.begin(), b.end(), b_str.begin(), narrow_char<CharT>(std::locale()));
    
    BOOST_TEST_EQ(a_str, b_str);
  }
}

// 
// Function to test two strings to verify that they're equal in a way that makes
// the contents of both strings print out if they're not equal.
// 
// The std::string version simply uses BOOST_TEST_EQ().
// 
void test_equal_string(std::string const& a, std::string const& b)
{
  BOOST_TEST_EQ(a, b);
}

// 
// This macro allows the comparison of two strings even if they're not narrow
// character strings.
// 
// You can't simply put two generic strings into BOOST_TEST_EQ(), because it
// will choke if the strings are not char strings (because they can't be
// streamed to a narrow char ostream). You *can* test any two generic strings
// using BOOST_TEST(), but if the test fails you don't get to see the two
// strings to see what the difference is.
// 
// This macro makes a shamefully hackish attempt to overcome that deficiency by
// first doing a BOOST_TEST(s1 == s2) - which works for any string type but
// doesn't print the two strings on error - then calls
// test_equal_string(s1, s2). That function tries to compare the two strings in
// a way that prints both if they're not equal.
// 
// Maybe this and its helper functions should be moved to its own file and made
// more reusable?
// 
#define BOOST_RANGEIO_TEST_EQ_STR(expr1, expr2) \
  BOOST_TEST(expr1 == expr2); \
  write_all_tests::test_equal_string(expr1, expr2);

namespace empty_range {

template <typename CharT>
void do_test()
{
  std::vector<int> r;
  std::basic_ostringstream<CharT> out;
  
  // Set the width, just to test it later.
  out.width(7);
  
  std::vector<int>::iterator i = r.begin();
  std::size_t const n_init = 69;
  std::size_t n = n_init;
  boost::rangeio::detail::write_all(out, i, r.end(), n);
  
  BOOST_TEST_EQ(n_init, n);
  BOOST_TEST(r.begin() == i);
  BOOST_TEST(r.end() == i);
  
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
  std::vector<int> const r;
  std::basic_ostringstream<CharT> out;
  
  // Set the width, just to test it later.
  out.width(7);
  
  std::vector<int>::const_iterator i = r.begin();
  std::size_t const n_init = 69;
  std::size_t n = n_init;
  boost::rangeio::detail::write_all(out, i, r.end(), n);
  
  BOOST_TEST_EQ(n_init, n);
  BOOST_TEST(r.begin() == i);
  BOOST_TEST(r.end() == i);
  
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
  int* i = r;
  std::size_t const n_init = 42;
  std::size_t n = n_init;
  boost::rangeio::detail::write_all(out, i, i + r_size, n);
  
  // Make sure the "out" arguments were properly set
  BOOST_TEST_EQ(r_size, n - n_init);
  BOOST_TEST(r + r_size == i);
  
  // Make sure the write happened as it should have
  BOOST_RANGEIO_TEST_EQ_STR(widen_string<CharT>("a112358"), out.str());
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
  BOOST_RANGEIO_TEST_EQ_STR(widen_string<CharT>("a112358x"), out.str());
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
  unsigned long const* i = r;
  std::size_t const n_init = 42;
  std::size_t n = n_init;
  boost::rangeio::detail::write_all(out, i, i + r_size, n);
  
  // Make sure the "out" arguments were properly set
  BOOST_TEST_EQ(r_size, n - n_init);
  BOOST_TEST(r + r_size == i);
  
  // Make sure the write happened as it should have
  BOOST_RANGEIO_TEST_EQ_STR(widen_string<CharT>("y86753095552368"), out.str());
  BOOST_TEST_EQ(std::streamsize(0), out.width());
  
  // Make sure the stream is still good for future writes
  BOOST_TEST(out << 'q');
  BOOST_RANGEIO_TEST_EQ_STR(widen_string<CharT>("y86753095552368q"), out.str());
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
  
  std::istream_iterator<int> i(in);
  std::istream_iterator<int> const e;
  std::size_t const n_init = 32;
  std::size_t n = n_init;
  boost::rangeio::detail::write_all(out, i, e, n);
  
  // Make sure the "out" arguments were properly set
  BOOST_TEST_EQ(in_size, n - n_init);
  BOOST_TEST(i == e);
  BOOST_TEST(in.eof());
  
  // Make sure the write happened as it should have
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
  int const* i = r;
  std::size_t const n_init = 13;
  std::size_t n = n_init;
  boost::rangeio::detail::write_all(out, i, i + r_size, n);
  
  // Make sure the "out" arguments were properly set
  BOOST_TEST_EQ(r_size, n - n_init);
  BOOST_TEST(r + r_size == i);
  
  // Make sure the write happened as it should have
  BOOST_TEST_EQ(std::streamsize(0), out.width());
  BOOST_TEST(out << " }");
  BOOST_TEST_EQ("{ " "0X287.." "0X71A.." "0XE6..." "0X1A..." "0X29E.." " }", out.str());
}

} // namespace formatting

} // namespace write_all_tests

int main()
{
  using namespace write_all_tests;
  
  empty_range::test();
  empty_const_range::test();
  
  normal_range::test();
  normal_const_range::test();
  
  input_iterator_range::test();
  
  formatting::test();
  
  return boost::report_errors();
}
