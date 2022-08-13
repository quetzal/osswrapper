#include "oss_interface.h"
#include "oss_entity.h"

#include <QCryptographicHash>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <iostream>

namespace oss_wrapper{

OSSInterface::OSSInterface() {}

OSSInterface::~OSSInterface() {}

bool OSSInterface::initOSSConfig(const QString accessKeyId, 
    const QString secretAccessKey, 
    const QString endPoint, 
    const QString bucket,
    const QString token)
{
    std::string key_id = accessKeyId.toStdString();
    std::string key_secret = secretAccessKey.toStdString();
    std::string end_point = endPoint.toStdString();
    std::string oss_bucket = bucket.toStdString();
    std::string security_token = token.toStdString();
    return OSSEntity::instance().initOSSConfigure(key_id, key_secret, end_point, security_token,
        oss_bucket);
}

size_t OSSInterface::uploadObject(const QString& object_name, const QString& local_file_name, 
      const ProgressCallback& progress_callback, const TaskResultCallback& outcome_callback)
{
    // check local file exist
    if (!fileIsExist(local_file_name)) {
        std::cout << "Local file not available" << std::endl;
        return 0;
    }

    QFileInfo fileInfo(local_file_name);
    QString file_name = fileInfo.fileName().toUtf8();

    std::string checkpoint_path = getObjectCheckPointPath(local_file_name);
    return OSSEntity::instance().uploadObject(
        object_name.toStdString(), file_name.toStdString().c_str(), std::string(local_file_name.toLocal8Bit()), checkpoint_path,
        progress_callback, outcome_callback);
}

size_t OSSInterface::downloadObject(
    const QString& object_url,
    const QString& local_file_path,
    const QString& file_name,
    const ProgressCallback& progress_callback,
    const TaskResultCallback& outcome_callback) {
  // check local dir exist
  QDir dir(local_file_path);
  if (!dir.exists()) {
    return 0;
  }

  std::string object_name = getObjectNameFromUrl(object_url);
  std::string save_file_name = getLocalFileName(local_file_path, file_name);
  return OSSEntity::instance().downloadObject(
      object_name, save_file_name, local_file_path.toStdString(),
      progress_callback, outcome_callback);
}
void OSSInterface::cancleLoadTask(const size_t key) {
  OSSEntity::instance().cancleLoadTask(key);
}

void OSSInterface::deleteObjects(const QStringList& object_urls,
                                 const TaskResultCallback& outcome_callback) {
  std::list<std::string> object_names;
  for (const auto& object_url : object_urls) {
    object_names.push_back(getObjectNameFromUrl(object_url));
  }
  OSSEntity::instance().deleteObjects(object_names, outcome_callback);
}

bool OSSInterface::fileIsExist(const QString& file_name) {
  QFile file(file_name);
  return file.exists();
}

std::string OSSInterface::getObjectNameFromUrl(const QString& object_url) {
    return "";
}

std::string OSSInterface::getLocalFileName(const QString& local_file_path,
                                           const QString& file_name) {
  QFileInfo file_info(local_file_path + file_name);
  QString file_base_name = file_info.baseName();
  QString stuffix = file_info.completeSuffix();
  QString complete_local_file_name = local_file_path + file_name;
  if (fileIsExist(complete_local_file_name)) {
    for (int i = 1;; i++) {
      QString tmp_file_name =
          local_file_path + file_base_name + QString("(%1)." + stuffix).arg(i);
      if (!fileIsExist(tmp_file_name)) {
        complete_local_file_name = tmp_file_name;
        break;
      }
    }
  }

  return complete_local_file_name.toStdString();
}

std::string OSSInterface::getObjectCheckPointPath(
    const QString& local_file_name) {
    return std::string(QFileInfo(local_file_name).dir().absolutePath().toLocal8Bit());//.toStdString();
}
}  // namespace oss
