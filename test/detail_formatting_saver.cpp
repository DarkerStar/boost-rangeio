//
// Copyright (c) Mark A. Gibbs, 2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// 

#include <sstream>

#include <boost/core/lightweight_test.hpp>

#include <boost/rangeio/detail/formatting_saver.hpp>

namespace formatting_saver_tests {

// 
// Handy struct for bundling the entire set of stream state variables that are
// relevant.
// 
template <typename CharT>
struct stream_state
{
  std::ios_base::fmtflags flags;
  CharT                   fill;
  std::streamsize         precision;
  std::streamsize         width;
  
  stream_state(std::ios_base::fmtflags fl, CharT fi, std::streamsize p, std::streamsize w) :
    flags(fl),
    fill(fi),
    precision(p),
    width(w)
  {}
};

// 
// Templated test function, so that the different stream types can all be tested
// with a single function.
// 
template <typename IOStream>
void test_formatting_saver(IOStream& s)
{
  typedef typename IOStream::char_type   char_type;
  typedef typename IOStream::traits_type traits_type;
  
  stream_state<char_type> const a(
    std::ios_base::dec | std::ios_base::left | std::ios_base::scientific,
    s.widen('x'),
    5,
    10);
  stream_state<char_type> const b(
    std::ios_base::hex | std::ios_base::right |
      std::ios_base::fixed | std::ios_base::boolalpha | std::ios_base::showbase |
      std::ios_base::showpoint | std::ios_base::showpos | std::ios_base::skipws |
      std::ios_base::unitbuf | std::ios_base::uppercase,
    s.widen('-'),
    6,
    8);
  
  s.flags(a.flags);
  s.fill(a.fill);
  s.precision(a.precision);
  s.width(a.width);
  
  {
    // Make sure we're starting out sane.
    BOOST_TEST_EQ(a.flags,     s.flags());
    BOOST_TEST_EQ(a.fill,      s.fill());
    BOOST_TEST_EQ(a.precision, s.precision());
    BOOST_TEST_EQ(a.width,     s.width());
    
    ::boost::rangeio::detail::formatting_saver<char_type, traits_type> const fs(s);
    
    // Confirm that no state is changed by the constructor.
    BOOST_TEST_EQ(a.flags,     s.flags());
    BOOST_TEST_EQ(a.fill,      s.fill());
    BOOST_TEST_EQ(a.precision, s.precision());
    BOOST_TEST_EQ(a.width,     s.width());
    
    fs.restore();
    
    // Check that the original state has been restored.
    BOOST_TEST_EQ(a.flags,     s.flags());
    BOOST_TEST_EQ(a.fill,      s.fill());
    BOOST_TEST_EQ(a.precision, s.precision());
    BOOST_TEST_EQ(a.width,     s.width());
    
    s.flags(b.flags);
    s.fill(b.fill);
    s.precision(b.precision);
    s.width(b.width);
    
    // Confirm that we've changed the state.
    BOOST_TEST_EQ(b.flags,     s.flags());
    BOOST_TEST_EQ(b.fill,      s.fill());
    BOOST_TEST_EQ(b.precision, s.precision());
    BOOST_TEST_EQ(b.width,     s.width());
    
    fs.restore();
    
    // Check that the original state has been restored.
    BOOST_TEST_EQ(a.flags,     s.flags());
    BOOST_TEST_EQ(a.fill,      s.fill());
    BOOST_TEST_EQ(a.precision, s.precision());
    BOOST_TEST_EQ(a.width,     s.width());
    
    s.flags(b.flags);
    s.fill(b.fill);
    s.precision(b.precision);
    s.width(b.width);
    
    // Confirm that we've changed the state.
    BOOST_TEST_EQ(b.flags,     s.flags());
    BOOST_TEST_EQ(b.fill,      s.fill());
    BOOST_TEST_EQ(b.precision, s.precision());
    BOOST_TEST_EQ(b.width,     s.width());
  }
  
  // Check that the destructor did not change the state.
  BOOST_TEST_EQ(b.flags,     s.flags());
  BOOST_TEST_EQ(b.fill,      s.fill());
  BOOST_TEST_EQ(b.precision, s.precision());
  BOOST_TEST_EQ(b.width,     s.width());
}

namespace istream_formatting_saver {

void test()
{
  std::istringstream s;
  test_formatting_saver(s);
}

} // namespace istream_formatting_saver

namespace ostream_formatting_saver {

void test()
{
  std::ostringstream s;
  test_formatting_saver(s);
}

} // namespace istream_formatting_saver

namespace wistream_formatting_saver {

void test()
{
  std::wistringstream s;
  test_formatting_saver(s);
}

} // namespace wistream_formatting_saver

namespace wostream_formatting_saver {

void test()
{
  std::wostringstream s;
  test_formatting_saver(s);
}

} // namespace wistream_formatting_saver

} // namespace formatting_saver_tests

int main()
{
  using namespace formatting_saver_tests;
  
  istream_formatting_saver::test();
  ostream_formatting_saver::test();
  wistream_formatting_saver::test();
  wostream_formatting_saver::test();
  
  return boost::report_errors();
}
