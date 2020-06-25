/*
 * Copyright (c) 2011-2020 The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * DOC: wlan_hdd_sysfs_dp_trace.c
 *
 * implementation for creating sysfs files:
 *
 * set_dp_trace
 * dump_dp_trace
 * clear_dp_trace
 */

#include <wlan_hdd_includes.h>
#include "osif_psoc_sync.h"
#include <wlan_hdd_sysfs.h>
#include <wlan_hdd_sysfs_dp_trace.h>
#include "qdf_trace.h"

static ssize_t
__hdd_sysfs_set_dp_trace_store(struct hdd_context *hdd_ctx,
			       struct kobj_attribute *attr,
			       const char *buf,
			       size_t count)
{
	char buf_local[MAX_SYSFS_USER_COMMAND_SIZE_LENGTH + 1];
	char *sptr, *token;
	uint32_t val1;
	uint8_t val2, val3;
	int ret;

	if (!wlan_hdd_validate_modules_state(hdd_ctx))
		return -EINVAL;

	ret = hdd_sysfs_validate_and_copy_buf(buf_local, sizeof(buf_local),
					      buf, count);

	if (ret) {
		hdd_err_rl("invalid input");
		return ret;
	}

	sptr = buf_local;
	hdd_debug("set_dp_trace: count %zu buf_local:(%s)",
		  count, buf_local);

	/* Get val1 */
	token = strsep(&sptr, " ");
	if (!token)
		return -EINVAL;
	if (kstrtou32(token, 0, &val1))
		return -EINVAL;

	/* Get val2 */
	token = strsep(&sptr, " ");
	if (!token)
		return -EINVAL;
	if (kstrtou8(token, 0, &val2))
		return -EINVAL;

	/* Get val3 */
	token = strsep(&sptr, " ");
	if (!token)
		return -EINVAL;
	if (kstrtou8(token, 0, &val3))
		return -EINVAL;

	qdf_dp_trace_set_value(val1, val2, val3);

	return count;
}

static ssize_t
hdd_sysfs_set_dp_trace_store(struct kobject *kobj,
			     struct kobj_attribute *attr,
			     const char *buf,
			     size_t count)
{
	struct osif_psoc_sync *psoc_sync;
	struct hdd_context *hdd_ctx = cds_get_context(QDF_MODULE_ID_HDD);
	ssize_t errno_size;
	int ret;

	ret = wlan_hdd_validate_context(hdd_ctx);
	if (ret != 0)
		return ret;

	errno_size = osif_psoc_sync_op_start(wiphy_dev(hdd_ctx->wiphy),
					     &psoc_sync);
	if (errno_size)
		return errno_size;

	errno_size = __hdd_sysfs_set_dp_trace_store(hdd_ctx, attr,
						    buf, count);

	osif_psoc_sync_op_stop(psoc_sync);

	return errno_size;
}

static struct kobj_attribute set_dp_trace_attribute =
	__ATTR(set_dp_trace, 0220, NULL,
	       hdd_sysfs_set_dp_trace_store);

static uint32_t dump_dp_trace_count = 0;

static ssize_t
__hdd_sysfs_dump_dp_trace_store(struct hdd_context *hdd_ctx,
				struct kobj_attribute *attr,
				char const *buf, size_t count)
{
	char buf_local[MAX_SYSFS_USER_COMMAND_SIZE_LENGTH + 1];
	char *sptr, *token;
	int value, ret;

	if (!wlan_hdd_validate_modules_state(hdd_ctx))
		return -EINVAL;

	ret = hdd_sysfs_validate_and_copy_buf(buf_local, sizeof(buf_local),
					      buf, count);
	if (ret) {
		hdd_err_rl("invalid input");
		return ret;
	}

	sptr = buf_local;
	token = strsep(&sptr, " ");
	if (!token)
		return -EINVAL;
	if (kstrtou32(token, 0, &value))
		return -EINVAL;

	hdd_debug("dump_dp_trace %d", value);

	switch (value) {
	case HDD_SYSFS_DISABLE_DP_TRACE_LIVE_MODE:
		qdf_dp_trace_disable_live_mode();
		break;
	case HDD_SYSFS_ENABLE_DP_TRACE_LIVE_MODE:
		qdf_dp_trace_enable_live_mode();
		break;
	case HDD_SYSFS_DUMP_DP_TRACE:
		token = strsep(&sptr, " ");
		if (!token)
			return -EINVAL;
		if (kstrtou32(token, 0, &dump_dp_trace_count))
			return -EINVAL;
		break;
	default:
		hdd_err_rl("invalid input");
		return -EINVAL;
	}

	return count;
}

static ssize_t hdd_sysfs_dump_dp_trace_store(struct kobject *kobj,
					     struct kobj_attribute *attr,
					     char const *buf, size_t count)
{
	struct osif_psoc_sync *psoc_sync;
	struct hdd_context *hdd_ctx = cds_get_context(QDF_MODULE_ID_HDD);
	ssize_t errno_size;
	int ret;

	ret = wlan_hdd_validate_context(hdd_ctx);
	if (ret != 0)
		return ret;

	errno_size = osif_psoc_sync_op_start(wiphy_dev(hdd_ctx->wiphy),
					     &psoc_sync);
	if (errno_size)
		return errno_size;

	errno_size = __hdd_sysfs_dump_dp_trace_store(hdd_ctx, attr,
						     buf, count);

	osif_psoc_sync_op_stop(psoc_sync);

	return errno_size;
}

static ssize_t
__hdd_sysfs_dump_dp_trace_show(struct hdd_context *hdd_ctx,
			       struct kobj_attribute *attr, char *buf)
{
	if (!wlan_hdd_validate_modules_state(hdd_ctx))
		return -EINVAL;

	qdf_dp_trace_dump_all(dump_dp_trace_count, QDF_TRACE_DEFAULT_PDEV_ID);

	return 0;
}

static ssize_t hdd_sysfs_dump_dp_trace_show(struct kobject *kobj,
					    struct kobj_attribute *attr,
					    char *buf)
{
	struct osif_psoc_sync *psoc_sync;
	struct hdd_context *hdd_ctx = cds_get_context(QDF_MODULE_ID_HDD);
	ssize_t errno_size;
	int ret;

	ret = wlan_hdd_validate_context(hdd_ctx);
	if (ret != 0)
		return ret;

	errno_size = osif_psoc_sync_op_start(wiphy_dev(hdd_ctx->wiphy),
					     &psoc_sync);
	if (errno_size)
		return errno_size;

	errno_size = __hdd_sysfs_dump_dp_trace_show(hdd_ctx, attr, buf);

	osif_psoc_sync_op_stop(psoc_sync);

	return errno_size;
}

static struct kobj_attribute dump_dp_trace_attribute =
	__ATTR(dump_dp_trace, 0660, hdd_sysfs_dump_dp_trace_show,
	       hdd_sysfs_dump_dp_trace_store);

static ssize_t
__hdd_sysfs_clear_dp_trace_store(struct hdd_context *hdd_ctx,
				 struct kobj_attribute *attr,
				 char const *buf, size_t count)
{
	char buf_local[MAX_SYSFS_USER_COMMAND_SIZE_LENGTH + 1];
	char *sptr, *token;
	int value, ret;

	if (!wlan_hdd_validate_modules_state(hdd_ctx))
		return -EINVAL;

	ret = hdd_sysfs_validate_and_copy_buf(buf_local, sizeof(buf_local),
					      buf, count);
	if (ret) {
		hdd_err_rl("invalid input");
		return ret;
	}

	sptr = buf_local;
	token = strsep(&sptr, " ");
	if (!token)
		return -EINVAL;
	if (kstrtou32(token, 0, &value))
		return -EINVAL;

	hdd_debug("clear_dp_trace %d", value);

	qdf_dp_trace_clear_buffer();

	return count;
}

static ssize_t
hdd_sysfs_clear_dp_trace_store(struct kobject *kobj,
			       struct kobj_attribute *attr,
			       char const *buf, size_t count)
{
	struct osif_psoc_sync *psoc_sync;
	struct hdd_context *hdd_ctx = cds_get_context(QDF_MODULE_ID_HDD);
	ssize_t errno_size;
	int ret;

	ret = wlan_hdd_validate_context(hdd_ctx);
	if (ret != 0)
		return ret;

	errno_size = osif_psoc_sync_op_start(wiphy_dev(hdd_ctx->wiphy),
					     &psoc_sync);
	if (errno_size)
		return errno_size;

	errno_size = __hdd_sysfs_clear_dp_trace_store(hdd_ctx, attr,
						      buf, count);

	osif_psoc_sync_op_stop(psoc_sync);

	return errno_size;
}

static struct kobj_attribute clear_dp_trace_attribute =
	__ATTR(clear_dp_trace, 0220, NULL,
	       hdd_sysfs_clear_dp_trace_store);

int hdd_sysfs_dp_trace_create(struct kobject *driver_kobject)
{
	int error;

	if (!driver_kobject) {
		hdd_err("could not get driver kobject!");
		return -EINVAL;
	}

	error = sysfs_create_file(driver_kobject,
				  &set_dp_trace_attribute.attr);
	if (error)
		hdd_err("could not create set_dp_trace sysfs file");

	error = sysfs_create_file(driver_kobject,
				  &dump_dp_trace_attribute.attr);
	if (error)
		hdd_err("could not create dump_dp_trace sysfs file");

	error = sysfs_create_file(driver_kobject,
				  &clear_dp_trace_attribute.attr);
	if (error)
		hdd_err("could not create clear_dp_trace sysfs file");

	return error;
}

void
hdd_sysfs_dp_trace_destroy(struct kobject *driver_kobject)
{
	if (!driver_kobject) {
		hdd_err("could not get driver kobject!");
		return;
	}
	sysfs_remove_file(driver_kobject, &clear_dp_trace_attribute.attr);
	sysfs_remove_file(driver_kobject, &dump_dp_trace_attribute.attr);
	sysfs_remove_file(driver_kobject, &set_dp_trace_attribute.attr);
}
