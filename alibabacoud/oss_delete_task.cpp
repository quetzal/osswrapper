#include "oss_delete_task.h"
#include "oss_entity.h"

namespace oss_wrapper{
OSSDeleteTask::OSSDeleteTask(const size_t key,
                             OSSBaseEntity* entity,
                             const std::string& bucket_name,
                             AlibabaCloud::OSS::OssClient* client,
                             const std::list<std::string>& object_names)
    : OSSBaseTask(key, entity, bucket_name, client),
      object_names_(object_names) {}

OSSDeleteTask::~OSSDeleteTask() {}

void OSSDeleteTask::run() {
  AlibabaCloud::OSS::DeleteObjectsRequest request(bucket_name_);
  for (const auto& object_name : object_names_) {
    request.addKey(object_name);
  }
  auto outcome = client_->DeleteObjects(request);

  OSSError error;
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