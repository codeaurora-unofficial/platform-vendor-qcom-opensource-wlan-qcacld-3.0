/*
 * Copyright (c) 2011-2015 The Linux Foundation. All rights reserved.
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

/**=========================================================================

   \file  lim_session.c

   \brief implementation for lim Session related APIs

   \author Sunit Bhatia

   ========================================================================*/

/*--------------------------------------------------------------------------
   Include Files
   ------------------------------------------------------------------------*/
#include "ani_global.h"
#include "lim_debug.h"
#ifdef WLAN_FEATURE_VOWIFI_11R
#include "lim_ft_defs.h"
#include "lim_ft.h"
#endif
#include "lim_session.h"
#include "lim_utils.h"

#include "sch_api.h"
#include "lim_send_messages.h"

/*--------------------------------------------------------------------------

   \brief pe_init_beacon_params() - Initialize the beaconParams structure

   \param tpPESession          - pointer to the session context or NULL if session can not be created.
   \return void
   \sa

   --------------------------------------------------------------------------*/

void pe_init_beacon_params(tpAniSirGlobal pMac, tpPESession psessionEntry)
{
	psessionEntry->beaconParams.beaconInterval = 0;
	psessionEntry->beaconParams.fShortPreamble = 0;
	psessionEntry->beaconParams.llaCoexist = 0;
	psessionEntry->beaconParams.llbCoexist = 0;
	psessionEntry->beaconParams.llgCoexist = 0;
	psessionEntry->beaconParams.ht20Coexist = 0;
	psessionEntry->beaconParams.llnNonGFCoexist = 0;
	psessionEntry->beaconParams.fRIFSMode = 0;
	psessionEntry->beaconParams.fLsigTXOPProtectionFullSupport = 0;
	psessionEntry->beaconParams.gHTObssMode = 0;

	/* Number of legacy STAs associated */
	cdf_mem_set((void *)&psessionEntry->gLim11bParams,
		    sizeof(tLimProtStaParams), 0);
	cdf_mem_set((void *)&psessionEntry->gLim11aParams,
		    sizeof(tLimProtStaParams), 0);
	cdf_mem_set((void *)&psessionEntry->gLim11gParams,
		    sizeof(tLimProtStaParams), 0);
	cdf_mem_set((void *)&psessionEntry->gLimNonGfParams,
		    sizeof(tLimProtStaParams), 0);
	cdf_mem_set((void *)&psessionEntry->gLimHt20Params,
		    sizeof(tLimProtStaParams), 0);
	cdf_mem_set((void *)&psessionEntry->gLimLsigTxopParams,
		    sizeof(tLimProtStaParams), 0);
	cdf_mem_set((void *)&psessionEntry->gLimOlbcParams,
		    sizeof(tLimProtStaParams), 0);
}

/*
 * pe_reset_protection_callback() - resets protection structs so that when an AP
 * causing use of protection goes away, corresponding protection bit can be
 * reset
 * @ptr:        pointer to pSessionEntry
 *
 * This function resets protection structs so that when an AP causing use of
 * protection goes away, corresponding protection bit can be reset. This allowes
 * protection bits to be reset once legacy overlapping APs are gone.
 *
 * Return: void
 */
void pe_reset_protection_callback(void *ptr)
{
	tpPESession pe_session_entry = (tpPESession)ptr;
	tpAniSirGlobal mac_ctx = (tpAniSirGlobal)pe_session_entry->mac_ctx;
	int8_t i = 0;
	tUpdateBeaconParams beacon_params;
	uint16_t current_protection_state = 0;
	tpDphHashNode station_hash_node = NULL;
	tSirMacHTOperatingMode old_op_mode;
	bool bcn_prms_changed = false;

	if (pe_session_entry->valid == false) {
		CDF_TRACE(CDF_MODULE_ID_PE,
			  CDF_TRACE_LEVEL_ERROR,
			  FL("session already deleted. exiting timer callback"));
		return;
	}

	current_protection_state |=
	       pe_session_entry->gLimOverlap11gParams.protectionEnabled        |
	       pe_session_entry->gLimOverlap11aParams.protectionEnabled   << 1 |
	       pe_session_entry->gLimOverlapHt20Params.protectionEnabled  << 2 |
	       pe_session_entry->gLimOverlapNonGfParams.protectionEnabled << 3 |
	       pe_session_entry->gLimOlbcParams.protectionEnabled         << 4;

	CDF_TRACE(CDF_MODULE_ID_PE,
		  CDF_TRACE_LEVEL_INFO,
		  FL("old protection state: 0x%04X, new protection state: 0x%04X\n"),
		  pe_session_entry->old_protection_state,
		  current_protection_state);

	cdf_mem_zero(&pe_session_entry->gLimOverlap11gParams,
		     sizeof(pe_session_entry->gLimOverlap11gParams));
	cdf_mem_zero(&pe_session_entry->gLimOverlap11aParams,
		     sizeof(pe_session_entry->gLimOverlap11aParams));
	cdf_mem_zero(&pe_session_entry->gLimOverlapHt20Params,
		     sizeof(pe_session_entry->gLimOverlapHt20Params));
	cdf_mem_zero(&pe_session_entry->gLimOverlapNonGfParams,
		     sizeof(pe_session_entry->gLimOverlapNonGfParams));

	cdf_mem_zero(&pe_session_entry->gLimOlbcParams,
		     sizeof(pe_session_entry->gLimOlbcParams));

	cdf_mem_zero(&pe_session_entry->beaconParams,
		     sizeof(pe_session_entry->beaconParams));

	cdf_mem_zero(&mac_ctx->lim.gLimOverlap11gParams,
		     sizeof(mac_ctx->lim.gLimOverlap11gParams));
	cdf_mem_zero(&mac_ctx->lim.gLimOverlap11aParams,
		     sizeof(mac_ctx->lim.gLimOverlap11aParams));
	cdf_mem_zero(&mac_ctx->lim.gLimOverlapHt20Params,
		     sizeof(mac_ctx->lim.gLimOverlapHt20Params));
	cdf_mem_zero(&mac_ctx->lim.gLimOverlapNonGfParams,
		     sizeof(mac_ctx->lim.gLimOverlapNonGfParams));

	old_op_mode = pe_session_entry->htOperMode;
	pe_session_entry->htOperMode = eSIR_HT_OP_MODE_PURE;

	cdf_mem_zero(&beacon_params, sizeof(tUpdateBeaconParams));
	/* index 0, is self node, peers start from 1 */
	for (i = 1 ; i <= mac_ctx->lim.gLimAssocStaLimit ; i++)
	{
		station_hash_node = dph_get_hash_entry(mac_ctx, i,
					&pe_session_entry->dph.dphHashTable);
		if (NULL == station_hash_node)
			continue;
		lim_decide_ap_protection(mac_ctx, station_hash_node->staAddr,
		&beacon_params, pe_session_entry);
	}

	if (pe_session_entry->htOperMode != old_op_mode)
		bcn_prms_changed = true;

	if ((current_protection_state !=
		pe_session_entry->old_protection_state) &&
		(false == mac_ctx->sap.SapDfsInfo.is_dfs_cac_timer_running)) {
		CDF_TRACE(CDF_MODULE_ID_PE,
			  CDF_TRACE_LEVEL_ERROR,
			  FL("protection changed, update beacon template\n"));
		/* update beacon fix params and send update to FW */
		cdf_mem_zero(&beacon_params, sizeof(tUpdateBeaconParams));
		beacon_params.bssIdx = pe_session_entry->bssIdx;
		beacon_params.fShortPreamble =
				pe_session_entry->beaconParams.fShortPreamble;
		beacon_params.beaconInterval =
				pe_session_entry->beaconParams.beaconInterval;
		beacon_params.llaCoexist =
				pe_session_entry->beaconParams.llaCoexist;
		beacon_params.llbCoexist =
				pe_session_entry->beaconParams.llbCoexist;
		beacon_params.llgCoexist =
				pe_session_entry->beaconParams.llgCoexist;
		beacon_params.ht20MhzCoexist =
				pe_session_entry->beaconParams.ht20Coexist;
		beacon_params.llnNonGFCoexist =
				pe_session_entry->beaconParams.llnNonGFCoexist;
		beacon_params.fLsigTXOPProtectionFullSupport =
				pe_session_entry->beaconParams.
					fLsigTXOPProtectionFullSupport;
		beacon_params.fRIFSMode =
				pe_session_entry->beaconParams.fRIFSMode;
		beacon_params.smeSessionId =
				pe_session_entry->smeSessionId;
		bcn_prms_changed = true;
	}

	if (bcn_prms_changed) {
		sch_set_fixed_beacon_fields(mac_ctx, pe_session_entry);
		lim_send_beacon_params(mac_ctx, &beacon_params, pe_session_entry);
	}

	pe_session_entry->old_protection_state = current_protection_state;
	if (cdf_mc_timer_start(&pe_session_entry->
				protection_fields_reset_timer,
				SCH_PROTECTION_RESET_TIME)
		!= CDF_STATUS_SUCCESS) {
		CDF_TRACE(CDF_MODULE_ID_PE,
			CDF_TRACE_LEVEL_ERROR,
			FL("cannot create or start protectionFieldsResetTimer\n"));
	}
}

/**
 * pe_create_session() creates a new PE session given the BSSID
 * @param pMac:        pointer to global adapter context
 * @param bssid:       BSSID of the new session
 * @param sessionId:   session ID is returned here, if session is created.
 * @param bssType:     station or a
 *
 * This function returns the session context and the session ID if the session
 * corresponding to the passed BSSID is found in the PE session table.
 *
 * Return: tpPESession:   pointer to the session context or NULL if session
 *                        can not be created.
 */

tpPESession
pe_create_session(tpAniSirGlobal pMac, uint8_t *bssid, uint8_t *sessionId,
		  uint16_t numSta, tSirBssType bssType)
{
	CDF_STATUS status;
	uint8_t i;
	tpPESession session_ptr;
	for (i = 0; i < pMac->lim.maxBssId; i++) {
		/* Find first free room in session table */
		if (pMac->lim.gpSession[i].valid == true)
			continue;
		break;
	}

	if (i == pMac->lim.maxBssId) {
		lim_log(pMac, LOGE,
			FL("Session can't be created. Reached max sessions\n"));
		return NULL;
	}

	session_ptr = &pMac->lim.gpSession[i];
	cdf_mem_set((void *)session_ptr, sizeof(tPESession), 0);
	/* Allocate space for Station Table for this session. */
	session_ptr->dph.dphHashTable.pHashTable =
		cdf_mem_malloc(sizeof(tpDphHashNode) * (numSta + 1));
	if (NULL == session_ptr->dph.dphHashTable.pHashTable) {
		lim_log(pMac, LOGE, FL("memory allocate failed!"));
		return NULL;
	}

	session_ptr->dph.dphHashTable.pDphNodeArray =
		cdf_mem_malloc(sizeof(tDphHashNode) * (numSta + 1));
	if (NULL == session_ptr->dph.dphHashTable.pDphNodeArray) {
		lim_log(pMac, LOGE, FL("memory allocate failed!"));
		cdf_mem_free(session_ptr->dph.dphHashTable.pHashTable);
		session_ptr->dph.dphHashTable.
		pHashTable = NULL;
		return NULL;
	}

	session_ptr->dph.dphHashTable.size = numSta + 1;
	dph_hash_table_class_init(pMac, &session_ptr->dph.dphHashTable);
	session_ptr->gpLimPeerIdxpool = cdf_mem_malloc(
		sizeof(*(session_ptr->gpLimPeerIdxpool)) * (numSta + 1));
	if (NULL == session_ptr->gpLimPeerIdxpool) {
		lim_log(pMac, LOGE, FL("memory allocate failed!"));
		cdf_mem_free(session_ptr->dph.dphHashTable.pHashTable);
		cdf_mem_free(session_ptr->dph.dphHashTable.pDphNodeArray);
		session_ptr->dph.dphHashTable.pHashTable = NULL;
		session_ptr->dph.dphHashTable.pDphNodeArray = NULL;
		return NULL;
	}
	cdf_mem_set(session_ptr->gpLimPeerIdxpool,
		    sizeof(*session_ptr->gpLimPeerIdxpool) * (numSta + 1),
		    0);
	session_ptr->freePeerIdxHead = 0;
	session_ptr->freePeerIdxTail = 0;
	session_ptr->gLimNumOfCurrentSTAs = 0;
	/* Copy the BSSID to the session table */
	sir_copy_mac_addr(session_ptr->bssId, bssid);
	session_ptr->valid = true;
	/* Intialize the SME and MLM states to IDLE */
	session_ptr->limMlmState = eLIM_MLM_IDLE_STATE;
	session_ptr->limSmeState = eLIM_SME_IDLE_STATE;
	session_ptr->limCurrentAuthType = eSIR_OPEN_SYSTEM;
	pe_init_beacon_params(pMac, &pMac->lim.gpSession[i]);
#ifdef WLAN_FEATURE_VOWIFI_11R
	session_ptr->is11Rconnection = false;
#endif
#ifdef FEATURE_WLAN_ESE
	session_ptr->isESEconnection = false;
#endif
#if defined WLAN_FEATURE_VOWIFI_11R || defined FEATURE_WLAN_ESE || defined(FEATURE_WLAN_LFR)
	session_ptr->isFastTransitionEnabled = false;
#endif
#ifdef FEATURE_WLAN_LFR
	session_ptr->isFastRoamIniFeatureEnabled = false;
#endif
	*sessionId = i;
	session_ptr->gLimPhyMode = WNI_CFG_PHY_MODE_11G;
	/* Initialize CB mode variables when session is created */
	session_ptr->htSupportedChannelWidthSet = 0;
	session_ptr->htRecommendedTxWidthSet = 0;
	session_ptr->htSecondaryChannelOffset = 0;
#ifdef FEATURE_WLAN_TDLS
	cdf_mem_set(session_ptr->peerAIDBitmap,
		    sizeof(session_ptr->peerAIDBitmap), 0);
	session_ptr->tdls_prohibited = false;
	session_ptr->tdls_chan_swit_prohibited = false;
#endif
	session_ptr->fWaitForProbeRsp = 0;
	session_ptr->fIgnoreCapsChange = 0;

	CDF_TRACE(CDF_MODULE_ID_PE, CDF_TRACE_LEVEL_DEBUG,
		FL("Create a new PE session(%d), BSSID: "MAC_ADDRESS_STR" Max No. of STA %d"),
		session_ptr->peSessionId, MAC_ADDR_ARRAY(bssid), numSta);

	if (eSIR_INFRA_AP_MODE == bssType
	    || eSIR_IBSS_MODE == bssType
	    || eSIR_BTAMP_AP_MODE == bssType) {
		session_ptr->pSchProbeRspTemplate =
			cdf_mem_malloc(SCH_MAX_PROBE_RESP_SIZE);
		session_ptr->pSchBeaconFrameBegin =
			cdf_mem_malloc(SCH_MAX_BEACON_SIZE);
		session_ptr->pSchBeaconFrameEnd =
			cdf_mem_malloc(SCH_MAX_BEACON_SIZE);
		if ((NULL == session_ptr->pSchProbeRspTemplate)
		    || (NULL == session_ptr->pSchBeaconFrameBegin)
		    || (NULL == session_ptr->pSchBeaconFrameEnd)) {
			lim_log(pMac, LOGE, FL("memory allocate failed!"));
			cdf_mem_free(session_ptr->dph.dphHashTable.pHashTable);
			cdf_mem_free(session_ptr->dph.dphHashTable.pDphNodeArray);
			cdf_mem_free(session_ptr->gpLimPeerIdxpool);
			cdf_mem_free(session_ptr->pSchProbeRspTemplate);
			cdf_mem_free(session_ptr->pSchBeaconFrameBegin);
			cdf_mem_free(session_ptr->pSchBeaconFrameEnd);

			session_ptr->dph.dphHashTable.pHashTable = NULL;
			session_ptr->dph.dphHashTable.pDphNodeArray = NULL;
			session_ptr->gpLimPeerIdxpool = NULL;
			session_ptr->pSchProbeRspTemplate = NULL;
			session_ptr->pSchBeaconFrameBegin = NULL;
			session_ptr->pSchBeaconFrameEnd = NULL;
			return NULL;
		}
	}
#if defined WLAN_FEATURE_VOWIFI_11R
	if (eSIR_INFRASTRUCTURE_MODE == bssType)
		lim_ft_open(pMac, &pMac->lim.gpSession[i]);
#endif
	if (eSIR_INFRA_AP_MODE == bssType) {
		session_ptr->old_protection_state = 0;
		session_ptr->mac_ctx = (void *)pMac;
		status = cdf_mc_timer_init(
			&session_ptr->protection_fields_reset_timer,
			CDF_TIMER_TYPE_SW, pe_reset_protection_callback,
			(void *)&pMac->lim.gpSession[i]);
		if (status == CDF_STATUS_SUCCESS) {
			status = cdf_mc_timer_start(
				&session_ptr->protection_fields_reset_timer,
				SCH_PROTECTION_RESET_TIME);
		}
		if (status != CDF_STATUS_SUCCESS)
			CDF_TRACE(CDF_MODULE_ID_PE, CDF_TRACE_LEVEL_ERROR,
				FL("cannot create or start protectionFieldsResetTimer\n"));
	}
	return &pMac->lim.gpSession[i];
}

/*--------------------------------------------------------------------------
   \brief pe_find_session_by_bssid() - looks up the PE session given the BSSID.

   This function returns the session context and the session ID if the session
   corresponding to the given BSSID is found in the PE session table.

   \param pMac                   - pointer to global adapter context
   \param bssid                   - BSSID of the session
   \param sessionId             -session ID is returned here, if session is found.

   \return tpPESession          - pointer to the session context or NULL if session is not found.

   \sa
   --------------------------------------------------------------------------*/
tpPESession pe_find_session_by_bssid(tpAniSirGlobal pMac, uint8_t *bssid,
				     uint8_t *sessionId)
{
	uint8_t i;

	for (i = 0; i < pMac->lim.maxBssId; i++) {
		/* If BSSID matches return corresponding tables address */
		if ((pMac->lim.gpSession[i].valid)
		    && (sir_compare_mac_addr(pMac->lim.gpSession[i].bssId, bssid)))
		{
			*sessionId = i;
			return (&pMac->lim.gpSession[i]);
		}
	}

	lim_log(pMac, LOG4, FL("Session lookup fails for BSSID: \n "));
	lim_print_mac_addr(pMac, bssid, LOG4);
	return (NULL);

}

/*--------------------------------------------------------------------------
   \brief pe_find_session_by_bss_idx() - looks up the PE session given the bssIdx.

   This function returns the session context  if the session
   corresponding to the given bssIdx is found in the PE session table.
   \param pMac                   - pointer to global adapter context
   \param bssIdx                   - bss index of the session
   \return tpPESession          - pointer to the session context or NULL if session is not found.
   \sa
   --------------------------------------------------------------------------*/
tpPESession pe_find_session_by_bss_idx(tpAniSirGlobal pMac, uint8_t bssIdx)
{
	uint8_t i;
	for (i = 0; i < pMac->lim.maxBssId; i++) {
		/* If BSSID matches return corresponding tables address */
		if ((pMac->lim.gpSession[i].valid)
		    && (pMac->lim.gpSession[i].bssIdx == bssIdx)) {
			return &pMac->lim.gpSession[i];
		}
	}
	lim_log(pMac, LOG4, FL("Session lookup fails for bssIdx: %d"), bssIdx);
	return NULL;
}

/*--------------------------------------------------------------------------
   \brief pe_find_session_by_session_id() - looks up the PE session given the session ID.

   This function returns the session context  if the session
   corresponding to the given session ID is found in the PE session table.

   \param pMac                   - pointer to global adapter context
   \param sessionId             -session ID for which session context needs to be looked up.

   \return tpPESession          - pointer to the session context or NULL if session is not found.

   \sa
   --------------------------------------------------------------------------*/
tpPESession pe_find_session_by_session_id(tpAniSirGlobal pMac, uint8_t sessionId)
{
	if (sessionId >= pMac->lim.maxBssId) {
		lim_log(pMac, LOGE, FL("Invalid sessionId: %d \n "), sessionId);
		return (NULL);
	}
	if ((pMac->lim.gpSession[sessionId].valid == true)) {
		return (&pMac->lim.gpSession[sessionId]);
	}
	return (NULL);

}

/**
 * pe_find_session_by_sta_id() - looks up the PE session given staid.
 * @mac_ctx:       pointer to global adapter context
 * @staid:         StaId of the session
 * @session_id:    session ID is returned here, if session is found.
 *
 * This function returns the session context and the session ID if the session
 * corresponding to the given StaId is found in the PE session table.
 *
 * Return: session pointer
 */
tpPESession
pe_find_session_by_sta_id(tpAniSirGlobal mac_ctx,
			  uint8_t staid,
			  uint8_t *session_id)
{
	uint8_t i, j;
	tpPESession session_ptr;
	dphHashTableClass *dph_ptr;

	for (i = 0; i < mac_ctx->lim.maxBssId; i++) {
		if (!mac_ctx->lim.gpSession[i].valid)
			continue;
		session_ptr = &mac_ctx->lim.gpSession[i];
		dph_ptr = &session_ptr->dph.dphHashTable;
		for (j = 0; j < dph_ptr->size; j++) {
			if (dph_ptr->pDphNodeArray[j].valid
			    && dph_ptr->pDphNodeArray[j].added
			    && staid == dph_ptr->pDphNodeArray[j].staIndex) {
				*session_id = i;
				return session_ptr;
			}
		}
	}

	lim_log(mac_ctx, LOG4,
		FL("Session lookup fails for StaId: %d\n "), staid);
	return NULL;
}

/**
 * pe_delete_session() - deletes the PE session given the session ID.
 * @mac_ctx: pointer to global adapter context
 * @session: session to be deleted.
 *
 * Deletes the given PE session
 *
 * Return: void
 */
void pe_delete_session(tpAniSirGlobal mac_ctx, tpPESession session)
{
	uint16_t i = 0;
	uint16_t n;
	TX_TIMER *timer_ptr;

	CDF_TRACE(CDF_MODULE_ID_PE, CDF_TRACE_LEVEL_DEBUG,
		FL("Trying to delete PE session %d Opmode %d BssIdx %d BSSID: "MAC_ADDRESS_STR),
		session->peSessionId, session->operMode,
		session->bssIdx,
		MAC_ADDR_ARRAY(session->bssId));
	for (n = 0; n < (mac_ctx->lim.maxStation + 1); n++) {
		timer_ptr = &mac_ctx->lim.limTimers.gpLimCnfWaitTimer[n];
		if (session->peSessionId == timer_ptr->sessionId)
			if (true == tx_timer_running(timer_ptr))
				tx_timer_deactivate(timer_ptr);
	}

	if (LIM_IS_AP_ROLE(session)) {
		cdf_mc_timer_stop(&session->protection_fields_reset_timer);
		cdf_mc_timer_destroy(&session->protection_fields_reset_timer);
	}

#if defined (WLAN_FEATURE_VOWIFI_11R)
	/* Delete FT related information */
	lim_ft_cleanup(mac_ctx, session);
#endif
	if (session->pLimStartBssReq != NULL) {
		cdf_mem_free(session->pLimStartBssReq);
		session->pLimStartBssReq = NULL;
	}

	if (session->pLimJoinReq != NULL) {
		cdf_mem_free(session->pLimJoinReq);
		session->pLimJoinReq = NULL;
	}

	if (session->pLimReAssocReq != NULL) {
		cdf_mem_free(session->pLimReAssocReq);
		session->pLimReAssocReq = NULL;
	}

	if (session->pLimMlmJoinReq != NULL) {
		cdf_mem_free(session->pLimMlmJoinReq);
		session->pLimMlmJoinReq = NULL;
	}

	if (session->dph.dphHashTable.pHashTable != NULL) {
		cdf_mem_free(session->dph.dphHashTable.pHashTable);
		session->dph.dphHashTable.pHashTable = NULL;
	}

	if (session->dph.dphHashTable.pDphNodeArray != NULL) {
		cdf_mem_free(session->dph.dphHashTable.pDphNodeArray);
		session->dph.dphHashTable.pDphNodeArray = NULL;
	}

	if (session->gpLimPeerIdxpool != NULL) {
		cdf_mem_free(session->gpLimPeerIdxpool);
		session->gpLimPeerIdxpool = NULL;
	}

	if (session->beacon != NULL) {
		cdf_mem_free(session->beacon);
		session->beacon = NULL;
	}

	if (session->assocReq != NULL) {
		cdf_mem_free(session->assocReq);
		session->assocReq = NULL;
	}

	if (session->assocRsp != NULL) {
		cdf_mem_free(session->assocRsp);
		session->assocRsp = NULL;
	}

	if (session->parsedAssocReq != NULL) {
		tpSirAssocReq tmp_ptr = NULL;
		/* Cleanup the individual allocation first */
		for (i = 0; i < session->dph.dphHashTable.size; i++) {
			if (session->parsedAssocReq[i] == NULL)
				continue;
			tmp_ptr = ((tpSirAssocReq)
				  (session->parsedAssocReq[i]));
			if (tmp_ptr->assocReqFrame) {
				cdf_mem_free(tmp_ptr->assocReqFrame);
				tmp_ptr->assocReqFrame = NULL;
				tmp_ptr->assocReqFrameLength = 0;
			}
			cdf_mem_free(session->parsedAssocReq[i]);
			session->parsedAssocReq[i] = NULL;
		}
		/* Cleanup the whole block */
		cdf_mem_free(session->parsedAssocReq);
		session->parsedAssocReq = NULL;
	}
	if (NULL != session->limAssocResponseData) {
		cdf_mem_free(session->limAssocResponseData);
		session->limAssocResponseData = NULL;
	}
#if  defined (WLAN_FEATURE_VOWIFI_11R) || defined (FEATURE_WLAN_ESE) || defined(FEATURE_WLAN_LFR)
	if (NULL != session->pLimMlmReassocRetryReq) {
		cdf_mem_free(session->pLimMlmReassocRetryReq);
		session->pLimMlmReassocRetryReq = NULL;
	}
#endif
	if (NULL != session->pLimMlmReassocReq) {
		cdf_mem_free(session->pLimMlmReassocReq);
		session->pLimMlmReassocReq = NULL;
	}

	if (NULL != session->pSchProbeRspTemplate) {
		cdf_mem_free(session->pSchProbeRspTemplate);
		session->pSchProbeRspTemplate = NULL;
	}

	if (NULL != session->pSchBeaconFrameBegin) {
		cdf_mem_free(session->pSchBeaconFrameBegin);
		session->pSchBeaconFrameBegin = NULL;
	}

	if (NULL != session->pSchBeaconFrameEnd) {
		cdf_mem_free(session->pSchBeaconFrameEnd);
		session->pSchBeaconFrameEnd = NULL;
	}

	/* Must free the buffer before peSession invalid */
	if (NULL != session->addIeParams.probeRespData_buff) {
		cdf_mem_free(session->addIeParams.probeRespData_buff);
		session->addIeParams.probeRespData_buff = NULL;
		session->addIeParams.probeRespDataLen = 0;
	}
	if (NULL != session->addIeParams.assocRespData_buff) {
		cdf_mem_free(session->addIeParams.assocRespData_buff);
		session->addIeParams.assocRespData_buff = NULL;
		session->addIeParams.assocRespDataLen = 0;
	}
	if (NULL != session->addIeParams.probeRespBCNData_buff) {
		cdf_mem_free(session->addIeParams.probeRespBCNData_buff);
		session->addIeParams.probeRespBCNData_buff = NULL;
		session->addIeParams.probeRespBCNDataLen = 0;
	}
#ifdef WLAN_FEATURE_11W
	/* if PMF connection */
	if (session->limRmfEnabled)
		cdf_mc_timer_destroy(&session->pmfComebackTimer);
#endif
	session->valid = false;

	if (LIM_IS_AP_ROLE(session))
		lim_check_and_reset_protection_params(mac_ctx);

	return;
}

/*--------------------------------------------------------------------------
   \brief pe_find_session_by_peer_sta() - looks up the PE session given the Station Address.

   This function returns the session context and the session ID if the session
   corresponding to the given station address is found in the PE session table.

   \param pMac                   - pointer to global adapter context
   \param sa                       - Peer STA Address of the session
   \param sessionId             -session ID is returned here, if session is found.

   \return tpPESession          - pointer to the session context or NULL if session is not found.

   \sa
   --------------------------------------------------------------------------*/

tpPESession pe_find_session_by_peer_sta(tpAniSirGlobal pMac, uint8_t *sa,
					uint8_t *sessionId)
{
	uint8_t i;
	tpDphHashNode pSta;
	uint16_t aid;

	for (i = 0; i < pMac->lim.maxBssId; i++) {
		if ((pMac->lim.gpSession[i].valid)) {
			pSta =
				dph_lookup_hash_entry(pMac, sa, &aid,
						      &pMac->lim.gpSession[i].dph.
						      dphHashTable);
			if (pSta != NULL) {
				*sessionId = i;
				return &pMac->lim.gpSession[i];
			}
		}
	}

	lim_log(pMac, LOG1, FL("Session lookup fails for Peer StaId: \n "));
	lim_print_mac_addr(pMac, sa, LOG1);
	return NULL;
}

/**
 * pe_find_session_by_sme_session_id() - looks up the PE session for given sme
 * session id
 * @mac_ctx:          pointer to global adapter context
 * @sme_session_id:   sme session id
 *
 * looks up the PE session for given sme session id
 *
 * Return: pe session entry for given sme session if found else NULL
 */
tpPESession pe_find_session_by_sme_session_id(tpAniSirGlobal mac_ctx,
					      uint8_t sme_session_id)
{
	uint8_t i;
	for (i = 0; i < mac_ctx->lim.maxBssId; i++) {
		if ((mac_ctx->lim.gpSession[i].valid) &&
		     (mac_ctx->lim.gpSession[i].smeSessionId ==
			sme_session_id)) {
			return &mac_ctx->lim.gpSession[i];
		}
	}
	lim_log(mac_ctx, LOG4,
		FL("Session lookup fails for smeSessionID: %d"),
		sme_session_id);
	return NULL;
}

/**
 * pe_get_active_session_count() - function to return active pe session count
 *
 * @mac_ctx: pointer to global mac structure
 *
 * returns number of active pe session count
 *
 * Return: 0 if there are no active sessions else return number of active
 *          sessions
 */
uint8_t pe_get_active_session_count(tpAniSirGlobal mac_ctx)
{
	uint8_t i, active_session_count = 0;

	for (i = 0; i < mac_ctx->lim.maxBssId; i++)
		if (mac_ctx->lim.gpSession[i].valid)
			active_session_count++;

	return active_session_count;
}
