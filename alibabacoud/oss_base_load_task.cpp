#include "oss_base_load_task.h"
#include "oss_entity.h"

namespace oss_wrapper{
OSSBaseLoadTask::OSSBaseLoadTask(size_t key,
                                 OSSBaseEntity* entity,
                                 const std::string& bucket_name,
                                 AlibabaCloud::OSS::OssClient* client)
    : OSSBaseTask(key, entity, bucket_name, client) {}

OSSBaseLoadTask::~OSSBaseLoadTask() {}

void OSSBaseLoadTask::progressCallback(size_t /*increment*/,
                                       int64_t transfered,
                                       int64_t total,
                                       void* /*user_data*/) {
    entity_->onProgressChanged(key_, transfered, total);
}
}  // namespace oss
