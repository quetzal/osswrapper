__tree_add_current(SRC)

set(DEFS OSS_WRAPPER_DLL)

__cc_find(AliyunOss)

if(NOT TARGET Qt5::Core)
	__prepare_core_qt5()
endif()

__add_real_target(ossWrapper dll SOURCE ${SRC}
							 DEF ${DEFS} 
							 LIB Qt5::Core AliyunOss
							 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/alibabacoud/
							 )

if(CC_INSTALL_OSSWRAPPER)
	__install_directory_specif(${CMAKE_CURRENT_SOURCE_DIR}/alibabacoud/ osswrapper)
endif()


