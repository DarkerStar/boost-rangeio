//
// Copyright (c) Mark A. Gibbs, 2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// 

#ifndef BOOST_RANGEIO_Inc_detail_X_formatting_saver_2015_01_01_
#define BOOST_RANGEIO_Inc_detail_X_formatting_saver_2015_01_01_

#include <iosfwd>

namespace boost {
namespace rangeio {
namespace detail {

// 
// Simple class that stores the relevant formatting data from a stream on
// construction, and restores it when desired.
// 
// NOTE: This is similar to the formatting savers from Boost.IOStateSavers, but
// with one crucial difference: it doesn't restore in the destructor. That
// behaviour is undesirable for what this class is used for, and it cannot be
// prevented (there is no "release" function in Boost.IOStateSavers), so
// Boost.IOStateSavers could not be used.
// 
template <typename CharT, typename Traits>
class formatting_saver
{
public:
  explicit formatting_saver(::std::basic_ios<CharT, Traits>& s) :
    stream_(s),
    width_(s.width())
  {}
  
  void restore() const
  {
    stream_.width(width_);
  }
  
private:
  std::basic_ios<CharT, Traits>& stream_;
  std::streamsize const width_;
};

} // namespace detail
} // namespace rangeio
} // namespace boost

#endif  // include guard
