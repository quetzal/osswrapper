// Copyright (C) 2021 Creality3D company. All rights reserved.
// Use of this source code is governed by a GNU General Public License(GPL) 
// and a GNU Lesser General Public License (LGPL).

// Created: 2021.06.21 @ xx

// Constant of oss_wrapperinterface

#ifndef OSS_INTERFACE_OSS_CONSTANT_H_
#define OSS_INTERFACE_OSS_CONSTANT_H_

//#include "alibabacloud/oss/OssError.h"

#include <string>

namespace oss_wrapper{
struct OSSError {
  int key_ = 0;
  std::string code_ = "";
  std::string message_ = "";

  OSSError() {}
  OSSError(const std::string& code, const std::string& message) {
      code_ = code;
      message_ = message;
  }
  //OSSError(const AlibabaCloud::OSS::OssError& error) {
  //  code_ = error.Code();
  //  message_ = error.Message();
  //}
};
}  // namespace oss

#endif // ! OSS_INTERFACE_OSS_CONSTANT_H_