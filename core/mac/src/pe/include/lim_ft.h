/*
 * Copyright (c) 2011-2014 The Linux Foundation. All rights reserved.
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

#if defined WLAN_FEATURE_VOWIFI_11R
/**=========================================================================

   Macros and Function prototypes FT and 802.11R purposes

   ========================================================================*/

#ifndef __LIMFT_H__
#define __LIMFT_H__

#include <cds_api.h>
#include <lim_global.h>
#include <ani_global.h>
#include <lim_debug.h>
#include <lim_ser_des_utils.h>

/*-------------------------------------------------------------------------
   Function declarations and documenation
   ------------------------------------------------------------------------*/
void lim_ft_open(tpAniSirGlobal pMac, tpPESession psessionEntry);
void lim_ft_cleanup(tpAniSirGlobal pMac, tpPESession psessionEntry);
void lim_ft_cleanup_pre_auth_info(tpAniSirGlobal pMac, tpPESession psessionEntry);
int lim_process_ft_pre_auth_req(tpAniSirGlobal pMac, tpSirMsgQ pMsg);
void lim_perform_ft_pre_auth(tpAniSirGlobal pMac, CDF_STATUS status,
			     uint32_t *data, tpPESession psessionEntry);
void limPerformPostFTPreAuth(tpAniSirGlobal pMac, CDF_STATUS status,
			     uint32_t *data, tpPESession psessionEntry);
void limFTResumeLinkCb(tpAniSirGlobal pMac, CDF_STATUS status, uint32_t *data);
void lim_post_ft_pre_auth_rsp(tpAniSirGlobal pMac, tSirRetStatus status,
			      uint8_t *auth_rsp, uint16_t auth_rsp_length,
			      tpPESession psessionEntry);
void lim_handle_ft_pre_auth_rsp(tpAniSirGlobal pMac, tSirRetStatus status,
				uint8_t *auth_rsp, uint16_t auth_rsp_len,
				tpPESession psessionEntry);
void lim_process_mlm_ft_reassoc_req(tpAniSirGlobal pMac, uint32_t *pMsgBuf,
				    tpPESession psessionEntry);
void lim_process_ft_preauth_rsp_timeout(tpAniSirGlobal pMac);

bool lim_process_ft_update_key(tpAniSirGlobal pMac, uint32_t *pMsgBuf);
tSirRetStatus lim_process_ft_aggr_qos_req(tpAniSirGlobal pMac, uint32_t *pMsgBuf);
void lim_process_ft_aggr_qo_s_rsp(tpAniSirGlobal pMac, tpSirMsgQ limMsg);
void lim_ft_cleanup_all_ft_sessions(tpAniSirGlobal pMac);
void lim_fill_ft_session(tpAniSirGlobal pMac,
		tpSirBssDescription pbssDescription,
		tpPESession pftSessionEntry,
		tpPESession psessionEntry);
tSirRetStatus lim_ft_prepare_add_bss_req(tpAniSirGlobal pMac,
		uint8_t updateEntry,
		tpPESession pftSessionEntry,
		tpSirBssDescription bssDescription);
#endif /* __LIMFT_H__ */

#endif /* WLAN_FEATURE_VOWIFI_11R */
