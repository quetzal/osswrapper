// Copyright (C) 2021 Creality3D company. All rights reserved.
// Use of this source code is governed by a GNU General Public License(GPL) 
// and a GNU Lesser General Public License (LGPL).

// Created: 2021.06.21 @ xx

// Upload task

#ifndef OSS_INTERFACE_OSS_LOAD_TASK_H_
#define OSS_INTERFACE_OSS_LOAD_TASK_H_

#include "oss_base_load_task.h"

namespace oss_wrapper{
class OSSUploadTask : public OSSBaseLoadTask {
 public:
  // object_name: pc_test/test/test.jpg
  // local_file_name: C:/test/test.jpg
  // check_point_path: C:/test/
  // uploader_id: uploader user id
  OSSUploadTask(const size_t key,
                OSSBaseEntity* entity,
                const std::string& bucket_name,
                AlibabaCloud::OSS::OssClient* client,
                const std::string& object_name,
                const std::string& file_name,
                const std::string& local_file_name,
                const std::string& check_point_path);
  ~OSSUploadTask();

  void run() override;

 private:
  std::string object_name_ = "";
  std::string file_name_ = "";
  std::string local_file_name_ = "";
  std::string check_point_path_ = "";
};
}  // namespace oss

#endif // ! OSS_INTERFACE_OSS_LOAD_TASK_H_
