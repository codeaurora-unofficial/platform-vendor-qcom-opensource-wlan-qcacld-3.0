/*
 * Copyright (c) 2012, 2014-2015 The Linux Foundation. All rights reserved.
 *
 * Previously licensed under the ISC license by Qualcomm Atheros, Inc.
 *
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * This file was originally distributed by Qualcomm Atheros, Inc.
 * under proprietary terms before Copyright ownership was assigned
 * to the Linux Foundation.
 */

/**
 * @file ol_txrx_status.h
 * @brief Functions provided for visibility and debugging.
 * NOTE: This file is used by both kernel driver SW and userspace SW.
 * Thus, do not reference use any kernel header files or defs in this file!
 */
#ifndef _OL_TXRX_STATS__H_
#define _OL_TXRX_STATS__H_

#include <athdefs.h>            /* uint64_t */



struct ol_txrx_stats_elem {
	uint64_t pkts;
	uint64_t bytes;
};

#define NUM_MAX_TSO_SEGS 8
#define NUM_MAX_TSO_SEGS_MASK (NUM_MAX_TSO_SEGS - 1)

#define NUM_MAX_TSO_MSDUS 128
#define NUM_MAX_TSO_MSDUS_MASK (NUM_MAX_TSO_MSDUS - 1)

struct ol_txrx_stats_tso_msdu {
	struct cdf_tso_seg_t tso_segs[NUM_MAX_TSO_SEGS];
	uint32_t num_seg;
	uint32_t tso_seg_idx;
};

struct ol_txrx_stats_tso_info {
	struct ol_txrx_stats_tso_msdu tso_msdu_info[NUM_MAX_TSO_MSDUS];
	uint32_t tso_msdu_idx;
};

/**
 * @brief data stats published by the host txrx layer
 */
struct ol_txrx_stats {
	struct {
		/* MSDUs given to the txrx layer by the management stack */
		struct ol_txrx_stats_elem mgmt;
		/* MSDUs successfully sent across the WLAN */
		struct ol_txrx_stats_elem delivered;
		struct {
			/* MSDUs that the host did not accept */
			struct ol_txrx_stats_elem host_reject;
			/* MSDUs which could not be downloaded to the target */
			struct ol_txrx_stats_elem download_fail;
			/* MSDUs which the target discarded
			   (lack of memory or old age) */
			struct ol_txrx_stats_elem target_discard;
			/* MSDUs which the target sent but
			   couldn't get an ack for */
			struct ol_txrx_stats_elem no_ack;
		} dropped;
		/* contains information of packets recevied per tx completion*/
		struct {
			uint32_t pkts_1;
			uint32_t pkts_2_10;
			uint32_t pkts_11_20;
			uint32_t pkts_21_30;
			uint32_t pkts_31_40;
			uint32_t pkts_41_50;
			uint32_t pkts_51_60;
			uint32_t pkts_61_plus;
		} comp_histogram;
		/* TSO (TCP segmentation offload) information */
		struct {
			struct ol_txrx_stats_elem tso_pkts;
#if defined(FEATURE_TSO)
			struct ol_txrx_stats_tso_info tso_info;
#endif
		} tso;
	} tx;
	struct {
		/* MSDUs given to the OS shim */
		struct ol_txrx_stats_elem delivered;
		struct {
			/* MSDUs forwarded to network stack */
			u_int32_t packets_stack;
			/* MSDUs forwarded from the rx path to the tx path */
			u_int32_t packets_fwd;
			/* MSDUs forwarded to stack and tx path */
			u_int32_t packets_stack_n_fwd;
		} intra_bss_fwd;
	} rx;
};

/*
 * Structure to consolidate host stats
 */
struct ieee80211req_ol_ath_host_stats {
	struct ol_txrx_stats txrx_stats;
	struct {
		int pkt_q_fail_count;
		int pkt_q_empty_count;
		int send_q_empty_count;
	} htc;
	struct {
		int pipe_no_resrc_count;
		int ce_ring_delta_fail_count;
	} hif;
};

#endif /* _OL_TXRX_STATS__H_ */
