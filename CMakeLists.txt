__tree_add_current(SRC)

set(DEFS OSS_WRAPPER_DLL)
__find_alibabacloud()
__add_real_target(ossWrapper dll SOURCE ${SRC} DEF ${DEFS} LIB Qt5::Core AliyunOss)




