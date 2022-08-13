// Copyright (C) 2021 Creality3D company. All rights reserved.
// Use of this source code is governed by a GNU General Public License(GPL) 
// and a GNU Lesser General Public License (LGPL).

// Created: 2021.06.21 @ xx

// Creating various tasks 

#ifndef OSS_INTERFACE_OSS_ENTITY_H_
#define OSS_INTERFACE_OSS_ENTITY_H_

#include "oss_constant.h"

#include <QMap>
#include <QObject>
#include <mutex>

namespace AlibabaCloud
{
    namespace OSS
    {
        class OssClient;
    }
}

namespace oss_wrapper{

enum class LoadType { DOWNLOAD = 0, UPLOAD };
typedef std::function<void(long long, long long)> ProgressCallback;
typedef std::function<void(const OSSError&)> TaskResultCallback;
typedef std::function<void(const QString&)> GetLastModifyCallback;

class OSSBaseEntity : public QObject {
  Q_OBJECT
 public:
  virtual ~OSSBaseEntity();
  virtual void onProgressFinished(size_t key, const OSSError& error) = 0;
  virtual void onProgressChanged(size_t key, long long transfered, long long total) = 0;
};

class OSSEntity : public OSSBaseEntity {
  Q_OBJECT
 public:
  static OSSEntity& instance();
  bool is_inited() { return is_inited_; }

  bool initOSSConfigure(const std::string& access_key_id,
                        const std::string& access_key_secret,
                        const std::string& end_point,
                        const std::string& security_token,
                        std::string& bucket_name);

  size_t downloadObject(const std::string& object_name,
                        const std::string& local_file_name,
                        const std::string& check_point_path,
                        const ProgressCallback& progress_callback,
                        const TaskResultCallback& outcome_callback);
  size_t uploadObject(const std::string& object_name,
                      const std::string& file_name,
                      const std::string& local_file_name,
                      const std::string& check_point_path,
                      const ProgressCallback& progress_callback,
                      const TaskResultCallback& outcome_callback);
  void cancleLoadTask(const size_t key);
  void deleteObjects(const std::list<std::string>& object_names,
                     const TaskResultCallback& outcome_callback);
  // object_name: 3/2/1/avatar.jpg
  std::string generateOSSUrl(const std::string& path, const std::string& object_name);

 private:
  OSSEntity();
  ~OSSEntity();
  OSSEntity(const OSSEntity&) = delete;
  OSSEntity& operator=(const OSSEntity&) = delete;

  bool is_inited_ = false;
  std::string end_point_ = "";
  std::string bucket_name_ = "";
  std::string access_key_id_ = "";
  std::string access_key_secret_ = "";
  std::string security_token_ = "";
  QMap<size_t, std::tuple<AlibabaCloud::OSS::OssClient*, ProgressCallback, TaskResultCallback> > client_map_;
  std::mutex m_mutex;

  void onProgressChanged(size_t key, long long transfered, long long total) override;
  void onProgressFinished(size_t key, const OSSError& error) override;
  size_t getRandomNumber();
  size_t createLoadTask(const std::string& object_name,
                        const std::string& file_name,
                        const std::string& local_file_name,
                        const std::string& check_point_path,
                        const LoadType type,
                        const ProgressCallback& progress_callback,
                        const TaskResultCallback& outcome_callback);
};
}  // namespace oss

#endif // ! OSS_INTERFACE_OSS_ENTITY_H_
