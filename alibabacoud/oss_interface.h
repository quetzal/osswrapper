// Copyright (C) 2021 Creality3D company. All rights reserved.
// Use of this source code is governed by a GNU General Public License(GPL) 
// and a GNU Lesser General Public License (LGPL).

// Created: 2021.06.21 @ xx

// Interface of oss

#ifndef COMMON_OSS_INTERFACE_OSS_INTERFACE_H_
#define COMMON_OSS_INTERFACE_OSS_INTERFACE_H_
#include "osswrapperexport.h"
#include "oss_entity.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QString>

namespace oss_wrapper{

class OSS_WRAPPER_API OSSInterface {
 public:
  OSSInterface();
  ~OSSInterface();

  static bool initOSSConfig(const QString accessKeyId,
      const QString secretAccessKey,
      const QString endPoint,
      const QString bucket,
      const QString token);
  static size_t uploadObject(const QString& object_name,
      const QString& local_file_name,
      const ProgressCallback& progress_callback,
      const TaskResultCallback& outcome_callback);
  // local_file_path: path to save
  static size_t downloadObject(const QString& object_url,
                               const QString& local_file_path,
                               const QString& file_name,
                               const ProgressCallback& progress_callback,
                               const TaskResultCallback& outcome_callback);
  static void cancleLoadTask(const size_t key);
  static void deleteObjects(const QStringList& object_urls,
                            const TaskResultCallback& outcome_callback);

 private:
  static bool fileIsExist(const QString& file_name);
  static std::string getObjectNameFromUrl(const QString& object_url);
  static std::string getLocalFileName(const QString& local_file_path,
                                      const QString& local_file_name);
  static std::string getObjectCheckPointPath(const QString& local_file_name);
};
}  // namespace oss
#endif  // ! COMMON_OSS_INTERFACE_OSS_INTERFACE_H_
