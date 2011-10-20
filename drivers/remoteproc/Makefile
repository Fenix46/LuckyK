#
# Generic framework for controlling remote processors
#

obj-$(CONFIG_REMOTEPROC)		+= remoteproc.o
remoteproc-y				:= remoteproc_core.o
obj-$(CONFIG_OMAP_REMOTE_PROC)		+= omap_remoteproc.o
obj-$(CONFIG_OMAP_RPRES)		+= rpres.o rpres_dev.o
remoteproc-y				+= remoteproc_debugfs.o
remoteproc-y				+= remoteproc_rpmsg.o
