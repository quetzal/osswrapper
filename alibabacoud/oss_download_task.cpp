#include "oss_download_task.h"
#include "oss_entity.h"

namespace oss_wrapper{
OSSDownloadTask::OSSDownloadTask(const size_t key,
                                 OSSBaseEntity* entity,
                                 const std::string& bucket_name,
                                 AlibabaCloud::OSS::OssClient* client,
                                 const std::string& object_name,
                                 const std::string& local_file_name,
                                 const std::string& check_point_path)
    : OSSBaseLoadTask(key, entity, bucket_name, client),
      object_name_(object_name),
      local_file_name_(local_file_name),
      check_point_path_(check_point_path) {}

OSSDownloadTask::~OSSDownloadTask() {}

void OSSDownloadTask::run() {
  auto call_back = std::bind(&OSSDownloadTask::progressCallback, this,
                             std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3, std::placeholders::_4);
  AlibabaCloud::OSS::TransferProgress progress_callback = {call_back, nullptr};

  AlibabaCloud::OSS::DownloadObjectRequest request(bucket_name_, object_name_,
                                                   local_file_name_);

  request.setTransferProgress(progress_callback);
  request.setPartSize(100 * 1024);
  request.setThreadNum(4);
  request.setCheckpointDir(check_point_path_);

  OSSError error;
  auto outcome = client_->ResumableDownloadObject(request);
  if (!outcome.isSuccess()) {
    error = OSSError(outcome.error().Code(), outcome.error().Message());
  } else {
    error.code_ = "200";
    error.message_ = "success";
  }
  error.key_ = key_;
  entity_->onProgressFinished(key_, error);
}
}  // namespace oss
