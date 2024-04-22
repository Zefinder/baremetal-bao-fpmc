# SPDX-License-Identifier: GPL-2.0
# Copyright (c) Bao Project and Contributors. All rights reserved.

# Prepare enviroment for baremetal-runtime
NAME:=baremetal
ROOT_DIR:=$(realpath .)
BUILD_DIR:=$(ROOT_DIR)/build/$(PLATFORM)

# Setup baremetal-runtime build
include $(ROOT_DIR)/setup.mk

app_src_dir:=$(ROOT_DIR)/src
include $(app_src_dir)/sources.mk
C_SRC+=$(addprefix $(app_src_dir)/, $(src_c_srcs))
ASM_SRC+=$(addprefix $(app_src_dir)/, $(src_s_srcs))

ifeq ($(SELECTED_MAIN),)
	$(error Application not selected, you must choose an application)
endif
task_src_dir:=$(app_src_dir)/$(SELECTED_MAIN)
include $(task_src_dir)/sources.mk
C_SRC+=$(addprefix $(task_src_dir)/, $(task_c_srcs))
ASM_SRC+=$(addprefix $(task_src_dir)/, $(task_s_srcs))

# Include the final baremetal-runtime build
include $(ROOT_DIR)/build.mk
