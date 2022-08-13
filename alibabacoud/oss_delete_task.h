// Copyright (C) 2021 Creality3D company. All rights reserved.
// Use of this source code is governed by a GNU General Public License(GPL) 
// and a GNU Lesser General Public License (LGPL).

// Created: 2021.06.21 @ xx

#ifndef OSS_INTERFACE_OSS_DELETE_TASk_H_
#define OSS_INTERFACE_OSS_DELETE_TASk_H_

#include "oss_base_task.h"

namespace oss_wrapper{
class OSSDeleteTask : public OSSBaseTask {
  Q_OBJECT
 public:
  OSSDeleteTask(const size_t key,
                OSSBaseEntity* entity,
                const std::string& bucket_name,
                AlibabaCloud::OSS::OssClient* client,
                const std::list<std::string>& object_names);
  ~OSSDeleteTask();

 protected:
  void run() override;

 private:
  std::list<std::string> object_names_;
};
}  // namespace oss

#endif  // ! OSS_INTERFACE_OSS_DELETE_TASk_H_
