/*
 * omap iommu: omap device registration
 *
 * Copyright (C) 2008-2009 Nokia Corporation
 *
 * Written by Hiroshi DOYU <Hiroshi.DOYU@nokia.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/platform_device.h>
#include <linux/err.h>

#include <plat/iommu.h>
#include <plat/omap_device.h>
#include <plat/omap_hwmod.h>

static int __init omap_iommu_dev_init(struct omap_hwmod *oh, void *unused)
{
	struct platform_device *pdev;
	struct resource res[2];
	struct iommu_platform_data pdata;
	struct omap_mmu_dev_attr *a = (struct omap_mmu_dev_attr *)oh->dev_attr;
	static int i;

	pdata.name = oh->name;
	pdata.clk_name = oh->main_clk;
	pdata.nr_tlb_entries = a->nr_tlb_entries;
	pdata.da_start = a->da_start;
	pdata.da_end = a->da_end;

	pdev = omap_device_build("omap-iommu", i, oh, &pdata, sizeof(pdata),
				NULL, 0, 0);
	if (IS_ERR(pdev)) {
		pr_err("%s: device build error: %ld\n",
				__func__, PTR_ERR(pdev));
		return PTR_ERR(pdev);
	}

	i++;

	return 0;
};
static struct iommu_platform_data *devices_data;
static int num_iommu_devices;

#ifdef CONFIG_ARCH_OMAP3
static struct iommu_platform_data omap3_devices_data[] = {
	{
		.name = "isp",
		.oh_name = "isp",
		.nr_tlb_entries = 8,
		.da_start = 0x0,
		.da_end = 0xFFFFF000,
	},
#if defined(CONFIG_OMAP_IOMMU_IVA2)
	{
		.name = "iva2",
		.oh_name = "dsp",
		.nr_tlb_entries = 32,
		.da_start = 0x11000000,
		.da_end = 0xFFFFF000,
	},
#endif
};
#define NR_OMAP3_IOMMU_DEVICES ARRAY_SIZE(omap3_devices_data)
#else
#define omap3_devices_data	NULL
#define NR_OMAP3_IOMMU_DEVICES	0
#endif

#ifdef CONFIG_ARCH_OMAP4

#define SET_DSP_CONSTRAINT	400
#define SET_MPU_CORE_CONSTRAINT	400

static struct iommu_platform_data omap4_devices_data[] = {
	{
		.name = "ducati",
		.oh_name = "ipu",
		.nr_tlb_entries = 32,
		.da_start = 0x0,
		.da_end = 0xFFFFF000,
		.pm_constraint = SET_MPU_CORE_CONSTRAINT,
	},
	{
		.name = "tesla",
		.oh_name = "dsp",
		.nr_tlb_entries = 32,
		.da_start = 0x0,
		.da_end = 0xFFFFF000,
		.pm_constraint = SET_DSP_CONSTRAINT,
	},
};
#define NR_OMAP4_IOMMU_DEVICES ARRAY_SIZE(omap4_devices_data)
#else
#define omap4_devices_data	NULL
#define NR_OMAP4_IOMMU_DEVICES	0
#endif

static struct omap_device_pm_latency omap_iommu_latency[] = {
	[0] = {
		.deactivate_func = omap_device_idle_hwmods,
		.activate_func	 = omap_device_enable_hwmods,
		.flags = OMAP_DEVICE_LATENCY_AUTO_ADJUST,
	},
};

int iommu_get_plat_data_size(void)
{
	return num_iommu_devices;
}
EXPORT_SYMBOL(iommu_get_plat_data_size);

struct iommu_platform_data *iommu_get_device_data(void)
{
	return devices_data;
}

static int __init omap_iommu_init(void)
{

	return omap_hwmod_for_each_by_class("mmu", omap_iommu_dev_init, NULL);
}
subsys_initcall(omap_iommu_init);

MODULE_AUTHOR("Hiroshi DOYU");
MODULE_AUTHOR("Hari Kanigeri");
MODULE_DESCRIPTION("omap iommu: omap device registration");
MODULE_LICENSE("GPL v2");
