ifeq ($(LEGACY),y)
task_c_srcs:= task.c
else
task_c_srcs:= task_prem.c
endif