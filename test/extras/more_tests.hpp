//
// Copyright (c) Mark A. Gibbs, 2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// 

#ifndef BOOST_RANGEIO_TestInc_extras_X_more_tests_2015_01_01_
#define BOOST_RANGEIO_TestInc_extras_X_more_tests_2015_01_01_

#include <boost/config.hpp>

#include <locale>
#include <string>

#include <boost/core/lightweight_test.hpp>

namespace boost {
namespace rangeio {
namespace test_extras {

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

template <typename CharT, typename Traits>
inline void test_str_eq_impl(
  char const* expr1,
  char const* expr2,
  char const* file,
  int line,
  char const* function,
  std::basic_string<CharT, Traits> const& s1,
  std::basic_string<CharT, Traits> const& s2)
{
  if (s1 == s2)
  {
    ::boost::detail::report_errors_remind();
  }
  else
  {
    std::string s1_str(s1.size(), char());
    std::string s2_str(s2.size(), char());
    
    std::transform(s1.begin(), s1.end(), s1_str.begin(), narrow_char<CharT>(std::locale()));
    std::transform(s2.begin(), s2.end(), s2_str.begin(), narrow_char<CharT>(std::locale()));
    
    BOOST_LIGHTWEIGHT_TEST_OSTREAM
      << file << "(" << line << "): test '" << expr1 << " == " << expr2
      << "' failed in function '" << function << "': "
      << "'" << s1_str << "' != '" << s2_str << "'" << std::endl;
    ++::boost::detail::test_errors();
  }
}

template <typename CharT, typename Traits>
inline void test_str_eq_impl(
  char const* expr1,
  char const* expr2,
  char const* file,
  int line,
  char const* function,
  std::string const& s1,
  std::basic_string<CharT, Traits> const& s2)
{
  std::basic_string<CharT, Traits> str(s1.size(), CharT());
  std::transform(s1.begin(), s1.end(), str.begin(), widen_char<CharT>(std::locale()));
  
  test_str_eq_impl(expr1, expr2, file, line, function, str, s2);
}

template <typename CharT, typename Traits>
inline void test_str_eq_impl(
  char const* expr1,
  char const* expr2,
  char const* file,
  int line,
  char const* function,
  std::basic_string<CharT, Traits> const& s1,
  std::string const& s2)
{
  std::basic_string<CharT, Traits> str(s2.size(), CharT());
  std::transform(s2.begin(), s2.end(), str.begin(), widen_char<CharT>(std::locale()));
  
  test_str_eq_impl(expr1, expr2, file, line, function, s1, str);
}

inline void test_str_eq_impl(
  char const* expr1,
  char const* expr2,
  char const* file,
  int line,
  char const* function,
  std::string const& s1,
  std::string const& s2)
{
  ::boost::detail::test_eq_impl(expr1, expr2, file, line, function, s1, s2);
}

} // namespace test_extras
} // namespace rangeio
} // namespace boost

#define BOOST_RANGEIO_TEST_STR_EQ(expr1,expr2) ( ::boost::rangeio::test_extras::test_str_eq_impl(#expr1, #expr2, __FILE__, __LINE__, BOOST_CURRENT_FUNCTION, expr1, expr2) )

#endif  // include guard
