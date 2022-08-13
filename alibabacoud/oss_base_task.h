// Copyright (C) 2021 Creality3D company. All rights reserved.
// Use of this source code is governed by a GNU General Public License(GPL) 
// and a GNU Lesser General Public License (LGPL).

// Created: 2021.06.21 @ xx

// Base class of all task

#ifndef OSS_INTERFACE_OSS_BASE_TASK_H_
#define OSS_INTERFACE_OSS_BASE_TASK_H_

#include "alibabacloud/oss/OssClient.h"
#include "oss_constant.h"

#include <QObject>
#include <QRunnable>

namespace oss_wrapper{
class OSSBaseEntity;

class OSSBaseTask : public QObject, public QRunnable {
  Q_OBJECT
 protected:
  size_t key_ = 0;
  std::string bucket_name_ = "";
  OSSBaseEntity* entity_ = nullptr;
  AlibabaCloud::OSS::OssClient* client_ = nullptr;

  OSSBaseTask(size_t key,
              OSSBaseEntity* entity,
              const std::string& bucket_name,
              AlibabaCloud::OSS::OssClient* client);
  virtual ~OSSBaseTask();
};
}  // namespace oss

#endif  // ! OSS_INTERFACE_OSS_BASE_TASK_H_
