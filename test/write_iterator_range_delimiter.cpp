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

#include <boost/core/lightweight_test.hpp>

#include <boost/rangeio/write_iterator_range.hpp>

namespace write_iterator_range_delimiter_tests {

} // namespace write_iterator_range_delimiter_tests

int main()
{
  using namespace write_iterator_range_delimiter_tests;
  
  return boost::report_errors();
}

#endif  // BOOST_NO_CXX11_RVALUE_REFERENCES
