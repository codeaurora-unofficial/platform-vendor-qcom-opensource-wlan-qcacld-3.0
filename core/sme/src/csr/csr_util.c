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

/** ------------------------------------------------------------------------- *
    ------------------------------------------------------------------------- *

    \file csr_util.c

    Implementation supporting routines for CSR.
   ========================================================================== */

#include "ani_global.h"

#include "csr_support.h"
#include "csr_inside_api.h"
#include "sms_debug.h"
#include "sme_qos_internal.h"
#include "wma_types.h"
#include "cds_utils.h"
#include "cds_concurrency.h"


uint8_t csr_wpa_oui[][CSR_WPA_OUI_SIZE] = {
	{0x00, 0x50, 0xf2, 0x00}
	,
	{0x00, 0x50, 0xf2, 0x01}
	,
	{0x00, 0x50, 0xf2, 0x02}
	,
	{0x00, 0x50, 0xf2, 0x03}
	,
	{0x00, 0x50, 0xf2, 0x04}
	,
	{0x00, 0x50, 0xf2, 0x05}
	,
#ifdef FEATURE_WLAN_ESE
	{0x00, 0x40, 0x96, 0x00}
	,                       /* CCKM */
#endif /* FEATURE_WLAN_ESE */
};

uint8_t csr_rsn_oui[][CSR_RSN_OUI_SIZE] = {
	{0x00, 0x0F, 0xAC, 0x00}
	,                       /* group cipher */
	{0x00, 0x0F, 0xAC, 0x01}
	,                       /* WEP-40 or RSN */
	{0x00, 0x0F, 0xAC, 0x02}
	,                       /* TKIP or RSN-PSK */
	{0x00, 0x0F, 0xAC, 0x03}
	,                       /* Reserved */
	{0x00, 0x0F, 0xAC, 0x04}
	,                       /* AES-CCMP */
	{0x00, 0x0F, 0xAC, 0x05}
	,                       /* WEP-104 */
	{0x00, 0x40, 0x96, 0x00}
	,                       /* CCKM */
	{0x00, 0x0F, 0xAC, 0x06}
	,                       /* BIP (encryption type) or
				RSN-PSK-SHA256 (authentication type) */
	/* RSN-8021X-SHA256 (authentication type) */
	{0x00, 0x0F, 0xAC, 0x05}
};

#ifdef FEATURE_WLAN_WAPI
uint8_t csr_wapi_oui[][CSR_WAPI_OUI_SIZE] = {
	{0x00, 0x14, 0x72, 0x00}
	,                       /* Reserved */
	{0x00, 0x14, 0x72, 0x01}
	,                       /* WAI certificate or SMS4 */
	{0x00, 0x14, 0x72, 0x02} /* WAI PSK */
};
#endif /* FEATURE_WLAN_WAPI */
uint8_t csr_wme_info_oui[CSR_WME_OUI_SIZE] = { 0x00, 0x50, 0xf2, 0x02 };
uint8_t csr_wme_parm_oui[CSR_WME_OUI_SIZE] = { 0x00, 0x50, 0xf2, 0x02 };


/* ////////////////////////////////////////////////////////////////////// */

/**
 * \var g_phy_rates_suppt
 *
 * \brief Rate support lookup table
 *
 *
 * This is a  lookup table indexing rates &  configuration parameters to
 * support.  Given a rate (in  unites of 0.5Mpbs) & three bools (MIMO
 * Enabled, Channel  Bonding Enabled, & Concatenation  Enabled), one can
 * determine  whether  the given  rate  is  supported  by computing  two
 * indices.  The  first maps  the rate to  table row as  indicated below
 * (i.e. eHddSuppRate_6Mbps maps to  row zero, eHddSuppRate_9Mbps to row
 * 1, and so on).  Index two can be computed like so:
 *
 * \code
 *  idx2 = ( fEsf  ? 0x4 : 0x0 ) |
 *         ( fCb   ? 0x2 : 0x0 ) |
 *         ( fMimo ? 0x1 : 0x0 );
 * \endcode
 *
 *
 * Given that:
 *
 *  \code
 *  fSupported = g_phy_rates_suppt[idx1][idx2];
 *  \endcode
 *
 *
 * This table is based on  the document "PHY Supported Rates.doc".  This
 * table is  permissive in that a  rate is reflected  as being supported
 * even  when turning  off an  enabled feature  would be  required.  For
 * instance, "PHY Supported Rates"  lists 42Mpbs as unsupported when CB,
 * ESF, &  MIMO are all  on.  However,  if we turn  off either of  CB or
 * MIMO, it then becomes supported.   Therefore, we mark it as supported
 * even in index 7 of this table.
 *
 *
 */

static const bool g_phy_rates_suppt[24][8] = {

	/* SSF   SSF    SSF    SSF    ESF    ESF    ESF    ESF */
	/* SIMO  MIMO   SIMO   MIMO   SIMO   MIMO   SIMO   MIMO */
	/* No CB No CB  CB     CB     No CB  No CB  CB     CB */
	{true, true, true, true, true, true, true, true},       /* 6Mbps */
	{true, true, true, true, true, true, true, true},       /* 9Mbps */
	{true, true, true, true, true, true, true, true},       /* 12Mbps */
	{true, true, true, true, true, true, true, true},       /* 18Mbps */
	{false, false, true, true, false, false, true, true},   /* 20Mbps */
	{true, true, true, true, true, true, true, true},       /* 24Mbps */
	{true, true, true, true, true, true, true, true},       /* 36Mbps */
	{false, false, true, true, false, true, true, true},    /* 40Mbps */
	{false, false, true, true, false, true, true, true},    /* 42Mbps */
	{true, true, true, true, true, true, true, true},       /* 48Mbps */
	{true, true, true, true, true, true, true, true},       /* 54Mbps */
	{false, true, true, true, false, true, true, true},     /* 72Mbps */
	{false, false, true, true, false, true, true, true},    /* 80Mbps */
	{false, false, true, true, false, true, true, true},    /* 84Mbps */
	{false, true, true, true, false, true, true, true},     /* 96Mbps */
	{false, true, true, true, false, true, true, true},     /* 108Mbps */
	{false, false, true, true, false, true, true, true},    /* 120Mbps */
	{false, false, true, true, false, true, true, true},    /* 126Mbps */
	{false, false, false, true, false, false, false, true}, /* 144Mbps */
	{false, false, false, true, false, false, false, true}, /* 160Mbps */
	{false, false, false, true, false, false, false, true}, /* 168Mbps */
	{false, false, false, true, false, false, false, true}, /* 192Mbps */
	{false, false, false, true, false, false, false, true}, /* 216Mbps */
	{false, false, false, true, false, false, false, true}, /* 240Mbps */

};

#define CASE_RETURN_STR(n) {\
	case (n): return (# n);\
}

const char *get_e_roam_cmd_status_str(eRoamCmdStatus val)
{
	switch (val) {
		CASE_RETURN_STR(eCSR_ROAM_CANCELLED);
		CASE_RETURN_STR(eCSR_ROAM_ROAMING_START);
		CASE_RETURN_STR(eCSR_ROAM_ROAMING_COMPLETION);
		CASE_RETURN_STR(eCSR_ROAM_ASSOCIATION_START);
		CASE_RETURN_STR(eCSR_ROAM_ASSOCIATION_COMPLETION);
		CASE_RETURN_STR(eCSR_ROAM_DISASSOCIATED);
		CASE_RETURN_STR(eCSR_ROAM_SHOULD_ROAM);
		CASE_RETURN_STR(eCSR_ROAM_SCAN_FOUND_NEW_BSS);
		CASE_RETURN_STR(eCSR_ROAM_LOSTLINK);
	default:
		return "unknown";
	}
}

const char *get_e_csr_roam_result_str(eCsrRoamResult val)
{
	switch (val) {
		CASE_RETURN_STR(eCSR_ROAM_RESULT_NONE);
		CASE_RETURN_STR(eCSR_ROAM_RESULT_FAILURE);
		CASE_RETURN_STR(eCSR_ROAM_RESULT_ASSOCIATED);
		CASE_RETURN_STR(eCSR_ROAM_RESULT_NOT_ASSOCIATED);
		CASE_RETURN_STR(eCSR_ROAM_RESULT_MIC_FAILURE);
		CASE_RETURN_STR(eCSR_ROAM_RESULT_FORCED);
		CASE_RETURN_STR(eCSR_ROAM_RESULT_DISASSOC_IND);
		CASE_RETURN_STR(eCSR_ROAM_RESULT_DEAUTH_IND);
		CASE_RETURN_STR(eCSR_ROAM_RESULT_CAP_CHANGED);
		CASE_RETURN_STR(eCSR_ROAM_RESULT_IBSS_CONNECT);
		CASE_RETURN_STR(eCSR_ROAM_RESULT_IBSS_INACTIVE);
		CASE_RETURN_STR(eCSR_ROAM_RESULT_IBSS_NEW_PEER);
		CASE_RETURN_STR(eCSR_ROAM_RESULT_IBSS_COALESCED);
	default:
		return "unknown";
	}
}

bool csr_get_bss_id_bss_desc(tHalHandle hHal, tSirBssDescription *pSirBssDesc,
			     struct cdf_mac_addr *pBssId)
{
	cdf_mem_copy(pBssId, &pSirBssDesc->bssId[0],
			sizeof(struct cdf_mac_addr));
	return true;
}

bool csr_is_bss_id_equal(tHalHandle hHal, tSirBssDescription *pSirBssDesc1,
			 tSirBssDescription *pSirBssDesc2)
{
	tpAniSirGlobal pMac = PMAC_STRUCT(hHal);
	bool fEqual = false;
	struct cdf_mac_addr bssId1;
	struct cdf_mac_addr bssId2;

	do {
		if (!pSirBssDesc1)
			break;
		if (!pSirBssDesc2)
			break;

		if (!csr_get_bss_id_bss_desc(pMac, pSirBssDesc1, &bssId1))
			break;
		if (!csr_get_bss_id_bss_desc(pMac, pSirBssDesc2, &bssId2))
			break;

		fEqual = cdf_is_macaddr_equal(&bssId1, &bssId2);
	} while (0);

	return fEqual;
}

bool csr_is_conn_state_connected_ibss(tpAniSirGlobal pMac, uint32_t sessionId)
{
	return eCSR_ASSOC_STATE_TYPE_IBSS_CONNECTED ==
		pMac->roam.roamSession[sessionId].connectState;
}

bool csr_is_conn_state_disconnected_ibss(tpAniSirGlobal pMac,
					 uint32_t sessionId)
{
	return eCSR_ASSOC_STATE_TYPE_IBSS_DISCONNECTED ==
		pMac->roam.roamSession[sessionId].connectState;
}

bool csr_is_conn_state_connected_infra(tpAniSirGlobal pMac, uint32_t sessionId)
{
	return eCSR_ASSOC_STATE_TYPE_INFRA_ASSOCIATED ==
		pMac->roam.roamSession[sessionId].connectState;
}

bool csr_is_conn_state_connected(tpAniSirGlobal pMac, uint32_t sessionId)
{
	if (csr_is_conn_state_connected_ibss(pMac, sessionId)
	    || csr_is_conn_state_connected_infra(pMac, sessionId)
	    || csr_is_conn_state_connected_wds(pMac, sessionId))
		return true;
	else
		return false;
}

bool csr_is_conn_state_infra(tpAniSirGlobal pMac, uint32_t sessionId)
{
	return csr_is_conn_state_connected_infra(pMac, sessionId);
}

bool csr_is_conn_state_ibss(tpAniSirGlobal pMac, uint32_t sessionId)
{
	return csr_is_conn_state_connected_ibss(pMac, sessionId) ||
	       csr_is_conn_state_disconnected_ibss(pMac, sessionId);
}

bool csr_is_conn_state_connected_wds(tpAniSirGlobal pMac, uint32_t sessionId)
{
	return eCSR_ASSOC_STATE_TYPE_WDS_CONNECTED ==
		pMac->roam.roamSession[sessionId].connectState;
}

bool csr_is_conn_state_connected_infra_ap(tpAniSirGlobal pMac,
					  uint32_t sessionId)
{
	return (eCSR_ASSOC_STATE_TYPE_INFRA_CONNECTED ==
		 pMac->roam.roamSession[sessionId].connectState) ||
	       (eCSR_ASSOC_STATE_TYPE_INFRA_DISCONNECTED ==
		 pMac->roam.roamSession[sessionId].connectState);
}

bool csr_is_conn_state_disconnected_wds(tpAniSirGlobal pMac, uint32_t sessionId)
{
	return eCSR_ASSOC_STATE_TYPE_WDS_DISCONNECTED ==
		pMac->roam.roamSession[sessionId].connectState;
}

bool csr_is_conn_state_wds(tpAniSirGlobal pMac, uint32_t sessionId)
{
	return csr_is_conn_state_connected_wds(pMac, sessionId) ||
	       csr_is_conn_state_disconnected_wds(pMac, sessionId);
}

bool csr_is_conn_state_ap(tpAniSirGlobal pMac, uint32_t sessionId)
{
	tCsrRoamSession *pSession;
	pSession = CSR_GET_SESSION(pMac, sessionId);
	if (!pSession)
		return false;
	if (CSR_IS_INFRA_AP(&pSession->connectedProfile))
		return true;
	return false;
}

bool csr_is_any_session_in_connect_state(tpAniSirGlobal pMac)
{
	uint32_t i;
	bool fRc = false;

	for (i = 0; i < CSR_ROAM_SESSION_MAX; i++) {
		if (CSR_IS_SESSION_VALID(pMac, i) &&
		    (csr_is_conn_state_infra(pMac, i)
		     || csr_is_conn_state_ibss(pMac, i)
		     || csr_is_conn_state_ap(pMac, i))) {
			fRc = true;
			break;
		}
	}

	return fRc;
}

int8_t csr_get_infra_session_id(tpAniSirGlobal pMac)
{
	uint8_t i;
	int8_t sessionid = -1;

	for (i = 0; i < CSR_ROAM_SESSION_MAX; i++) {
		if (CSR_IS_SESSION_VALID(pMac, i)
		    && csr_is_conn_state_infra(pMac, i)) {
			sessionid = i;
			break;
		}
	}

	return sessionid;
}

uint8_t csr_get_infra_operation_channel(tpAniSirGlobal pMac, uint8_t sessionId)
{
	uint8_t channel;

	if (CSR_IS_SESSION_VALID(pMac, sessionId)) {
		channel =
			pMac->roam.roamSession[sessionId].connectedProfile.
			operationChannel;
	} else {
		channel = 0;
	}
	return channel;
}

bool csr_is_session_client_and_connected(tpAniSirGlobal pMac, uint8_t sessionId)
{
	tCsrRoamSession *pSession = NULL;
	if (CSR_IS_SESSION_VALID(pMac, sessionId)
	    && csr_is_conn_state_infra(pMac, sessionId)) {
		pSession = CSR_GET_SESSION(pMac, sessionId);
		if (NULL != pSession->pCurRoamProfile) {
			if ((pSession->pCurRoamProfile->csrPersona ==
			     CDF_STA_MODE)
			    || (pSession->pCurRoamProfile->csrPersona ==
				CDF_P2P_CLIENT_MODE))
				return true;
		}
	}
	return false;
}

/**
 * csr_get_concurrent_operation_channel() - To get concurrent operating channel
 * @mac_ctx: Pointer to mac context
 *
 * This routine will return operating channel on FIRST BSS that is
 * active/operating to be used for concurrency mode.
 * If other BSS is not up or not connected it will return 0
 *
 * Return: uint8_t
 */
uint8_t csr_get_concurrent_operation_channel(tpAniSirGlobal mac_ctx)
{
	tCsrRoamSession *session = NULL;
	uint8_t i = 0;
	tCDF_CON_MODE persona;

	for (i = 0; i < CSR_ROAM_SESSION_MAX; i++) {
		if (!CSR_IS_SESSION_VALID(mac_ctx, i))
			continue;
		session = CSR_GET_SESSION(mac_ctx, i);
		if (NULL == session->pCurRoamProfile)
			continue;
		persona = session->pCurRoamProfile->csrPersona;
		if ((((persona == CDF_STA_MODE) ||
			(persona == CDF_P2P_CLIENT_MODE)) &&
			(session->connectState ==
				eCSR_ASSOC_STATE_TYPE_INFRA_ASSOCIATED)) ||
			(((persona == CDF_P2P_GO_MODE) ||
				(persona == CDF_SAP_MODE))
				 && (session->connectState !=
					 eCSR_ASSOC_STATE_TYPE_NOT_CONNECTED)))
			return session->connectedProfile.operationChannel;

	}
	return 0;
}

#ifdef FEATURE_WLAN_MCC_TO_SCC_SWITCH

#define HALF_BW_OF(eCSR_bw_val) ((eCSR_bw_val)/2)

/* calculation of center channel based on V/HT BW and WIFI channel bw=5MHz) */

#define CSR_GET_HT40_PLUS_CCH(och) ((och)+2)
#define CSR_GET_HT40_MINUS_CCH(och) ((och)-2)

#define CSR_GET_HT80_PLUS_LL_CCH(och) ((och)+6)
#define CSR_GET_HT80_PLUS_HL_CCH(och) ((och)+2)
#define CSR_GET_HT80_MINUS_LH_CCH(och) ((och)-2)
#define CSR_GET_HT80_MINUS_HH_CCH(och) ((och)-6)

/**
 * csr_get_ch_from_ht_profile() - to get channel from HT profile
 * @pMac: pointer to Mac context
 * @htp: pointer to HT profile
 * @och: operating channel
 * @cfreq: channel frequency
 * @hbw: half bandwidth
 *
 * This function will fill half bandwidth and channel frequency based
 * on the HT profile
 *
 * Return: none
 */
void csr_get_ch_from_ht_profile(tpAniSirGlobal pMac, tCsrRoamHTProfile *htp,
				uint16_t och, uint16_t *cfreq, uint16_t *hbw)
{
	uint16_t cch, ch_bond;

	if (och > 14)
		ch_bond = pMac->roam.configParam.channelBondingMode5GHz;
	else
		ch_bond = pMac->roam.configParam.channelBondingMode24GHz;

	cch = och;
	*hbw = HALF_BW_OF(eCSR_BW_20MHz_VAL);

	if (!ch_bond)
		goto ret;

	sms_log(pMac, LOG1, FL("##HTC: %d scbw: %d rcbw: %d sco: %d"
#ifdef WLAN_FEATURE_11AC
				"VHTC: %d apc: %d apbw: %d"
#endif
			      ),
			htp->htCapability, htp->htSupportedChannelWidthSet,
			htp->htRecommendedTxWidthSet,
			htp->htSecondaryChannelOffset,
#ifdef WLAN_FEATURE_11AC
			htp->vhtCapability, htp->apCenterChan, htp->apChanWidth
#endif
	       );

#ifdef WLAN_FEATURE_11AC
	if (htp->vhtCapability) {
		cch = htp->apCenterChan;
		if (htp->apChanWidth == WNI_CFG_VHT_CHANNEL_WIDTH_80MHZ)
			*hbw = HALF_BW_OF(eCSR_BW_80MHz_VAL);
		else if (htp->apChanWidth == WNI_CFG_VHT_CHANNEL_WIDTH_160MHZ)
			*hbw = HALF_BW_OF(eCSR_BW_160MHz_VAL);

		if (!*hbw && htp->htCapability) {
			if (htp->htSupportedChannelWidthSet ==
				eHT_CHANNEL_WIDTH_40MHZ)
				*hbw = HALF_BW_OF(eCSR_BW_40MHz_VAL);
			else
				*hbw = HALF_BW_OF(eCSR_BW_20MHz_VAL);
		}
	} else
#endif
		if (htp->htCapability) {
			if (htp->htSupportedChannelWidthSet ==
					eHT_CHANNEL_WIDTH_40MHZ) {
				*hbw = HALF_BW_OF(eCSR_BW_40MHz_VAL);
				if (htp->htSecondaryChannelOffset ==
					PHY_DOUBLE_CHANNEL_LOW_PRIMARY)
					cch = CSR_GET_HT40_PLUS_CCH(och);
				else if (htp->htSecondaryChannelOffset ==
					PHY_DOUBLE_CHANNEL_HIGH_PRIMARY)
					cch = CSR_GET_HT40_MINUS_CCH(och);
			} else {
				cch = och;
				*hbw = HALF_BW_OF(eCSR_BW_20MHz_VAL);
			}
		}

ret:
	*cfreq = cds_chan_to_freq(cch);
	return;
}

/**
 * csr_calc_chb_for_sap_phymode() - to calc channel bandwidth for sap phymode
 * @mac_ctx: pointer to mac context
 * @sap_ch: SAP operating channel
 * @sap_phymode: SAP physical mode
 * @sap_cch: concurrency channel
 * @sap_hbw: SAP half bw
 * @chb: channel bandwidth
 *
 * This routine is called to calculate channel bandwidth
 *
 * Return: none
 */
static void csr_calc_chb_for_sap_phymode(tpAniSirGlobal mac_ctx,
		uint16_t *sap_ch, eCsrPhyMode *sap_phymode,
		uint16_t *sap_cch, uint16_t *sap_hbw, uint8_t *chb)
{
	if (*sap_phymode == eCSR_DOT11_MODE_11n ||
			*sap_phymode == eCSR_DOT11_MODE_11n_ONLY) {

		*sap_hbw = HALF_BW_OF(eCSR_BW_40MHz_VAL);
		if (*chb == PHY_DOUBLE_CHANNEL_LOW_PRIMARY)
			*sap_cch = CSR_GET_HT40_PLUS_CCH(*sap_ch);
		else if (*chb == PHY_DOUBLE_CHANNEL_HIGH_PRIMARY)
			*sap_cch = CSR_GET_HT40_MINUS_CCH(*sap_ch);

	}
#ifdef WLAN_FEATURE_11AC
	else if (*sap_phymode == eCSR_DOT11_MODE_11ac ||
			*sap_phymode == eCSR_DOT11_MODE_11ac_ONLY) {
		/*11AC only 80/40/20 Mhz supported in Rome */
		if (mac_ctx->roam.configParam.nVhtChannelWidth ==
				(WNI_CFG_VHT_CHANNEL_WIDTH_80MHZ + 1)) {
			*sap_hbw = HALF_BW_OF(eCSR_BW_80MHz_VAL);
			if (*chb ==
				(PHY_QUADRUPLE_CHANNEL_20MHZ_LOW_40MHZ_LOW - 1))
				*sap_cch = CSR_GET_HT80_PLUS_LL_CCH(*sap_ch);
			else if (*chb ==
				(PHY_QUADRUPLE_CHANNEL_20MHZ_HIGH_40MHZ_LOW
				     - 1))
				*sap_cch = CSR_GET_HT80_PLUS_HL_CCH(*sap_ch);
			else if (*chb ==
				 (PHY_QUADRUPLE_CHANNEL_20MHZ_LOW_40MHZ_HIGH
				     - 1))
				*sap_cch = CSR_GET_HT80_MINUS_LH_CCH(*sap_ch);
			else if (*chb ==
				(PHY_QUADRUPLE_CHANNEL_20MHZ_HIGH_40MHZ_HIGH
				     - 1))
				*sap_cch = CSR_GET_HT80_MINUS_HH_CCH(*sap_ch);
		} else {
			*sap_hbw = HALF_BW_OF(eCSR_BW_40MHz_VAL);
			if (*chb == (PHY_QUADRUPLE_CHANNEL_20MHZ_LOW_40MHZ_LOW
					- 1))
				*sap_cch = CSR_GET_HT40_PLUS_CCH(*sap_ch);
			else if (*chb ==
				(PHY_QUADRUPLE_CHANNEL_20MHZ_HIGH_40MHZ_LOW
				     - 1))
				*sap_cch = CSR_GET_HT40_MINUS_CCH(*sap_ch);
			else if (*chb ==
				(PHY_QUADRUPLE_CHANNEL_20MHZ_LOW_40MHZ_HIGH
				     - 1))
				*sap_cch = CSR_GET_HT40_PLUS_CCH(*sap_ch);
			else if (*chb ==
				(PHY_QUADRUPLE_CHANNEL_20MHZ_HIGH_40MHZ_HIGH
				     - 1))
				*sap_cch = CSR_GET_HT40_MINUS_CCH(*sap_ch);
		}
	}
#endif
}

/**
 * csr_handle_conc_chnl_overlap_for_sap_go - To handle overlap for AP+AP
 * @mac_ctx: pointer to mac context
 * @session: Current session
 * @sap_ch: SAP/GO operating channel
 * @sap_hbw: SAP/GO half bw
 * @sap_cfreq: SAP/GO channel frequency
 * @intf_ch: concurrent SAP/GO operating channel
 * @intf_hbw: concurrent SAP/GO half bw
 * @intf_cfreq: concurrent SAP/GO channel frequency
 *
 * This routine is called to check if one SAP/GO channel is overlapping with
 * other SAP/GO channel
 *
 * Return: none
 */
static void csr_handle_conc_chnl_overlap_for_sap_go(tpAniSirGlobal mac_ctx,
		tCsrRoamSession *session,
		uint16_t *sap_ch, uint16_t *sap_hbw, uint16_t *sap_cfreq,
		uint16_t *intf_ch, uint16_t *intf_hbw, uint16_t *intf_cfreq)
{
	/*
	 * if conc_custom_rule1 is defined then we don't
	 * want p2pgo to follow SAP's channel or SAP to
	 * follow P2PGO's channel.
	 */
	if (0 == mac_ctx->roam.configParam.conc_custom_rule1 &&
		0 == mac_ctx->roam.configParam.conc_custom_rule2) {
		if (*sap_ch == 0) {
			*sap_ch = session->connectedProfile.operationChannel;
			csr_get_ch_from_ht_profile(mac_ctx,
				&session->connectedProfile.HTProfile,
				*sap_ch, sap_cfreq, sap_hbw);
		} else if (*sap_ch !=
				session->connectedProfile.operationChannel) {
			*intf_ch = session->connectedProfile.operationChannel;
			csr_get_ch_from_ht_profile(mac_ctx,
					&session->connectedProfile.HTProfile,
					*intf_ch, intf_cfreq, intf_hbw);
		}
	} else if (*sap_ch == 0 &&
			(session->pCurRoamProfile->csrPersona ==
					CDF_SAP_MODE)) {
		*sap_ch = session->connectedProfile.operationChannel;
		csr_get_ch_from_ht_profile(mac_ctx,
				&session->connectedProfile.HTProfile,
				*sap_ch, sap_cfreq, sap_hbw);
	}
}


/**
 * csr_check_concurrent_channel_overlap() - To check concurrent overlap chnls
 * @mac_ctx: Pointer to mac context
 * @sap_ch: SAP channel
 * @sap_phymode: SAP phy mode
 * @cc_switch_mode: concurrent switch mode
 *
 * This routine will be called to check concurrent overlap channels
 *
 * Return: uint16_t
 */
uint16_t csr_check_concurrent_channel_overlap(tpAniSirGlobal mac_ctx,
			uint16_t sap_ch, eCsrPhyMode sap_phymode,
			uint8_t cc_switch_mode)
{
	tCsrRoamSession *session = NULL;
	uint8_t i = 0, chb = PHY_SINGLE_CHANNEL_CENTERED;
	uint16_t intf_ch = 0, sap_hbw = 0, intf_hbw = 0, intf_cfreq = 0;
	uint16_t sap_cfreq = 0;
	uint16_t sap_lfreq, sap_hfreq, intf_lfreq, intf_hfreq, sap_cch;

	if (mac_ctx->roam.configParam.cc_switch_mode ==
			CDF_MCC_TO_SCC_SWITCH_DISABLE)
		return 0;

	if (sap_ch != 0) {
		sap_cch = sap_ch;
		sap_hbw = HALF_BW_OF(eCSR_BW_20MHz_VAL);

		if (sap_ch > 14)
			chb = mac_ctx->roam.configParam.channelBondingMode5GHz;
		else
			chb = mac_ctx->roam.configParam.channelBondingMode24GHz;

		if (chb)
			csr_calc_chb_for_sap_phymode(mac_ctx, &sap_ch,
					&sap_phymode, &sap_cch, &sap_hbw, &chb);
		sap_cfreq = cds_chan_to_freq(sap_cch);
	}

	for (i = 0; i < CSR_ROAM_SESSION_MAX; i++) {
		if (!CSR_IS_SESSION_VALID(mac_ctx, i))
			continue;

		session = CSR_GET_SESSION(mac_ctx, i);
		if (NULL == session->pCurRoamProfile)
			continue;
		if (((session->pCurRoamProfile->csrPersona == CDF_STA_MODE) ||
			(session->pCurRoamProfile->csrPersona ==
				CDF_P2P_CLIENT_MODE)) &&
			(session->connectState ==
				eCSR_ASSOC_STATE_TYPE_INFRA_ASSOCIATED)) {
			intf_ch = session->connectedProfile.operationChannel;
			csr_get_ch_from_ht_profile(mac_ctx,
				&session->connectedProfile.HTProfile,
				intf_ch, &intf_cfreq, &intf_hbw);
		} else if (((session->pCurRoamProfile->csrPersona ==
					CDF_P2P_GO_MODE) ||
				(session->pCurRoamProfile->csrPersona ==
					CDF_SAP_MODE)) &&
				(session->connectState !=
					eCSR_ASSOC_STATE_TYPE_NOT_CONNECTED)) {
				if (pSession->ch_switch_in_progress)
					continue;

				csr_handle_conc_chnl_overlap_for_sap_go(mac_ctx,
					session, &sap_ch, &sap_hbw, &sap_cfreq,
					&intf_ch, &intf_hbw, &intf_cfreq);
		}
	}

	if (intf_ch && sap_ch != intf_ch &&
			cc_switch_mode != CDF_MCC_TO_SCC_SWITCH_FORCE) {
		sap_lfreq = sap_cfreq - sap_hbw;
		sap_hfreq = sap_cfreq + sap_hbw;
		intf_lfreq = intf_cfreq - intf_hbw;
		intf_hfreq = intf_cfreq + intf_hbw;

		sms_log(mac_ctx, LOGE,
			FL("\nSAP:  OCH: %03d OCF: %d CCH: %03d CF: %d BW: %d LF: %d HF: %d\n"
			"INTF: OCH: %03d OCF: %d CCH: %03d CF: %d BW: %d LF: %d HF: %d"),
			sap_ch, cds_chan_to_freq(sap_ch),
			cds_freq_to_chan(sap_cfreq), sap_cfreq, sap_hbw * 2,
			sap_lfreq, sap_hfreq, intf_ch,
			cds_chan_to_freq(intf_ch), cds_freq_to_chan(intf_cfreq),
			intf_cfreq, intf_hbw * 2, intf_lfreq, intf_hfreq);

		if (!(((sap_lfreq > intf_lfreq && sap_lfreq < intf_hfreq) ||
			(sap_hfreq > intf_lfreq && sap_hfreq < intf_hfreq)) ||
			((intf_lfreq > sap_lfreq && intf_lfreq < sap_hfreq) ||
			(intf_hfreq > sap_lfreq && intf_hfreq < sap_hfreq))))
			intf_ch = 0;
	} else if (intf_ch && sap_ch != intf_ch &&
				cc_switch_mode == CDF_MCC_TO_SCC_SWITCH_FORCE) {
		if (!((intf_ch < 14 && sap_ch < 14) ||
			(intf_ch > 14 && sap_ch > 14)))
			intf_ch = 0;
	} else if (intf_ch == sap_ch) {
		intf_ch = 0;
	}

	sms_log(mac_ctx, LOGE, FL("##Concurrent Channels %s Interfering"),
		intf_ch == 0 ? "Not" : "Are");
	return intf_ch;
}
#endif

bool csr_is_all_session_disconnected(tpAniSirGlobal pMac)
{
	uint32_t i;
	bool fRc = true;

	for (i = 0; i < CSR_ROAM_SESSION_MAX; i++) {
		if (CSR_IS_SESSION_VALID(pMac, i)
		    && !csr_is_conn_state_disconnected(pMac, i)) {
			fRc = false;
			break;
		}
	}

	return fRc;
}

/**
 * csr_is_sta_session_connected() - to find if concurrent sta is active
 * @mac_ctx: pointer to mac context
 *
 * This function will iterate through each session and check if sta
 * session exist and active
 *
 * Return: true or false
 */
bool csr_is_sta_session_connected(tpAniSirGlobal mac_ctx)
{
	uint32_t i;
	tCsrRoamSession *pSession = NULL;

	for (i = 0; i < CSR_ROAM_SESSION_MAX; i++) {
		if (CSR_IS_SESSION_VALID(mac_ctx, i) &&
			!csr_is_conn_state_disconnected(mac_ctx, i)) {
			pSession = CSR_GET_SESSION(mac_ctx, i);

			if ((NULL != pSession->pCurRoamProfile) &&
				(CDF_STA_MODE ==
					pSession->pCurRoamProfile->csrPersona))
				return true;
		}
	}

	return false;
}

/**
 * csr_is_p2p_session_connected() - to find if any p2p session is active
 * @mac_ctx: pointer to mac context
 *
 * This function will iterate through each session and check if any p2p
 * session exist and active
 *
 * Return: true or false
 */
bool csr_is_p2p_session_connected(tpAniSirGlobal pMac)
{
	uint32_t i;
	tCsrRoamSession *pSession = NULL;
	tCDF_CON_MODE persona;

	for (i = 0; i < CSR_ROAM_SESSION_MAX; i++) {
		if (CSR_IS_SESSION_VALID(pMac, i)
		    && !csr_is_conn_state_disconnected(pMac, i)) {
			pSession = CSR_GET_SESSION(pMac, i);
			persona = pSession->pCurRoamProfile->csrPersona;
			if ((NULL != pSession->pCurRoamProfile) &&
				((CDF_P2P_CLIENT_MODE == persona) ||
				(CDF_P2P_GO_MODE == persona))) {
				return true;
			}
		}
	}

	return false;
}

bool csr_is_any_session_connected(tpAniSirGlobal pMac)
{
	uint32_t i, count;
	bool fRc = false;

	count = 0;
	for (i = 0; i < CSR_ROAM_SESSION_MAX; i++) {
		if (CSR_IS_SESSION_VALID(pMac, i)
		    && !csr_is_conn_state_disconnected(pMac, i))
			count++;
	}

	if (count > 0)
		fRc = true;
	return fRc;
}

bool csr_is_infra_connected(tpAniSirGlobal pMac)
{
	uint32_t i;
	bool fRc = false;

	for (i = 0; i < CSR_ROAM_SESSION_MAX; i++) {
		if (CSR_IS_SESSION_VALID(pMac, i)
		    && csr_is_conn_state_connected_infra(pMac, i)) {
			fRc = true;
			break;
		}
	}

	return fRc;
}

bool csr_is_concurrent_infra_connected(tpAniSirGlobal pMac)
{
	uint32_t i, noOfConnectedInfra = 0;

	bool fRc = false;

	for (i = 0; i < CSR_ROAM_SESSION_MAX; i++) {
		if (CSR_IS_SESSION_VALID(pMac, i)
		    && csr_is_conn_state_connected_infra(pMac, i)) {
			++noOfConnectedInfra;
		}
	}

	/* More than one Infra Sta Connected */
	if (noOfConnectedInfra > 1)
		fRc = true;
	return fRc;
}

bool csr_is_ibss_started(tpAniSirGlobal pMac)
{
	uint32_t i;
	bool fRc = false;

	for (i = 0; i < CSR_ROAM_SESSION_MAX; i++) {
		if (CSR_IS_SESSION_VALID(pMac, i)
		    && csr_is_conn_state_ibss(pMac, i)) {
			fRc = true;
			break;
		}
	}

	return fRc;
}

bool csr_is_btamp_started(tpAniSirGlobal pMac)
{
	uint32_t i;
	bool fRc = false;

	for (i = 0; i < CSR_ROAM_SESSION_MAX; i++) {
		if (CSR_IS_SESSION_VALID(pMac, i)
		    && csr_is_conn_state_connected_wds(pMac, i)) {
			fRc = true;
			break;
		}
	}

	return fRc;
}

bool csr_is_concurrent_session_running(tpAniSirGlobal pMac)
{
	uint32_t sessionId, noOfCocurrentSession = 0;
	eCsrConnectState connectState;

	bool fRc = false;

	for (sessionId = 0; sessionId < CSR_ROAM_SESSION_MAX; sessionId++) {
		if (CSR_IS_SESSION_VALID(pMac, sessionId)) {
			connectState =
				pMac->roam.roamSession[sessionId].connectState;
			if ((eCSR_ASSOC_STATE_TYPE_INFRA_ASSOCIATED ==
			     connectState)
			    || (eCSR_ASSOC_STATE_TYPE_INFRA_CONNECTED ==
				connectState)
			    || (eCSR_ASSOC_STATE_TYPE_INFRA_DISCONNECTED ==
				connectState)) {
				++noOfCocurrentSession;
			}
		}
	}

	/* More than one session is Up and Running */
	if (noOfCocurrentSession > 1)
		fRc = true;
	return fRc;
}

bool csr_is_infra_ap_started(tpAniSirGlobal pMac)
{
	uint32_t sessionId;
	bool fRc = false;

	for (sessionId = 0; sessionId < CSR_ROAM_SESSION_MAX; sessionId++) {
		if (CSR_IS_SESSION_VALID(pMac, sessionId)
		    && (csr_is_conn_state_connected_infra_ap(pMac, sessionId))) {
			fRc = true;
			break;
		}
	}

	return fRc;

}

bool csr_is_btamp(tpAniSirGlobal pMac, uint32_t sessionId)
{
	return csr_is_conn_state_connected_wds(pMac, sessionId);
}

bool csr_is_conn_state_disconnected(tpAniSirGlobal pMac, uint32_t sessionId)
{
	return eCSR_ASSOC_STATE_TYPE_NOT_CONNECTED ==
	       pMac->roam.roamSession[sessionId].connectState;
}

/**
 * csr_is_valid_mc_concurrent_session() - To check concurren session is valid
 * @mac_ctx: pointer to mac context
 * @session_id: session id
 * @bss_descr: bss description
 *
 * This function validates the concurrent session
 *
 * Return: true or false
 */
bool csr_is_valid_mc_concurrent_session(tpAniSirGlobal mac_ctx,
		uint32_t session_id,
		tSirBssDescription *bss_descr)
{
	tCsrRoamSession *pSession = NULL;

	/* Check for MCC support */
	if (!mac_ctx->roam.configParam.fenableMCCMode)
		return false;
	if (!CSR_IS_SESSION_VALID(mac_ctx, session_id))
		return false;
	/* Validate BeaconInterval */
	pSession = CSR_GET_SESSION(mac_ctx, session_id);
	if (NULL == pSession->pCurRoamProfile)
		return false;
	if (CDF_STATUS_SUCCESS ==
		csr_isconcurrentsession_valid(mac_ctx, session_id,
			pSession->pCurRoamProfile->csrPersona)) {
		if (CDF_STATUS_SUCCESS ==
			csr_validate_mcc_beacon_interval(mac_ctx,
				bss_descr->channelId,
				&bss_descr->beaconInterval, session_id,
				pSession->pCurRoamProfile->csrPersona))
			return true;
	}
	return false;
}

static tSirMacCapabilityInfo csr_get_bss_capabilities(tSirBssDescription *
						      pSirBssDesc)
{
	tSirMacCapabilityInfo dot11Caps;

	/* tSirMacCapabilityInfo is 16-bit */
	cdf_get_u16((uint8_t *) &pSirBssDesc->capabilityInfo,
		    (uint16_t *) &dot11Caps);

	return dot11Caps;
}

bool csr_is_infra_bss_desc(tSirBssDescription *pSirBssDesc)
{
	tSirMacCapabilityInfo dot11Caps = csr_get_bss_capabilities(pSirBssDesc);

	return (bool) dot11Caps.ess;
}

bool csr_is_ibss_bss_desc(tSirBssDescription *pSirBssDesc)
{
	tSirMacCapabilityInfo dot11Caps = csr_get_bss_capabilities(pSirBssDesc);

	return (bool) dot11Caps.ibss;
}

bool csr_is_qo_s_bss_desc(tSirBssDescription *pSirBssDesc)
{
	tSirMacCapabilityInfo dot11Caps = csr_get_bss_capabilities(pSirBssDesc);

	return (bool) dot11Caps.qos;
}

bool csr_is_privacy(tSirBssDescription *pSirBssDesc)
{
	tSirMacCapabilityInfo dot11Caps = csr_get_bss_capabilities(pSirBssDesc);

	return (bool) dot11Caps.privacy;
}

bool csr_is11d_supported(tpAniSirGlobal pMac)
{
	return pMac->roam.configParam.Is11dSupportEnabled;
}

bool csr_is11h_supported(tpAniSirGlobal pMac)
{
	return pMac->roam.configParam.Is11hSupportEnabled;
}

bool csr_is11e_supported(tpAniSirGlobal pMac)
{
	return pMac->roam.configParam.Is11eSupportEnabled;
}

bool csr_is_mcc_supported(tpAniSirGlobal pMac)
{
	return pMac->roam.configParam.fenableMCCMode;

}

bool csr_is_wmm_supported(tpAniSirGlobal pMac)
{
	if (eCsrRoamWmmNoQos == pMac->roam.configParam.WMMSupportMode)
		return false;
	else
		return true;
}

/* pIes is the IEs for pSirBssDesc2 */
bool csr_is_ssid_equal(tHalHandle hHal, tSirBssDescription *pSirBssDesc1,
		       tSirBssDescription *pSirBssDesc2, tDot11fBeaconIEs *pIes2)
{
	bool fEqual = false;
	tSirMacSSid Ssid1, Ssid2;
	tpAniSirGlobal pMac = PMAC_STRUCT(hHal);
	tDot11fBeaconIEs *pIes1 = NULL;
	tDot11fBeaconIEs *pIesLocal = pIes2;

	do {
		if ((NULL == pSirBssDesc1) || (NULL == pSirBssDesc2))
			break;
		if (!pIesLocal
		    &&
		    !CDF_IS_STATUS_SUCCESS(csr_get_parsed_bss_description_ies
						   (pMac, pSirBssDesc2,
						    &pIesLocal))) {
			sms_log(pMac, LOGE, FL("  fail to parse IEs"));
			break;
		}
		if (!CDF_IS_STATUS_SUCCESS
			(csr_get_parsed_bss_description_ies(pMac,
				pSirBssDesc1, &pIes1))) {
			break;
		}
		if ((!pIes1->SSID.present) || (!pIesLocal->SSID.present))
			break;
		if (pIes1->SSID.num_ssid != pIesLocal->SSID.num_ssid)
			break;
		cdf_mem_copy(Ssid1.ssId, pIes1->SSID.ssid,
			     pIes1->SSID.num_ssid);
		cdf_mem_copy(Ssid2.ssId, pIesLocal->SSID.ssid,
			     pIesLocal->SSID.num_ssid);

		fEqual =
			cdf_mem_compare(Ssid1.ssId, Ssid2.ssId,
					pIesLocal->SSID.num_ssid);

	} while (0);
	if (pIes1)
		cdf_mem_free(pIes1);
	if (pIesLocal && !pIes2)
		cdf_mem_free(pIesLocal);

	return fEqual;
}

/* pIes can be passed in as NULL if the caller doesn't have one prepared */
bool csr_is_bss_description_wme(tHalHandle hHal, tSirBssDescription *pSirBssDesc,
				tDot11fBeaconIEs *pIes)
{
	tpAniSirGlobal pMac = PMAC_STRUCT(hHal);
	/* Assume that WME is found... */
	bool fWme = true;
	tDot11fBeaconIEs *pIesTemp = pIes;

	do {
		if (pIesTemp == NULL) {
			if (!CDF_IS_STATUS_SUCCESS
				    (csr_get_parsed_bss_description_ies
					    (pMac, pSirBssDesc, &pIesTemp))) {
				fWme = false;
				break;
			}
		}
		/* if the Wme Info IE is found, then WME is supported... */
		if (CSR_IS_QOS_BSS(pIesTemp))
			break;
		/* if none of these are found, then WME is NOT supported... */
		fWme = false;
	} while (0);
	if (!csr_is_wmm_supported(pMac) && fWme) {
		if (!pIesTemp->HTCaps.present) {
			fWme = false;
		}
	}
	if ((pIes == NULL) && (NULL != pIesTemp)) {
		/* we allocate memory here so free it before returning */
		cdf_mem_free(pIesTemp);
	}

	return fWme;
}

eCsrMediaAccessType csr_get_qo_s_from_bss_desc(tHalHandle hHal,
					       tSirBssDescription *pSirBssDesc,
					       tDot11fBeaconIEs *pIes)
{
	eCsrMediaAccessType qosType = eCSR_MEDIUM_ACCESS_DCF;

	if (NULL == pIes) {
		CDF_ASSERT(pIes != NULL);
		return qosType;
	}

	do {
		/* if we find WMM in the Bss Description, then we let this */
		/* override and use WMM. */
		if (csr_is_bss_description_wme(hHal, pSirBssDesc, pIes)) {
			qosType = eCSR_MEDIUM_ACCESS_WMM_eDCF_DSCP;
		} else {
			/* if the QoS bit is on, then the AP is advertising 11E QoS... */
			if (csr_is_qo_s_bss_desc(pSirBssDesc)) {
				qosType = eCSR_MEDIUM_ACCESS_11e_eDCF;
			} else {
				qosType = eCSR_MEDIUM_ACCESS_DCF;
			}
			/* scale back based on the types turned on for the adapter... */
			if (eCSR_MEDIUM_ACCESS_11e_eDCF == qosType
			    && !csr_is11e_supported(hHal)) {
				qosType = eCSR_MEDIUM_ACCESS_DCF;
			}
		}

	} while (0);

	return qosType;
}

/* Caller allocates memory for pIEStruct */
CDF_STATUS csr_parse_bss_description_ies(tHalHandle hHal,
					  tSirBssDescription *pBssDesc,
					  tDot11fBeaconIEs *pIEStruct)
{
	CDF_STATUS status = CDF_STATUS_E_FAILURE;
	tpAniSirGlobal pMac = PMAC_STRUCT(hHal);
	int ieLen =
		(int)(pBssDesc->length + sizeof(pBssDesc->length) -
		      GET_FIELD_OFFSET(tSirBssDescription, ieFields));

	if (ieLen > 0 && pIEStruct) {
		if (!DOT11F_FAILED
			    (dot11f_unpack_beacon_i_es
				    (pMac, (uint8_t *) pBssDesc->ieFields, ieLen,
				    pIEStruct))) {
			status = CDF_STATUS_SUCCESS;
		}
	}

	return status;
}

/* This function will allocate memory for the parsed IEs to the caller. Caller must free the memory */
/* after it is done with the data only if this function succeeds */
CDF_STATUS csr_get_parsed_bss_description_ies(tHalHandle hHal,
					       tSirBssDescription *pBssDesc,
					       tDot11fBeaconIEs **ppIEStruct)
{
	CDF_STATUS status = CDF_STATUS_E_INVAL;
	tpAniSirGlobal pMac = PMAC_STRUCT(hHal);

	if (pBssDesc && ppIEStruct) {
		*ppIEStruct = cdf_mem_malloc(sizeof(tDot11fBeaconIEs));
		if ((*ppIEStruct) != NULL) {
			cdf_mem_set((void *)*ppIEStruct,
				    sizeof(tDot11fBeaconIEs), 0);
			status =
				csr_parse_bss_description_ies(hHal, pBssDesc,
							       *ppIEStruct);
			if (!CDF_IS_STATUS_SUCCESS(status)) {
				cdf_mem_free(*ppIEStruct);
				*ppIEStruct = NULL;
			}
		} else {
			sms_log(pMac, LOGE, FL(" failed to allocate memory"));
			CDF_ASSERT(0);
			return CDF_STATUS_E_NOMEM;
		}
	}

	return status;
}

bool csr_is_nullssid(uint8_t *pBssSsid, uint8_t len)
{
	bool fNullSsid = false;

	uint32_t SsidLength;
	uint8_t *pSsidStr;

	do {
		if (0 == len) {
			fNullSsid = true;
			break;
		}
		/* Consider 0 or space for hidden SSID */
		if (0 == pBssSsid[0]) {
			fNullSsid = true;
			break;
		}

		SsidLength = len;
		pSsidStr = pBssSsid;

		while (SsidLength) {
			if (*pSsidStr)
				break;

			pSsidStr++;
			SsidLength--;
		}

		if (0 == SsidLength) {
			fNullSsid = true;
			break;
		}
	} while (0);

	return fNullSsid;
}

uint32_t csr_get_frag_thresh(tHalHandle hHal)
{
	tpAniSirGlobal pMac = PMAC_STRUCT(hHal);

	return pMac->roam.configParam.FragmentationThreshold;
}

uint32_t csr_get_rts_thresh(tHalHandle hHal)
{
	tpAniSirGlobal pMac = PMAC_STRUCT(hHal);

	return pMac->roam.configParam.RTSThreshold;
}

eCsrPhyMode csr_translate_to_phy_mode_from_bss_desc(tSirBssDescription *pSirBssDesc)
{
	eCsrPhyMode phyMode;

	switch (pSirBssDesc->nwType) {
	case eSIR_11A_NW_TYPE:
		phyMode = eCSR_DOT11_MODE_11a;
		break;

	case eSIR_11B_NW_TYPE:
		phyMode = eCSR_DOT11_MODE_11b;
		break;

	case eSIR_11G_NW_TYPE:
		phyMode = eCSR_DOT11_MODE_11g;
		break;

	case eSIR_11N_NW_TYPE:
		phyMode = eCSR_DOT11_MODE_11n;
		break;
#ifdef WLAN_FEATURE_11AC
	case eSIR_11AC_NW_TYPE:
	default:
		phyMode = eCSR_DOT11_MODE_11ac;
#else
	default:
		phyMode = eCSR_DOT11_MODE_11n;
#endif
		break;
	}
	return phyMode;
}

uint32_t csr_translate_to_wni_cfg_dot11_mode(tpAniSirGlobal pMac,
					     eCsrCfgDot11Mode csrDot11Mode)
{
	uint32_t ret;

	switch (csrDot11Mode) {
	case eCSR_CFG_DOT11_MODE_AUTO:
		sms_log(pMac, LOGW,
			FL("  Warning: sees eCSR_CFG_DOT11_MODE_AUTO "));
		if (IS_FEATURE_SUPPORTED_BY_FW(DOT11AC))
			ret = WNI_CFG_DOT11_MODE_11AC;
		else
			ret = WNI_CFG_DOT11_MODE_11N;
		break;
	case eCSR_CFG_DOT11_MODE_11A:
		ret = WNI_CFG_DOT11_MODE_11A;
		break;
	case eCSR_CFG_DOT11_MODE_11B:
		ret = WNI_CFG_DOT11_MODE_11B;
		break;
	case eCSR_CFG_DOT11_MODE_11G:
		ret = WNI_CFG_DOT11_MODE_11G;
		break;
	case eCSR_CFG_DOT11_MODE_11N:
		ret = WNI_CFG_DOT11_MODE_11N;
		break;
	case eCSR_CFG_DOT11_MODE_11G_ONLY:
		ret = WNI_CFG_DOT11_MODE_11G_ONLY;
		break;
	case eCSR_CFG_DOT11_MODE_11N_ONLY:
		ret = WNI_CFG_DOT11_MODE_11N_ONLY;
		break;
	case eCSR_CFG_DOT11_MODE_11AC_ONLY:
		if (IS_FEATURE_SUPPORTED_BY_FW(DOT11AC))
			ret = WNI_CFG_DOT11_MODE_11AC_ONLY;
		else
			ret = WNI_CFG_DOT11_MODE_11N;
		break;
	case eCSR_CFG_DOT11_MODE_11AC:
		if (IS_FEATURE_SUPPORTED_BY_FW(DOT11AC))
			ret = WNI_CFG_DOT11_MODE_11AC;
		else
			ret = WNI_CFG_DOT11_MODE_11N;
		break;
	default:
		sms_log(pMac, LOGW, FL("doesn't expect %d as csrDo11Mode"),
			csrDot11Mode);
		if (eCSR_BAND_24 == pMac->roam.configParam.eBand) {
			ret = WNI_CFG_DOT11_MODE_11G;
		} else {
			ret = WNI_CFG_DOT11_MODE_11A;
		}
		break;
	}

	return ret;
}

/**
 * csr_get_phy_mode_from_bss() - Get Phy Mode
 * @pMac:           Global MAC context
 * @pBSSDescription: BSS Descriptor
 * @pPhyMode:        Physical Mode
 * @pIes:            Pointer to the IE fields
 *
 * This function should only return the super set of supported modes
 * 11n implies 11b/g/a/n.
 *
 * Return: success
 **/
CDF_STATUS csr_get_phy_mode_from_bss(tpAniSirGlobal pMac,
		tSirBssDescription *pBSSDescription,
		eCsrPhyMode *pPhyMode, tDot11fBeaconIEs *pIes)
{
	CDF_STATUS status = CDF_STATUS_SUCCESS;
	eCsrPhyMode phyMode =
		csr_translate_to_phy_mode_from_bss_desc(pBSSDescription);

	if (pIes) {
		if (pIes->HTCaps.present) {
			phyMode = eCSR_DOT11_MODE_11n;
#ifdef WLAN_FEATURE_11AC
			if (IS_BSS_VHT_CAPABLE(pIes->VHTCaps) ||
				IS_BSS_VHT_CAPABLE(pIes->vendor2_ie.VHTCaps))
				phyMode = eCSR_DOT11_MODE_11ac;
#endif
		}
		*pPhyMode = phyMode;
	}

	return status;
}

/**
 * csr_get_phy_mode_in_use() - to get phymode
 * @phyModeIn: physical mode
 * @bssPhyMode: physical mode in bss
 * @f5GhzBand: 5Ghz band
 * @pCfgDot11ModeToUse: dot11 mode in use
 *
 * This function returns the correct eCSR_CFG_DOT11_MODE is the two phyModes
 * matches. bssPhyMode is the mode derived from the BSS description
 * f5GhzBand is derived from the channel id of BSS description
 *
 * Return: true or false
 */
bool csr_get_phy_mode_in_use(eCsrPhyMode phyModeIn, eCsrPhyMode bssPhyMode,
			     bool f5GhzBand, eCsrCfgDot11Mode *pCfgDot11ModeToUse)
{
	bool fMatch = false;
	eCsrCfgDot11Mode cfgDot11Mode;
	cfgDot11Mode = eCSR_CFG_DOT11_MODE_11N;

	switch (phyModeIn) {
	/* 11a or 11b or 11g */
	case eCSR_DOT11_MODE_abg:
		fMatch = true;
		if (f5GhzBand)
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11A;
		else if (eCSR_DOT11_MODE_11b == bssPhyMode)
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11B;
		else
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11G;
		break;

	case eCSR_DOT11_MODE_11a:
		if (f5GhzBand) {
			fMatch = true;
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11A;
		}
		break;

	case eCSR_DOT11_MODE_11g:
		if (!f5GhzBand) {
			fMatch = true;
			if (eCSR_DOT11_MODE_11b == bssPhyMode)
				cfgDot11Mode = eCSR_CFG_DOT11_MODE_11B;
			else
				cfgDot11Mode = eCSR_CFG_DOT11_MODE_11G;
		}
		break;

	case eCSR_DOT11_MODE_11g_ONLY:
		if (eCSR_DOT11_MODE_11g == bssPhyMode) {
			fMatch = true;
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11G;
		}
		break;

	case eCSR_DOT11_MODE_11b:
		if (!f5GhzBand) {
			fMatch = true;
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11B;
		}
		break;

	case eCSR_DOT11_MODE_11b_ONLY:
		if (eCSR_DOT11_MODE_11b == bssPhyMode) {
			fMatch = true;
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11B;
		}
		break;

	case eCSR_DOT11_MODE_11n:
		fMatch = true;
		switch (bssPhyMode) {
		case eCSR_DOT11_MODE_11g:
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11G;
			break;
		case eCSR_DOT11_MODE_11b:
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11B;
			break;
		case eCSR_DOT11_MODE_11a:
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11A;
			break;
		case eCSR_DOT11_MODE_11n:
#ifdef WLAN_FEATURE_11AC
		case eCSR_DOT11_MODE_11ac:
#endif
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11N;
			break;

		default:
#ifdef WLAN_FEATURE_11AC
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11AC;
#else
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11N;
#endif
			break;
		}
		break;

	case eCSR_DOT11_MODE_11n_ONLY:
		if ((eCSR_DOT11_MODE_11n == bssPhyMode)) {
			fMatch = true;
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11N;

		}

		break;
#ifdef WLAN_FEATURE_11AC
	case eCSR_DOT11_MODE_11ac:
		fMatch = true;
		switch (bssPhyMode) {
		case eCSR_DOT11_MODE_11g:
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11G;
			break;
		case eCSR_DOT11_MODE_11b:
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11B;
			break;
		case eCSR_DOT11_MODE_11a:
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11A;
			break;
		case eCSR_DOT11_MODE_11n:
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11N;
			break;
		case eCSR_DOT11_MODE_11ac:
		default:
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11AC;
			break;
		}
		break;

	case eCSR_DOT11_MODE_11ac_ONLY:
		if ((eCSR_DOT11_MODE_11ac == bssPhyMode)) {
			fMatch = true;
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11AC;
		}
		break;
#endif

	default:
		fMatch = true;
		switch (bssPhyMode) {
		case eCSR_DOT11_MODE_11g:
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11G;
			break;
		case eCSR_DOT11_MODE_11b:
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11B;
			break;
		case eCSR_DOT11_MODE_11a:
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11A;
			break;
		case eCSR_DOT11_MODE_11n:
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11N;
			break;
#ifdef WLAN_FEATURE_11AC
		case eCSR_DOT11_MODE_11ac:
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11AC;
			break;
#endif
		default:
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_AUTO;
			break;
		}
		break;
	}

	if (fMatch && pCfgDot11ModeToUse) {
#ifdef WLAN_FEATURE_11AC
		if (cfgDot11Mode == eCSR_CFG_DOT11_MODE_11AC
		    && (!IS_FEATURE_SUPPORTED_BY_FW(DOT11AC)))
			*pCfgDot11ModeToUse = eCSR_CFG_DOT11_MODE_11N;
		else
#endif
			*pCfgDot11ModeToUse = cfgDot11Mode;
	}
	return fMatch;
}

/**
 * csr_is_phy_mode_match() - to find if phy mode matches
 * @pMac: pointer to mac context
 * @phyMode: physical mode
 * @pSirBssDesc: bss description
 * @pProfile: pointer to roam profile
 * @pReturnCfgDot11Mode: dot1 mode to return
 * @pIes: pointer to IEs
 *
 * This function decides whether the one of the bit of phyMode is matching the
 * mode in the BSS and allowed by the user setting
 *
 * Return: true or false based on mode that fits the criteria
 */
bool csr_is_phy_mode_match(tpAniSirGlobal pMac, uint32_t phyMode,
			   tSirBssDescription *pSirBssDesc,
			   tCsrRoamProfile *pProfile,
			   eCsrCfgDot11Mode *pReturnCfgDot11Mode,
			   tDot11fBeaconIEs *pIes)
{
	bool fMatch = false;
	eCsrPhyMode phyModeInBssDesc = eCSR_DOT11_MODE_AUTO, phyMode2;
	eCsrCfgDot11Mode cfgDot11ModeToUse = eCSR_CFG_DOT11_MODE_AUTO;
	uint32_t bitMask, loopCount;

	if (!CDF_IS_STATUS_SUCCESS(csr_get_phy_mode_from_bss(pMac, pSirBssDesc,
					&phyModeInBssDesc, pIes)))
		return fMatch;

	if ((0 == phyMode) || (eCSR_DOT11_MODE_AUTO & phyMode)) {
		if (eCSR_CFG_DOT11_MODE_ABG ==
				pMac->roam.configParam.uCfgDot11Mode)
			phyMode = eCSR_DOT11_MODE_abg;
		else if (eCSR_CFG_DOT11_MODE_AUTO ==
				pMac->roam.configParam.uCfgDot11Mode)
#ifdef WLAN_FEATURE_11AC
			phyMode = eCSR_DOT11_MODE_11ac;
#else
			phyMode = eCSR_DOT11_MODE_11n;
#endif

		else
			/* user's pick */
			phyMode = pMac->roam.configParam.phyMode;
	}

	if ((0 == phyMode) || (eCSR_DOT11_MODE_AUTO & phyMode)) {
		if (0 != phyMode) {
			if (eCSR_DOT11_MODE_AUTO & phyMode) {
				phyMode2 =
					eCSR_DOT11_MODE_AUTO & phyMode;
			}
		} else {
			phyMode2 = phyMode;
		}
		fMatch = csr_get_phy_mode_in_use(phyMode2, phyModeInBssDesc,
				CDS_IS_CHANNEL_5GHZ(pSirBssDesc->channelId),
				&cfgDot11ModeToUse);
	} else {
		bitMask = 1;
		loopCount = 0;
		while (loopCount < eCSR_NUM_PHY_MODE) {
			phyMode2 = (phyMode & (bitMask << loopCount++));
			if (0 != phyMode2 && csr_get_phy_mode_in_use(phyMode2,
						phyModeInBssDesc,
						CDS_IS_CHANNEL_5GHZ
						(pSirBssDesc->channelId),
						&cfgDot11ModeToUse)) {
				fMatch = true;
				break;
			}
		}
	}
	if (fMatch && pReturnCfgDot11Mode) {
		if (pProfile) {
			/*
			 * IEEE 11n spec (8.4.3): HT STA shall
			 * eliminate TKIP as a choice for the pairwise
			 * cipher suite if CCMP is advertised by the AP
			 * or if the AP included an HT capabilities
			 * element in its Beacons and Probe Response.
			 */
			if ((!CSR_IS_11n_ALLOWED(
					pProfile->negotiatedUCEncryptionType))
					&& ((eCSR_CFG_DOT11_MODE_11N ==
						cfgDot11ModeToUse) ||
#ifdef WLAN_FEATURE_11AC
					(eCSR_CFG_DOT11_MODE_11AC ==
						cfgDot11ModeToUse)
#endif
				)) {
				/* We cannot do 11n here */
				if (!CDS_IS_CHANNEL_5GHZ
						(pSirBssDesc->channelId)) {
					cfgDot11ModeToUse =
						eCSR_CFG_DOT11_MODE_11G;
				} else {
					cfgDot11ModeToUse =
						eCSR_CFG_DOT11_MODE_11A;
				}
			}
		}
		*pReturnCfgDot11Mode = cfgDot11ModeToUse;
	}

	return fMatch;
}

eCsrCfgDot11Mode csr_find_best_phy_mode(tpAniSirGlobal pMac, uint32_t phyMode)
{
	eCsrCfgDot11Mode cfgDot11ModeToUse;
	eCsrBand eBand = pMac->roam.configParam.eBand;

	if ((0 == phyMode) ||
#ifdef WLAN_FEATURE_11AC
	    (eCSR_DOT11_MODE_11ac & phyMode) ||
#endif
	    (eCSR_DOT11_MODE_AUTO & phyMode)) {
#ifdef WLAN_FEATURE_11AC
		if (IS_FEATURE_SUPPORTED_BY_FW(DOT11AC)) {
			cfgDot11ModeToUse = eCSR_CFG_DOT11_MODE_11AC;
		} else
#endif
		{
			/* Default to 11N mode if user has configured 11ac mode
			 * and FW doesn't supports 11ac mode .
			 */
			cfgDot11ModeToUse = eCSR_CFG_DOT11_MODE_11N;
		}
	} else {
		if ((eCSR_DOT11_MODE_11n | eCSR_DOT11_MODE_11n_ONLY) & phyMode) {
			cfgDot11ModeToUse = eCSR_CFG_DOT11_MODE_11N;
		} else if (eCSR_DOT11_MODE_abg & phyMode) {
			if (eCSR_BAND_24 != eBand) {
				cfgDot11ModeToUse = eCSR_CFG_DOT11_MODE_11A;
			} else {
				cfgDot11ModeToUse = eCSR_CFG_DOT11_MODE_11G;
			}
		} else if (eCSR_DOT11_MODE_11a & phyMode) {
			cfgDot11ModeToUse = eCSR_CFG_DOT11_MODE_11A;
		} else if ((eCSR_DOT11_MODE_11g | eCSR_DOT11_MODE_11g_ONLY) &
			   phyMode) {
			cfgDot11ModeToUse = eCSR_CFG_DOT11_MODE_11G;
		} else {
			cfgDot11ModeToUse = eCSR_CFG_DOT11_MODE_11B;
		}
	}

	return cfgDot11ModeToUse;
}

uint32_t csr_get11h_power_constraint(tHalHandle hHal,
				     tDot11fIEPowerConstraints *pPowerConstraint)
{
	tpAniSirGlobal pMac = PMAC_STRUCT(hHal);
	uint32_t localPowerConstraint = 0;

	/* check if .11h support is enabled, if not, the power constraint is 0. */
	if (pMac->roam.configParam.Is11hSupportEnabled
	    && pPowerConstraint->present) {
		localPowerConstraint = pPowerConstraint->localPowerConstraints;
	}

	return localPowerConstraint;
}

bool csr_is_profile_wpa(tCsrRoamProfile *pProfile)
{
	bool fWpaProfile = false;

	switch (pProfile->negotiatedAuthType) {
	case eCSR_AUTH_TYPE_WPA:
	case eCSR_AUTH_TYPE_WPA_PSK:
	case eCSR_AUTH_TYPE_WPA_NONE:
#ifdef FEATURE_WLAN_ESE
	case eCSR_AUTH_TYPE_CCKM_WPA:
#endif
		fWpaProfile = true;
		break;

	default:
		fWpaProfile = false;
		break;
	}

	if (fWpaProfile) {
		switch (pProfile->negotiatedUCEncryptionType) {
		case eCSR_ENCRYPT_TYPE_WEP40:
		case eCSR_ENCRYPT_TYPE_WEP104:
		case eCSR_ENCRYPT_TYPE_TKIP:
		case eCSR_ENCRYPT_TYPE_AES:
			fWpaProfile = true;
			break;

		default:
			fWpaProfile = false;
			break;
		}
	}
	return fWpaProfile;
}

bool csr_is_profile_rsn(tCsrRoamProfile *pProfile)
{
	bool fRSNProfile = false;

	switch (pProfile->negotiatedAuthType) {
	case eCSR_AUTH_TYPE_RSN:
	case eCSR_AUTH_TYPE_RSN_PSK:
#ifdef WLAN_FEATURE_VOWIFI_11R
	case eCSR_AUTH_TYPE_FT_RSN:
	case eCSR_AUTH_TYPE_FT_RSN_PSK:
#endif
#ifdef FEATURE_WLAN_ESE
	case eCSR_AUTH_TYPE_CCKM_RSN:
#endif
#ifdef WLAN_FEATURE_11W
	case eCSR_AUTH_TYPE_RSN_PSK_SHA256:
	case eCSR_AUTH_TYPE_RSN_8021X_SHA256:
#endif
		fRSNProfile = true;
		break;

	default:
		fRSNProfile = false;
		break;
	}

	if (fRSNProfile) {
		switch (pProfile->negotiatedUCEncryptionType) {
		/* !!REVIEW - For WPA2, use of RSN IE mandates */
		/* use of AES as encryption. Here, we qualify */
		/* even if encryption type is WEP or TKIP */
		case eCSR_ENCRYPT_TYPE_WEP40:
		case eCSR_ENCRYPT_TYPE_WEP104:
		case eCSR_ENCRYPT_TYPE_TKIP:
		case eCSR_ENCRYPT_TYPE_AES:
			fRSNProfile = true;
			break;

		default:
			fRSNProfile = false;
			break;
		}
	}
	return fRSNProfile;
}

/**
 * csr_isconcurrentsession_valid() - check if concurrent session is valid
 * @mac_ctx: pointer to mac context
 * @cur_sessionid: current session id
 * @cur_bss_persona: current BSS persona
 *
 * This function will check if concurrent session is valid
 *
 * Return: CDF_STATUS
 */
CDF_STATUS
csr_isconcurrentsession_valid(tpAniSirGlobal mac_ctx, uint32_t cur_sessionid,
			      tCDF_CON_MODE cur_bss_persona)
{
	uint32_t sessionid = 0;
	tCDF_CON_MODE bss_persona;
	eCsrConnectState connect_state, temp;
	tCsrRoamSession *roam_session;

	for (sessionid = 0; sessionid < CSR_ROAM_SESSION_MAX; sessionid++) {
		if (cur_sessionid == sessionid)
			continue;
		if (!CSR_IS_SESSION_VALID(mac_ctx, sessionid))
			continue;
		roam_session = &mac_ctx->roam.roamSession[sessionid];
		bss_persona = roam_session->bssParams.bssPersona;
		connect_state = roam_session->connectState;

		switch (cur_bss_persona) {
		case CDF_STA_MODE:
			CDF_TRACE(CDF_MODULE_ID_SME,
					CDF_TRACE_LEVEL_INFO,
					FL("** STA session **"));
			return CDF_STATUS_SUCCESS;

		case CDF_SAP_MODE:
			temp = eCSR_ASSOC_STATE_TYPE_IBSS_DISCONNECTED;
#ifndef WLAN_FEATURE_MBSSID
			if ((bss_persona == CDF_SAP_MODE) &&
					(connect_state !=
					 eCSR_ASSOC_STATE_TYPE_NOT_CONNECTED)) {
				CDF_TRACE(CDF_MODULE_ID_SME,
						CDF_TRACE_LEVEL_ERROR,
						FL("sap mode already exist"));
				return CDF_STATUS_E_FAILURE;
			} else
#endif
				if ((bss_persona == CDF_IBSS_MODE)
					&& (connect_state != temp)) {
					CDF_TRACE(CDF_MODULE_ID_SME,
							CDF_TRACE_LEVEL_ERROR,
							FL("Can't start GO"));
					return CDF_STATUS_E_FAILURE;
				}
			break;

		case CDF_P2P_GO_MODE:
			temp = eCSR_ASSOC_STATE_TYPE_IBSS_DISCONNECTED;
#ifndef WLAN_FEATURE_MBSSID
			if ((bss_persona == CDF_P2P_GO_MODE) &&
					(connect_state !=
					 eCSR_ASSOC_STATE_TYPE_NOT_CONNECTED)) {
				CDF_TRACE(CDF_MODULE_ID_SME,
						CDF_TRACE_LEVEL_ERROR,
						FL("GO mode already exists"));
				return CDF_STATUS_E_FAILURE;
			}
#endif
			if ((bss_persona == CDF_IBSS_MODE)
					&& (connect_state != temp)) {
				CDF_TRACE(CDF_MODULE_ID_SME,
						CDF_TRACE_LEVEL_ERROR,
						FL("Can't start SAP"));
				return CDF_STATUS_E_FAILURE;
			}
			break;
		case CDF_IBSS_MODE:
			if ((bss_persona == CDF_IBSS_MODE) && (connect_state !=
					eCSR_ASSOC_STATE_TYPE_IBSS_CONNECTED)) {
				CDF_TRACE(CDF_MODULE_ID_SME,
						CDF_TRACE_LEVEL_ERROR,
						FL("IBSS mode already exist"));
				return CDF_STATUS_E_FAILURE;
			} else if (((bss_persona == CDF_P2P_GO_MODE) ||
					(bss_persona == CDF_SAP_MODE)) &&
					(connect_state !=
					 eCSR_ASSOC_STATE_TYPE_NOT_CONNECTED)) {
				CDF_TRACE(CDF_MODULE_ID_SME,
						CDF_TRACE_LEVEL_ERROR,
						FL("Can't start GO"));
				return CDF_STATUS_E_FAILURE;
			}
			break;
		case CDF_P2P_CLIENT_MODE:
			CDF_TRACE(CDF_MODULE_ID_SME,
				CDF_TRACE_LEVEL_INFO,
				FL("**P2P-Client session**"));
			return CDF_STATUS_SUCCESS;
		default:
			CDF_TRACE(CDF_MODULE_ID_SME,
				CDF_TRACE_LEVEL_ERROR,
				FL("Persona not handled = %d"),
				cur_bss_persona);
			break;
		}
	}
	return CDF_STATUS_SUCCESS;

}

/**
 * csr_update_mcc_p2p_beacon_interval() - update p2p beacon interval
 * @mac_ctx: pointer to mac context
 *
 * This function is to update the mcc p2p beacon interval
 *
 * Return: CDF_STATUS
 */
CDF_STATUS csr_update_mcc_p2p_beacon_interval(tpAniSirGlobal mac_ctx)
{
	uint32_t session_id = 0;
	tCsrRoamSession *roam_session;

	/* If MCC is not supported just break and return SUCCESS */
	if (!mac_ctx->roam.configParam.fenableMCCMode)
		return CDF_STATUS_E_FAILURE;

	for (session_id = 0; session_id < CSR_ROAM_SESSION_MAX; session_id++) {
		/*
		 * If GO in MCC support different beacon interval,
		 * change the BI of the P2P-GO
		 */
		roam_session = &mac_ctx->roam.roamSession[session_id];
		if (roam_session->bssParams.bssPersona != CDF_P2P_GO_MODE)
			continue;
		/*
		 * Handle different BI scneario based on the
		 * configuration set.If Config is set to 0x02 then
		 * Disconnect all the P2P clients associated. If config
		 * is set to 0x04 then update the BI without
		 * disconnecting all the clients
		 */
		if ((mac_ctx->roam.configParam.fAllowMCCGODiffBI == 0x04)
				&& (roam_session->bssParams.
					updatebeaconInterval)) {
			return csr_send_chng_mcc_beacon_interval(mac_ctx,
					session_id);
		} else if (roam_session->bssParams.updatebeaconInterval) {
			/*
			 * If the configuration of fAllowMCCGODiffBI is set to
			 * other than 0x04
			 */
			return csr_roam_call_callback(mac_ctx,
					session_id,
					NULL, 0,
					eCSR_ROAM_DISCONNECT_ALL_P2P_CLIENTS,
					eCSR_ROAM_RESULT_NONE);
		}
	}
	return CDF_STATUS_E_FAILURE;
}

uint16_t csr_calculate_mcc_beacon_interval(tpAniSirGlobal pMac, uint16_t sta_bi,
					   uint16_t go_gbi)
{
	uint8_t num_beacons = 0;
	uint8_t is_multiple = 0;
	uint16_t go_cbi = 0;
	uint16_t go_fbi = 0;
	uint16_t sta_cbi = 0;

	/* If GO's given beacon Interval is less than 100 */
	if (go_gbi < 100)
		go_cbi = 100;
	/* if GO's given beacon Interval is greater than or equal to 100 */
	else
		go_cbi = 100 + (go_gbi % 100);

	if (sta_bi == 0) {
		/* There is possibility to receive zero as value.
		   Which will cause divide by zero. Hence initialise with 100
		 */
		sta_bi = 100;
		sms_log(pMac, LOGW,
			FL("sta_bi 2nd parameter is zero, initialize to %d"),
			sta_bi);
	}
	/* check, if either one is multiple of another */
	if (sta_bi > go_cbi) {
		is_multiple = !(sta_bi % go_cbi);
	} else {
		is_multiple = !(go_cbi % sta_bi);
	}
	/* if it is multiple, then accept GO's beacon interval range [100,199] as it  is */
	if (is_multiple) {
		return go_cbi;
	}
	/* else , if it is not multiple, then then check for number of beacons to be */
	/* inserted based on sta BI */
	num_beacons = sta_bi / 100;
	if (num_beacons) {
		/* GO's final beacon interval will be aligned to sta beacon interval, but */
		/* in the range of [100, 199]. */
		sta_cbi = sta_bi / num_beacons;
		go_fbi = sta_cbi;
	} else {
		/* if STA beacon interval is less than 100, use GO's change bacon interval */
		/* instead of updating to STA's beacon interval. */
		go_fbi = go_cbi;
	}
	return go_fbi;
}

CDF_STATUS csr_validate_mcc_beacon_interval(tpAniSirGlobal pMac, uint8_t channelId,
					    uint16_t *beaconInterval,
					    uint32_t cursessionId,
					    tCDF_CON_MODE currBssPersona)
{
	uint32_t sessionId = 0;
	uint16_t new_beaconInterval = 0;

	/* If MCC is not supported just break */
	if (!pMac->roam.configParam.fenableMCCMode) {
		return CDF_STATUS_E_FAILURE;
	}

	for (sessionId = 0; sessionId < CSR_ROAM_SESSION_MAX; sessionId++) {
		if (cursessionId != sessionId) {
			if (!CSR_IS_SESSION_VALID(pMac, sessionId)) {
				continue;
			}

			switch (currBssPersona) {
			case CDF_STA_MODE:
				if (pMac->roam.roamSession[sessionId].
					pCurRoamProfile &&
				   (pMac->roam.roamSession[sessionId].
					pCurRoamProfile->csrPersona ==
				    CDF_P2P_CLIENT_MODE)) {
					/* check for P2P client mode */
					sms_log(pMac, LOG1,
						FL
							(" Beacon Interval Validation not required for STA/CLIENT"));
				}
				/*
				 * IF SAP has started and STA wants to connect
				 * on different channel MCC should
				 *  MCC should not be enabled so making it
				 * false to enforce on same channel
				 */
				else if (pMac->roam.roamSession[sessionId].
					 bssParams.bssPersona ==
					 CDF_SAP_MODE) {
					if (pMac->roam.roamSession[sessionId].
					    bssParams.operationChn !=
					    channelId) {
						sms_log(pMac, LOGE,
							FL
								("*** MCC with SAP+STA sessions ****"));
						return CDF_STATUS_SUCCESS;
					}
				} else if (pMac->roam.roamSession[sessionId].
						bssParams.bssPersona ==
						CDF_P2P_GO_MODE) {
					/*
					 * Check for P2P go scenario
					 * if GO in MCC support different
					 * beacon interval,
					 * change the BI of the P2P-GO
					 */
					if ((pMac->roam.roamSession[sessionId].
					     bssParams.operationChn !=
					     channelId)
					    && (pMac->roam.
						roamSession[sessionId].
						bssParams.beaconInterval !=
						*beaconInterval)) {
						/* if GO in MCC support different beacon interval, return success */
						if (pMac->roam.configParam.
						    fAllowMCCGODiffBI == 0x01) {
							return
								CDF_STATUS_SUCCESS;
						}
						/* Send only Broadcast disassoc and update beaconInterval */
						/* If configuration is set to 0x04 then dont */
						/* disconnect all the station */
						else if ((pMac->roam.
							  configParam.
							  fAllowMCCGODiffBI ==
							  0x02)
							 || (pMac->roam.
							     configParam.
							     fAllowMCCGODiffBI
							     == 0x04)) {
							/* Check to pass the right beacon Interval */
							 if (pMac->roam.configParam.conc_custom_rule1 ||
								pMac->roam.configParam.conc_custom_rule2) {
								 new_beaconInterval = CSR_CUSTOM_CONC_GO_BI;
							 } else {
								 new_beaconInterval =
									 csr_calculate_mcc_beacon_interval(pMac,
										*beaconInterval,
										pMac->roam.
										roamSession
										[sessionId].
										bssParams.
										beaconInterval);
							 }
							sms_log(pMac, LOG1,
								FL
									(" Peer AP BI : %d, new Beacon Interval: %d"),
								*beaconInterval,
								new_beaconInterval);
							/* Update the becon Interval */
							if (new_beaconInterval
							    !=
							    pMac->roam.
							    roamSession
							    [sessionId].
							    bssParams.
							    beaconInterval) {
								/* Update the beaconInterval now */
								sms_log(pMac,
									LOGE,
									FL
										(" Beacon Interval got changed config used: %d\n"),
									pMac->
									roam.
									configParam.
									fAllowMCCGODiffBI);

								pMac->roam.
								roamSession
								[sessionId].
								bssParams.
								beaconInterval
									=
										new_beaconInterval;
								pMac->roam.
								roamSession
								[sessionId].
								bssParams.
								updatebeaconInterval
									= true;
								return
									csr_update_mcc_p2p_beacon_interval
										(pMac);
							}
							return
								CDF_STATUS_SUCCESS;
						}
						/* Disconnect the P2P session */
						else if (pMac->roam.configParam.
							 fAllowMCCGODiffBI ==
							 0x03) {
							pMac->roam.roamSession[sessionId].bssParams.updatebeaconInterval = false;
							return
								csr_roam_call_callback
									(pMac, sessionId,
									NULL, 0,
									eCSR_ROAM_SEND_P2P_STOP_BSS,
									eCSR_ROAM_RESULT_NONE);
						} else {
							sms_log(pMac, LOGE,
								FL
									("BeaconInterval is different cannot connect to preferred AP..."));
							return
								CDF_STATUS_E_FAILURE;
						}
					}
				}
				break;

			case CDF_P2P_CLIENT_MODE:
				if (pMac->roam.roamSession[sessionId].
					pCurRoamProfile &&
				   (pMac->roam.roamSession[sessionId].
					pCurRoamProfile->csrPersona ==
					CDF_STA_MODE)) {
					/* check for P2P client mode */
					sms_log(pMac, LOG1,
						FL
							(" Ignore Beacon Interval Validation..."));
				} else if (pMac->roam.roamSession[sessionId].
						bssParams.bssPersona ==
						CDF_P2P_GO_MODE) {
					/* Check for P2P go scenario */
					if ((pMac->roam.roamSession[sessionId].
					     bssParams.operationChn !=
					     channelId)
					    && (pMac->roam.
						roamSession[sessionId].
						bssParams.beaconInterval !=
						*beaconInterval)) {
						sms_log(pMac, LOGE,
							FL
								("BeaconInterval is different cannot connect to P2P_GO network ..."));
						return CDF_STATUS_E_FAILURE;
					}
				}
				break;

			case CDF_SAP_MODE:
				break;

			case CDF_P2P_GO_MODE:
			{
				if (pMac->roam.roamSession[sessionId].
					pCurRoamProfile &&
				   ((pMac->roam.roamSession[sessionId].
					pCurRoamProfile->csrPersona ==
					CDF_P2P_CLIENT_MODE) ||
				    (pMac->roam.roamSession[sessionId].
					pCurRoamProfile->csrPersona ==
					CDF_STA_MODE))) {
					/* check for P2P_client scenario */
					if ((pMac->roam.
					     roamSession[sessionId].
					     connectedProfile.
					     operationChannel == 0)
					    && (pMac->roam.
						roamSession[sessionId].
						connectedProfile.
						beaconInterval == 0)) {
						continue;
					}

					if (csr_is_conn_state_connected_infra
						    (pMac, sessionId)
					    && (pMac->roam.
						roamSession[sessionId].
						connectedProfile.
						operationChannel !=
						channelId)
					    && (pMac->roam.
						roamSession[sessionId].
						connectedProfile.
						beaconInterval !=
						*beaconInterval)) {
						/*
						 * Updated beaconInterval should be used only when we are starting a new BSS
						 * not incase of client or STA case
						 */
						/* Calculate beacon Interval for P2P-GO incase of MCC */
						if (pMac->roam.configParam.conc_custom_rule1 ||
							pMac->roam.configParam.conc_custom_rule2) {
							new_beaconInterval = CSR_CUSTOM_CONC_GO_BI;
						} else {
							new_beaconInterval =
								csr_calculate_mcc_beacon_interval
								(pMac,
								 pMac->roam.
								 roamSession
								 [sessionId].
								 connectedProfile.
								 beaconInterval,
								 *beaconInterval);
						}
						if (*beaconInterval !=
						    new_beaconInterval)
							*beaconInterval
								=
									new_beaconInterval;
						return
							CDF_STATUS_SUCCESS;
					}
				}
			}
			break;

			default:
				sms_log(pMac, LOGE,
					FL(" Persona not supported : %d"),
					currBssPersona);
				return CDF_STATUS_E_FAILURE;
			}
		}
	}

	return CDF_STATUS_SUCCESS;
}

#ifdef WLAN_FEATURE_VOWIFI_11R
/* Function to return true if the authtype is 11r */
bool csr_is_auth_type11r(eCsrAuthType AuthType, uint8_t mdiePresent)
{
	switch (AuthType) {
	case eCSR_AUTH_TYPE_OPEN_SYSTEM:
		if (mdiePresent)
			return true;
		break;
	case eCSR_AUTH_TYPE_FT_RSN_PSK:
	case eCSR_AUTH_TYPE_FT_RSN:
		return true;
		break;
	default:
		break;
	}
	return false;
}

/* Function to return true if the profile is 11r */
bool csr_is_profile11r(tCsrRoamProfile *pProfile)
{
	return csr_is_auth_type11r(pProfile->negotiatedAuthType,
				   pProfile->MDID.mdiePresent);
}

#endif

#ifdef FEATURE_WLAN_ESE

/* Function to return true if the authtype is ESE */
bool csr_is_auth_type_ese(eCsrAuthType AuthType)
{
	switch (AuthType) {
	case eCSR_AUTH_TYPE_CCKM_WPA:
	case eCSR_AUTH_TYPE_CCKM_RSN:
		return true;
		break;
	default:
		break;
	}
	return false;
}

/* Function to return true if the profile is ESE */
bool csr_is_profile_ese(tCsrRoamProfile *pProfile)
{
	return csr_is_auth_type_ese(pProfile->negotiatedAuthType);
}

#endif

#ifdef FEATURE_WLAN_WAPI
bool csr_is_profile_wapi(tCsrRoamProfile *pProfile)
{
	bool fWapiProfile = false;

	switch (pProfile->negotiatedAuthType) {
	case eCSR_AUTH_TYPE_WAPI_WAI_CERTIFICATE:
	case eCSR_AUTH_TYPE_WAPI_WAI_PSK:
		fWapiProfile = true;
		break;

	default:
		fWapiProfile = false;
		break;
	}

	if (fWapiProfile) {
		switch (pProfile->negotiatedUCEncryptionType) {
		case eCSR_ENCRYPT_TYPE_WPI:
			fWapiProfile = true;
			break;

		default:
			fWapiProfile = false;
			break;
		}
	}
	return fWapiProfile;
}

static bool csr_is_wapi_oui_equal(tpAniSirGlobal pMac, uint8_t *Oui1,
				  uint8_t *Oui2)
{
	return cdf_mem_compare(Oui1, Oui2, CSR_WAPI_OUI_SIZE);
}

static bool csr_is_wapi_oui_match(tpAniSirGlobal pMac,
				  uint8_t AllCyphers[][CSR_WAPI_OUI_SIZE],
				  uint8_t cAllCyphers, uint8_t Cypher[],
				  uint8_t Oui[])
{
	bool fYes = false;
	uint8_t idx;

	for (idx = 0; idx < cAllCyphers; idx++) {
		if (csr_is_wapi_oui_equal(pMac, AllCyphers[idx], Cypher)) {
			fYes = true;
			break;
		}
	}

	if (fYes && Oui) {
		cdf_mem_copy(Oui, AllCyphers[idx], CSR_WAPI_OUI_SIZE);
	}

	return fYes;
}
#endif /* FEATURE_WLAN_WAPI */

static bool csr_is_wpa_oui_equal(tpAniSirGlobal pMac, uint8_t *Oui1,
				 uint8_t *Oui2)
{
	return cdf_mem_compare(Oui1, Oui2, CSR_WPA_OUI_SIZE);
}

static bool csr_is_oui_match(tpAniSirGlobal pMac,
			     uint8_t AllCyphers[][CSR_WPA_OUI_SIZE],
			     uint8_t cAllCyphers, uint8_t Cypher[], uint8_t Oui[])
{
	bool fYes = false;
	uint8_t idx;

	for (idx = 0; idx < cAllCyphers; idx++) {
		if (csr_is_wpa_oui_equal(pMac, AllCyphers[idx], Cypher)) {
			fYes = true;
			break;
		}
	}

	if (fYes && Oui) {
		cdf_mem_copy(Oui, AllCyphers[idx], CSR_WPA_OUI_SIZE);
	}

	return fYes;
}

static bool csr_match_rsnoui_index(tpAniSirGlobal pMac,
				   uint8_t AllCyphers[][CSR_RSN_OUI_SIZE],
				   uint8_t cAllCyphers, uint8_t ouiIndex,
				   uint8_t Oui[])
{
	return csr_is_oui_match
		(pMac, AllCyphers, cAllCyphers, csr_rsn_oui[ouiIndex], Oui);

}

#ifdef FEATURE_WLAN_WAPI
static bool csr_match_wapi_oui_index(tpAniSirGlobal pMac,
				     uint8_t AllCyphers[][CSR_WAPI_OUI_SIZE],
				     uint8_t cAllCyphers, uint8_t ouiIndex,
				     uint8_t Oui[])
{
	return csr_is_wapi_oui_match
		(pMac, AllCyphers, cAllCyphers, csr_wapi_oui[ouiIndex], Oui);

}
#endif /* FEATURE_WLAN_WAPI */

static bool csr_match_wpaoui_index(tpAniSirGlobal pMac,
				   uint8_t AllCyphers[][CSR_RSN_OUI_SIZE],
				   uint8_t cAllCyphers, uint8_t ouiIndex,
				   uint8_t Oui[])
{
	return csr_is_oui_match
		(pMac, AllCyphers, cAllCyphers, csr_wpa_oui[ouiIndex], Oui);

}

#ifdef FEATURE_WLAN_WAPI
static bool csr_is_auth_wapi_cert(tpAniSirGlobal pMac,
				  uint8_t AllSuites[][CSR_WAPI_OUI_SIZE],
				  uint8_t cAllSuites, uint8_t Oui[])
{
	return csr_is_wapi_oui_match
		(pMac, AllSuites, cAllSuites, csr_wapi_oui[1], Oui);
}

static bool csr_is_auth_wapi_psk(tpAniSirGlobal pMac,
				 uint8_t AllSuites[][CSR_WAPI_OUI_SIZE],
				 uint8_t cAllSuites, uint8_t Oui[])
{
	return csr_is_wapi_oui_match
		(pMac, AllSuites, cAllSuites, csr_wapi_oui[2], Oui);
}
#endif /* FEATURE_WLAN_WAPI */

#ifdef WLAN_FEATURE_VOWIFI_11R

/*
 * Function for 11R FT Authentication. We match the FT Authentication Cipher
 * suite here. This matches for FT Auth with the 802.1X exchange.
 */
static bool csr_is_ft_auth_rsn(tpAniSirGlobal pMac,
			       uint8_t AllSuites[][CSR_RSN_OUI_SIZE],
			       uint8_t cAllSuites, uint8_t Oui[])
{
	return csr_is_oui_match
		(pMac, AllSuites, cAllSuites, csr_rsn_oui[03], Oui);
}

/*
 * Function for 11R FT Authentication. We match the FT Authentication Cipher
 * suite here. This matches for FT Auth with the PSK.
 */
static bool csr_is_ft_auth_rsn_psk(tpAniSirGlobal pMac,
				   uint8_t AllSuites[][CSR_RSN_OUI_SIZE],
				   uint8_t cAllSuites, uint8_t Oui[])
{
	return csr_is_oui_match
		(pMac, AllSuites, cAllSuites, csr_rsn_oui[04], Oui);
}

#endif

#ifdef FEATURE_WLAN_ESE

/*
 * Function for ESE CCKM AKM Authentication. We match the CCKM AKM
 * Authentication Key Management suite here. This matches for CCKM AKM Auth
 * with the 802.1X exchange.
 */
static bool csr_is_ese_cckm_auth_rsn(tpAniSirGlobal pMac,
				     uint8_t AllSuites[][CSR_RSN_OUI_SIZE],
				     uint8_t cAllSuites, uint8_t Oui[])
{
	return csr_is_oui_match
		(pMac, AllSuites, cAllSuites, csr_rsn_oui[06], Oui);
}

static bool csr_is_ese_cckm_auth_wpa(tpAniSirGlobal pMac,
				     uint8_t AllSuites[][CSR_WPA_OUI_SIZE],
				     uint8_t cAllSuites, uint8_t Oui[])
{
	return csr_is_oui_match
		(pMac, AllSuites, cAllSuites, csr_wpa_oui[06], Oui);
}

#endif

static bool csr_is_auth_rsn(tpAniSirGlobal pMac,
			    uint8_t AllSuites[][CSR_RSN_OUI_SIZE],
			    uint8_t cAllSuites, uint8_t Oui[])
{
	return csr_is_oui_match
		(pMac, AllSuites, cAllSuites, csr_rsn_oui[01], Oui);
}

static bool csr_is_auth_rsn_psk(tpAniSirGlobal pMac,
				uint8_t AllSuites[][CSR_RSN_OUI_SIZE],
				uint8_t cAllSuites, uint8_t Oui[])
{
	return csr_is_oui_match
		(pMac, AllSuites, cAllSuites, csr_rsn_oui[02], Oui);
}

#ifdef WLAN_FEATURE_11W
static bool csr_is_auth_rsn_psk_sha256(tpAniSirGlobal pMac,
				       uint8_t AllSuites[][CSR_RSN_OUI_SIZE],
				       uint8_t cAllSuites, uint8_t Oui[])
{
	return csr_is_oui_match
		(pMac, AllSuites, cAllSuites, csr_rsn_oui[07], Oui);
}
static bool csr_is_auth_rsn8021x_sha256(tpAniSirGlobal pMac,
					uint8_t AllSuites[][CSR_RSN_OUI_SIZE],
					uint8_t cAllSuites, uint8_t Oui[])
{
	return csr_is_oui_match
		(pMac, AllSuites, cAllSuites, csr_rsn_oui[8], Oui);
}
#endif

static bool csr_is_auth_wpa(tpAniSirGlobal pMac,
			    uint8_t AllSuites[][CSR_WPA_OUI_SIZE],
			    uint8_t cAllSuites, uint8_t Oui[])
{
	return csr_is_oui_match
		(pMac, AllSuites, cAllSuites, csr_wpa_oui[01], Oui);
}

static bool csr_is_auth_wpa_psk(tpAniSirGlobal pMac,
				uint8_t AllSuites[][CSR_WPA_OUI_SIZE],
				uint8_t cAllSuites, uint8_t Oui[])
{
	return csr_is_oui_match
		(pMac, AllSuites, cAllSuites, csr_wpa_oui[02], Oui);
}

uint8_t csr_get_oui_index_from_cipher(eCsrEncryptionType enType)
{
	uint8_t OUIIndex;

	switch (enType) {
	case eCSR_ENCRYPT_TYPE_WEP40:
	case eCSR_ENCRYPT_TYPE_WEP40_STATICKEY:
		OUIIndex = CSR_OUI_WEP40_OR_1X_INDEX;
		break;
	case eCSR_ENCRYPT_TYPE_WEP104:
	case eCSR_ENCRYPT_TYPE_WEP104_STATICKEY:
		OUIIndex = CSR_OUI_WEP104_INDEX;
		break;
	case eCSR_ENCRYPT_TYPE_TKIP:
		OUIIndex = CSR_OUI_TKIP_OR_PSK_INDEX;
		break;
	case eCSR_ENCRYPT_TYPE_AES:
		OUIIndex = CSR_OUI_AES_INDEX;
		break;
	case eCSR_ENCRYPT_TYPE_NONE:
		OUIIndex = CSR_OUI_USE_GROUP_CIPHER_INDEX;
		break;
#ifdef FEATURE_WLAN_WAPI
	case eCSR_ENCRYPT_TYPE_WPI:
		OUIIndex = CSR_OUI_WAPI_WAI_CERT_OR_SMS4_INDEX;
		break;
#endif /* FEATURE_WLAN_WAPI */
	default:                /* HOWTO handle this? */
		OUIIndex = CSR_OUI_RESERVED_INDEX;
		break;
	} /* switch */

	return OUIIndex;
}
/**
 * csr_get_rsn_information() - to get RSN infomation
 * @hal: pointer to HAL
 * @auth_type: auth type
 * @encr_type: encryption type
 * @mc_encryption: multicast encryption type
 * @rsn_ie: pointer to RSN IE
 * @ucast_cipher: Unicast cipher
 * @mcast_cipher: Multicast cipher
 * @auth_suite: Authentication suite
 * @capabilities: RSN capabilities
 * @negotiated_authtype: Negotiated auth type
 * @negotiated_mccipher: negotiated multicast cipher
 *
 * This routine will get all RSN information
 *
 * Return: bool
 */
bool csr_get_rsn_information(tHalHandle hal, tCsrAuthList *auth_type,
			     eCsrEncryptionType encr_type,
			     tCsrEncryptionList *mc_encryption,
			     tDot11fIERSN *rsn_ie, uint8_t *ucast_cipher,
			     uint8_t *mcast_cipher, uint8_t *auth_suite,
			     tCsrRSNCapabilities *capabilities,
			     eCsrAuthType *negotiated_authtype,
			     eCsrEncryptionType *negotiated_mccipher)
{
	tpAniSirGlobal mac_ctx = PMAC_STRUCT(hal);
	bool acceptable_cipher = false;
	uint8_t c_ucast_cipher = 0;
	uint8_t c_mcast_cipher = 0;
	uint8_t c_auth_suites = 0, i;
	uint8_t unicast[CSR_RSN_OUI_SIZE];
	uint8_t multicast[CSR_RSN_OUI_SIZE];
	uint8_t authsuites[CSR_RSN_MAX_AUTH_SUITES][CSR_RSN_OUI_SIZE];
	uint8_t authentication[CSR_RSN_OUI_SIZE];
	uint8_t mccipher_arr[CSR_RSN_MAX_MULTICAST_CYPHERS][CSR_RSN_OUI_SIZE];
	eCsrAuthType neg_authtype = eCSR_AUTH_TYPE_UNKNOWN;

	if (!rsn_ie->present)
		goto end;
	c_mcast_cipher++;
	cdf_mem_copy(mccipher_arr, rsn_ie->gp_cipher_suite,
			CSR_RSN_OUI_SIZE);
	c_ucast_cipher =
		(uint8_t) (rsn_ie->pwise_cipher_suite_count);
	c_auth_suites = (uint8_t) (rsn_ie->akm_suite_count);
	for (i = 0; i < c_auth_suites && i < CSR_RSN_MAX_AUTH_SUITES; i++) {
		cdf_mem_copy((void *)&authsuites[i],
			(void *)&rsn_ie->akm_suites[i], CSR_RSN_OUI_SIZE);
	}

	/* Check - Is requested unicast Cipher supported by the BSS. */
	acceptable_cipher = csr_match_rsnoui_index(mac_ctx,
				rsn_ie->pwise_cipher_suites, c_ucast_cipher,
				csr_get_oui_index_from_cipher(encr_type),
				unicast);

	if (!acceptable_cipher)
		goto end;

	/* unicast is supported. Pick the first matching Group cipher, if any */
	for (i = 0; i < mc_encryption->numEntries; i++) {
		acceptable_cipher = csr_match_rsnoui_index(mac_ctx,
					mccipher_arr, c_mcast_cipher,
					csr_get_oui_index_from_cipher(
					    mc_encryption->encryptionType[i]),
					multicast);
		if (acceptable_cipher)
			break;
	}
	if (!acceptable_cipher)
		goto end;

	if (negotiated_mccipher)
		*negotiated_mccipher = mc_encryption->encryptionType[i];

	/* Initializing with false as it has true value already */
	acceptable_cipher = false;
	for (i = 0; i < auth_type->numEntries; i++) {
		/*
		 * Ciphers are supported, Match authentication algorithm and
		 * pick first matching authtype.
		 */
#ifdef WLAN_FEATURE_VOWIFI_11R
		/* Changed the AKM suites according to order of preference */
		if (csr_is_ft_auth_rsn(mac_ctx, authsuites,
					c_auth_suites, authentication)) {
			if (eCSR_AUTH_TYPE_FT_RSN == auth_type->authType[i])
				neg_authtype = eCSR_AUTH_TYPE_FT_RSN;
		}
		if ((neg_authtype == eCSR_AUTH_TYPE_UNKNOWN)
				&& csr_is_ft_auth_rsn_psk(mac_ctx, authsuites,
					c_auth_suites, authentication)) {
			if (eCSR_AUTH_TYPE_FT_RSN_PSK ==
					auth_type->authType[i])
				neg_authtype = eCSR_AUTH_TYPE_FT_RSN_PSK;
		}
#endif
#ifdef FEATURE_WLAN_ESE
		/* ESE only supports 802.1X.  No PSK. */
		if ((neg_authtype == eCSR_AUTH_TYPE_UNKNOWN) &&
				csr_is_ese_cckm_auth_rsn(mac_ctx, authsuites,
					c_auth_suites, authentication)) {
			if (eCSR_AUTH_TYPE_CCKM_RSN == auth_type->authType[i])
				neg_authtype = eCSR_AUTH_TYPE_CCKM_RSN;
		}
#endif
		if ((neg_authtype == eCSR_AUTH_TYPE_UNKNOWN)
				&& csr_is_auth_rsn(mac_ctx, authsuites,
					c_auth_suites, authentication)) {
			if (eCSR_AUTH_TYPE_RSN == auth_type->authType[i])
				neg_authtype = eCSR_AUTH_TYPE_RSN;
		}
		if ((neg_authtype == eCSR_AUTH_TYPE_UNKNOWN)
				&& csr_is_auth_rsn_psk(mac_ctx, authsuites,
					c_auth_suites, authentication)) {
			if (eCSR_AUTH_TYPE_RSN_PSK == auth_type->authType[i])
				neg_authtype = eCSR_AUTH_TYPE_RSN_PSK;
		}
#ifdef WLAN_FEATURE_11W
		if ((neg_authtype == eCSR_AUTH_TYPE_UNKNOWN)
			&& csr_is_auth_rsn_psk_sha256(mac_ctx, authsuites,
					c_auth_suites, authentication)) {
			if (eCSR_AUTH_TYPE_RSN_PSK_SHA256 ==
					auth_type->authType[i])
				neg_authtype = eCSR_AUTH_TYPE_RSN_PSK_SHA256;
		}
		if ((neg_authtype == eCSR_AUTH_TYPE_UNKNOWN) &&
				csr_is_auth_rsn8021x_sha256(mac_ctx, authsuites,
					c_auth_suites, authentication)) {
			if (eCSR_AUTH_TYPE_RSN_8021X_SHA256 ==
					auth_type->authType[i])
				neg_authtype = eCSR_AUTH_TYPE_RSN_8021X_SHA256;
		}
#endif

		/*
		 * The 1st auth type in the APs RSN IE, to match stations
		 * connecting profiles auth type will cause us to exit this
		 * loop. This is added as some APs advertise multiple akms in
		 * the RSN IE
		 */
		if (eCSR_AUTH_TYPE_UNKNOWN != neg_authtype) {
			acceptable_cipher = true;
			break;
		}
	} /* for */
end:
	if (acceptable_cipher) {
		if (mcast_cipher)
			cdf_mem_copy(mcast_cipher, multicast,
					CSR_RSN_OUI_SIZE);

		if (ucast_cipher)
			cdf_mem_copy(ucast_cipher, unicast, CSR_RSN_OUI_SIZE);

		if (auth_suite)
			cdf_mem_copy(auth_suite, authentication,
					CSR_RSN_OUI_SIZE);

		if (negotiated_authtype)
			*negotiated_authtype = neg_authtype;

		if (capabilities) {
			/* Bit 0 Preauthentication */
			capabilities->PreAuthSupported =
				(rsn_ie->RSN_Cap[0] >> 0) & 0x1;
			/* Bit 1 No Pairwise */
			capabilities->NoPairwise =
				(rsn_ie->RSN_Cap[0] >> 1) & 0x1;
			/* Bit 2, 3 PTKSA Replay Counter */
			capabilities->PTKSAReplayCounter =
				(rsn_ie->RSN_Cap[0] >> 2) & 0x3;
			/* Bit 4, 5 GTKSA Replay Counter */
			capabilities->GTKSAReplayCounter =
				(rsn_ie->RSN_Cap[0] >> 4) & 0x3;
#ifdef WLAN_FEATURE_11W
			/* Bit 6 MFPR */
			capabilities->MFPRequired =
				(rsn_ie->RSN_Cap[0] >> 6) & 0x1;
			/* Bit 7 MFPC */
			capabilities->MFPCapable =
				(rsn_ie->RSN_Cap[0] >> 7) & 0x1;
#else
			/* Bit 6 MFPR */
			capabilities->MFPRequired = 0;
			/* Bit 7 MFPC */
			capabilities->MFPCapable = 0;
#endif
			/* remaining reserved */
			capabilities->Reserved = rsn_ie->RSN_Cap[1] & 0xff;
		}
	}
	return acceptable_cipher;
}

#ifdef WLAN_FEATURE_11W
/**
 * csr_is_pmf_capabilities_in_rsn_match() - check for PMF capability
 * @hHal:                  Global HAL handle
 * @pFilterMFPEnabled:     given by supplicant to us to specify what kind
 *                         of connection supplicant is expecting to make
 *                         if it is enabled then make PMF connection.
 *                         if it is disabled then make normal connection.
 * @pFilterMFPRequired:    given by supplicant based on our configuration
 *                         if it is 1 then we will require mandatory
 *                         PMF connection and if it is 0 then we PMF
 *                         connection is optional.
 * @pFilterMFPCapable:     given by supplicant based on our configuration
 *                         if it 1 then we are PMF capable and if it 0
 *                         then we are not PMF capable.
 * @pRSNIe:                RSNIe from Beacon/probe response of
 *                         neighbor AP against which we will compare
 *                         our capabilities.
 *
 * This function is to match our current capabilities with the AP
 * to which we are expecting make the connection.
 *
 * Return:   if our PMF capabilities matches with AP then we
 *           will return true to indicate that we are good
 *           to make connection with it. Else we will return false
 **/
static bool
csr_is_pmf_capabilities_in_rsn_match(tHalHandle hHal,
				     bool *pFilterMFPEnabled,
				     uint8_t *pFilterMFPRequired,
				     uint8_t *pFilterMFPCapable,
				     tDot11fIERSN *pRSNIe)
{
	uint8_t apProfileMFPCapable = 0;
	uint8_t apProfileMFPRequired = 0;
	if (pRSNIe && pFilterMFPEnabled && pFilterMFPCapable
	    && pFilterMFPRequired) {
		/* Extracting MFPCapable bit from RSN Ie */
		apProfileMFPCapable = (pRSNIe->RSN_Cap[0] >> 7) & 0x1;
		apProfileMFPRequired = (pRSNIe->RSN_Cap[0] >> 6) & 0x1;
		if (*pFilterMFPEnabled && *pFilterMFPCapable
		    && *pFilterMFPRequired && (apProfileMFPCapable == 0)) {
			CDF_TRACE(CDF_MODULE_ID_SME, CDF_TRACE_LEVEL_INFO,
				  "AP is not capable to make PMF connection");
			return false;
		} else if (*pFilterMFPEnabled && *pFilterMFPCapable &&
			   !(*pFilterMFPRequired) &&
				(apProfileMFPCapable == 0)) {
			/*
			 * This is tricky, because supplicant asked us to
			 * make mandatory PMF connection eventhough PMF
			 * connection is optional here.
			 * so if AP is not capable of PMF then drop it.
			 * Don't try to connect with it.
			 */
			CDF_TRACE(CDF_MODULE_ID_SME, CDF_TRACE_LEVEL_INFO,
				  "we need PMF connection & AP isn't capable to make PMF connection");
			return false;
		} else if (!(*pFilterMFPCapable) &&
			   apProfileMFPCapable && apProfileMFPRequired) {

			/*
			 * In this case, AP with whom we trying to connect
			 * requires mandatory PMF connections and we are not
			 * capable so this AP is not good choice to connect
			 */
			CDF_TRACE(CDF_MODULE_ID_SME, CDF_TRACE_LEVEL_INFO,
				  "AP needs PMF connection and we are not capable of pmf connection");
			return false;
		} else if (!(*pFilterMFPEnabled) && *pFilterMFPCapable &&
			   (apProfileMFPCapable == 1)) {
			CDF_TRACE(CDF_MODULE_ID_SME, CDF_TRACE_LEVEL_INFO,
				  "we don't need PMF connection even though both parties are capable");
			return false;
		}
	}
	return true;
}
#endif

bool csr_is_rsn_match(tHalHandle hHal, tCsrAuthList *pAuthType,
		      eCsrEncryptionType enType,
		      tCsrEncryptionList *pEnMcType,
		      bool *pMFPEnabled, uint8_t *pMFPRequired,
		      uint8_t *pMFPCapable,
		      tDot11fBeaconIEs *pIes,
		      eCsrAuthType *pNegotiatedAuthType,
		      eCsrEncryptionType *pNegotiatedMCCipher)
{
	bool fRSNMatch = false;

	/* See if the cyphers in the Bss description match with the settings in the profile. */
	fRSNMatch =
		csr_get_rsn_information(hHal, pAuthType, enType, pEnMcType, &pIes->RSN,
					NULL, NULL, NULL, NULL, pNegotiatedAuthType,
					pNegotiatedMCCipher);
#ifdef WLAN_FEATURE_11W
	/* If all the filter matches then finally checks for PMF capabilities */
	if (fRSNMatch) {
		fRSNMatch = csr_is_pmf_capabilities_in_rsn_match(hHal, pMFPEnabled,
								 pMFPRequired,
								 pMFPCapable,
								 &pIes->RSN);
	}
#endif
	return fRSNMatch;
}

bool csr_lookup_pmkid(tpAniSirGlobal pMac, uint32_t sessionId, uint8_t *pBSSId,
		      uint8_t *pPMKId)
{
	bool fRC = false, fMatchFound = false;
	uint32_t Index;
	tCsrRoamSession *pSession = CSR_GET_SESSION(pMac, sessionId);

	if (!pSession) {
		sms_log(pMac, LOGE, FL("  session %d not found "), sessionId);
		return false;
	}

	do {
		for (Index = 0; Index < CSR_MAX_PMKID_ALLOWED; Index++) {
			sms_log(pMac, LOG1,
				"match PMKID " MAC_ADDRESS_STR " to ",
				MAC_ADDR_ARRAY(pBSSId));
			if (cdf_mem_compare
			    (pBSSId, pSession->PmkidCacheInfo[Index].BSSID.bytes,
			    sizeof(struct cdf_mac_addr))) {
				/* match found */
				fMatchFound = true;
				break;
			}
		}

		if (!fMatchFound)
			break;

		cdf_mem_copy(pPMKId, pSession->PmkidCacheInfo[Index].PMKID,
			     CSR_RSN_PMKID_SIZE);

		fRC = true;
	} while (0);
	sms_log(pMac, LOGW,
		"csr_lookup_pmkid called return match = %d pMac->roam.NumPmkidCache = %d",
		fRC, pSession->NumPmkidCache);

	return fRC;
}

uint8_t csr_construct_rsn_ie(tHalHandle hHal, uint32_t sessionId,
			     tCsrRoamProfile *pProfile,
			     tSirBssDescription *pSirBssDesc,
			     tDot11fBeaconIEs *pIes, tCsrRSNIe *pRSNIe)
{
	tpAniSirGlobal pMac = PMAC_STRUCT(hHal);
	bool fRSNMatch;
	uint8_t cbRSNIe = 0;
	uint8_t UnicastCypher[CSR_RSN_OUI_SIZE];
	uint8_t MulticastCypher[CSR_RSN_OUI_SIZE];
	uint8_t AuthSuite[CSR_RSN_OUI_SIZE];
	tCsrRSNAuthIe *pAuthSuite;
	tCsrRSNCapabilities RSNCapabilities;
	tCsrRSNPMKIe *pPMK;
	uint8_t PMKId[CSR_RSN_PMKID_SIZE];
#ifdef WLAN_FEATURE_11W
	uint8_t *pGroupMgmtCipherSuite;
#endif
	tDot11fBeaconIEs *pIesLocal = pIes;
	eCsrAuthType negAuthType = eCSR_AUTH_TYPE_UNKNOWN;

	sms_log(pMac, LOGW, "%s called...", __func__);

	do {
		if (!csr_is_profile_rsn(pProfile))
			break;

		if (!pIesLocal
		    &&
		    (!CDF_IS_STATUS_SUCCESS
			     (csr_get_parsed_bss_description_ies
				     (pMac, pSirBssDesc, &pIesLocal)))) {
			break;
		}
		/* See if the cyphers in the Bss description match with the settings in the profile. */
		fRSNMatch =
			csr_get_rsn_information(hHal, &pProfile->AuthType,
						pProfile->negotiatedUCEncryptionType,
						&pProfile->mcEncryptionType,
						&pIesLocal->RSN, UnicastCypher,
						MulticastCypher, AuthSuite,
						&RSNCapabilities, &negAuthType, NULL);
		if (!fRSNMatch)
			break;

		pRSNIe->IeHeader.ElementID = SIR_MAC_RSN_EID;

		pRSNIe->Version = CSR_RSN_VERSION_SUPPORTED;

		cdf_mem_copy(pRSNIe->MulticastOui, MulticastCypher,
			     sizeof(MulticastCypher));

		pRSNIe->cUnicastCyphers = 1;

		cdf_mem_copy(&pRSNIe->UnicastOui[0], UnicastCypher,
			     sizeof(UnicastCypher));

		pAuthSuite =
			(tCsrRSNAuthIe *) (&pRSNIe->
					   UnicastOui[pRSNIe->cUnicastCyphers]);

		pAuthSuite->cAuthenticationSuites = 1;
		cdf_mem_copy(&pAuthSuite->AuthOui[0], AuthSuite,
			     sizeof(AuthSuite));

		/* RSN capabilities follows the Auth Suite (two octects) */
		/* !!REVIEW - What should STA put in RSN capabilities, currently */
		/* just putting back APs capabilities */
		/* For one, we shouldn't EVER be sending out "pre-auth supported".  It is an AP only capability */
		/* For another, we should use the Management Frame Protection values given by the supplicant */
		RSNCapabilities.PreAuthSupported = 0;
#ifdef WLAN_FEATURE_11W
		if (RSNCapabilities.MFPCapable && pProfile->MFPCapable) {
			RSNCapabilities.MFPCapable = pProfile->MFPCapable;
			RSNCapabilities.MFPRequired = pProfile->MFPRequired;
		} else {
			RSNCapabilities.MFPCapable = 0;
			RSNCapabilities.MFPRequired = 0;
		}
#endif
		*(uint16_t *) (&pAuthSuite->AuthOui[1]) =
			*((uint16_t *) (&RSNCapabilities));

		pPMK =
			(tCsrRSNPMKIe *) (((uint8_t *) (&pAuthSuite->AuthOui[1])) +
					  sizeof(uint16_t));

		/* Don't include the PMK SA IDs for CCKM associations. */
		if (
#ifdef FEATURE_WLAN_ESE
			(eCSR_AUTH_TYPE_CCKM_RSN != negAuthType) &&
#endif
			csr_lookup_pmkid(pMac, sessionId, pSirBssDesc->bssId,
					 &(PMKId[0]))) {
			pPMK->cPMKIDs = 1;

			cdf_mem_copy(pPMK->PMKIDList[0].PMKID, PMKId,
				     CSR_RSN_PMKID_SIZE);
		} else {
			pPMK->cPMKIDs = 0;
		}

#ifdef WLAN_FEATURE_11W
		if (pProfile->MFPEnabled) {
			pGroupMgmtCipherSuite =
				(uint8_t *) pPMK + sizeof(uint16_t) +
				(pPMK->cPMKIDs * CSR_RSN_PMKID_SIZE);
			cdf_mem_copy(pGroupMgmtCipherSuite, csr_rsn_oui[07],
				     CSR_WPA_OUI_SIZE);
		}
#endif

		/* Add in the fixed fields plus 1 Unicast cypher, less the IE Header length */
		/* Add in the size of the Auth suite (count plus a single OUI) */
		/* Add in the RSN caps field. */
		/* Add PMKID count and PMKID (if any) */
		/* Add group management cipher suite */
		pRSNIe->IeHeader.Length =
			(uint8_t) (sizeof(*pRSNIe) - sizeof(pRSNIe->IeHeader) +
				   sizeof(*pAuthSuite) +
				   sizeof(tCsrRSNCapabilities));
		if (pPMK->cPMKIDs) {
			pRSNIe->IeHeader.Length += (uint8_t) (sizeof(uint16_t) +
							      (pPMK->cPMKIDs *
							       CSR_RSN_PMKID_SIZE));
		}
#ifdef WLAN_FEATURE_11W
		if (pProfile->MFPEnabled) {
			if (0 == pPMK->cPMKIDs)
				pRSNIe->IeHeader.Length += sizeof(uint16_t);
			pRSNIe->IeHeader.Length += CSR_WPA_OUI_SIZE;
		}
#endif

		/* return the size of the IE header (total) constructed... */
		cbRSNIe = pRSNIe->IeHeader.Length + sizeof(pRSNIe->IeHeader);

	} while (0);

	if (!pIes && pIesLocal) {
		/* locally allocated */
		cdf_mem_free(pIesLocal);
	}

	return cbRSNIe;
}

#ifdef FEATURE_WLAN_WAPI
/**
 * csr_get_wapi_information() - to get WAPI infomation
 * @hal: pointer to HAL
 * @auth_type: auth type
 * @encr_type: encryption type
 * @mc_encryption: multicast encryption type
 * @wapi_ie: pointer to WAPI IE
 * @ucast_cipher: Unicast cipher
 * @mcast_cipher: Multicast cipher
 * @auth_suite: Authentication suite
 * @negotiated_authtype: Negotiated auth type
 * @negotiated_mccipher: negotiated multicast cipher
 *
 * This routine will get all WAPI information
 *
 * Return: bool
 */
bool csr_get_wapi_information(tHalHandle hal, tCsrAuthList *auth_type,
			      eCsrEncryptionType encr_type,
			      tCsrEncryptionList *mc_encryption,
			      tDot11fIEWAPI *wapi_ie, uint8_t *ucast_cipher,
			      uint8_t *mcast_cipher, uint8_t *auth_suite,
			      eCsrAuthType *negotiated_authtype,
			      eCsrEncryptionType *negotiated_mccipher)
{
	tpAniSirGlobal mac_ctx = PMAC_STRUCT(hal);
	bool acceptable_cipher = false;
	uint8_t c_ucast_cipher = 0;
	uint8_t c_mcast_cipher = 0;
	uint8_t c_auth_suites = 0, i;
	uint8_t unicast[CSR_WAPI_OUI_SIZE];
	uint8_t multicast[CSR_WAPI_OUI_SIZE];
	uint8_t authsuites[CSR_WAPI_MAX_AUTH_SUITES][CSR_WAPI_OUI_SIZE];
	uint8_t authentication[CSR_WAPI_OUI_SIZE];
	uint8_t mccipher_arr[CSR_WAPI_MAX_MULTICAST_CYPHERS][CSR_WAPI_OUI_SIZE];
	eCsrAuthType neg_authtype = eCSR_AUTH_TYPE_UNKNOWN;
	uint8_t wapioui_idx = 0;

	if (!wapi_ie->present)
		goto end;

	c_mcast_cipher++;
	cdf_mem_copy(mccipher_arr, wapi_ie->multicast_cipher_suite,
			CSR_WAPI_OUI_SIZE);
	c_ucast_cipher = (uint8_t) (wapi_ie->unicast_cipher_suite_count);
	c_auth_suites = (uint8_t) (wapi_ie->akm_suite_count);
	for (i = 0; i < c_auth_suites && i < CSR_WAPI_MAX_AUTH_SUITES; i++)
		cdf_mem_copy((void *)&authsuites[i],
			(void *)&wapi_ie->akm_suites[i], CSR_WAPI_OUI_SIZE);

	wapioui_idx = csr_get_oui_index_from_cipher(encr_type);
	if (wapioui_idx >= CSR_OUI_WAPI_WAI_MAX_INDEX) {
		sms_log(mac_ctx, LOGE,
			FL("Wapi OUI index = %d out of limit"),
			wapioui_idx);
		acceptable_cipher = false;
		goto end;
	}
	/* Check - Is requested unicast Cipher supported by the BSS. */
	acceptable_cipher = csr_match_wapi_oui_index(mac_ctx,
				wapi_ie->unicast_cipher_suites,
				c_ucast_cipher, wapioui_idx, unicast);
	if (!acceptable_cipher)
		goto end;

	/* unicast is supported. Pick the first matching Group cipher, if any */
	for (i = 0; i < mc_encryption->numEntries; i++) {
		wapioui_idx = csr_get_oui_index_from_cipher(
					mc_encryption->encryptionType[i]);
		if (wapioui_idx >= CSR_OUI_WAPI_WAI_MAX_INDEX) {
			sms_log(mac_ctx, LOGE,
				FL("Wapi OUI index = %d out of limit"),
				wapioui_idx);
			acceptable_cipher = false;
			break;
		}
		acceptable_cipher = csr_match_wapi_oui_index(mac_ctx,
						mccipher_arr, c_mcast_cipher,
						wapioui_idx, multicast);
		if (acceptable_cipher)
			break;
	}
	if (!acceptable_cipher)
		goto end;

	if (negotiated_mccipher)
		*negotiated_mccipher =
			mc_encryption->encryptionType[i];

	/*
	 * Ciphers are supported, Match authentication algorithm and
	 * pick first matching authtype
	 */
	if (csr_is_auth_wapi_cert
			(mac_ctx, authsuites, c_auth_suites, authentication)) {
		neg_authtype =
			eCSR_AUTH_TYPE_WAPI_WAI_CERTIFICATE;
	} else if (csr_is_auth_wapi_psk(mac_ctx, authsuites,
				c_auth_suites, authentication)) {
		neg_authtype = eCSR_AUTH_TYPE_WAPI_WAI_PSK;
	} else {
		acceptable_cipher = false;
		neg_authtype = eCSR_AUTH_TYPE_UNKNOWN;
	}

	/* Caller doesn't care about auth type, or BSS doesn't match */
	if ((0 == auth_type->numEntries) || (false == acceptable_cipher))
		goto end;

	acceptable_cipher = false;
	for (i = 0; i < auth_type->numEntries; i++) {
		if (auth_type->authType[i] == neg_authtype) {
			acceptable_cipher = true;
			break;
		}
	}

end:
	if (acceptable_cipher) {
		if (mcast_cipher)
			cdf_mem_copy(mcast_cipher, multicast,
					CSR_WAPI_OUI_SIZE);
		if (ucast_cipher)
			cdf_mem_copy(ucast_cipher, unicast, CSR_WAPI_OUI_SIZE);
		if (auth_suite)
			cdf_mem_copy(auth_suite, authentication,
					CSR_WAPI_OUI_SIZE);
		if (negotiated_authtype)
			*negotiated_authtype = neg_authtype;
	}
	return acceptable_cipher;
}

bool csr_is_wapi_match(tHalHandle hHal, tCsrAuthList *pAuthType,
		       eCsrEncryptionType enType, tCsrEncryptionList *pEnMcType,
		       tDot11fBeaconIEs *pIes, eCsrAuthType *pNegotiatedAuthType,
		       eCsrEncryptionType *pNegotiatedMCCipher)
{
	bool fWapiMatch = false;

	/* See if the cyphers in the Bss description match with the settings in the profile. */
	fWapiMatch =
		csr_get_wapi_information(hHal, pAuthType, enType, pEnMcType,
					 &pIes->WAPI, NULL, NULL, NULL,
					 pNegotiatedAuthType, pNegotiatedMCCipher);

	return fWapiMatch;
}

bool csr_lookup_bkid(tpAniSirGlobal pMac, uint32_t sessionId, uint8_t *pBSSId,
		     uint8_t *pBKId)
{
	bool fRC = false, fMatchFound = false;
	uint32_t Index;
	tCsrRoamSession *pSession = CSR_GET_SESSION(pMac, sessionId);

	if (!pSession) {
		sms_log(pMac, LOGE, FL("  session %d not found "), sessionId);
		return false;
	}

	do {
		for (Index = 0; Index < pSession->NumBkidCache; Index++) {
			sms_log(pMac, LOGW, "match BKID " MAC_ADDRESS_STR " to ",
				MAC_ADDR_ARRAY(pBSSId));
			if (cdf_mem_compare
			    (pBSSId, pSession->BkidCacheInfo[Index].BSSID.bytes,
				    sizeof(struct cdf_mac_addr))) {
				/* match found */
				fMatchFound = true;
				break;
			}
		}

		if (!fMatchFound)
			break;

		cdf_mem_copy(pBKId, pSession->BkidCacheInfo[Index].BKID,
			     CSR_WAPI_BKID_SIZE);

		fRC = true;
	} while (0);
	sms_log(pMac, LOGW,
		"csr_lookup_bkid called return match = %d pMac->roam.NumBkidCache = %d",
		fRC, pSession->NumBkidCache);

	return fRC;
}

uint8_t csr_construct_wapi_ie(tpAniSirGlobal pMac, uint32_t sessionId,
			      tCsrRoamProfile *pProfile,
			      tSirBssDescription *pSirBssDesc,
			      tDot11fBeaconIEs *pIes, tCsrWapiIe *pWapiIe)
{
	bool fWapiMatch = false;
	uint8_t cbWapiIe = 0;
	uint8_t UnicastCypher[CSR_WAPI_OUI_SIZE];
	uint8_t MulticastCypher[CSR_WAPI_OUI_SIZE];
	uint8_t AuthSuite[CSR_WAPI_OUI_SIZE];
	uint8_t BKId[CSR_WAPI_BKID_SIZE];
	uint8_t *pWapi = NULL;
	bool fBKIDFound = false;
	tDot11fBeaconIEs *pIesLocal = pIes;

	do {
		if (!csr_is_profile_wapi(pProfile))
			break;

		if (!pIesLocal
		    &&
		    (!CDF_IS_STATUS_SUCCESS
			     (csr_get_parsed_bss_description_ies
				     (pMac, pSirBssDesc, &pIesLocal)))) {
			break;
		}
		/* See if the cyphers in the Bss description match with the settings in the profile. */
		fWapiMatch =
			csr_get_wapi_information(pMac, &pProfile->AuthType,
						 pProfile->negotiatedUCEncryptionType,
						 &pProfile->mcEncryptionType,
						 &pIesLocal->WAPI, UnicastCypher,
						 MulticastCypher, AuthSuite, NULL,
						 NULL);
		if (!fWapiMatch)
			break;

		cdf_mem_set(pWapiIe, sizeof(tCsrWapiIe), 0);

		pWapiIe->IeHeader.ElementID = DOT11F_EID_WAPI;

		pWapiIe->Version = CSR_WAPI_VERSION_SUPPORTED;

		pWapiIe->cAuthenticationSuites = 1;
		cdf_mem_copy(&pWapiIe->AuthOui[0], AuthSuite,
			     sizeof(AuthSuite));

		pWapi = (uint8_t *) (&pWapiIe->AuthOui[1]);

		*pWapi = (uint16_t) 1;  /* cUnicastCyphers */
		pWapi += 2;
		cdf_mem_copy(pWapi, UnicastCypher, sizeof(UnicastCypher));
		pWapi += sizeof(UnicastCypher);

		cdf_mem_copy(pWapi, MulticastCypher, sizeof(MulticastCypher));
		pWapi += sizeof(MulticastCypher);

		/* WAPI capabilities follows the Auth Suite (two octects) */
		/* we shouldn't EVER be sending out "pre-auth supported".  It is an AP only capability */
		/* & since we already did a memset pWapiIe to 0, skip these fields */
		pWapi += 2;

		fBKIDFound =
			csr_lookup_bkid(pMac, sessionId, pSirBssDesc->bssId,
					&(BKId[0]));

		if (fBKIDFound) {
			/* Do we need to change the endianness here */
			*pWapi = (uint16_t) 1;  /* cBKIDs */
			pWapi += 2;
			cdf_mem_copy(pWapi, BKId, CSR_WAPI_BKID_SIZE);
		} else {
			*pWapi = 0;
			pWapi += 1;
			*pWapi = 0;
			pWapi += 1;
		}

		/* Add in the IE fields except the IE header */
		/* Add BKID count and BKID (if any) */
		pWapiIe->IeHeader.Length =
			(uint8_t) (sizeof(*pWapiIe) - sizeof(pWapiIe->IeHeader));

		/*2 bytes for BKID Count field */
		pWapiIe->IeHeader.Length += sizeof(uint16_t);

		if (fBKIDFound) {
			pWapiIe->IeHeader.Length += CSR_WAPI_BKID_SIZE;
		}
		/* return the size of the IE header (total) constructed... */
		cbWapiIe = pWapiIe->IeHeader.Length + sizeof(pWapiIe->IeHeader);

	} while (0);

	if (!pIes && pIesLocal) {
		/* locally allocated */
		cdf_mem_free(pIesLocal);
	}

	return cbWapiIe;
}
#endif /* FEATURE_WLAN_WAPI */

/**
 * csr_get_wpa_cyphers() - to get WPA cipher info
 * @mac_ctx: pointer to mac context
 * @auth_type: auth type
 * @encr_type: encryption type
 * @mc_encryption: multicast encryption type
 * @wpa_ie: pointer to WPA IE
 * @ucast_cipher: Unicast cipher
 * @mcast_cipher: Multicast cipher
 * @auth_suite: Authentication suite
 * @negotiated_authtype: Negotiated auth type
 * @negotiated_mccipher: negotiated multicast cipher
 *
 * This routine will get all WPA information
 *
 * Return: bool
 */
bool csr_get_wpa_cyphers(tpAniSirGlobal mac_ctx, tCsrAuthList *auth_type,
		eCsrEncryptionType encr_type, tCsrEncryptionList *mc_encryption,
		tDot11fIEWPA *wpa_ie, uint8_t *ucast_cipher,
		uint8_t *mcast_cipher, uint8_t *auth_suite,
		eCsrAuthType *negotiated_authtype,
		eCsrEncryptionType *negotiated_mccipher)
{
	bool acceptable_cipher = false;
	uint8_t c_ucast_cipher = 0;
	uint8_t c_mcast_cipher = 0;
	uint8_t c_auth_suites = 0;
	uint8_t unicast[CSR_WPA_OUI_SIZE];
	uint8_t multicast[CSR_WPA_OUI_SIZE];
	uint8_t authentication[CSR_WPA_OUI_SIZE];
	uint8_t mccipher_arr[1][CSR_WPA_OUI_SIZE];
	uint8_t i;
	eCsrAuthType neg_authtype = eCSR_AUTH_TYPE_UNKNOWN;

	if (!wpa_ie->present)
		goto end;
	c_mcast_cipher = 1;
	cdf_mem_copy(mccipher_arr, wpa_ie->multicast_cipher, CSR_WPA_OUI_SIZE);
	c_ucast_cipher = (uint8_t) (wpa_ie->unicast_cipher_count);
	c_auth_suites = (uint8_t) (wpa_ie->auth_suite_count);

	/* Check - Is requested unicast Cipher supported by the BSS. */
	acceptable_cipher = csr_match_wpaoui_index(mac_ctx,
				wpa_ie->unicast_ciphers, c_ucast_cipher,
				csr_get_oui_index_from_cipher(encr_type),
				unicast);
	if (!acceptable_cipher)
		goto end;
	/* unicast is supported. Pick the first matching Group cipher, if any */
	for (i = 0; i < mc_encryption->numEntries; i++) {
		acceptable_cipher = csr_match_wpaoui_index(mac_ctx,
					mccipher_arr, c_mcast_cipher,
					csr_get_oui_index_from_cipher(
					    mc_encryption->encryptionType[i]),
					multicast);
		if (acceptable_cipher)
			break;
	}
	if (!acceptable_cipher)
		goto end;

	if (negotiated_mccipher)
		*negotiated_mccipher = mc_encryption->encryptionType[i];

	/* Initializing with false as it has true value already */
	acceptable_cipher = false;
	for (i = 0; i < auth_type->numEntries; i++) {
		/*
		 * Ciphers are supported, Match authentication algorithm and
		 * pick first matching authtype
		 */
		if (csr_is_auth_wpa(mac_ctx, wpa_ie->auth_suites, c_auth_suites,
			authentication)) {
			if (eCSR_AUTH_TYPE_WPA == auth_type->authType[i])
				neg_authtype = eCSR_AUTH_TYPE_WPA;
		}
		if ((neg_authtype == eCSR_AUTH_TYPE_UNKNOWN) &&
			csr_is_auth_wpa_psk(mac_ctx,
				wpa_ie->auth_suites, c_auth_suites,
				authentication)) {
			if (eCSR_AUTH_TYPE_WPA_PSK == auth_type->authType[i])
				neg_authtype = eCSR_AUTH_TYPE_WPA_PSK;
		}
#ifdef FEATURE_WLAN_ESE
		if ((neg_authtype == eCSR_AUTH_TYPE_UNKNOWN)
			&& csr_is_ese_cckm_auth_wpa(mac_ctx,
				wpa_ie->auth_suites, c_auth_suites,
				authentication)) {
			if (eCSR_AUTH_TYPE_CCKM_WPA == auth_type->authType[i])
				neg_authtype = eCSR_AUTH_TYPE_CCKM_WPA;
		}
#endif /* FEATURE_WLAN_ESE */

		/*
		 * The 1st auth type in the APs WPA IE, to match stations
		 * connecting profiles auth type will cause us to exit this
		 * loop. This is added as some APs advertise multiple akms in
		 * the WPA IE
		 */
		if (eCSR_AUTH_TYPE_UNKNOWN != neg_authtype) {
			acceptable_cipher = true;
			break;
		}
	}

end:
	if (acceptable_cipher) {
		if (mcast_cipher)
			cdf_mem_copy((uint8_t **) mcast_cipher, multicast,
					CSR_WPA_OUI_SIZE);

		if (ucast_cipher)
			cdf_mem_copy((uint8_t **) ucast_cipher, unicast,
					CSR_WPA_OUI_SIZE);

		if (auth_suite)
			cdf_mem_copy((uint8_t **) auth_suite, authentication,
					CSR_WPA_OUI_SIZE);

		if (negotiated_authtype)
			*negotiated_authtype = neg_authtype;
	}

	return acceptable_cipher;
}

bool csr_is_wpa_encryption_match(tpAniSirGlobal pMac, tCsrAuthList *pAuthType,
				 eCsrEncryptionType enType,
				 tCsrEncryptionList *pEnMcType,
				 tDot11fBeaconIEs *pIes,
				 eCsrAuthType *pNegotiatedAuthtype,
				 eCsrEncryptionType *pNegotiatedMCCipher)
{
	bool fWpaMatch = false;

	/* See if the cyphers in the Bss description match with the settings in the profile. */
	fWpaMatch =
		csr_get_wpa_cyphers(pMac, pAuthType, enType, pEnMcType, &pIes->WPA,
				    NULL, NULL, NULL, pNegotiatedAuthtype,
				    pNegotiatedMCCipher);

	return fWpaMatch;
}

uint8_t csr_construct_wpa_ie(tHalHandle hHal, tCsrRoamProfile *pProfile,
			     tSirBssDescription *pSirBssDesc,
			     tDot11fBeaconIEs *pIes, tCsrWpaIe *pWpaIe)
{
	tpAniSirGlobal pMac = PMAC_STRUCT(hHal);
	bool fWpaMatch;
	uint8_t cbWpaIe = 0;
	uint8_t UnicastCypher[CSR_WPA_OUI_SIZE];
	uint8_t MulticastCypher[CSR_WPA_OUI_SIZE];
	uint8_t AuthSuite[CSR_WPA_OUI_SIZE];
	tCsrWpaAuthIe *pAuthSuite;
	tDot11fBeaconIEs *pIesLocal = pIes;

	do {
		if (!csr_is_profile_wpa(pProfile))
			break;

		if (!pIesLocal
		    &&
		    (!CDF_IS_STATUS_SUCCESS
			     (csr_get_parsed_bss_description_ies
				     (pMac, pSirBssDesc, &pIesLocal)))) {
			break;
		}
		/* See if the cyphers in the Bss description match with the settings in the profile. */
		fWpaMatch =
			csr_get_wpa_cyphers(hHal, &pProfile->AuthType,
					    pProfile->negotiatedUCEncryptionType,
					    &pProfile->mcEncryptionType,
					    &pIesLocal->WPA, UnicastCypher,
					    MulticastCypher, AuthSuite, NULL, NULL);
		if (!fWpaMatch)
			break;

		pWpaIe->IeHeader.ElementID = SIR_MAC_WPA_EID;

		cdf_mem_copy(pWpaIe->Oui, csr_wpa_oui[01], sizeof(pWpaIe->Oui));

		pWpaIe->Version = CSR_WPA_VERSION_SUPPORTED;

		cdf_mem_copy(pWpaIe->MulticastOui, MulticastCypher,
			     sizeof(MulticastCypher));

		pWpaIe->cUnicastCyphers = 1;

		cdf_mem_copy(&pWpaIe->UnicastOui[0], UnicastCypher,
			     sizeof(UnicastCypher));

		pAuthSuite =
			(tCsrWpaAuthIe *) (&pWpaIe->
					   UnicastOui[pWpaIe->cUnicastCyphers]);

		pAuthSuite->cAuthenticationSuites = 1;
		cdf_mem_copy(&pAuthSuite->AuthOui[0], AuthSuite,
			     sizeof(AuthSuite));

		/* The WPA capabilities follows the Auth Suite (two octects)-- */
		/* this field is optional, and we always "send" zero, so just */
		/* remove it.  This is consistent with our assumptions in the */
		/* frames compiler; c.f. bug 15234: */
		/* http://gold.woodsidenet.com/bugzilla/show_bug.cgi?id=15234 */

		/* Add in the fixed fields plus 1 Unicast cypher, less the IE Header length */
		/* Add in the size of the Auth suite (count plus a single OUI) */
		pWpaIe->IeHeader.Length =
			sizeof(*pWpaIe) - sizeof(pWpaIe->IeHeader) +
			sizeof(*pAuthSuite);

		/* return the size of the IE header (total) constructed... */
		cbWpaIe = pWpaIe->IeHeader.Length + sizeof(pWpaIe->IeHeader);

	} while (0);

	if (!pIes && pIesLocal) {
		/* locally allocated */
		cdf_mem_free(pIesLocal);
	}

	return cbWpaIe;
}

/* If a WPAIE exists in the profile, just use it. Or else construct one from the BSS */
/* Caller allocated memory for pWpaIe and guarrantee it can contain a max length WPA IE */
uint8_t csr_retrieve_wpa_ie(tHalHandle hHal, tCsrRoamProfile *pProfile,
			    tSirBssDescription *pSirBssDesc,
			    tDot11fBeaconIEs *pIes, tCsrWpaIe *pWpaIe)
{
	tpAniSirGlobal pMac = PMAC_STRUCT(hHal);
	uint8_t cbWpaIe = 0;

	do {
		if (!csr_is_profile_wpa(pProfile))
			break;
		if (pProfile->nWPAReqIELength && pProfile->pWPAReqIE) {
			if (SIR_MAC_WPA_IE_MAX_LENGTH >=
			    pProfile->nWPAReqIELength) {
				cbWpaIe = (uint8_t) pProfile->nWPAReqIELength;
				cdf_mem_copy(pWpaIe, pProfile->pWPAReqIE,
					     cbWpaIe);
			} else {
				sms_log(pMac, LOGW,
					"  csr_retrieve_wpa_ie detect invalid WPA IE length (%d) ",
					pProfile->nWPAReqIELength);
			}
		} else {
			cbWpaIe =
				csr_construct_wpa_ie(pMac, pProfile, pSirBssDesc, pIes,
						     pWpaIe);
		}
	} while (0);

	return cbWpaIe;
}

/* If a RSNIE exists in the profile, just use it. Or else construct one from the BSS */
/* Caller allocated memory for pWpaIe and guarrantee it can contain a max length WPA IE */
uint8_t csr_retrieve_rsn_ie(tHalHandle hHal, uint32_t sessionId,
			    tCsrRoamProfile *pProfile,
			    tSirBssDescription *pSirBssDesc,
			    tDot11fBeaconIEs *pIes, tCsrRSNIe *pRsnIe)
{
	tpAniSirGlobal pMac = PMAC_STRUCT(hHal);
	uint8_t cbRsnIe = 0;

	do {
		if (!csr_is_profile_rsn(pProfile))
			break;
#ifdef FEATURE_WLAN_LFR
		if (csr_roam_is_fast_roam_enabled(pMac, sessionId)) {
			/* If "Legacy Fast Roaming" is enabled ALWAYS rebuild the RSN IE from */
			/* scratch. So it contains the current PMK-IDs */
			cbRsnIe =
				csr_construct_rsn_ie(pMac, sessionId, pProfile,
						     pSirBssDesc, pIes, pRsnIe);
		} else
#endif
		if (pProfile->nRSNReqIELength && pProfile->pRSNReqIE) {
			/* If you have one started away, re-use it. */
			if (SIR_MAC_WPA_IE_MAX_LENGTH >=
			    pProfile->nRSNReqIELength) {
				cbRsnIe = (uint8_t) pProfile->nRSNReqIELength;
				cdf_mem_copy(pRsnIe, pProfile->pRSNReqIE,
					     cbRsnIe);
			} else {
				sms_log(pMac, LOGW,
					"  csr_retrieve_rsn_ie detect invalid RSN IE length (%d) ",
					pProfile->nRSNReqIELength);
			}
		} else {
			cbRsnIe =
				csr_construct_rsn_ie(pMac, sessionId, pProfile,
						     pSirBssDesc, pIes, pRsnIe);
		}
	} while (0);

	return cbRsnIe;
}

#ifdef FEATURE_WLAN_WAPI
/* If a WAPI IE exists in the profile, just use it. Or else construct one from the BSS */
/* Caller allocated memory for pWapiIe and guarrantee it can contain a max length WAPI IE */
uint8_t csr_retrieve_wapi_ie(tHalHandle hHal, uint32_t sessionId,
			     tCsrRoamProfile *pProfile,
			     tSirBssDescription *pSirBssDesc,
			     tDot11fBeaconIEs *pIes, tCsrWapiIe *pWapiIe)
{
	tpAniSirGlobal pMac = PMAC_STRUCT(hHal);
	uint8_t cbWapiIe = 0;

	do {
		if (!csr_is_profile_wapi(pProfile))
			break;
		if (pProfile->nWAPIReqIELength && pProfile->pWAPIReqIE) {
			if (DOT11F_IE_WAPI_MAX_LEN >=
			    pProfile->nWAPIReqIELength) {
				cbWapiIe = (uint8_t) pProfile->nWAPIReqIELength;
				cdf_mem_copy(pWapiIe, pProfile->pWAPIReqIE,
					     cbWapiIe);
			} else {
				sms_log(pMac, LOGW,
					"  csr_retrieve_wapi_ie detect invalid WAPI IE length (%d) ",
					pProfile->nWAPIReqIELength);
			}
		} else {
			cbWapiIe =
				csr_construct_wapi_ie(pMac, sessionId, pProfile,
						      pSirBssDesc, pIes, pWapiIe);
		}
	} while (0);

	return cbWapiIe;
}
#endif /* FEATURE_WLAN_WAPI */

bool csr_rates_is_dot11_rate11b_supported_rate(uint8_t dot11Rate)
{
	bool fSupported = false;
	uint16_t nonBasicRate =
		(uint16_t) (BITS_OFF(dot11Rate, CSR_DOT11_BASIC_RATE_MASK));

	switch (nonBasicRate) {
	case eCsrSuppRate_1Mbps:
	case eCsrSuppRate_2Mbps:
	case eCsrSuppRate_5_5Mbps:
	case eCsrSuppRate_11Mbps:
		fSupported = true;
		break;

	default:
		break;
	}

	return fSupported;
}

bool csr_rates_is_dot11_rate11a_supported_rate(uint8_t dot11Rate)
{
	bool fSupported = false;
	uint16_t nonBasicRate =
		(uint16_t) (BITS_OFF(dot11Rate, CSR_DOT11_BASIC_RATE_MASK));

	switch (nonBasicRate) {
	case eCsrSuppRate_6Mbps:
	case eCsrSuppRate_9Mbps:
	case eCsrSuppRate_12Mbps:
	case eCsrSuppRate_18Mbps:
	case eCsrSuppRate_24Mbps:
	case eCsrSuppRate_36Mbps:
	case eCsrSuppRate_48Mbps:
	case eCsrSuppRate_54Mbps:
		fSupported = true;
		break;

	default:
		break;
	}

	return fSupported;
}

tAniEdType csr_translate_encrypt_type_to_ed_type(eCsrEncryptionType EncryptType)
{
	tAniEdType edType;

	switch (EncryptType) {
	default:
	case eCSR_ENCRYPT_TYPE_NONE:
		edType = eSIR_ED_NONE;
		break;

	case eCSR_ENCRYPT_TYPE_WEP40_STATICKEY:
	case eCSR_ENCRYPT_TYPE_WEP40:
		edType = eSIR_ED_WEP40;
		break;

	case eCSR_ENCRYPT_TYPE_WEP104_STATICKEY:
	case eCSR_ENCRYPT_TYPE_WEP104:
		edType = eSIR_ED_WEP104;
		break;

	case eCSR_ENCRYPT_TYPE_TKIP:
		edType = eSIR_ED_TKIP;
		break;

	case eCSR_ENCRYPT_TYPE_AES:
		edType = eSIR_ED_CCMP;
		break;
#ifdef FEATURE_WLAN_WAPI
	case eCSR_ENCRYPT_TYPE_WPI:
		edType = eSIR_ED_WPI;
		break;
#endif
#ifdef WLAN_FEATURE_11W
	/* 11w BIP */
	case eCSR_ENCRYPT_TYPE_AES_CMAC:
		edType = eSIR_ED_AES_128_CMAC;
		break;
#endif
	}

	return edType;
}

/**
 * csr_validate_wep() - to validate wep
 * @uc_encry_type: unicast encryption type
 * @auth_list: Auth list
 * @mc_encryption_list: multicast encryption type
 * @negotiated_authtype: negotiated auth type
 * @negotiated_mc_encry: negotiated mc encry type
 * @bss_descr: BSS description
 * @ie_ptr: IE pointer
 *
 * This function just checks whether HDD is giving correct values for
 * Multicast cipher and Auth
 *
 * Return: bool
 */
bool csr_validate_wep(tpAniSirGlobal mac_ctx, eCsrEncryptionType uc_encry_type,
		      tCsrAuthList *auth_list,
		      tCsrEncryptionList *mc_encryption_list,
		      eCsrAuthType *negotiated_authtype,
		      eCsrEncryptionType *negotiated_mc_encry,
		      tSirBssDescription *bss_descr, tDot11fBeaconIEs *ie_ptr)
{
	uint32_t idx;
	bool match = false;
	eCsrAuthType negotiated_auth = eCSR_AUTH_TYPE_OPEN_SYSTEM;
	eCsrEncryptionType negotiated_mccipher = eCSR_ENCRYPT_TYPE_UNKNOWN;

	/* If privacy bit is not set, consider no match */
	if (!csr_is_privacy(bss_descr))
		goto end;

	for (idx = 0; idx < mc_encryption_list->numEntries; idx++) {
		switch (mc_encryption_list->encryptionType[idx]) {
		case eCSR_ENCRYPT_TYPE_WEP40_STATICKEY:
		case eCSR_ENCRYPT_TYPE_WEP104_STATICKEY:
		case eCSR_ENCRYPT_TYPE_WEP40:
		case eCSR_ENCRYPT_TYPE_WEP104:
			/*
			 * Multicast list may contain WEP40/WEP104.
			 * Check whether it matches UC.
			 */
			if (uc_encry_type ==
				mc_encryption_list->encryptionType[idx]) {
				match = true;
				negotiated_mccipher =
					mc_encryption_list->encryptionType[idx];
			}
			break;
		default:
			match = false;
			break;
		}
		if (match)
			break;
	}

	if (!match)
		goto end;

	for (idx = 0; idx < auth_list->numEntries; idx++) {
		switch (auth_list->authType[idx]) {
		case eCSR_AUTH_TYPE_OPEN_SYSTEM:
		case eCSR_AUTH_TYPE_SHARED_KEY:
		case eCSR_AUTH_TYPE_AUTOSWITCH:
			match = true;
			negotiated_auth = auth_list->authType[idx];
			break;
		default:
			match = false;
		}
		if (match)
			break;
	}

	if (!match)
		goto end;

	if (!ie_ptr)
		goto end;

	/*
	 * In case of WPA / WPA2, check whether it supports WEP as well.
	 * Prepare the encryption type for WPA/WPA2 functions
	 */
	if (eCSR_ENCRYPT_TYPE_WEP40_STATICKEY == uc_encry_type)
		uc_encry_type = eCSR_ENCRYPT_TYPE_WEP40;
	else if (eCSR_ENCRYPT_TYPE_WEP104 == uc_encry_type)
		uc_encry_type = eCSR_ENCRYPT_TYPE_WEP104;

	/* else we can use the encryption type directly */
	if (ie_ptr->WPA.present) {
		match = cdf_mem_compare(ie_ptr->WPA.multicast_cipher,
				csr_wpa_oui[csr_get_oui_index_from_cipher(
					uc_encry_type)],
				CSR_WPA_OUI_SIZE);
		if (match)
			goto end;
	}
	if (ie_ptr->RSN.present) {
		match = cdf_mem_compare(ie_ptr->RSN.gp_cipher_suite,
				csr_rsn_oui[csr_get_oui_index_from_cipher(
					uc_encry_type)],
				CSR_RSN_OUI_SIZE);
	}


end:
	if (match) {
		if (negotiated_authtype)
			*negotiated_authtype = negotiated_auth;
		if (negotiated_mc_encry)
			*negotiated_mc_encry = negotiated_mccipher;
	}
	return match;
}

/**
 * csr_validate_open_none() - Check if the security is matching
 * @bss_desc:          BSS Descriptor on which the check is done
 * @mc_enc_type:       Multicast encryption type
 * @mc_cipher:         Multicast Cipher
 * @auth_type:         Authentication type
 * @neg_auth_type:     Negotiated Auth type with the AP
 *
 * Return: Boolean value to tell if matched or not.
 */
static bool csr_validate_open_none(tSirBssDescription *bss_desc,
	tCsrEncryptionList *mc_enc_type, eCsrEncryptionType *mc_cipher,
	tCsrAuthList *auth_type, eCsrAuthType *neg_auth_type)
{
	bool match;
	uint8_t idx;

	/*
	 * for NO encryption, if the Bss description has the
	 * Privacy bit turned on, then encryption is required
	 * so we have to reject this Bss.
	 */
	if (csr_is_privacy(bss_desc))
		match = false;
	else
		match = true;
	if (match) {
		match = false;
		/* Check MC cipher and Auth type requested. */
		for (idx = 0; idx < mc_enc_type->numEntries; idx++) {
			if (eCSR_ENCRYPT_TYPE_NONE ==
				mc_enc_type->encryptionType[idx]) {
				match = true;
				*mc_cipher = mc_enc_type->encryptionType[idx];
				break;
			}
		}
		if (!match)
			return match;

		match = false;
		/* Check Auth list. It should contain AuthOpen. */
		for (idx = 0; idx < auth_type->numEntries; idx++) {
			if ((eCSR_AUTH_TYPE_OPEN_SYSTEM ==
				auth_type->authType[idx]) ||
				(eCSR_AUTH_TYPE_AUTOSWITCH ==
				auth_type->authType[idx])) {
				match = true;
				*neg_auth_type =
					eCSR_AUTH_TYPE_OPEN_SYSTEM;
				break;
			}
		}
		if (!match)
			return match;

	}
	return match;
}

/**
 * csr_validate_any_default() - Check if the security is matching
 * @hal:               Global HAL handle
 * @auth_type:         Authentication type
 * @mc_enc_type:       Multicast encryption type
 * @mfp_enabled:       Management frame protection feature
 * @mfp_required:      Mangement frame protection mandatory
 * @mfp_capable:       Device capable of MFP
 * @ies_ptr:           Pointer to the IE fields
 * @neg_auth_type:     Negotiated Auth type with the AP
 * @bss_desc:          BSS Descriptor
 * @neg_uc_cipher:     Negotiated unicast cipher suite
 * @neg_mc_cipher:     Negotiated multicast cipher
 *
 * Return: Boolean value to tell if matched or not.
 */
static bool csr_validate_any_default(tHalHandle hal, tCsrAuthList *auth_type,
	tCsrEncryptionList *mc_enc_type, bool *mfp_enabled,
	uint8_t *mfp_required, uint8_t *mfp_capable,
	tDot11fBeaconIEs *ies_ptr, eCsrAuthType *neg_auth_type,
	tSirBssDescription *bss_desc, eCsrEncryptionType *uc_cipher,
	eCsrEncryptionType *mc_cipher)
{
	bool match_any = false;
	bool match = true;
	tpAniSirGlobal mac_ctx = PMAC_STRUCT(hal);
	/* It is allowed to match anything. Try the more secured ones first. */
	if (ies_ptr) {
		/* Check AES first */
		*uc_cipher = eCSR_ENCRYPT_TYPE_AES;
		match_any = csr_is_rsn_match(hal, auth_type,
				*uc_cipher, mc_enc_type, mfp_enabled,
				mfp_required, mfp_capable, ies_ptr,
				neg_auth_type, mc_cipher);
		if (!match_any) {
			/* Check TKIP */
			*uc_cipher = eCSR_ENCRYPT_TYPE_TKIP;
			match_any = csr_is_rsn_match(hal, auth_type, *uc_cipher,
					mc_enc_type, mfp_enabled, mfp_required,
					mfp_capable, ies_ptr, neg_auth_type,
					mc_cipher);
		}
#ifdef FEATURE_WLAN_WAPI
		if (!match_any) {
			/* Check WAPI */
			*uc_cipher = eCSR_ENCRYPT_TYPE_WPI;
			match_any = csr_is_wapi_match(hal, auth_type,
					*uc_cipher, mc_enc_type, ies_ptr,
					neg_auth_type, mc_cipher);
		}
#endif
	}
	if (match_any)
		return match;
	*uc_cipher = eCSR_ENCRYPT_TYPE_WEP104;
	if (csr_validate_wep(mac_ctx, *uc_cipher, auth_type, mc_enc_type,
			neg_auth_type, mc_cipher, bss_desc, ies_ptr))
		return match;
	*uc_cipher = eCSR_ENCRYPT_TYPE_WEP40;
	if (csr_validate_wep(mac_ctx, *uc_cipher, auth_type, mc_enc_type,
			neg_auth_type, mc_cipher, bss_desc, ies_ptr))
		return match;
	*uc_cipher = eCSR_ENCRYPT_TYPE_WEP104_STATICKEY;
	if (csr_validate_wep(mac_ctx, *uc_cipher, auth_type, mc_enc_type,
			neg_auth_type, mc_cipher, bss_desc, ies_ptr))
		return match;
	*uc_cipher = eCSR_ENCRYPT_TYPE_WEP40_STATICKEY;
	if (csr_validate_wep(mac_ctx, *uc_cipher, auth_type, mc_enc_type,
			neg_auth_type, mc_cipher, bss_desc, ies_ptr))
		return match;
	/* It must be open and no enc */
	if (csr_is_privacy(bss_desc)) {
		match = false;
		return match;
	}

	*neg_auth_type = eCSR_AUTH_TYPE_OPEN_SYSTEM;
	*mc_cipher = eCSR_ENCRYPT_TYPE_NONE;
	*uc_cipher = eCSR_ENCRYPT_TYPE_NONE;
	return match;

}

/**
 * csr_is_security_match() - Check if the security is matching
 * @hal:               Global HAL handle
 * @auth_type:         Authentication type
 * @uc_enc_type:       Unicast Encryption type
 * @mc_enc_type:       Multicast encryption type
 * @mfp_enabled:       Management frame protection feature
 * @mfp_required:      Mangement frame protection mandatory
 * @mfp_capable:       Device capable of MFP
 * @bss_desc:          BSS Descriptor
 * @ies_ptr:           Pointer to the IE fields
 * @neg_auth_type:     Negotiated Auth type with the AP
 * @neg_uc_cipher:     Negotiated unicast cipher suite
 * @neg_mc_cipher:     Negotiated multicast cipher
 *
 * Return: Boolean value to tell if matched or not.
 */
bool csr_is_security_match(tHalHandle hal, tCsrAuthList *auth_type,
	tCsrEncryptionList *uc_enc_type,
	tCsrEncryptionList *mc_enc_type, bool *mfp_enabled,
	uint8_t *mfp_required, uint8_t *mfp_capable,
	tSirBssDescription *bss_desc, tDot11fBeaconIEs *ies_ptr,
	eCsrAuthType *neg_auth_type,
	eCsrEncryptionType *neg_uc_cipher,
	eCsrEncryptionType *neg_mc_cipher)
{
	tpAniSirGlobal mac_ctx = PMAC_STRUCT(hal);
	bool match = false;
	uint8_t i;
	eCsrEncryptionType mc_cipher = eCSR_ENCRYPT_TYPE_UNKNOWN;
	eCsrEncryptionType uc_cipher = eCSR_ENCRYPT_TYPE_UNKNOWN;
	eCsrAuthType local_neg_auth_type = eCSR_AUTH_TYPE_UNKNOWN;

	for (i = 0; ((i < uc_enc_type->numEntries) && (!match)); i++) {
		uc_cipher = uc_enc_type->encryptionType[i];
		/*
		 * If the Bss description shows the Privacy bit is on, then we
		 * must have some sort of encryption configured for the profile
		 * to work.  Don't attempt to join networks with Privacy bit
		 * set when profiles say NONE for encryption type.
		 */
		switch (uc_cipher) {
		case eCSR_ENCRYPT_TYPE_NONE:
			match = csr_validate_open_none(bss_desc, mc_enc_type,
					&mc_cipher, auth_type,
					&local_neg_auth_type);
			break;

		case eCSR_ENCRYPT_TYPE_WEP40_STATICKEY:
		case eCSR_ENCRYPT_TYPE_WEP104_STATICKEY:
			/*
			 * !! might want to check for WEP keys set in the
			 * Profile.... ? !! don't need to have the privacy bit
			 * in the Bss description.  Many AP policies make
			 * legacy encryption 'optional' so we don't know if we
			 * can associate or not.  The AP will reject if
			 * encryption is not allowed without the Privacy bit
			 * turned on.
			 */
			match = csr_validate_wep(mac_ctx, uc_cipher, auth_type,
					mc_enc_type, &local_neg_auth_type,
					&mc_cipher, bss_desc, ies_ptr);

			break;
		/* these are all of the WPA encryption types... */
		case eCSR_ENCRYPT_TYPE_WEP40:
		case eCSR_ENCRYPT_TYPE_WEP104:
			match = csr_validate_wep(mac_ctx, uc_cipher, auth_type,
					mc_enc_type, &local_neg_auth_type,
					&mc_cipher, bss_desc, ies_ptr);
			break;

		case eCSR_ENCRYPT_TYPE_TKIP:
		case eCSR_ENCRYPT_TYPE_AES:
			if (!ies_ptr) {
				match = false;
				break;
			}
			/* First check if there is a RSN match */
			match = csr_is_rsn_match(mac_ctx, auth_type,
					uc_cipher, mc_enc_type,
					mfp_enabled, mfp_required,
					mfp_capable, ies_ptr,
					&local_neg_auth_type,
					&mc_cipher);
			/* If not RSN, then check WPA match */
			if (!match)
				match = csr_is_wpa_encryption_match(
						mac_ctx, auth_type,
						uc_cipher, mc_enc_type,
						ies_ptr,
						&local_neg_auth_type,
						&mc_cipher);
			break;
#ifdef FEATURE_WLAN_WAPI
		case eCSR_ENCRYPT_TYPE_WPI:     /* WAPI */
			if (ies_ptr)
				match = csr_is_wapi_match(hal, auth_type,
						uc_cipher, mc_enc_type, ies_ptr,
						&local_neg_auth_type,
						&mc_cipher);
			else
				match = false;
			break;
#endif /* FEATURE_WLAN_WAPI */
		case eCSR_ENCRYPT_TYPE_ANY:
		default:
			match  = csr_validate_any_default(hal, auth_type,
					mc_enc_type, mfp_enabled, mfp_required,
					mfp_capable, ies_ptr,
					&local_neg_auth_type, bss_desc,
					&uc_cipher, &mc_cipher);
			break;
		}

	}

	if (match) {
		if (neg_uc_cipher)
			*neg_uc_cipher = uc_cipher;
		if (neg_mc_cipher)
			*neg_mc_cipher = mc_cipher;
		if (neg_auth_type)
			*neg_auth_type = local_neg_auth_type;
	}
	return match;
}

bool csr_is_ssid_match(tpAniSirGlobal pMac, uint8_t *ssid1, uint8_t ssid1Len,
		       uint8_t *bssSsid, uint8_t bssSsidLen, bool fSsidRequired)
{
	bool fMatch = false;

	do {
		/*
		 * Check for the specification of the Broadcast SSID at the
		 * beginning of the list. If specified, then all SSIDs are
		 * matches (broadcast SSID means accept all SSIDs).
		 */
		if (ssid1Len == 0) {
			fMatch = true;
			break;
		}

		/* There are a few special cases.  If the Bss description has a Broadcast SSID, */
		/* then our Profile must have a single SSID without Wildcards so we can program */
		/* the SSID. */
		/* SSID could be suppressed in beacons. In that case SSID IE has valid length */
		/* but the SSID value is all NULL characters. That condition is trated same */
		/* as NULL SSID */
		if (csr_is_nullssid(bssSsid, bssSsidLen)) {
			if (false == fSsidRequired) {
				fMatch = true;
				break;
			}
		}

		if (ssid1Len != bssSsidLen)
			break;
		if (cdf_mem_compare(bssSsid, ssid1, bssSsidLen)) {
			fMatch = true;
			break;
		}

	} while (0);

	return fMatch;
}

/* Null ssid means match */
bool csr_is_ssid_in_list(tHalHandle hHal, tSirMacSSid *pSsid,
			 tCsrSSIDs *pSsidList)
{
	bool fMatch = false;
	uint32_t i;

	if (pSsidList && pSsid) {
		for (i = 0; i < pSsidList->numOfSSIDs; i++) {
			if (csr_is_nullssid
				    (pSsidList->SSIDList[i].SSID.ssId,
				    pSsidList->SSIDList[i].SSID.length)
			    ||
			    ((pSsidList->SSIDList[i].SSID.length ==
			      pSsid->length)
			     && cdf_mem_compare(pSsid->ssId,
						pSsidList->SSIDList[i].SSID.
						ssId, pSsid->length))) {
				fMatch = true;
				break;
			}
		}
	}

	return fMatch;
}

bool csr_is_bssid_match(tHalHandle hHal, struct cdf_mac_addr *pProfBssid,
			struct cdf_mac_addr *BssBssid)
{
	bool fMatch = false;
	struct cdf_mac_addr ProfileBssid;

	/* for efficiency of the MAC_ADDRESS functions, move the */
	/* Bssid's into MAC_ADDRESS structs. */
	cdf_mem_copy(&ProfileBssid, pProfBssid, sizeof(struct cdf_mac_addr));

	do {

		/* Give the profile the benefit of the doubt... accept either all 0 or */
		/* the real broadcast Bssid (all 0xff) as broadcast Bssids (meaning to */
		/* match any Bssids). */
		if (cdf_is_macaddr_zero(&ProfileBssid) ||
		    cdf_is_macaddr_broadcast(&ProfileBssid)) {
			fMatch = true;
			break;
		}

		if (cdf_is_macaddr_equal(BssBssid, &ProfileBssid)) {
			fMatch = true;
			break;
		}

	} while (0);

	return fMatch;
}

bool csr_is_bss_type_match(eCsrRoamBssType bssType1, eCsrRoamBssType bssType2)
{
	if ((eCSR_BSS_TYPE_ANY != bssType1 && eCSR_BSS_TYPE_ANY != bssType2)
	    && (bssType1 != bssType2))
		return false;
	else
		return true;
}

bool csr_is_bss_type_ibss(eCsrRoamBssType bssType)
{
	return (bool)
		(eCSR_BSS_TYPE_START_IBSS == bssType
		 || eCSR_BSS_TYPE_IBSS == bssType);
}

bool csr_is_bss_type_wds(eCsrRoamBssType bssType)
{
	return (bool)
		(eCSR_BSS_TYPE_WDS_STA == bssType
		 || eCSR_BSS_TYPE_WDS_AP == bssType);
}

bool csr_is_bss_type_caps_match(eCsrRoamBssType bssType,
				tSirBssDescription *pSirBssDesc)
{
	bool fMatch = true;

	do {
		switch (bssType) {
		case eCSR_BSS_TYPE_ANY:
			break;

		case eCSR_BSS_TYPE_INFRASTRUCTURE:
		case eCSR_BSS_TYPE_WDS_STA:
			if (!csr_is_infra_bss_desc(pSirBssDesc))
				fMatch = false;

			break;

		case eCSR_BSS_TYPE_IBSS:
		case eCSR_BSS_TYPE_START_IBSS:
			if (!csr_is_ibss_bss_desc(pSirBssDesc))
				fMatch = false;

			break;

		case eCSR_BSS_TYPE_WDS_AP:      /* For WDS AP, no need to match anything */
		default:
			fMatch = false;
			break;
		}
	} while (0);

	return fMatch;
}

static bool csr_is_capabilities_match(tpAniSirGlobal pMac, eCsrRoamBssType bssType,
				      tSirBssDescription *pSirBssDesc)
{
	return csr_is_bss_type_caps_match(bssType, pSirBssDesc);
}

static bool csr_is_specific_channel_match(tpAniSirGlobal pMac,
					  tSirBssDescription *pSirBssDesc,
					  uint8_t Channel)
{
	bool fMatch = true;

	do {
		/* if the channel is ANY, then always match... */
		if (eCSR_OPERATING_CHANNEL_ANY == Channel)
			break;
		if (Channel == pSirBssDesc->channelId)
			break;

		/* didn't match anything.. so return NO match */
		fMatch = false;

	} while (0);

	return fMatch;
}

bool csr_is_channel_band_match(tpAniSirGlobal pMac, uint8_t channelId,
			       tSirBssDescription *pSirBssDesc)
{
	bool fMatch = true;

	do {
		/* if the profile says Any channel AND the global settings says ANY channel, then we */
		/* always match... */
		if (eCSR_OPERATING_CHANNEL_ANY == channelId)
			break;

		if (eCSR_OPERATING_CHANNEL_ANY != channelId) {
			fMatch =
				csr_is_specific_channel_match(pMac, pSirBssDesc,
							      channelId);
		}

	} while (0);

	return fMatch;
}

/**
 * csr_is_aggregate_rate_supported() - to check if aggregate rate is supported
 * @mac_ctx: pointer to mac context
 * @rate: A rate in units of 500kbps
 *
 *
 * The rate encoding  is just as in 802.11  Information Elements, except
 * that the high bit is \em  not interpreted as indicating a Basic Rate,
 * and proprietary rates are allowed, too.
 *
 * Note  that if the  adapter's dot11Mode  is g,  we don't  restrict the
 * rates.  According to hwReadEepromParameters, this will happen when:
 * ... the  card is  configured for ALL  bands through  the property
 * page.  If this occurs, and the card is not an ABG card ,then this
 * code  is  setting the  dot11Mode  to  assume  the mode  that  the
 * hardware can support.   For example, if the card  is an 11BG card
 * and we  are configured to support  ALL bands, then  we change the
 * dot11Mode  to 11g  because  ALL in  this  case is  only what  the
 * hardware can support.
 *
 * Return: true if  the adapter is currently capable of supporting this rate
 */

static bool csr_is_aggregate_rate_supported(tpAniSirGlobal mac_ctx,
			uint16_t rate)
{
	bool supported = false;
	uint16_t idx, new_rate;

	/* In case basic rate flag is set */
	new_rate = BITS_OFF(rate, CSR_DOT11_BASIC_RATE_MASK);
	if (eCSR_CFG_DOT11_MODE_11A ==
			mac_ctx->roam.configParam.uCfgDot11Mode) {
		switch (new_rate) {
		case eCsrSuppRate_6Mbps:
		case eCsrSuppRate_9Mbps:
		case eCsrSuppRate_12Mbps:
		case eCsrSuppRate_18Mbps:
		case eCsrSuppRate_24Mbps:
		case eCsrSuppRate_36Mbps:
		case eCsrSuppRate_48Mbps:
		case eCsrSuppRate_54Mbps:
			supported = true;
			break;
		default:
			supported = false;
			break;
		}

	} else if (eCSR_CFG_DOT11_MODE_11B ==
		   mac_ctx->roam.configParam.uCfgDot11Mode) {
		switch (new_rate) {
		case eCsrSuppRate_1Mbps:
		case eCsrSuppRate_2Mbps:
		case eCsrSuppRate_5_5Mbps:
		case eCsrSuppRate_11Mbps:
			supported = true;
			break;
		default:
			supported = false;
			break;
		}
	} else if (!mac_ctx->roam.configParam.ProprietaryRatesEnabled) {

		switch (new_rate) {
		case eCsrSuppRate_1Mbps:
		case eCsrSuppRate_2Mbps:
		case eCsrSuppRate_5_5Mbps:
		case eCsrSuppRate_6Mbps:
		case eCsrSuppRate_9Mbps:
		case eCsrSuppRate_11Mbps:
		case eCsrSuppRate_12Mbps:
		case eCsrSuppRate_18Mbps:
		case eCsrSuppRate_24Mbps:
		case eCsrSuppRate_36Mbps:
		case eCsrSuppRate_48Mbps:
		case eCsrSuppRate_54Mbps:
			supported = true;
			break;
		default:
			supported = false;
			break;
		}
	} else if (eCsrSuppRate_1Mbps == new_rate ||
			eCsrSuppRate_2Mbps == new_rate ||
			eCsrSuppRate_5_5Mbps == new_rate ||
			eCsrSuppRate_11Mbps == new_rate) {
			supported = true;
	} else {
		idx = 0x1;

		switch (new_rate) {
		case eCsrSuppRate_6Mbps:
			supported = g_phy_rates_suppt[0][idx];
			break;
		case eCsrSuppRate_9Mbps:
			supported = g_phy_rates_suppt[1][idx];
			break;
		case eCsrSuppRate_12Mbps:
			supported = g_phy_rates_suppt[2][idx];
			break;
		case eCsrSuppRate_18Mbps:
			supported = g_phy_rates_suppt[3][idx];
			break;
		case eCsrSuppRate_20Mbps:
			supported = g_phy_rates_suppt[4][idx];
			break;
		case eCsrSuppRate_24Mbps:
			supported = g_phy_rates_suppt[5][idx];
			break;
		case eCsrSuppRate_36Mbps:
			supported = g_phy_rates_suppt[6][idx];
			break;
		case eCsrSuppRate_40Mbps:
			supported = g_phy_rates_suppt[7][idx];
			break;
		case eCsrSuppRate_42Mbps:
			supported = g_phy_rates_suppt[8][idx];
			break;
		case eCsrSuppRate_48Mbps:
			supported = g_phy_rates_suppt[9][idx];
			break;
		case eCsrSuppRate_54Mbps:
			supported = g_phy_rates_suppt[10][idx];
			break;
		case eCsrSuppRate_72Mbps:
			supported = g_phy_rates_suppt[11][idx];
			break;
		case eCsrSuppRate_80Mbps:
			supported = g_phy_rates_suppt[12][idx];
			break;
		case eCsrSuppRate_84Mbps:
			supported = g_phy_rates_suppt[13][idx];
			break;
		case eCsrSuppRate_96Mbps:
			supported = g_phy_rates_suppt[14][idx];
			break;
		case eCsrSuppRate_108Mbps:
			supported = g_phy_rates_suppt[15][idx];
			break;
		case eCsrSuppRate_120Mbps:
			supported = g_phy_rates_suppt[16][idx];
			break;
		case eCsrSuppRate_126Mbps:
			supported = g_phy_rates_suppt[17][idx];
			break;
		case eCsrSuppRate_144Mbps:
			supported = g_phy_rates_suppt[18][idx];
			break;
		case eCsrSuppRate_160Mbps:
			supported = g_phy_rates_suppt[19][idx];
			break;
		case eCsrSuppRate_168Mbps:
			supported = g_phy_rates_suppt[20][idx];
			break;
		case eCsrSuppRate_192Mbps:
			supported = g_phy_rates_suppt[21][idx];
			break;
		case eCsrSuppRate_216Mbps:
			supported = g_phy_rates_suppt[22][idx];
			break;
		case eCsrSuppRate_240Mbps:
			supported = g_phy_rates_suppt[23][idx];
			break;
		default:
			supported = false;
			break;
		}
	}
	return supported;
}

/**
 * csr_is_rate_set_match() - to check if rate set is matching
 * @mac_ctx: pointer to mac context
 * @bss_supported_rates: supported rates of BSS
 * @bss_ext_supp_rates: extended rates of bss
 *
 * This routine is to checke if rate set is matched or no
 *
 * Return: bool
 */
static bool csr_is_rate_set_match(tpAniSirGlobal mac_ctx,
				  tDot11fIESuppRates *bss_supported_rates,
				  tDot11fIEExtSuppRates *bss_ext_supp_rates)
{
	bool match = true;
	uint32_t i;

	/*
	 * Validate that all of the Basic rates advertised in the Bss
	 * description are supported
	 */
	if (bss_supported_rates) {
		for (i = 0; i < bss_supported_rates->num_rates; i++) {
			if (!CSR_IS_BASIC_RATE(bss_supported_rates->rates[i]))
				continue;
			if (!csr_is_aggregate_rate_supported(mac_ctx,
					bss_supported_rates->rates[i])) {
				match = false;
				break;
			}
		}
	}
	if (match && bss_ext_supp_rates) {
		for (i = 0; i < bss_ext_supp_rates->num_rates; i++) {
			if (!CSR_IS_BASIC_RATE(bss_ext_supp_rates->rates[i]))
				continue;
			if (!csr_is_aggregate_rate_supported(mac_ctx,
					bss_ext_supp_rates->rates[i])) {
				match = false;
				break;
			}
		}
	}
	return match;
}

/**
 * csr_match_bss() - to compare the bss
 * @hal: pointer to hal context
 * @bss_descr: pointer bss description
 * @filter: scan filter
 * @neg_auth: negotiated auth
 * @neg_uc: negotiated for unicast
 * @neg_mc: negotiated for multicast
 * @ie_dblptr: double pointer to IE
 *
 * This routine will be called to match the bss
 * If caller want to get the *ie_dblptr allocated by this function,
 * pass in *ie_dblptr = NULL
 *
 * Return: bool
 */
bool csr_match_bss(tHalHandle hal, tSirBssDescription *bss_descr,
		   tCsrScanResultFilter *filter, eCsrAuthType *neg_auth,
		   eCsrEncryptionType *neg_uc, eCsrEncryptionType *neg_mc,
		   tDot11fBeaconIEs **ie_dblptr)
{
	tpAniSirGlobal mac_ctx = PMAC_STRUCT(hal);
	bool rc = false, check, blacklist_check;
	uint32_t i;
	tDot11fBeaconIEs *ie_ptr = NULL;
	uint8_t *pb;
	struct roam_ext_params *roam_params;
	uint8_t *p2p_macaddr = NULL;

	roam_params = &mac_ctx->roam.configParam.roam_params;
	if ((NULL == ie_dblptr) || (*ie_dblptr) == NULL) {
		/* If no IEs passed in, get our own. */
		if (!CDF_IS_STATUS_SUCCESS(
			csr_get_parsed_bss_description_ies(mac_ctx,
				bss_descr, &ie_ptr))) {
			goto end;
		}
	} else {
		/* Save the one pass in for local use */
		ie_ptr = *ie_dblptr;
	}

	/* Check if caller wants P2P */
	check = (!filter->p2pResult || ie_ptr->P2PBeaconProbeRes.present);
	if (!check)
		goto end;

	/* Check for Blacklist BSSID's and avoid connections */
	blacklist_check = false;
	for (i = 0; i < roam_params->num_bssid_avoid_list; i++) {
		if (cdf_is_macaddr_equal((struct cdf_mac_addr *)
					&roam_params->bssid_avoid_list[i],
				(struct cdf_mac_addr *)bss_descr->bssId)) {
			blacklist_check = true;
			break;
		}
	}
	if (blacklist_check) {
		sms_log(mac_ctx, LOGE,
			FL("Don't Attempt connect to blacklist bssid"));
		goto end;
	}

	if (ie_ptr->SSID.present) {
		for (i = 0; i < filter->SSIDs.numOfSSIDs; i++) {
			check = csr_is_ssid_match(mac_ctx,
					filter->SSIDs.SSIDList[i].SSID.ssId,
					filter->SSIDs.SSIDList[i].SSID.length,
					ie_ptr->SSID.ssid,
					ie_ptr->SSID.num_ssid, true);
			if (check)
				break;
		}
		if (!check)
			goto end;
	}
	check = true;
	p2p_macaddr = ie_ptr->P2PBeaconProbeRes.P2PDeviceInfo.P2PDeviceAddress;
	for (i = 0; i < filter->BSSIDs.numOfBSSIDs; i++) {
		check = csr_is_bssid_match(mac_ctx,
				(struct cdf_mac_addr *)&filter->BSSIDs.bssid[i],
				(struct cdf_mac_addr *)bss_descr->bssId);
		if (check)
			break;

		if (filter->p2pResult && ie_ptr->P2PBeaconProbeRes.present) {
			check = csr_is_bssid_match(mac_ctx,
					(struct cdf_mac_addr *)
						&filter->BSSIDs.bssid[i],
					(struct cdf_mac_addr *)p2p_macaddr);
			if (check)
				break;
		}
	}
	if (!check)
		goto end;

	check = true;
	for (i = 0; i < filter->ChannelInfo.numOfChannels; i++) {
		check = csr_is_channel_band_match(mac_ctx,
				filter->ChannelInfo.ChannelList[i], bss_descr);
		if (check)
			break;
	}
	if (!check)
		goto end;
#if defined WLAN_FEATURE_VOWIFI
	/* If this is for measurement filtering */
	if (filter->fMeasurement) {
		rc = true;
		goto end;
	}
#endif
	if (!csr_is_phy_mode_match(mac_ctx, filter->phyMode, bss_descr,
			NULL, NULL, ie_ptr))
		goto end;

#ifdef WLAN_FEATURE_11W
	if ((!filter->bWPSAssociation) && (!filter->bOSENAssociation) &&
			!csr_is_security_match(mac_ctx, &filter->authType,
				&filter->EncryptionType,
				&filter->mcEncryptionType,
				&filter->MFPEnabled,
				&filter->MFPRequired,
				&filter->MFPCapable,
				bss_descr, ie_ptr, neg_auth,
				neg_uc, neg_mc))
#else
	if ((!filter->bWPSAssociation) && (!filter->bOSENAssociation) &&
			!csr_is_security_match(mac_ctx, &filter->authType,
				&filter->EncryptionType,
				&filter->mcEncryptionType,
				NULL, NULL, NULL,
				bss_descr, ie_ptr, neg_auth,
				neg_uc, neg_mc))
#endif
		goto end;
	if (!csr_is_capabilities_match(mac_ctx, filter->BSSType, bss_descr))
		goto end;
	if (!csr_is_rate_set_match(mac_ctx, &ie_ptr->SuppRates,
			&ie_ptr->ExtSuppRates))
		goto end;
	if ((eCsrRoamWmmQbssOnly == mac_ctx->roam.configParam.WMMSupportMode)
			&& !CSR_IS_QOS_BSS(ie_ptr))
		goto end;
	/*
	 * Check country. check even when pb is NULL because we may
	 * want to make sure
	 */
	pb = (filter->countryCode[0]) ? (filter->countryCode) : NULL;
	check = csr_match_country_code(mac_ctx, pb, ie_ptr);
	if (!check)
		goto end;

#ifdef WLAN_FEATURE_VOWIFI_11R
	if (filter->MDID.mdiePresent && csr_roam_is11r_assoc(mac_ctx,
			mac_ctx->roam.roamSession->sessionId)) {
		if (bss_descr->mdiePresent) {
			if (filter->MDID.mobilityDomain !=
					(bss_descr->mdie[1] << 8 |
						bss_descr->mdie[0]))
				goto end;
		} else {
			goto end;
		}
	}
#endif
	rc = true;

end:
	if (ie_dblptr)
		*ie_dblptr = ie_ptr;
	else if (ie_ptr)
		cdf_mem_free(ie_ptr);
	return rc;
}

bool csr_match_connected_bss_security(tpAniSirGlobal pMac,
				      tCsrRoamConnectedProfile *pProfile,
				      tSirBssDescription *pBssDesc,
				      tDot11fBeaconIEs *pIes)
{
	tCsrEncryptionList ucEncryptionList, mcEncryptionList;
	tCsrAuthList authList;

	ucEncryptionList.numEntries = 1;
	ucEncryptionList.encryptionType[0] = pProfile->EncryptionType;

	mcEncryptionList.numEntries = 1;
	mcEncryptionList.encryptionType[0] = pProfile->mcEncryptionType;

	authList.numEntries = 1;
	authList.authType[0] = pProfile->AuthType;

#ifdef WLAN_FEATURE_11W
	return csr_is_security_match(pMac, &authList, &ucEncryptionList,
					&mcEncryptionList,
					&pProfile->MFPEnabled,
					&pProfile->MFPRequired,
					&pProfile->MFPCapable,
					pBssDesc, pIes, NULL, NULL, NULL);
#else
	return csr_is_security_match(pMac, &authList, &ucEncryptionList,
				      &mcEncryptionList, NULL, NULL, NULL,
				      pBssDesc, pIes, NULL, NULL, NULL);
#endif

}

bool csr_match_bss_to_connect_profile(tHalHandle hHal,
				      tCsrRoamConnectedProfile *pProfile,
				      tSirBssDescription *pBssDesc,
				      tDot11fBeaconIEs *pIes)
{
	tpAniSirGlobal pMac = PMAC_STRUCT(hHal);
	bool fRC = false, fCheck;
	tDot11fBeaconIEs *pIesLocal = pIes;

	do {
		if (!pIes) {
			if (!CDF_IS_STATUS_SUCCESS
				    (csr_get_parsed_bss_description_ies
					    (pMac, pBssDesc, &pIesLocal))) {
				break;
			}
		}
		fCheck = true;
		if (pIesLocal->SSID.present) {
			bool fCheckSsid = false;
			if (pProfile->SSID.length) {
				fCheckSsid = true;
			}
			fCheck =
				csr_is_ssid_match(pMac, pProfile->SSID.ssId,
						  pProfile->SSID.length,
						  pIesLocal->SSID.ssid,
						  pIesLocal->SSID.num_ssid,
						  fCheckSsid);
			if (!fCheck)
				break;
		}
		if (!csr_match_connected_bss_security
			    (pMac, pProfile, pBssDesc, pIesLocal))
			break;
		if (!csr_is_capabilities_match(pMac, pProfile->BSSType, pBssDesc))
			break;
		if (!csr_is_rate_set_match
			    (pMac, &pIesLocal->SuppRates, &pIesLocal->ExtSuppRates))
			break;
		fCheck =
			csr_is_channel_band_match(pMac, pProfile->operationChannel,
						  pBssDesc);
		if (!fCheck)
			break;

		fRC = true;

	} while (0);

	if (!pIes && pIesLocal) {
		/* locally allocated */
		cdf_mem_free(pIesLocal);
	}

	return fRC;
}

void csr_add_rate_bitmap(uint8_t rate, uint16_t *pRateBitmap)
{
	uint16_t rateBitmap;
	uint16_t n = BITS_OFF(rate, CSR_DOT11_BASIC_RATE_MASK);
	rateBitmap = *pRateBitmap;
	switch (n) {
	case SIR_MAC_RATE_1:
		rateBitmap |= SIR_MAC_RATE_1_BITMAP;
		break;
	case SIR_MAC_RATE_2:
		rateBitmap |= SIR_MAC_RATE_2_BITMAP;
		break;
	case SIR_MAC_RATE_5_5:
		rateBitmap |= SIR_MAC_RATE_5_5_BITMAP;
		break;
	case SIR_MAC_RATE_11:
		rateBitmap |= SIR_MAC_RATE_11_BITMAP;
		break;
	case SIR_MAC_RATE_6:
		rateBitmap |= SIR_MAC_RATE_6_BITMAP;
		break;
	case SIR_MAC_RATE_9:
		rateBitmap |= SIR_MAC_RATE_9_BITMAP;
		break;
	case SIR_MAC_RATE_12:
		rateBitmap |= SIR_MAC_RATE_12_BITMAP;
		break;
	case SIR_MAC_RATE_18:
		rateBitmap |= SIR_MAC_RATE_18_BITMAP;
		break;
	case SIR_MAC_RATE_24:
		rateBitmap |= SIR_MAC_RATE_24_BITMAP;
		break;
	case SIR_MAC_RATE_36:
		rateBitmap |= SIR_MAC_RATE_36_BITMAP;
		break;
	case SIR_MAC_RATE_48:
		rateBitmap |= SIR_MAC_RATE_48_BITMAP;
		break;
	case SIR_MAC_RATE_54:
		rateBitmap |= SIR_MAC_RATE_54_BITMAP;
		break;
	}
	*pRateBitmap = rateBitmap;
}

bool csr_check_rate_bitmap(uint8_t rate, uint16_t rateBitmap)
{
	uint16_t n = BITS_OFF(rate, CSR_DOT11_BASIC_RATE_MASK);

	switch (n) {
	case SIR_MAC_RATE_1:
		rateBitmap &= SIR_MAC_RATE_1_BITMAP;
		break;
	case SIR_MAC_RATE_2:
		rateBitmap &= SIR_MAC_RATE_2_BITMAP;
		break;
	case SIR_MAC_RATE_5_5:
		rateBitmap &= SIR_MAC_RATE_5_5_BITMAP;
		break;
	case SIR_MAC_RATE_11:
		rateBitmap &= SIR_MAC_RATE_11_BITMAP;
		break;
	case SIR_MAC_RATE_6:
		rateBitmap &= SIR_MAC_RATE_6_BITMAP;
		break;
	case SIR_MAC_RATE_9:
		rateBitmap &= SIR_MAC_RATE_9_BITMAP;
		break;
	case SIR_MAC_RATE_12:
		rateBitmap &= SIR_MAC_RATE_12_BITMAP;
		break;
	case SIR_MAC_RATE_18:
		rateBitmap &= SIR_MAC_RATE_18_BITMAP;
		break;
	case SIR_MAC_RATE_24:
		rateBitmap &= SIR_MAC_RATE_24_BITMAP;
		break;
	case SIR_MAC_RATE_36:
		rateBitmap &= SIR_MAC_RATE_36_BITMAP;
		break;
	case SIR_MAC_RATE_48:
		rateBitmap &= SIR_MAC_RATE_48_BITMAP;
		break;
	case SIR_MAC_RATE_54:
		rateBitmap &= SIR_MAC_RATE_54_BITMAP;
		break;
	}
	return !!rateBitmap;
}

bool csr_rates_is_dot11_rate_supported(tHalHandle hHal, uint8_t rate)
{
	tpAniSirGlobal pMac = PMAC_STRUCT(hHal);
	uint16_t n = BITS_OFF(rate, CSR_DOT11_BASIC_RATE_MASK);

	return csr_is_aggregate_rate_supported(pMac, n);
}

uint16_t csr_rates_mac_prop_to_dot11(uint16_t Rate)
{
	uint16_t ConvertedRate = Rate;

	switch (Rate) {
	case SIR_MAC_RATE_1:
		ConvertedRate = 2;
		break;
	case SIR_MAC_RATE_2:
		ConvertedRate = 4;
		break;
	case SIR_MAC_RATE_5_5:
		ConvertedRate = 11;
		break;
	case SIR_MAC_RATE_11:
		ConvertedRate = 22;
		break;

	case SIR_MAC_RATE_6:
		ConvertedRate = 12;
		break;
	case SIR_MAC_RATE_9:
		ConvertedRate = 18;
		break;
	case SIR_MAC_RATE_12:
		ConvertedRate = 24;
		break;
	case SIR_MAC_RATE_18:
		ConvertedRate = 36;
		break;
	case SIR_MAC_RATE_24:
		ConvertedRate = 48;
		break;
	case SIR_MAC_RATE_36:
		ConvertedRate = 72;
		break;
	case SIR_MAC_RATE_42:
		ConvertedRate = 84;
		break;
	case SIR_MAC_RATE_48:
		ConvertedRate = 96;
		break;
	case SIR_MAC_RATE_54:
		ConvertedRate = 108;
		break;

	case SIR_MAC_RATE_72:
		ConvertedRate = 144;
		break;
	case SIR_MAC_RATE_84:
		ConvertedRate = 168;
		break;
	case SIR_MAC_RATE_96:
		ConvertedRate = 192;
		break;
	case SIR_MAC_RATE_108:
		ConvertedRate = 216;
		break;
	case SIR_MAC_RATE_126:
		ConvertedRate = 252;
		break;
	case SIR_MAC_RATE_144:
		ConvertedRate = 288;
		break;
	case SIR_MAC_RATE_168:
		ConvertedRate = 336;
		break;
	case SIR_MAC_RATE_192:
		ConvertedRate = 384;
		break;
	case SIR_MAC_RATE_216:
		ConvertedRate = 432;
		break;
	case SIR_MAC_RATE_240:
		ConvertedRate = 480;
		break;

	case 0xff:
		ConvertedRate = 0;
		break;
	}

	return ConvertedRate;
}

uint16_t csr_rates_find_best_rate(tSirMacRateSet *pSuppRates,
				  tSirMacRateSet *pExtRates,
				  tSirMacPropRateSet *pPropRates)
{
	uint8_t i;
	uint16_t nBest;

	nBest = pSuppRates->rate[0] & (~CSR_DOT11_BASIC_RATE_MASK);

	if (pSuppRates->numRates > SIR_MAC_RATESET_EID_MAX) {
		pSuppRates->numRates = SIR_MAC_RATESET_EID_MAX;
	}

	for (i = 1U; i < pSuppRates->numRates; ++i) {
		nBest =
			(uint16_t) CSR_MAX(nBest,
					   pSuppRates->
					   rate[i] & (~CSR_DOT11_BASIC_RATE_MASK));
	}

	if (NULL != pExtRates) {
		for (i = 0U; i < pExtRates->numRates; ++i) {
			nBest =
				(uint16_t) CSR_MAX(nBest,
						   pExtRates->
						   rate[i] &
						   (~CSR_DOT11_BASIC_RATE_MASK));
		}
	}

	if (NULL != pPropRates) {
		for (i = 0U; i < pPropRates->numPropRates; ++i) {
			nBest =
				(uint16_t) CSR_MAX(nBest,
						   csr_rates_mac_prop_to_dot11
							   (pPropRates->propRate[i]));
		}
	}

	return nBest;
}

void csr_release_profile(tpAniSirGlobal pMac, tCsrRoamProfile *pProfile)
{
	if (pProfile) {
		if (pProfile->BSSIDs.bssid) {
			cdf_mem_free(pProfile->BSSIDs.bssid);
			pProfile->BSSIDs.bssid = NULL;
		}
		if (pProfile->SSIDs.SSIDList) {
			cdf_mem_free(pProfile->SSIDs.SSIDList);
			pProfile->SSIDs.SSIDList = NULL;
		}
		if (pProfile->pWPAReqIE) {
			cdf_mem_free(pProfile->pWPAReqIE);
			pProfile->pWPAReqIE = NULL;
		}
		if (pProfile->pRSNReqIE) {
			cdf_mem_free(pProfile->pRSNReqIE);
			pProfile->pRSNReqIE = NULL;
		}
#ifdef FEATURE_WLAN_WAPI
		if (pProfile->pWAPIReqIE) {
			cdf_mem_free(pProfile->pWAPIReqIE);
			pProfile->pWAPIReqIE = NULL;
		}
#endif /* FEATURE_WLAN_WAPI */

		if (pProfile->pAddIEScan) {
			cdf_mem_free(pProfile->pAddIEScan);
			pProfile->pAddIEScan = NULL;
		}

		if (pProfile->pAddIEAssoc) {
			cdf_mem_free(pProfile->pAddIEAssoc);
			pProfile->pAddIEAssoc = NULL;
		}
		if (pProfile->ChannelInfo.ChannelList) {
			cdf_mem_free(pProfile->ChannelInfo.ChannelList);
			pProfile->ChannelInfo.ChannelList = NULL;
		}
		cdf_mem_set(pProfile, sizeof(tCsrRoamProfile), 0);
	}
}

void csr_free_scan_filter(tpAniSirGlobal pMac, tCsrScanResultFilter *pScanFilter)
{
	if (pScanFilter->BSSIDs.bssid) {
		cdf_mem_free(pScanFilter->BSSIDs.bssid);
		pScanFilter->BSSIDs.bssid = NULL;
	}
	if (pScanFilter->ChannelInfo.ChannelList) {
		cdf_mem_free(pScanFilter->ChannelInfo.ChannelList);
		pScanFilter->ChannelInfo.ChannelList = NULL;
	}
	if (pScanFilter->SSIDs.SSIDList) {
		cdf_mem_free(pScanFilter->SSIDs.SSIDList);
		pScanFilter->SSIDs.SSIDList = NULL;
	}
}

void csr_free_roam_profile(tpAniSirGlobal pMac, uint32_t sessionId)
{
	tCsrRoamSession *pSession = &pMac->roam.roamSession[sessionId];

	if (pSession->pCurRoamProfile) {
		csr_release_profile(pMac, pSession->pCurRoamProfile);
		cdf_mem_free(pSession->pCurRoamProfile);
		pSession->pCurRoamProfile = NULL;
	}
}

void csr_free_connect_bss_desc(tpAniSirGlobal pMac, uint32_t sessionId)
{
	tCsrRoamSession *pSession = &pMac->roam.roamSession[sessionId];

	if (pSession->pConnectBssDesc) {
		cdf_mem_free(pSession->pConnectBssDesc);
		pSession->pConnectBssDesc = NULL;
	}
}

tSirResultCodes csr_get_disassoc_rsp_status_code(tSirSmeDisassocRsp *
						 pSmeDisassocRsp)
{
	uint8_t *pBuffer = (uint8_t *) pSmeDisassocRsp;
	uint32_t ret;

	pBuffer += (sizeof(uint16_t) + sizeof(uint16_t) + sizeof(tSirMacAddr));
	/* tSirResultCodes is an enum, assuming is 32bit */
	/* If we cannot make this assumption, use copymemory */
	cdf_get_u32(pBuffer, &ret);

	return (tSirResultCodes) ret;
}

tSirResultCodes csr_get_de_auth_rsp_status_code(tSirSmeDeauthRsp *pSmeRsp)
{
	uint8_t *pBuffer = (uint8_t *) pSmeRsp;
	uint32_t ret;

	pBuffer +=
		(sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint8_t) +
		 sizeof(uint16_t));
	/* tSirResultCodes is an enum, assuming is 32bit */
	/* If we cannot make this assumption, use copymemory */
	cdf_get_u32(pBuffer, &ret);

	return (tSirResultCodes) ret;
}

tSirScanType csr_get_scan_type(tpAniSirGlobal pMac, uint8_t chnId)
{
	tSirScanType scanType = eSIR_PASSIVE_SCAN;
	CHANNEL_STATE channelEnabledType;

	channelEnabledType = cds_get_channel_state(chnId);
	if (CHANNEL_STATE_ENABLE == channelEnabledType) {
		scanType = eSIR_ACTIVE_SCAN;
	}
	return scanType;
}

uint8_t csr_to_upper(uint8_t ch)
{
	uint8_t chOut;

	if (ch >= 'a' && ch <= 'z') {
		chOut = ch - 'a' + 'A';
	} else {
		chOut = ch;
	}
	return chOut;
}

tSirBssType csr_translate_bsstype_to_mac_type(eCsrRoamBssType csrtype)
{
	tSirBssType ret;

	switch (csrtype) {
	case eCSR_BSS_TYPE_INFRASTRUCTURE:
		ret = eSIR_INFRASTRUCTURE_MODE;
		break;
	case eCSR_BSS_TYPE_IBSS:
	case eCSR_BSS_TYPE_START_IBSS:
		ret = eSIR_IBSS_MODE;
		break;
	case eCSR_BSS_TYPE_WDS_AP:
		ret = eSIR_BTAMP_AP_MODE;
		break;
	case eCSR_BSS_TYPE_WDS_STA:
		ret = eSIR_BTAMP_STA_MODE;
		break;
	case eCSR_BSS_TYPE_INFRA_AP:
		ret = eSIR_INFRA_AP_MODE;
		break;
	case eCSR_BSS_TYPE_ANY:
	default:
		ret = eSIR_AUTO_MODE;
		break;
	}

	return ret;
}

/* This function use the parameters to decide the CFG value. */
/* CSR never sets WNI_CFG_DOT11_MODE_ALL to the CFG */
/* So PE should not see WNI_CFG_DOT11_MODE_ALL when it gets the CFG value */
eCsrCfgDot11Mode csr_get_cfg_dot11_mode_from_csr_phy_mode(tCsrRoamProfile *pProfile,
							  eCsrPhyMode phyMode,
							  bool fProprietary)
{
	uint32_t cfgDot11Mode = eCSR_CFG_DOT11_MODE_ABG;

	switch (phyMode) {
	case eCSR_DOT11_MODE_11a:
		cfgDot11Mode = eCSR_CFG_DOT11_MODE_11A;
		break;
	case eCSR_DOT11_MODE_11b:
	case eCSR_DOT11_MODE_11b_ONLY:
		cfgDot11Mode = eCSR_CFG_DOT11_MODE_11B;
		break;
	case eCSR_DOT11_MODE_11g:
	case eCSR_DOT11_MODE_11g_ONLY:
		if (pProfile && (CSR_IS_INFRA_AP(pProfile))
		    && (phyMode == eCSR_DOT11_MODE_11g_ONLY))
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11G_ONLY;
		else
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11G;
		break;
	case eCSR_DOT11_MODE_11n:
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11N;
		break;
	case eCSR_DOT11_MODE_11n_ONLY:
		if (pProfile && CSR_IS_INFRA_AP(pProfile))
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11N_ONLY;
		else
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11N;
		break;
	case eCSR_DOT11_MODE_abg:
		cfgDot11Mode = eCSR_CFG_DOT11_MODE_ABG;
		break;
	case eCSR_DOT11_MODE_AUTO:
		cfgDot11Mode = eCSR_CFG_DOT11_MODE_AUTO;
		break;

#ifdef WLAN_FEATURE_11AC
	case eCSR_DOT11_MODE_11ac:
		if (IS_FEATURE_SUPPORTED_BY_FW(DOT11AC)) {
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11AC;
		} else {
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11N;
		}
		break;
	case eCSR_DOT11_MODE_11ac_ONLY:
		if (IS_FEATURE_SUPPORTED_BY_FW(DOT11AC)) {
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11AC_ONLY;
		} else {
			cfgDot11Mode = eCSR_CFG_DOT11_MODE_11N;
		}
		break;
#endif
	default:
		/* No need to assign anything here */
		break;
	}

	return cfgDot11Mode;
}

CDF_STATUS csr_get_regulatory_domain_for_country
	(tpAniSirGlobal pMac,
	uint8_t *pCountry,
	v_REGDOMAIN_t *pDomainId, v_CountryInfoSource_t source) {
	CDF_STATUS status = CDF_STATUS_E_INVAL;
	CDF_STATUS cdf_status;
	country_code_t countryCode;
	v_REGDOMAIN_t domainId;

	if (pCountry) {
		countryCode[0] = pCountry[0];
		countryCode[1] = pCountry[1];
		cdf_status = cds_get_reg_domain_from_country_code(&domainId,
								  countryCode,
								  source);

		if (CDF_IS_STATUS_SUCCESS(cdf_status)) {
			if (pDomainId) {
				*pDomainId = domainId;
			}
			status = CDF_STATUS_SUCCESS;
		} else {
			sms_log(pMac, LOGW,
				FL
					(" Couldn't find domain for country code  %c%c"),
				pCountry[0], pCountry[1]);
			status = CDF_STATUS_E_INVAL;
		}
	}

	return status;
}

/* To check whether a country code matches the one in the IE */
/* Only check the first two characters, ignoring in/outdoor */
/* pCountry -- caller allocated buffer contain the country code that is checking against */
/* the one in pIes. It can be NULL. */
/* caller must provide pIes, it cannot be NULL */
/* This function always return true if 11d support is not turned on. */
bool csr_match_country_code(tpAniSirGlobal pMac, uint8_t *pCountry,
			    tDot11fBeaconIEs *pIes)
{
	bool fRet = true;

	do {
		if (!csr_is11d_supported(pMac)) {
			break;
		}
		if (!pIes) {
			sms_log(pMac, LOGE, FL("  No IEs"));
			break;
		}

		if (pCountry) {
			uint32_t i;

			if (!pIes->Country.present) {
				fRet = false;
				break;
			}
			/* Convert the CountryCode characters to upper */
			for (i = 0; i < WNI_CFG_COUNTRY_CODE_LEN - 1; i++) {
				pCountry[i] = csr_to_upper(pCountry[i]);
			}
			if (!cdf_mem_compare(pIes->Country.country, pCountry,
					     WNI_CFG_COUNTRY_CODE_LEN - 1)) {
				fRet = false;
				break;
			}
		}
	} while (0);

	return fRet;
}

CDF_STATUS csr_get_modify_profile_fields(tpAniSirGlobal pMac, uint32_t sessionId,
					 tCsrRoamModifyProfileFields *
					 pModifyProfileFields)
{

	if (!pModifyProfileFields) {
		return CDF_STATUS_E_FAILURE;
	}

	cdf_mem_copy(pModifyProfileFields,
		     &pMac->roam.roamSession[sessionId].connectedProfile.
		     modifyProfileFields, sizeof(tCsrRoamModifyProfileFields));

	return CDF_STATUS_SUCCESS;
}

CDF_STATUS csr_set_modify_profile_fields(tpAniSirGlobal pMac, uint32_t sessionId,
					 tCsrRoamModifyProfileFields *
					 pModifyProfileFields)
{
	tCsrRoamSession *pSession = CSR_GET_SESSION(pMac, sessionId);

	cdf_mem_copy(&pSession->connectedProfile.modifyProfileFields,
		     pModifyProfileFields, sizeof(tCsrRoamModifyProfileFields));

	return CDF_STATUS_SUCCESS;
}


bool csr_is_set_key_allowed(tpAniSirGlobal pMac, uint32_t sessionId)
{
	bool fRet = true;
	tCsrRoamSession *pSession;

	pSession = CSR_GET_SESSION(pMac, sessionId);

	/*
	 * This condition is not working for infra state. When infra is in
	 * not-connected state the pSession->pCurRoamProfile is NULL, this
	 * function returns true, that is incorrect.
	 * Since SAP requires to set key without any BSS started, it needs
	 * this condition to be met. In other words, this function is useless.
	 * The current work-around is to process setcontext_rsp no matter
	 * what the state is.
	 */
	sms_log(pMac, LOG2,
		FL(" is not what it intends to. Must be revisit or removed"));
	if ((NULL == pSession)
	    || (csr_is_conn_state_disconnected(pMac, sessionId)
		&& (pSession->pCurRoamProfile != NULL)
		&& (!(CSR_IS_INFRA_AP(pSession->pCurRoamProfile))))
	    ) {
		fRet = false;
	}

	return fRet;
}

/* no need to acquire lock for this basic function */
uint16_t sme_chn_to_freq(uint8_t chanNum)
{
	int i;

	for (i = 0; i < NUM_RF_CHANNELS; i++) {
		if (rf_channels[i].channelNum == chanNum) {
			return rf_channels[i].targetFreq;
		}
	}

	return 0;
}

/* Disconnect all active sessions by sending disassoc. This is mainly used to disconnect the remaining session when we
 * transition from concurrent sessions to a single session. The use case is Infra STA and wifi direct multiple sessions are up and
 * P2P session is removed. The Infra STA session remains and should resume BMPS if BMPS is enabled by default. However, there
 * are some issues seen with BMPS resume during this transition and this is a workaround which will allow the Infra STA session to
 * disconnect and auto connect back and enter BMPS this giving the same effect as resuming BMPS
 */

/* Remove this code once SLM_Sessionization is supported */
/* BMPS_WORKAROUND_NOT_NEEDED */
void csr_disconnect_all_active_sessions(tpAniSirGlobal pMac)
{
	uint8_t i;

	/* Disconnect all the active sessions */
	for (i = 0; i < CSR_ROAM_SESSION_MAX; i++) {
		if (CSR_IS_SESSION_VALID(pMac, i)
		    && !csr_is_conn_state_disconnected(pMac, i)) {
			csr_roam_disconnect_internal(pMac, i,
						     eCSR_DISCONNECT_REASON_UNSPECIFIED);
		}
	}
}

#ifdef FEATURE_WLAN_LFR
bool csr_is_channel_present_in_list(uint8_t *pChannelList,
				    int numChannels, uint8_t channel)
{
	int i = 0;

	/* Check for NULL pointer */
	if (!pChannelList || (numChannels == 0)) {
		return false;
	}
	/* Look for the channel in the list */
	for (i = 0; (i < numChannels) &&
	     (i < WNI_CFG_VALID_CHANNEL_LIST_LEN); i++) {
		if (pChannelList[i] == channel)
			return true;
	}

	return false;
}

CDF_STATUS csr_add_to_channel_list_front(uint8_t *pChannelList,
					 int numChannels, uint8_t channel)
{
	int i = 0;

	/* Check for NULL pointer */
	if (!pChannelList)
		return CDF_STATUS_E_NULL_VALUE;

	/* Make room for the addition.  (Start moving from the back.) */
	for (i = numChannels; i > 0; i--) {
		pChannelList[i] = pChannelList[i - 1];
	}

	/* Now add the NEW channel...at the front */
	pChannelList[0] = channel;

	return CDF_STATUS_SUCCESS;
}
#endif
#ifdef FEATURE_WLAN_DIAG_SUPPORT
/**
 * csr_diag_event_report() - send PE diag event
 * @pmac:        pointer to global MAC context.
 * @event_typev: sub event type for DIAG event.
 * @status:      status of the event
 * @reasoncode:  reasoncode for the given status
 *
 * This function is called to send diag event
 *
 * Return:   NA
 */
void csr_diag_event_report(tpAniSirGlobal pmac, uint16_t event_type,
			   uint16_t status, uint16_t reasoncode)
{
	WLAN_HOST_DIAG_EVENT_DEF(diag_event, host_event_wlan_pe_payload_type);

	cdf_mem_zero(&diag_event, sizeof(host_event_wlan_pe_payload_type));

	/* diag_event.bssid is already all zeroes */
	diag_event.sme_state = sme_get_lim_sme_state(pmac);
	diag_event.mlm_state = sme_get_lim_mlm_state(pmac);
	diag_event.event_type = event_type;
	diag_event.status = status;
	diag_event.reason_code = reasoncode;

	WLAN_HOST_DIAG_EVENT_REPORT(&diag_event, EVENT_WLAN_PE);
	return;
}
#endif

/**
 * csr_wait_for_connection_update() - Wait for hw mode update
 * @mac: Pointer to the MAC context
 * @do_release_reacquire_lock: Indicates whether release and
 * re-acquisition of SME global lock is required.
 *
 * Waits for CONNECTION_UPDATE_TIMEOUT time so that the
 * hw mode update can get processed.
 *
 * Return: True if the wait was successful, false otherwise
 */
bool csr_wait_for_connection_update(tpAniSirGlobal mac,
		bool do_release_reacquire_lock)
{
	CDF_STATUS status, ret;

	if (do_release_reacquire_lock == true) {
		ret = sme_release_global_lock(&mac->sme);
		if (!CDF_IS_STATUS_SUCCESS(ret)) {
			cds_err("lock release fail %d", ret);
			return false;
		}
	}

	status = cdf_wait_for_connection_update();

	if (do_release_reacquire_lock == true) {
		ret = sme_acquire_global_lock(&mac->sme);
		if (!CDF_IS_STATUS_SUCCESS(ret)) {
			cds_err("lock acquire fail %d", ret);
			return false;
		}
	}

	if (!CDF_IS_STATUS_SUCCESS(status)) {
		cds_err("wait for event failed");
		return false;
	}

	return true;
}
