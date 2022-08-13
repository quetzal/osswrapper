#include "oss_base_task.h"
#include "oss_entity.h"

namespace oss_wrapper{
OSSBaseTask::OSSBaseTask(size_t key,
                         OSSBaseEntity* entity,
                         const std::string& bucket_name,
                         AlibabaCloud::OSS::OssClient* client)
    : key_(key), entity_(entity), bucket_name_(bucket_name), client_(client) {}

OSSBaseTask::~OSSBaseTask() {}
}  // namespace oss
