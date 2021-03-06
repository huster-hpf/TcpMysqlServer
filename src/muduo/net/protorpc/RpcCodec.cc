// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include <src/muduo/net/protorpc/RpcCodec.h>

#include <src/muduo/base/Logging.h>
#include <src/muduo/net/Endian.h>
#include <src/muduo/net/TcpConnection.h>

#include <src/muduo/net/protorpc/rpc.pb.h>
#include <src/muduo/net/protorpc/google-inl.h>

using namespace muduo;
using namespace muduo::net;

namespace
{
  int ProtobufVersionCheck()
  {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    return 0;
  }
  int dummy __attribute__ ((unused)) = ProtobufVersionCheck();
}

namespace muduo
{
namespace net
{
const char rpctag [] = "RPC0";
}
}
