//-----------------------------------------------------------------------------
// File:             ib_context.hpp
//
// Copyright:        Alexander (Oleksandr) Kalentev 2023
// email:            o.kalentev (at) gmail . com
// Created:          03-Apr-2023
// Description:      Contains singleton class to handle ib device
// License:
//              MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef IB_CONTEXT_HPP_LG75DA9K
#define IB_CONTEXT_HPP_LG75DA9K

#include <atomic>
#include <infiniband/verbs.h>
//BOOST
#include <boost/throw_exception.hpp>
//STD
#include <string>

namespace ddf
{
  /// \brief Wraps out ibv_device.
  ///
  /// Contains the device handle, ib port number and port attributes. Also
  /// contains the global id number which is used as a unique id request to the
  /// ib pipeline.
  class ib_context
  {
    public:
      ~ib_context()
      {
         ibv_close_device(pib_ctx_);
      }
      ib_context(std::string dev_name = std::string()
          , int ib_port = 1);// : ib_port_(ib_port), ib_glob_id_(0)
      /// \brief Returns the shared pointer to the ibv_context.
      ///
      /// Returns the shared pointer to the ibv_context created in the
      /// constructor.
      ///
      /// \return shared pointer to the ibv_context.
      ibv_context* get_ctx() const;
      /// \brief Returns a const reference to the ibv_port_attr.
      ///
      /// \return const reference to the ibv_port_attr
      inline const ibv_port_attr& get_port_attr() const {return port_attr_;}
      /// \brief Returns an ib port number.
      ///
      /// \return ib port number
      inline int get_ib_port() const {return ib_port_;}
    private:
      ibv_context* pib_ctx_;//device handle
      ibv_port_attr port_attr_;
      int ib_port_;
  };

}//ddf

#endif// IB_CONTEXT_HPP_LG75DA9K
