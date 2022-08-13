#include "oss_entity.h"
#include "alibabacloud/oss/OssClient.h"

#include "oss_base_load_task.h"
#include "oss_delete_task.h"
#include "oss_download_task.h"
#include "oss_upload_task.h"

#include <QThreadPool>
#include <ctime>
#include <random>

namespace oss_wrapper{
OSSEntity::OSSEntity() {}

OSSEntity::~OSSEntity() {
  if (AlibabaCloud::OSS::IsSdkInitialized()) {
    AlibabaCloud::OSS::ShutdownSdk();
  }
}

void OSSEntity::onProgressChanged(size_t key, long long transfered, long long total)
{
    ProgressCallback progress_callback = nullptr;
    m_mutex.lock();
    if (client_map_.contains(key))
    {
        progress_callback = std::get<1>(client_map_[key]);
    }
    m_mutex.unlock();
    if (progress_callback != nullptr) {
        progress_callback(transfered, total);
    }
}

void OSSEntity::onProgressFinished(size_t key, const OSSError& error) {
    AlibabaCloud::OSS::OssClient* client = nullptr;
    TaskResultCallback outcome_callback = nullptr;
    m_mutex.lock();
    if (client_map_.contains(key)) {
        client = std::get<0>(client_map_[key]);
        outcome_callback = std::get<2>(client_map_[key]);
    client_map_.remove(key);
    }
    m_mutex.unlock();
    if (outcome_callback)
    {
        outcome_callback(error);
    }

    if (client) {
        delete client;
        client = nullptr;
    }
}

size_t OSSEntity::getRandomNumber() {
  static std::default_random_engine e(time(0));
  static std::uniform_int_distribution<unsigned> u(0, 1000);
  static std::hash<int> hash_fn;
  size_t number = hash_fn(u(e));
  if (client_map_.contains(number)) {
    number = getRandomNumber();
  }
  return number;
}

size_t OSSEntity::createLoadTask(const std::string& object_name,
                                 const std::string& file_name,
                                 const std::string& local_file_name,
                                 const std::string& check_point_path,
                                 const LoadType type,
                                 const ProgressCallback& progress_callback,
                                 const TaskResultCallback& outcome_callback) {
  AlibabaCloud::OSS::ClientConfiguration conf;
  AlibabaCloud::OSS::OssClient* client = new AlibabaCloud::OSS::OssClient(
      end_point_, access_key_id_, access_key_secret_, security_token_,  conf);

  size_t key = getRandomNumber();
  client_map_[key] = std::make_tuple(client, progress_callback, outcome_callback);

  OSSBaseLoadTask* task = nullptr;
  switch (type) {
    case LoadType::DOWNLOAD:
      task = new OSSDownloadTask(key, this, bucket_name_, client, object_name,
                                 local_file_name, check_point_path);
      break;
    case LoadType::UPLOAD:
      task = new OSSUploadTask(key, this, bucket_name_, client, object_name, file_name,
                               local_file_name, check_point_path);
      break;
  }
  task->setAutoDelete(true);
  QThreadPool::globalInstance()->start(task);
  return key;
}

OSSEntity& OSSEntity::instance() {
  static OSSEntity entity;
  return entity;
}

bool OSSEntity::initOSSConfigure(const std::string& access_key_id,
                                 const std::string& access_key_secret,
                                 const std::string& end_point,
                                const std::string& security_token,
                                 std::string& bucket_name) {
  if (access_key_id == "" || access_key_secret == "" || end_point == "" ||
      bucket_name == "" || security_token == "") {
    return false;
  }
  end_point_ = end_point;
  bucket_name_ = bucket_name;
  access_key_id_ = access_key_id;
  access_key_secret_ = access_key_secret;
  security_token_ = security_token;

  AlibabaCloud::OSS::InitializeSdk();

  is_inited_ = true;
  return is_inited_;
}

size_t OSSEntity::downloadObject(const std::string& object_name,
                                 const std::string& local_file_name,
                                 const std::string& check_point_path,
                                 const ProgressCallback& progress_callback,
                                 const TaskResultCallback& outcome_callback) {
  if (!is_inited()) {
    return 0;
  }
  return instance().createLoadTask(object_name, local_file_name, local_file_name, 
                                   check_point_path, LoadType::DOWNLOAD,
                                   progress_callback, outcome_callback);
}

size_t OSSEntity::uploadObject(const std::string& object_name,
                               const std::string& file_name,
                               const std::string& local_file_name,
                               const std::string& check_point_path,
                               const ProgressCallback& progress_callback,
                               const TaskResultCallback& outcome_callback) {
  if (!is_inited()) {
    return 0;
  }
  return instance().createLoadTask(object_name, file_name, local_file_name,
                                   check_point_path, LoadType::UPLOAD,
                                   progress_callback, outcome_callback);
}

void OSSEntity::cancleLoadTask(const size_t key) {
  if (client_map_.contains(key)) {
    (std::get<0>(client_map_[key]))->DisableRequest();
  }
}
void OSSEntity::deleteObjects(const std::list<std::string>& object_names,
                              const TaskResultCallback& outcome_callback) {
  if (!is_inited()) {
    return;
  }
  AlibabaCloud::OSS::ClientConfiguration conf;
  AlibabaCloud::OSS::OssClient* client = new AlibabaCloud::OSS::OssClient(
      end_point_, access_key_id_, access_key_secret_, security_token_, conf);

  size_t key = getRandomNumber();
  client_map_[key] = std::make_tuple(client, nullptr, outcome_callback);
  OSSDeleteTask* task =
      new OSSDeleteTask(key, this, bucket_name_, client, object_names);
  QThreadPool::globalInstance()->start(task);
}

std::string OSSEntity::generateOSSUrl(const std::string& path,
                                      const std::string& object_name) {
  return "https://" + bucket_name_ + "." + end_point_ + "/" + path +
         object_name;
}

OSSBaseEntity::~OSSBaseEntity() {}
}  // namespace oss
