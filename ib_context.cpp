//-----------------------------------------------------------------------------
// File:             ib_context_impl.hpp
//
// Copyright:        Alexander (Oleksandr) Kalentev 2023
// email:            o.kalentev (at) gmail . com
// Created:          05-Apr-2023
// Description:      Contains implementation of the `ib_context` constructor
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

#include "ib_context.hpp"
//std
#include <memory>

ibv_context* ddf::ib_context::get_ctx() const {return pib_ctx_;}

ddf::ib_context:: ib_context(std::string dev_name , int ib_port) :
  ib_port_(ib_port)
{
  int num_devices;
  ibv_device* ib_dev=nullptr;
  //get device names in the system
  printf("Get devlist\n");
  std::shared_ptr<ibv_device*[]> dev_list
    (ibv_get_device_list(&num_devices),
     [](ibv_device **p)
     {
       if(p)
         ibv_free_device_list(p);
       printf("Devlist deleted!\n");
     }
    );
  if(!dev_list)
  {
    perror("Couldn't get device list!");
    abort();
  }
  //if there isn't any IB_CONTEXT device in host
  if(!num_devices)
  {
    perror("No ib devices found!");
    abort();
  }
  printf("Found %d device(s)\n", num_devices);

  //search for the specific device we want to work with
  for(int i = 0; i < num_devices; i ++)
  {
    if(dev_name.empty())
    {
      dev_name = std::string(ibv_get_device_name(dev_list[i]));
      ib_dev = dev_list[i];
      printf("Device not specified, using first one found: \"%s\"\n"
          , dev_name.c_str());
      break;
    }
    //find the specific device
    if(std::string(ibv_get_device_name(dev_list[i])) == dev_name)
    {
      ib_dev = dev_list[i];
      break;
    }
  }

  //if the device wasn't found in host
  if(!ib_dev)
  {
    fprintf(stderr,"IB_CONTEXT device \"%s\" wasn't found\n",dev_name.c_str());
    abort();
  }

  printf("Creating ib context!\n");
  //get device handle
  pib_ctx_ = ibv_open_device(ib_dev);
  if(!pib_ctx_)
  {
    fprintf(stderr,"Failed to open device \"%s\"\n",dev_name.c_str());
    abort();
  }

  //query port properties
  if(ibv_query_port(pib_ctx_, ib_port, &port_attr_))
  {
    fprintf(stderr,"ibv_query_port on port %d failed", ib_port);
    perror("ibv_query_port failed");
    abort();
  }
  printf("Local LID = 0x%x\n", port_attr_.lid);
}
