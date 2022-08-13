// Copyright (C) 2021 Creality3D company. All rights reserved.
// Use of this source code is governed by a GNU General Public License(GPL) 
// and a GNU Lesser General Public License (LGPL).

// Created: 2021.06.21 @ xx

// Base class of upload and download task
// The primary role is to provide public signals

#ifndef OSS_INTERFACE_OSS_BASE_LOAD_TASK_H_
#define OSS_INTERFACE_OSS_BASE_LOAD_TASK_H_

#include "oss_base_task.h"

namespace oss_wrapper{
class OSSBaseEntity;

class OSSBaseLoadTask : public OSSBaseTask{
  Q_OBJECT
 public:
  OSSBaseLoadTask(size_t key,
                  OSSBaseEntity* entity,
                  const std::string& bucket_name,
                  AlibabaCloud::OSS::OssClient* client);
  ~OSSBaseLoadTask();

 protected:
  void progressCallback(size_t increment,
                        int64_t transfered,
                        int64_t total,
                        void* user_data);
};
}  // namespace oss

#endif // ! OSS_INTERFACE_OSS_BASE_LOAD_TASK_H_
