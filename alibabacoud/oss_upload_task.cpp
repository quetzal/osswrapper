#include "oss_upload_task.h"
#include "oss_entity.h"
#include "alibabacloud/oss/model/ObjectMetaData.h"
#include <fstream>

namespace oss_wrapper{
OSSUploadTask::OSSUploadTask(const size_t key,
                             OSSBaseEntity* entity,
                             const std::string& bucket_name,
                             AlibabaCloud::OSS::OssClient* client,
                             const std::string& object_name,
                             const std::string& file_name,
                             const std::string& local_file_name,
                             const std::string& check_point_path)
    : OSSBaseLoadTask(key, entity, bucket_name, client),
      object_name_(object_name),
      file_name_(file_name),
      local_file_name_(local_file_name),
      check_point_path_(check_point_path) {}

OSSUploadTask::~OSSUploadTask() {}

static int64_t getFileSize(const std::string& file)
{
    std::fstream f(file, std::ios::in | std::ios::binary);
    f.seekg(0, f.end);
    int64_t size = f.tellg();
    f.close();
    return size;
}

void OSSUploadTask::run() {
  auto call_back = std::bind(&OSSUploadTask::progressCallback, this,
                             std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3, std::placeholders::_4);
  OSSError error;

  AlibabaCloud::OSS::InitiateMultipartUploadRequest initUploadRequest(bucket_name_, object_name_);
  auto uploadIdResult = client_->InitiateMultipartUpload(initUploadRequest);
  auto uploadId = uploadIdResult.result().UploadId();
  std::string fileToUpload = local_file_name_;
  int64_t partSize = 100 * 1024;
  AlibabaCloud::OSS::PartList partETagList;

  auto fileSize = getFileSize(fileToUpload);
  int partCount = static_cast<int>(fileSize / partSize);
  // Calculate how many parts to be divided
  if (fileSize % partSize != 0) {
      partCount++;
  }

  // Upload multiparts to bucket
  for (int i = 1; i <= partCount; i++) {
      auto skipBytes = partSize * (i - 1);
      auto size = (partSize < fileSize - skipBytes) ? partSize : (fileSize - skipBytes);
      std::shared_ptr<std::iostream> content = std::make_shared<std::fstream>(fileToUpload, std::ios::in | std::ios::binary);
      content->seekg(skipBytes, std::ios::beg);

      AlibabaCloud::OSS::UploadPartRequest uploadPartRequest(bucket_name_, object_name_, content);
      uploadPartRequest.setContentLength(size);
      uploadPartRequest.setUploadId(uploadId);
      uploadPartRequest.setPartNumber(i);
      auto uploadPartOutcome = client_->UploadPart(uploadPartRequest);
      if (uploadPartOutcome.isSuccess()) {
          AlibabaCloud::OSS::Part part(i, uploadPartOutcome.result().ETag());
          partETagList.push_back(part);
          progressCallback(key_, i, partCount, nullptr);
      }
      else {
          error = OSSError(uploadPartOutcome.error().Code(), uploadPartOutcome.error().Message());
          entity_->onProgressFinished(key_, error);
          return;
      }
  }

  // Complete to upload multiparts
  AlibabaCloud::OSS::CompleteMultipartUploadRequest request(bucket_name_, object_name_);
  request.setUploadId(uploadId);
  request.setPartList(partETagList);
  auto outcome = client_->CompleteMultipartUpload(request);
  if (outcome.isSuccess()) {
        error.code_ = "200";
        error.message_ = "success";
  }
  else {
        error = OSSError(outcome.error().Code(), outcome.error().Message());
  }
  entity_->onProgressFinished(key_, error);
}

}  // namespace oss
