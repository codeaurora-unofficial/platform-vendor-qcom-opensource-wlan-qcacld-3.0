/*
 * Copyright (c) 2012-2015 The Linux Foundation. All rights reserved.
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

/*
 * This file sir_api.h contains definitions exported by
 * Sirius software.
 * Author:        Chandra Modumudi
 * Date:          04/16/2002
 * History:-
 * Date           Modified by    Modification Information
 * --------------------------------------------------------------------
 */

#ifndef __SIR_API_H
#define __SIR_API_H

#include "sir_types.h"
#include "sir_mac_prot_def.h"
#include "ani_system_defs.h"
#include "sir_params.h"

#define OFFSET_OF(structType, fldName)   (&((structType *)0)->fldName)

/* / Max supported channel list */
#define SIR_MAX_SUPPORTED_CHANNEL_LIST      96

#define SIR_MDIE_ELEMENT_ID         54
#define SIR_MDIE_SIZE               3

/* Increase dwell time for P2P search in ms */
#define P2P_SEARCH_DWELL_TIME_INCREASE   20
#define P2P_SOCIAL_CHANNELS              3

/* Max number of channels are 165, but to access 165th element of array,
   *array of 166 is required.
 */
#define SIR_MAX_24G_5G_CHANNEL_RANGE      166
#define SIR_BCN_REPORT_MAX_BSS_DESC       4

#define SIR_NUM_11B_RATES 4     /* 1,2,5.5,11 */
#define SIR_NUM_11A_RATES 8     /* 6,9,12,18,24,36,48,54 */

#define SIR_PM_SLEEP_MODE   0
#define SIR_PM_ACTIVE_MODE        1

/* hidden SSID options */
#define SIR_SCAN_NO_HIDDEN_SSID                      0
#define SIR_SCAN_HIDDEN_SSID_PE_DECISION             1

#define SIR_MAC_ADDR_LEN        6
#define SIR_IPV4_ADDR_LEN       4

typedef uint8_t tSirIpv4Addr[SIR_IPV4_ADDR_LEN];

#define SIR_VERSION_STRING_LEN 64
typedef uint8_t tSirVersionString[SIR_VERSION_STRING_LEN];

/* Periodic Tx pattern offload feature */
#define PERIODIC_TX_PTRN_MAX_SIZE 1536
#define MAXNUM_PERIODIC_TX_PTRNS 6
#define WIFI_SCANNING_MAC_OUI_LENGTH 3

#ifdef FEATURE_WLAN_EXTSCAN

#define WLAN_EXTSCAN_MAX_CHANNELS                 36
#define WLAN_EXTSCAN_MAX_BUCKETS                  16
#define WLAN_EXTSCAN_MAX_HOTLIST_APS              128
#define WLAN_EXTSCAN_MAX_SIGNIFICANT_CHANGE_APS   64
#define WLAN_EXTSCAN_MAX_HOTLIST_SSIDS            8

/* This should not be greater than MAX_NUMBER_OF_CONC_CONNECTIONS */
#define MAX_VDEV_SUPPORTED                        4

/**
 * enum cds_conn_update_reason: Reason for conc connection update
 * @CDS_UPDATE_REASON_SET_OPER_CHAN: Set probable operating channel
 * @CDS_UPDATE_REASON_JOIN_IBSS: Join IBSS
 * @CDS_UPDATE_REASON_UT: Unit test related
 * @CDS_UPDATE_REASON_START_AP: Start AP
 * @CDS_UPDATE_REASON_NORMAL_STA: Connection to Normal STA
 * @CDS_UPDATE_REASON_HIDDEN_STA: Connection to Hidden STA
 * @CDS_UPDATE_REASON_OPPORTUNISTIC: Opportunistic HW mode update
 * @CDS_UPDATE_REASON_NSS_UPDATE: NSS update
 */
enum cds_conn_update_reason {
	CDS_UPDATE_REASON_SET_OPER_CHAN,
	CDS_UPDATE_REASON_JOIN_IBSS,
	CDS_UPDATE_REASON_UT,
	CDS_UPDATE_REASON_START_AP,
	CDS_UPDATE_REASON_NORMAL_STA,
	CDS_UPDATE_REASON_HIDDEN_STA,
	CDS_UPDATE_REASON_OPPORTUNISTIC,
	CDS_UPDATE_REASON_NSS_UPDATE,
};

typedef enum {
	eSIR_EXTSCAN_INVALID,
	eSIR_EXTSCAN_START_RSP,
	eSIR_EXTSCAN_STOP_RSP,
	eSIR_EXTSCAN_CACHED_RESULTS_RSP,
	eSIR_EXTSCAN_SET_BSSID_HOTLIST_RSP,
	eSIR_EXTSCAN_RESET_BSSID_HOTLIST_RSP,
	eSIR_EXTSCAN_SET_SIGNIFICANT_WIFI_CHANGE_RSP,
	eSIR_EXTSCAN_RESET_SIGNIFICANT_WIFI_CHANGE_RSP,

	eSIR_EXTSCAN_GET_CAPABILITIES_IND,
	eSIR_EXTSCAN_HOTLIST_MATCH_IND,
	eSIR_EXTSCAN_SIGNIFICANT_WIFI_CHANGE_RESULTS_IND,
	eSIR_EXTSCAN_CACHED_RESULTS_IND,
	eSIR_EXTSCAN_SCAN_RES_AVAILABLE_IND,
	eSIR_EXTSCAN_SCAN_PROGRESS_EVENT_IND,
	eSIR_EXTSCAN_FULL_SCAN_RESULT_IND,
	eSIR_EPNO_NETWORK_FOUND_IND,
	eSIR_PASSPOINT_NETWORK_FOUND_IND,
	eSIR_EXTSCAN_SET_SSID_HOTLIST_RSP,
	eSIR_EXTSCAN_RESET_SSID_HOTLIST_RSP,
	eSIR_EXTSCAN_HOTLIST_SSID_MATCH_IND,

	/* Keep this last */
	eSIR_EXTSCAN_CALLBACK_TYPE_MAX,
} tSirExtScanCallbackType;

#endif /* FEATURE_WLAN_EXTSCAN */

#define SIR_KRK_KEY_LEN 16
#ifdef WLAN_FEATURE_ROAM_OFFLOAD
#define SIR_BTK_KEY_LEN 32
#define SIR_KCK_KEY_LEN 16
#define SIR_KEK_KEY_LEN 16
#define SIR_REPLAY_CTR_LEN 8

#define SIR_UAPSD_BITOFFSET_ACVO     0
#define SIR_UAPSD_BITOFFSET_ACVI     1
#define SIR_UAPSD_BITOFFSET_ACBK     2
#define SIR_UAPSD_BITOFFSET_ACBE     3

#define SIR_UAPSD_FLAG_ACVO     (1 << SIR_UAPSD_BITOFFSET_ACVO)
#define SIR_UAPSD_FLAG_ACVI     (1 << SIR_UAPSD_BITOFFSET_ACVI)
#define SIR_UAPSD_FLAG_ACBK     (1 << SIR_UAPSD_BITOFFSET_ACBK)
#define SIR_UAPSD_FLAG_ACBE     (1 << SIR_UAPSD_BITOFFSET_ACBE)
#define SIR_UAPSD_GET(ac, mask)      (((mask) & (SIR_UAPSD_FLAG_ ## ac)) >> SIR_UAPSD_BITOFFSET_ ## ac)
#endif

/**
 * Module ID definitions.
 */
enum {
	SIR_BOOT_MODULE_ID = 1,
	SIR_HAL_MODULE_ID = 0x10,
	SIR_CFG_MODULE_ID = 0x12,
	SIR_LIM_MODULE_ID,
	SIR_ARQ_MODULE_ID,
	SIR_SCH_MODULE_ID,
	SIR_PMM_MODULE_ID,
	SIR_MNT_MODULE_ID,
	SIR_DBG_MODULE_ID,
	SIR_DPH_MODULE_ID,
	SIR_SYS_MODULE_ID,
	SIR_SMS_MODULE_ID,
};

#define SIR_WMA_MODULE_ID SIR_HAL_MODULE_ID

/**
 * First and last module definition for logging utility
 *
 * NOTE:  The following definitions need to be updated if
 *        the above list is changed.
 */
#define SIR_FIRST_MODULE_ID     SIR_HAL_MODULE_ID
#define SIR_LAST_MODULE_ID      SIR_SMS_MODULE_ID

/* Type declarations used by Firmware and Host software */

/* Scan type enum used in scan request */
typedef enum eSirScanType {
	eSIR_PASSIVE_SCAN,
	eSIR_ACTIVE_SCAN,
	eSIR_BEACON_TABLE,
} tSirScanType;

/* / Result codes Firmware return to Host SW */
typedef enum eSirResultCodes {
	eSIR_SME_SUCCESS,
	eSIR_LOGP_EXCEPTION,
	eSIR_SME_INVALID_PARAMETERS = 500,
	eSIR_SME_UNEXPECTED_REQ_RESULT_CODE,
	eSIR_SME_RESOURCES_UNAVAILABLE,
	/* Unable to find a BssDescription */
	eSIR_SME_SCAN_FAILED,
	/* matching requested scan criteria */
	eSIR_SME_BSS_ALREADY_STARTED_OR_JOINED,
	eSIR_SME_LOST_LINK_WITH_PEER_RESULT_CODE,
	eSIR_SME_REFUSED,
	eSIR_SME_JOIN_DEAUTH_FROM_AP_DURING_ADD_STA,
	eSIR_SME_JOIN_TIMEOUT_RESULT_CODE,
	eSIR_SME_AUTH_TIMEOUT_RESULT_CODE,
	eSIR_SME_ASSOC_TIMEOUT_RESULT_CODE,
	eSIR_SME_REASSOC_TIMEOUT_RESULT_CODE,
	eSIR_SME_MAX_NUM_OF_PRE_AUTH_REACHED,
	eSIR_SME_AUTH_REFUSED,
	eSIR_SME_INVALID_WEP_DEFAULT_KEY,
	eSIR_SME_NO_KEY_MAPPING_KEY_FOR_PEER,
	eSIR_SME_ASSOC_REFUSED,
	eSIR_SME_REASSOC_REFUSED,
	/* Recvd Deauth while join/pre-auth */
	eSIR_SME_DEAUTH_WHILE_JOIN,
	eSIR_SME_STA_NOT_AUTHENTICATED,
	eSIR_SME_STA_NOT_ASSOCIATED,
	eSIR_SME_ALREADY_JOINED_A_BSS,
	/* Given in SME_SCAN_RSP msg */
	eSIR_SME_MORE_SCAN_RESULTS_FOLLOW,
	/* that more SME_SCAN_RSP */
	/* messages are following. */
	/* SME_SCAN_RSP message with */
	/* eSIR_SME_SUCCESS status */
	/* code is the last one. */
	/* Sent in SME_JOIN/REASSOC_RSP */
	eSIR_SME_INVALID_ASSOC_RSP_RXED,
	/* messages upon receiving */
	/* invalid Re/Assoc Rsp frame. */
	/* STOP BSS triggered by MIC failures: MAC software to
	 * disassoc all stations
	 */
	eSIR_SME_MIC_COUNTER_MEASURES,
	/* with MIC_FAILURE reason code and perform the stop bss operation */
	/* didn't get rsp from peer within timeout interval */
	eSIR_SME_ADDTS_RSP_TIMEOUT,
	/* didn't get success rsp from HAL */
	eSIR_SME_ADDTS_RSP_FAILED,
	/* failed to send ch switch act frm */
	eSIR_SME_CHANNEL_SWITCH_FAIL,
	eSIR_SME_INVALID_STATE,
	/* SIR_HAL_SIR_HAL_INIT_SCAN_RSP returned failed status */
	eSIR_SME_HAL_SCAN_INIT_FAILED,
	/* SIR_HAL_END_SCAN_RSP returned failed status */
	eSIR_SME_HAL_SCAN_END_FAILED,
	/* SIR_HAL_FINISH_SCAN_RSP returned failed status */
	eSIR_SME_HAL_SCAN_FINISH_FAILED,
	/* Failed to send a message to HAL */
	eSIR_SME_HAL_SEND_MESSAGE_FAIL,
	/* Failed to stop the bss */
	eSIR_SME_STOP_BSS_FAILURE,
	eSIR_SME_WOWL_ENTER_REQ_FAILED,
	eSIR_SME_WOWL_EXIT_REQ_FAILED,
#if defined(WLAN_FEATURE_VOWIFI_11R) || defined(FEATURE_WLAN_ESE) || \
	defined(FEATURE_WLAN_LFR)
	eSIR_SME_FT_REASSOC_TIMEOUT_FAILURE,
	eSIR_SME_FT_REASSOC_FAILURE,
#endif
	eSIR_SME_SEND_ACTION_FAIL,
	eSIR_SME_DEAUTH_STATUS,
	eSIR_PNO_SCAN_SUCCESS,
	eSIR_DONOT_USE_RESULT_CODE = SIR_MAX_ENUM_SIZE
} tSirResultCodes;

#define RMENABLEDCAP_MAX_LEN 5

struct rrm_config_param {
	uint8_t rrm_enabled;
	uint8_t max_randn_interval;
	uint8_t rm_capability[RMENABLEDCAP_MAX_LEN];
};

/* each station added has a rate mode which specifies the sta attributes */
typedef enum eStaRateMode {
	eSTA_TAURUS = 0,
	eSTA_TITAN,
	eSTA_POLARIS,
	eSTA_11b,
	eSTA_11bg,
	eSTA_11a,
	eSTA_11n,
#ifdef WLAN_FEATURE_11AC
	eSTA_11ac,
#endif
	eSTA_INVALID_RATE_MODE
} tStaRateMode, *tpStaRateMode;

/*
 * although in tSirSupportedRates each IE is 16bit but PE only passes IEs in 8
 * bits with MSB=1 for basic rates. change the mask for bit0-7 only so HAL gets
 * correct basic rates for setting response rates.
 */
#define IERATE_BASICRATE_MASK     0x80
#define IERATE_RATE_MASK          0x7f
#define IERATE_IS_BASICRATE(x)   ((x) & IERATE_BASICRATE_MASK)

typedef struct sSirSupportedRates {
	/*
	 * For Self STA Entry: this represents Self Mode.
	 * For Peer Stations, this represents the mode of the peer.
	 * On Station:
	 * --this mode is updated when PE adds the Self Entry.
	 * -- OR when PE sends 'ADD_BSS' message and station context in BSS is
	 * used to indicate the mode of the AP.
	 * ON AP:
	 * -- this mode is updated when PE sends 'ADD_BSS' and Sta entry for
	 * that BSS is used to indicate the self mode of the AP.
	 * -- OR when a station is associated, PE sends 'ADD_STA' message with
	 * this mode updated.
	 */

	tStaRateMode opRateMode;
	/*
	 * 11b, 11a and aniLegacyRates are IE rates which gives rate in unit
	 * of 500Kbps
	 */
	uint16_t llbRates[SIR_NUM_11B_RATES];
	uint16_t llaRates[SIR_NUM_11A_RATES];
	/*
	 * 0-76 bits used, remaining reserved
	 * bits 0-15 and 32 should be set.
	 */
	uint8_t supportedMCSSet[SIR_MAC_MAX_SUPPORTED_MCS_SET];

	/*
	 * RX Highest Supported Data Rate defines the highest data
	 * rate that the STA is able to receive, in unites of 1Mbps.
	 * This value is derived from "Supported MCS Set field" inside
	 * the HT capability element.
	 */
	uint16_t rxHighestDataRate;

#ifdef WLAN_FEATURE_11AC
	/*Indicates the Maximum MCS that can be received for each number
	   of spacial streams */
	uint16_t vhtRxMCSMap;
	/*Indicate the highest VHT data rate that the STA is able to receive */
	uint16_t vhtRxHighestDataRate;
	/*Indicates the Maximum MCS that can be transmitted for each number
	   of spacial streams */
	uint16_t vhtTxMCSMap;
	/*Indicate the highest VHT data rate that the STA is able to transmit */
	uint16_t vhtTxHighestDataRate;
#endif
} tSirSupportedRates, *tpSirSupportedRates;

typedef enum eSirRFBand {
	SIR_BAND_UNKNOWN,
	SIR_BAND_2_4_GHZ,
	SIR_BAND_5_GHZ,
} tSirRFBand;

typedef struct sSirRemainOnChnReq {
	uint16_t messageType;
	uint16_t length;
	uint8_t sessionId;
	struct cdf_mac_addr selfMacAddr;
	uint8_t chnNum;
	uint8_t phyMode;
	uint32_t duration;
	uint8_t isProbeRequestAllowed;
	uint32_t scan_id;
	uint8_t probeRspIe[1];
} tSirRemainOnChnReq, *tpSirRemainOnChnReq;

/**
 * struct sir_roc_rsp - Structure to store the remain on channel response
 * @message_type: Message Type
 * @length: Message Length
 * @session_id: SME session Id
 * @scan_id : scan identifier
 * @status: result status
 */
struct sir_roc_rsp {
	uint16_t message_type;
	uint16_t length;
	uint8_t session_id;
	uint32_t scan_id;
	tSirResultCodes status;
};

typedef struct sSirRegisterMgmtFrame {
	uint16_t messageType;
	uint16_t length;
	uint8_t sessionId;
	bool registerFrame;
	uint16_t frameType;
	uint16_t matchLen;
	uint8_t matchData[1];
} tSirRegisterMgmtFrame, *tpSirRegisterMgmtFrame;

/* / Generic type for sending a response message */
/* / with result code to host software */
typedef struct sSirSmeRsp {
	uint16_t messageType;   /* eWNI_SME_*_RSP */
	uint16_t length;
	uint8_t sessionId;      /* To support BT-AMP */
	uint16_t transactionId; /* To support BT-AMP */
	tSirResultCodes statusCode;
} tSirSmeRsp, *tpSirSmeRsp;

/* / Definition for indicating all modules ready on STA */
typedef struct sSirSmeReadyReq {
	uint16_t messageType;   /* eWNI_SME_SYS_READY_IND */
	uint16_t length;
	uint16_t transactionId;
	void *add_bssdescr_cb;
} tSirSmeReadyReq, *tpSirSmeReadyReq;

/**
 * struct sir_hw_mode - Format of set HW mode
 * @hw_mode_index: Index of HW mode to be set
 * @set_hw_mode_cb: HDD set HW mode callback
 * @reason: Reason for HW mode change
 * @session_id: Session id
 */
struct sir_hw_mode {
	uint32_t hw_mode_index;
	void *set_hw_mode_cb;
	enum cds_conn_update_reason reason;
	uint32_t session_id;
};

/**
 * struct s_sir_set_hw_mode - Set HW mode request
 * @messageType: Message type
 * @length: Length of the message
 * @set_hw: Params containing the HW mode index and callback
 */
struct s_sir_set_hw_mode {
	uint16_t messageType;
	uint16_t length;
	struct sir_hw_mode set_hw;
};

/**
 * struct sir_dual_mac_config - Dual MAC configuration
 * @scan_config: Scan configuration
 * @fw_mode_config: FW mode configuration
 * @set_dual_mac_cb: Callback function to be executed on response to the command
 */
struct sir_dual_mac_config {
	uint32_t scan_config;
	uint32_t fw_mode_config;
	void *set_dual_mac_cb;
};

/**
 * struct sir_set_dual_mac_cfg - Set Dual mac config request
 * @message_type: Message type
 * @length: Length of the message
 * @set_dual_mac: Params containing the dual mac config and callback
 */
struct sir_set_dual_mac_cfg {
	uint16_t message_type;
	uint16_t length;
	struct sir_dual_mac_config set_dual_mac;
};

/* / BSS type enum used in while scanning/joining etc */
typedef enum eSirBssType {
	eSIR_INFRASTRUCTURE_MODE,
	eSIR_INFRA_AP_MODE,     /* Added for softAP support */
	eSIR_IBSS_MODE,
	eSIR_BTAMP_STA_MODE,    /* Added for BT-AMP support */
	eSIR_BTAMP_AP_MODE,     /* Added for BT-AMP support */
	eSIR_AUTO_MODE,
	eSIR_DONOT_USE_BSS_TYPE = SIR_MAX_ENUM_SIZE
} tSirBssType;

/* / Power Capability info used in 11H */
typedef struct sSirMacPowerCapInfo {
	uint8_t minTxPower;
	uint8_t maxTxPower;
} tSirMacPowerCapInfo, *tpSirMacPowerCapInfo;

/* / Supported Channel info used in 11H */
typedef struct sSirSupChnl {
	uint8_t numChnl;
	uint8_t channelList[SIR_MAX_SUPPORTED_CHANNEL_LIST];
} tSirSupChnl, *tpSirSupChnl;

typedef enum eSirNwType {
	eSIR_11A_NW_TYPE,
	eSIR_11B_NW_TYPE,
	eSIR_11G_NW_TYPE,
	eSIR_11N_NW_TYPE,
#ifdef WLAN_FEATURE_11AC
	eSIR_11AC_NW_TYPE,
#endif
	eSIR_DONOT_USE_NW_TYPE = SIR_MAX_ENUM_SIZE
} tSirNwType;

/* / Definition for new iBss peer info */
typedef struct sSirNewIbssPeerInfo {
	struct cdf_mac_addr peerAddr;
	uint16_t aid;
} tSirNewIbssPeerInfo, *tpSirNewIbssPeerInfo;

/* HT configuration values */
typedef struct sSirHtConfig {
	/* Enable/Disable receiving LDPC coded packets */
	uint32_t ht_rx_ldpc:1;
	/* Enable/Disable TX STBC */
	uint32_t ht_tx_stbc:1;
	/* Enable/Disable RX STBC */
	uint32_t ht_rx_stbc:2;
	/* Enable/Disable SGI */
	uint32_t ht_sgi:1;
	uint32_t unused:27;
} cdf_packed tSirHTConfig, *tpSirHTConfig;

typedef struct sSirAddIeParams {
	uint16_t probeRespDataLen;
	uint8_t *probeRespData_buff;
	uint16_t assocRespDataLen;
	uint8_t *assocRespData_buff;
	uint16_t probeRespBCNDataLen;
	uint8_t *probeRespBCNData_buff;
} tSirAddIeParams, *tpSirAddIeParams;

/* / Definition for kick starting BSS */
/* / ---> MAC */
/**
 * Usage of ssId, numSSID & ssIdList:
 * ---------------------------------
 * 1. ssId.length of zero indicates that Broadcast/Suppress SSID
 *    feature is enabled.
 * 2. If ssId.length is zero, MAC SW will advertise NULL SSID
 *    and interpret the SSID list from numSSID & ssIdList.
 * 3. If ssId.length is non-zero, MAC SW will advertise the SSID
 *    specified in the ssId field and it is expected that
 *    application will set numSSID to one (only one SSID present
 *    in the list) and SSID in the list is same as ssId field.
 * 4. Application will always set numSSID >= 1.
 */
/* ***** NOTE: Please make sure all codes are updated if inserting field into
 * this structure..********** */
typedef struct sSirSmeStartBssReq {
	uint16_t messageType;   /* eWNI_SME_START_BSS_REQ */
	uint16_t length;
	uint8_t sessionId;      /* Added for BT-AMP Support */
	uint16_t transactionId; /* Added for BT-AMP Support */
	struct cdf_mac_addr bssid;      /* Added for BT-AMP Support */
	struct cdf_mac_addr self_macaddr;        /* Added for BT-AMP Support */
	uint16_t beaconInterval;        /* Added for BT-AMP Support */
	uint8_t dot11mode;
#ifdef FEATURE_WLAN_MCC_TO_SCC_SWITCH
	uint8_t cc_switch_mode;
#endif
	tSirBssType bssType;
	tSirMacSSid ssId;
	uint8_t channelId;
	ePhyChanBondState cbMode;
	uint8_t vht_channel_width;
	uint8_t center_freq_seg0;
	uint8_t center_freq_seg1;
	uint8_t sec_ch_offset;

	uint8_t privacy;
	uint8_t apUapsdEnable;
	uint8_t ssidHidden;
	bool fwdWPSPBCProbeReq;
	bool protEnabled;
	bool obssProtEnabled;
	uint16_t ht_capab;
	tAniAuthType authType;
	uint32_t dtimPeriod;
	uint8_t wps_state;
	uint8_t isCoalesingInIBSSAllowed;       /* Coalesing on/off knob */
	tCDF_CON_MODE bssPersona;

	uint8_t txLdpcIniFeatureEnabled;

	tSirRSNie rsnIE;        /* RSN IE to be sent in */
	/* Beacon and Probe */
	/* Response frames */
	tSirNwType nwType;      /* Indicates 11a/b/g */
	tSirMacRateSet operationalRateSet;      /* Has 11a or 11b rates */
	tSirMacRateSet extendedRateSet; /* Has 11g rates */
	tSirHTConfig htConfig;

#ifdef WLAN_FEATURE_11W
	bool pmfCapable;
	bool pmfRequired;
#endif

	tSirAddIeParams addIeParams;

	bool obssEnabled;
	uint8_t sap_dot11mc;

} tSirSmeStartBssReq, *tpSirSmeStartBssReq;

#define GET_IE_LEN_IN_BSS(lenInBss) (lenInBss + sizeof(lenInBss) - \
				    ((uintptr_t)OFFSET_OF(tSirBssDescription,\
							  ieFields)))

#define WSCIE_PROBE_RSP_LEN (317 + 2)

typedef struct sSirBssDescription {
	/* offset of the ieFields from bssId. */
	uint16_t length;
	tSirMacAddr bssId;
	v_TIME_t scanSysTimeMsec;
	uint32_t timeStamp[2];
	uint16_t beaconInterval;
	uint16_t capabilityInfo;
	tSirNwType nwType;      /* Indicates 11a/b/g */
	uint8_t reserved_padding0;
	int8_t rssi;
	int8_t sinr;
	/* channelId what peer sent in beacon/probersp. */
	uint8_t channelId;
	/* channelId on which we are parked at. */
	/* used only in scan case. */
	uint8_t channelIdSelf;
	uint8_t sSirBssDescriptionRsvd[3];
	/* base on a tick count. It is a time stamp, not a relative time. */
	uint32_t nReceivedTime;
#if defined WLAN_FEATURE_VOWIFI
	uint32_t parentTSF;
	uint32_t startTSF[2];
#endif
#ifdef WLAN_FEATURE_VOWIFI_11R
	uint8_t mdiePresent;
	/* MDIE for 11r, picked from the beacons */
	uint8_t mdie[SIR_MDIE_SIZE];
#endif
#ifdef FEATURE_WLAN_ESE
	uint16_t QBSSLoad_present;
	uint16_t QBSSLoad_avail;
	/* To achieve 8-byte alignment with ESE enabled */
	uint32_t reservedPadding5;
#endif
	/* Please keep the structure 4 bytes aligned above the ieFields */

	/* whether it is from a probe rsp */
	uint8_t fProbeRsp;
	uint8_t reservedPadding1;
	uint8_t reservedPadding2;
	uint8_t reservedPadding3;
	uint32_t WscIeLen;
	uint8_t WscIeProbeRsp[WSCIE_PROBE_RSP_LEN];
	uint8_t reservedPadding4;
	uint32_t tsf_delta;

	uint32_t ieFields[1];
} tSirBssDescription, *tpSirBssDescription;

#ifdef FEATURE_WLAN_MCC_TO_SCC_SWITCH
typedef struct sSirSmeHTProfile {
	uint8_t dot11mode;
	uint8_t htCapability;
	uint8_t htSupportedChannelWidthSet;
	uint8_t htRecommendedTxWidthSet;
	ePhyChanBondState htSecondaryChannelOffset;
#ifdef WLAN_FEATURE_11AC
	uint8_t vhtCapability;
	uint8_t vhtTxChannelWidthSet;
	uint8_t apCenterChan;
	uint8_t apChanWidth;
#endif
} tSirSmeHTProfile;
#endif
/* / Definition for response message to previously */
/* / issued start BSS request */
/* / MAC ---> */
typedef struct sSirSmeStartBssRsp {
	uint16_t messageType;   /* eWNI_SME_START_BSS_RSP */
	uint16_t length;
	uint8_t sessionId;
	uint16_t transactionId; /* transaction ID for cmd */
	tSirResultCodes statusCode;
	tSirBssType bssType;    /* Add new type for WDS mode */
	uint16_t beaconInterval;        /* Beacon Interval for both type */
	uint32_t staId;         /* Staion ID for Self */
#ifdef FEATURE_WLAN_MCC_TO_SCC_SWITCH
	tSirSmeHTProfile HTProfile;
#endif
	tSirBssDescription bssDescription;      /* Peer BSS description */
} tSirSmeStartBssRsp, *tpSirSmeStartBssRsp;

typedef struct sSirChannelList {
	uint8_t numChannels;
	uint8_t channelNumber[SIR_ESE_MAX_MEAS_IE_REQS];
} tSirChannelList, *tpSirChannelList;

typedef struct sSirDFSChannelList {
	uint32_t timeStamp[SIR_MAX_24G_5G_CHANNEL_RANGE];

} tSirDFSChannelList, *tpSirDFSChannelList;

#ifdef FEATURE_WLAN_ESE
typedef struct sTspecInfo {
	uint8_t valid;
	tSirMacTspecIE tspec;
} tTspecInfo;

#define SIR_ESE_MAX_TSPEC_IES   4
typedef struct sESETspecTspecInfo {
	uint8_t numTspecs;
	tTspecInfo tspec[SIR_ESE_MAX_TSPEC_IES];
} tESETspecInfo;
#endif

/* / Two Background Scan mode */
typedef enum eSirBackgroundScanMode {
	eSIR_ROAMING_SCAN = 2,
} tSirBackgroundScanMode;

/* / Two types of traffic check */
typedef enum eSirLinkTrafficCheck {
	eSIR_DONT_CHECK_LINK_TRAFFIC_BEFORE_SCAN = 0,
	eSIR_CHECK_LINK_TRAFFIC_BEFORE_SCAN = 1,
	eSIR_CHECK_ROAMING_SCAN = 2,
} tSirLinkTrafficCheck;

#define SIR_BG_SCAN_RETURN_CACHED_RESULTS              0x0
#define SIR_BG_SCAN_PURGE_RESUTLS                      0x80
#define SIR_BG_SCAN_RETURN_FRESH_RESULTS               0x01
#define SIR_SCAN_MAX_NUM_SSID                          0x0A
#define SIR_BG_SCAN_RETURN_LFR_CACHED_RESULTS          0x02
#define SIR_BG_SCAN_PURGE_LFR_RESULTS                  0x40

/* / Definition for scan request */
typedef struct sSirSmeScanReq {
	uint16_t messageType;   /* eWNI_SME_SCAN_REQ */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId; /* Transaction ID for cmd */
	struct cdf_mac_addr bssId;
	tSirMacSSid ssId[SIR_SCAN_MAX_NUM_SSID];
	struct cdf_mac_addr selfMacAddr;        /* Added For BT-AMP Support */
	tSirBssType bssType;
	uint8_t dot11mode;
	tSirScanType scanType;
	uint32_t scan_id;
	/**
	 * minChannelTime. Not used if scanType is passive.
	 * 0x0 - Dont Use min channel timer. Only max channel timeout will used.
	 * 11k measurements set this to 0 to user only single duration for scan.
	 * <valid timeout> - Timeout value used for min channel timeout.
	 */
	uint32_t minChannelTime;
	/**
	 * maxChannelTime.
	 * 0x0 - Invalid. In case of active scan.
	 * In case of passive scan, MAX( maxChannelTime,
	 * WNI_CFG_PASSIVE_MAXIMUM_CHANNEL_TIME) is used.
	 */
	uint32_t maxChannelTime;
	/**
	 * returnAfterFirstMatch can take following values:
	 * 0x00 - Return SCAN_RSP message after complete channel scan
	 * 0x01 -  Return SCAN_RSP message after collecting BSS description
	 *        that matches scan criteria.
	 * 0xC0 - Return after collecting first 11d IE from 2.4 GHz &
	 *        5 GHz band channels
	 * 0x80 - Return after collecting first 11d IE from 5 GHz band
	 *        channels
	 * 0x40 - Return after collecting first 11d IE from 2.4 GHz
	 *        band channels
	 *
	 * Values of 0xC0, 0x80 & 0x40 are to be used by
	 * Roaming/application when 11d is enabled.
	 */
	/* in units of milliseconds, ignored when not connected */
	uint32_t restTime;
	uint8_t returnAfterFirstMatch;

	/**
	 * returnUniqueResults can take following values:
	 * 0 - Collect & report all received BSS descriptions from same BSS.
	 * 1 - Collect & report unique BSS description from same BSS.
	 */
	uint8_t returnUniqueResults;

	/**
	 * returnFreshResults can take following values:
	 * 0x00 - Return background scan results.
	 * 0x80 - Return & purge background scan results
	 * 0x01 - Trigger fresh scan instead of returning background scan
	 *        results.
	 * 0x81 - Trigger fresh scan instead of returning background scan
	 *        results and purge background scan results.
	 */
	uint8_t returnFreshResults;

	/*  backgroundScanMode can take following values:
	 *  0x0 - agressive scan
	 *  0x1 - normal scan where HAL will check for link traffic
	 *        prior to proceeding with the scan
	 */
	tSirBackgroundScanMode backgroundScanMode;

	uint8_t hiddenSsid;

	/* Number of SSIDs to scan */
	uint8_t numSsid;

	/* channelList has to be the last member of this structure. Check
	 * tSirChannelList for the reason. This MUST be the last field of the
	 * structure
	 */

	bool p2pSearch;
	uint16_t uIEFieldLen;
	uint16_t uIEFieldOffset;

	/* channelList MUST be the last field of this structure */
	tSirChannelList channelList;
	/*-----------------------------
	   tSirSmeScanReq....
	   -----------------------------
	   uIEFiledLen
	   -----------------------------
	   uIEFiledOffset               ----+
	   -----------------------------    |
	   channelList.numChannels          |
	   -----------------------------    |
	   ... variable size up to          |
	   channelNumber[numChannels-1]     |
	   This can be zero, if             |
	   numChannel is zero.              |
	   ----------------------------- <--+
	   ... variable size uIEFiled
	   up to uIEFieldLen (can be 0)
	   -----------------------------*/
} tSirSmeScanReq, *tpSirSmeScanReq;

typedef struct sSirSmeScanAbortReq {
	uint16_t type;
	uint16_t msgLen;
	uint8_t sessionId;
	uint32_t scan_id;
} tSirSmeScanAbortReq, *tpSirSmeScanAbortReq;

typedef struct sSirSmeScanChanReq {
	uint16_t type;
	uint16_t msgLen;
	uint8_t sessionId;
	uint16_t transcationId;
} tSirSmeGetScanChanReq, *tpSirSmeGetScanChanReq;

#ifdef FEATURE_OEM_DATA_SUPPORT

#ifndef OEM_DATA_REQ_SIZE
#define OEM_DATA_REQ_SIZE 280
#endif
#ifndef OEM_DATA_RSP_SIZE
#define OEM_DATA_RSP_SIZE 1724
#endif

typedef struct sSirOemDataReq {
	uint16_t messageType;   /* eWNI_SME_OEM_DATA_REQ */
	uint16_t messageLen;
	struct cdf_mac_addr selfMacAddr;
	uint8_t oemDataReq[OEM_DATA_REQ_SIZE];
} tSirOemDataReq, *tpSirOemDataReq;

typedef struct sSirOemDataRsp {
	uint16_t messageType;
	uint16_t length;
	uint8_t oemDataRsp[OEM_DATA_RSP_SIZE];
} tSirOemDataRsp, *tpSirOemDataRsp;

#endif /* FEATURE_OEM_DATA_SUPPORT */

/* / Definition for response message to previously issued scan request */
typedef struct sSirSmeScanRsp {
	uint16_t messageType;   /* eWNI_SME_SCAN_RSP */
	uint16_t length;
	uint8_t sessionId;
	tSirResultCodes statusCode;
	uint16_t transcationId;
	uint32_t scan_id;
} tSirSmeScanRsp, *tpSirSmeScanRsp;

/* / Definition for join request */
/* / ---> MAC */
/* / WARNING! If you add a field in JOIN REQ. */
/* /         Make sure to add it in REASSOC REQ */
/* / The Serdes function is the same and its */
/* / shared with REASSOC. So if we add a field */
/*  here and dont add it in REASSOC REQ. It will BREAK!!! REASSOC. */
typedef struct sSirSmeJoinReq {
	uint16_t messageType;   /* eWNI_SME_JOIN_REQ */
	uint16_t length;
	uint8_t sessionId;
	uint16_t transactionId;
	tSirMacSSid ssId;
	tSirMacAddr selfMacAddr;        /* self Mac address */
	tSirBssType bsstype;    /* add new type for BT-AMP STA and AP Modules */
	uint8_t dot11mode;      /* to support BT-AMP */
#ifdef FEATURE_WLAN_MCC_TO_SCC_SWITCH
	uint8_t cc_switch_mode;
#endif
	tCDF_CON_MODE staPersona;       /* Persona */
	ePhyChanBondState cbMode;       /* Pass CB mode value in Join. */

	/*This contains the UAPSD Flag for all 4 AC
	 * B0: AC_VO UAPSD FLAG
	 * B1: AC_VI UAPSD FLAG
	 * B2: AC_BK UAPSD FLAG
	 * B3: AC_BE UASPD FLAG
	 */
	uint8_t uapsdPerAcBitmask;

	tSirMacRateSet operationalRateSet;      /* Has 11a or 11b rates */
	tSirMacRateSet extendedRateSet; /* Has 11g rates */
	tSirRSNie rsnIE;        /* RSN IE to be sent in */
	/* (Re) Association Request */
#ifdef FEATURE_WLAN_ESE
	/* CCMK IE to be included as handler for join and reassoc is */
	tSirCCKMie cckmIE;
	/* the same. The join will never carry cckm, but will be set to */
	/* 0. */
#endif

	tSirAddie addIEScan;    /* Additional IE to be sent in */
	/* (unicast) Probe Request at the time of join */

	tSirAddie addIEAssoc;   /* Additional IE to be sent in */
	/* (Re) Association Request */

	tAniEdType UCEncryptionType;

	tAniEdType MCEncryptionType;

#ifdef WLAN_FEATURE_11W
	tAniEdType MgmtEncryptionType;
#endif

#ifdef WLAN_FEATURE_VOWIFI_11R
	tAniBool is11Rconnection;
#endif
#ifdef FEATURE_WLAN_ESE
	tAniBool isESEFeatureIniEnabled;
	tAniBool isESEconnection;
	tESETspecInfo eseTspecInfo;
#endif

#if defined WLAN_FEATURE_VOWIFI_11R || defined FEATURE_WLAN_ESE || defined(FEATURE_WLAN_LFR)
	tAniBool isFastTransitionEnabled;
#endif
#ifdef FEATURE_WLAN_LFR
	tAniBool isFastRoamIniFeatureEnabled;
#endif

	uint8_t txLdpcIniFeatureEnabled;
	tSirHTConfig htConfig;
#ifdef WLAN_FEATURE_11AC
	uint8_t txBFIniFeatureEnabled;
	uint8_t txBFCsnValue;
	uint8_t txMuBformee;
	uint8_t enableVhtpAid;
	uint8_t enableVhtGid;
#endif
	uint8_t enableAmpduPs;
	uint8_t enableHtSmps;
	uint8_t htSmps;

	uint8_t isAmsduSupportInAMPDU;
	tAniBool isWMEenabled;
	tAniBool isQosEnabled;
	tAniBool isOSENConnection;
	struct rrm_config_param rrm_config;
	tAniBool spectrumMgtIndicator;
	tSirMacPowerCapInfo powerCap;
	tSirSupChnl supportedChannels;
	tSirBssDescription bssDescription;

} tSirSmeJoinReq, *tpSirSmeJoinReq;

/* / Definition for reponse message to previously issued join request */
/* / MAC ---> */
typedef struct sSirSmeJoinRsp {
	uint16_t messageType;   /* eWNI_SME_JOIN_RSP */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId; /* Transaction ID for cmd */
	tSirResultCodes statusCode;
	tAniAuthType authType;
	uint32_t vht_channel_width;
	/* It holds reasonCode when join fails due to deauth/disassoc frame.
	 * Otherwise it holds status code.
	 */
	uint16_t protStatusCode;
	uint16_t aid;
	uint32_t beaconLength;
	uint32_t assocReqLength;
	uint32_t assocRspLength;
#ifdef WLAN_FEATURE_VOWIFI_11R
	uint32_t parsedRicRspLen;
#endif
#ifdef FEATURE_WLAN_ESE
	uint32_t tspecIeLen;
#endif
	uint32_t staId;         /* Station ID for peer */

	/* The DPU signatures will be sent eventually to TL to help it determine
	 * the association to which a packet belongs to
	 * Unicast DPU signature
	 */
	uint8_t ucastSig;

	/*Broadcast DPU signature */
	uint8_t bcastSig;

	/*Timing measurement capability */
	uint8_t timingMeasCap;

#ifdef FEATURE_WLAN_TDLS
	/* TDLS prohibited and TDLS channel switch prohibited are set as
	 * per ExtCap IE in received assoc/re-assoc response from AP
	 */
	bool tdls_prohibited;
	bool tdls_chan_swit_prohibited;
#endif
	uint8_t nss;
	uint32_t max_rate_flags;

#ifdef FEATURE_WLAN_MCC_TO_SCC_SWITCH
	tSirSmeHTProfile HTProfile;
#endif
	uint8_t frames[1];
} tSirSmeJoinRsp, *tpSirSmeJoinRsp;

/* / probereq from peer, when wsc is enabled */
typedef struct sSirSmeProbereq {
	uint16_t messageType;
	uint16_t length;
	uint8_t sessionId;
	struct cdf_mac_addr peer_macaddr;
	uint16_t devicePasswdId;
} tSirSmeProbeReq, *tpSirSmeProbeReq;

typedef struct sSirSmeChanInfo {
	uint8_t chan_id;
	uint32_t mhz;
	uint32_t band_center_freq1;
	uint32_t band_center_freq2;
	uint32_t info;
	uint32_t reg_info_1;
	uint32_t reg_info_2;
	uint8_t nss;
	uint32_t rate_flags;
} tSirSmeChanInfo, *tpSirSmeChanInfo;

/* / Definition for Association indication from peer */
/* / MAC ---> */
typedef struct sSirSmeAssocInd {
	uint16_t messageType;   /* eWNI_SME_ASSOC_IND */
	uint16_t length;
	uint8_t sessionId;
	tSirMacAddr peerMacAddr;
	uint16_t aid;
	tSirMacAddr bssId;      /* Self BSSID */
	uint16_t staId;         /* Station ID for peer */
	uint8_t uniSig;         /* DPU signature for unicast packets */
	uint8_t bcastSig;       /* DPU signature for broadcast packets */
	tAniAuthType authType;
	tAniSSID ssId;          /* SSID used by STA to associate */
	tSirWAPIie wapiIE;      /* WAPI IE received from peer */
	tSirRSNie rsnIE;        /* RSN IE received from peer */
	/* Additional IE received from peer, which possibly include
	 * WSC IE and/or P2P IE
	 */
	tSirAddie addIE;

	/* powerCap & supportedChannels are present only when */
	/* spectrumMgtIndicator flag is set */
	tAniBool spectrumMgtIndicator;
	tSirMacPowerCapInfo powerCap;
	tSirSupChnl supportedChannels;
	tAniBool wmmEnabledSta; /* if present - STA is WMM enabled */
	tAniBool reassocReq;
	/* Required for indicating the frames to upper layer */
	uint32_t beaconLength;
	uint8_t *beaconPtr;
	uint32_t assocReqLength;
	uint8_t *assocReqPtr;

	/* Timing measurement capability */
	uint8_t timingMeasCap;
	tSirSmeChanInfo chan_info;
} tSirSmeAssocInd, *tpSirSmeAssocInd;

/* / Definition for Association confirm */
/* / ---> MAC */
typedef struct sSirSmeAssocCnf {
	uint16_t messageType;   /* eWNI_SME_ASSOC_CNF */
	uint16_t length;
	tSirResultCodes statusCode;
	tSirMacAddr bssId;      /* Self BSSID */
	tSirMacAddr peerMacAddr;
	uint16_t aid;
	tSirMacAddr alternateBssId;
	uint8_t alternateChannelId;
} tSirSmeAssocCnf, *tpSirSmeAssocCnf;

/* / Enum definition for  Wireless medium status change codes */
typedef enum eSirSmeStatusChangeCode {
	eSIR_SME_DEAUTH_FROM_PEER,
	eSIR_SME_DISASSOC_FROM_PEER,
	eSIR_SME_LOST_LINK_WITH_PEER,
	eSIR_SME_CHANNEL_SWITCH,
	eSIR_SME_JOINED_NEW_BSS,
	eSIR_SME_LEAVING_BSS,
	eSIR_SME_IBSS_ACTIVE,
	eSIR_SME_IBSS_INACTIVE,
	eSIR_SME_IBSS_PEER_DEPARTED,
	eSIR_SME_RADAR_DETECTED,
	eSIR_SME_IBSS_NEW_PEER,
	eSIR_SME_AP_CAPS_CHANGED,
} tSirSmeStatusChangeCode;

typedef struct sSirSmeNewBssInfo {
	struct cdf_mac_addr bssId;
	uint8_t channelNumber;
	uint8_t reserved;
	tSirMacSSid ssId;
} tSirSmeNewBssInfo, *tpSirSmeNewBssInfo;

typedef struct sSirSmeApNewCaps {
	uint16_t capabilityInfo;
	struct cdf_mac_addr bssId;
	uint8_t channelId;
	uint8_t reserved[3];
	tSirMacSSid ssId;
} tSirSmeApNewCaps, *tpSirSmeApNewCaps;

/**
 * Table below indicates what information is passed for each of
 * the Wireless Media status change notifications:
 *
 * Status Change code           Status change info
 * ----------------------------------------------------------------------
 * eSIR_SME_DEAUTH_FROM_PEER        Reason code received in DEAUTH frame
 * eSIR_SME_DISASSOC_FROM_PEER      Reason code received in DISASSOC frame
 * eSIR_SME_LOST_LINK_WITH_PEER     None
 * eSIR_SME_CHANNEL_SWITCH          New channel number
 * eSIR_SME_JOINED_NEW_BSS          BSSID, SSID and channel number
 * eSIR_SME_LEAVING_BSS             None
 * eSIR_SME_IBSS_ACTIVE             Indicates that another STA joined
 *                                  IBSS apart from this STA that
 *                                  started IBSS
 * eSIR_SME_IBSS_INACTIVE           Indicates that only this STA is left
 *                                  in IBSS
 * eSIR_SME_RADAR_DETECTED          Indicates that radar is detected
 * eSIR_SME_IBSS_NEW_PEER           Indicates that a new peer is detected
 * eSIR_SME_AP_CAPS_CHANGED         Indicates that capabilities of the AP
 *                                  that STA is currently associated with
 *                                  have changed.
 */

/* / Definition for Wireless medium status change notification */
typedef struct sSirSmeWmStatusChangeNtf {
	uint16_t messageType;   /* eWNI_SME_WM_STATUS_CHANGE_NTF */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	tSirSmeStatusChangeCode statusChangeCode;
	struct cdf_mac_addr bssid;      /* Self BSSID */
	union {
		uint16_t deAuthReasonCode;      /* eSIR_SME_DEAUTH_FROM_PEER */
		/* eSIR_SME_DISASSOC_FROM_PEER */
		uint16_t disassocReasonCode;
		/* none for eSIR_SME_LOST_LINK_WITH_PEER */
		uint8_t newChannelId;   /* eSIR_SME_CHANNEL_SWITCH */
		tSirSmeNewBssInfo newBssInfo;   /* eSIR_SME_JOINED_NEW_BSS */
		/* none for eSIR_SME_LEAVING_BSS */
		/* none for eSIR_SME_IBSS_ACTIVE */
		/* none for eSIR_SME_IBSS_INACTIVE */
		/* eSIR_SME_IBSS_NEW_PEER */
		tSirNewIbssPeerInfo newIbssPeerInfo;
		tSirSmeApNewCaps apNewCaps;     /* eSIR_SME_AP_CAPS_CHANGED */
	} statusChangeInfo;
} tSirSmeWmStatusChangeNtf, *tpSirSmeWmStatusChangeNtf;

/* Definition for Disassociation request */
typedef struct sSirSmeDisassocReq {
	uint16_t messageType;   /* eWNI_SME_DISASSOC_REQ */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId; /* Transaction ID for cmd */
	tSirMacAddr bssId;      /* Peer BSSID */
	tSirMacAddr peerMacAddr;
	uint16_t reasonCode;
	/* This flag tells LIM whether to send the disassoc OTA or not */
	/* This will be set in while handing off from one AP to other */
	uint8_t doNotSendOverTheAir;
} cdf_packed tSirSmeDisassocReq, *tpSirSmeDisassocReq;

/* / Definition for Tkip countermeasures request */
typedef struct sSirSmeTkipCntrMeasReq {
	uint16_t messageType;   /* eWNI_SME_DISASSOC_REQ */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId; /* Transaction ID for cmd */
	struct cdf_mac_addr bssId; /* Peer BSSID */
	bool bEnable;           /* Start/stop countermeasures */
} cdf_packed tSirSmeTkipCntrMeasReq, *tpSirSmeTkipCntrMeasReq;

typedef struct sAni64BitCounters {
	uint32_t Hi;
	uint32_t Lo;
} tAni64BitCounters, *tpAni64BitCounters;

typedef struct sAniSecurityStat {
	tAni64BitCounters txBlks;
	tAni64BitCounters rxBlks;
	tAni64BitCounters formatErrorCnt;
	tAni64BitCounters decryptErr;
	tAni64BitCounters protExclCnt;
	tAni64BitCounters unDecryptableCnt;
	tAni64BitCounters decryptOkCnt;

} tAniSecurityStat, *tpAniSecurityStat;

typedef struct sAniTxRxStats {
	tAni64BitCounters txFrames;
	tAni64BitCounters rxFrames;
	tAni64BitCounters nRcvBytes;
	tAni64BitCounters nXmitBytes;

} tAniTxRxStats, *tpAniTxRxStats;

typedef struct sAniSecStats {
	tAniSecurityStat aes;
	tAni64BitCounters aesReplays;
	tAniSecurityStat tkip;
	tAni64BitCounters tkipReplays;
	tAni64BitCounters tkipMicError;

	tAniSecurityStat wep;
#if defined(FEATURE_WLAN_WAPI) && !defined(LIBRA_WAPI_SUPPORT)
	tAniSecurityStat wpi;
	tAni64BitCounters wpiReplays;
	tAni64BitCounters wpiMicError;
#endif
} tAniSecStats, *tpAniSecStats;

#define SIR_MAX_RX_CHAINS 3

typedef struct sAniStaStatStruct {
	/* following statistic elements till expandPktRxCntLo are not filled
	 * with valid data. These are kept as it is, since WSM is using this
	 * structure. These elements can be removed whenever WSM is updated.
	 * Phystats is used to hold phystats from BD.
	 */
	uint32_t sentAesBlksUcastHi;
	uint32_t sentAesBlksUcastLo;
	uint32_t recvAesBlksUcastHi;
	uint32_t recvAesBlksUcastLo;
	uint32_t aesFormatErrorUcastCnts;
	uint32_t aesReplaysUcast;
	uint32_t aesDecryptErrUcast;
	uint32_t singleRetryPkts;
	uint32_t failedTxPkts;
	uint32_t ackTimeouts;
	uint32_t multiRetryPkts;
	uint32_t fragTxCntsHi;
	uint32_t fragTxCntsLo;
	uint32_t transmittedPktsHi;
	uint32_t transmittedPktsLo;
	uint32_t phyStatHi;     /* These are used to fill in the phystats. */
	uint32_t phyStatLo;     /* This is only for private use. */

	uint32_t uplinkRssi;
	uint32_t uplinkSinr;
	uint32_t uplinkRate;
	uint32_t downlinkRssi;
	uint32_t downlinkSinr;
	uint32_t downlinkRate;
	uint32_t nRcvBytes;
	uint32_t nXmitBytes;

	/*
	 * Following elements are valid and filled in correctly. They have
	 * valid values.
	 */

	/* Unicast frames and bytes. */
	tAniTxRxStats ucStats;

	/* Broadcast frames and bytes. */
	tAniTxRxStats bcStats;

	/* Multicast frames and bytes. */
	tAniTxRxStats mcStats;

	uint32_t currentTxRate;
	uint32_t currentRxRate; /* Rate in 100Kbps */

	uint32_t maxTxRate;
	uint32_t maxRxRate;

	int8_t rssi[SIR_MAX_RX_CHAINS];

	tAniSecStats securityStats;

	uint8_t currentRxRateIdx;       /* This the softmac rate Index. */
	uint8_t currentTxRateIdx;

} tAniStaStatStruct, *tpAniStaStatStruct;

typedef enum sPacketType {
	ePACKET_TYPE_UNKNOWN,
	ePACKET_TYPE_11A,
	ePACKET_TYPE_11G,
	ePACKET_TYPE_11B,
	ePACKET_TYPE_11N
} tPacketType, *tpPacketType;

/* / Definition for Disassociation response */
typedef struct sSirSmeDisassocRsp {
	uint16_t messageType;   /* eWNI_SME_DISASSOC_RSP */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId; /* Transaction ID for cmd */
	tSirResultCodes statusCode;
	tSirMacAddr peerMacAddr;
	tAniStaStatStruct perStaStats;  /* STA stats */
	uint16_t staId;
} cdf_packed tSirSmeDisassocRsp, *tpSirSmeDisassocRsp;

/* / Definition for Disassociation indication from peer */
typedef struct sSirSmeDisassocInd {
	uint16_t messageType;   /* eWNI_SME_DISASSOC_IND */
	uint16_t length;
	uint8_t sessionId;      /* Session Identifier */
	uint16_t transactionId; /* Transaction Identifier with PE */
	tSirResultCodes statusCode;
	tSirMacAddr bssId;
	tSirMacAddr peerMacAddr;
	tAniStaStatStruct perStaStats;  /* STA stats */
	uint16_t staId;
	uint32_t reasonCode;
} tSirSmeDisassocInd, *tpSirSmeDisassocInd;

/* / Definition for Disassociation confirm */
/* / MAC ---> */
typedef struct sSirSmeDisassocCnf {
	uint16_t messageType;   /* eWNI_SME_DISASSOC_CNF */
	uint16_t length;
	tSirResultCodes statusCode;
	tSirMacAddr bssId;
	tSirMacAddr peerMacAddr;
} tSirSmeDisassocCnf, *tpSirSmeDisassocCnf,
	tSirSmeDeauthCnf, *tpSirSmeDeauthCnf;

/* / Definition for Deauthetication request */
typedef struct sSirSmeDeauthReq {
	uint16_t messageType;   /* eWNI_SME_DEAUTH_REQ */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId; /* Transaction ID for cmd */
	tSirMacAddr bssId;      /* AP BSSID */
	tSirMacAddr peerMacAddr;
	uint16_t reasonCode;
} tSirSmeDeauthReq, *tpSirSmeDeauthReq;

/* / Definition for Deauthetication response */
typedef struct sSirSmeDeauthRsp {
	uint16_t messageType;   /* eWNI_SME_DEAUTH_RSP */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId; /* Transaction ID for cmd */
	tSirResultCodes statusCode;
	tSirMacAddr peerMacAddr;
} tSirSmeDeauthRsp, *tpSirSmeDeauthRsp;

/* / Definition for Deauthetication indication from peer */
typedef struct sSirSmeDeauthInd {
	uint16_t messageType;   /* eWNI_SME_DEAUTH_IND */
	uint16_t length;
	uint8_t sessionId;      /* Added for BT-AMP */
	uint16_t transactionId; /* Added for BT-AMP */
	tSirResultCodes statusCode;
	tSirMacAddr bssId;      /* AP BSSID */
	tSirMacAddr peerMacAddr;

	uint16_t staId;
	uint32_t reasonCode;
} tSirSmeDeauthInd, *tpSirSmeDeauthInd;

/* / Definition for stop BSS request message */
typedef struct sSirSmeStopBssReq {
	uint16_t messageType;   /* eWNI_SME_STOP_BSS_REQ */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId; /* tranSaction ID for cmd */
	tSirResultCodes reasonCode;
	tSirMacAddr bssId;      /* Self BSSID */
} tSirSmeStopBssReq, *tpSirSmeStopBssReq;

/* / Definition for stop BSS response message */
typedef struct sSirSmeStopBssRsp {
	uint16_t messageType;   /* eWNI_SME_STOP_BSS_RSP */
	uint16_t length;
	tSirResultCodes statusCode;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId; /* Transaction ID for cmd */
} tSirSmeStopBssRsp, *tpSirSmeStopBssRsp;

/* / Definition for Channel Switch indication for station */
/* / MAC ---> */
typedef struct sSirSmeSwitchChannelInd {
	uint16_t messageType;   /* eWNI_SME_SWITCH_CHL_IND */
	uint16_t length;
	uint8_t sessionId;
	uint16_t newChannelId;
	tSirMacAddr bssId;      /* BSSID */
} tSirSmeSwitchChannelInd, *tpSirSmeSwitchChannelInd;

/* / Definition for Neighbor BSS indication */
/* / MAC ---> */
/* / MAC reports this each time a new I/BSS is detected */
typedef struct sSirSmeNeighborBssInd {
	uint16_t messageType;   /* eWNI_SME_NEIGHBOR_BSS_IND */
	uint16_t length;
	uint8_t sessionId;
	tSirBssDescription bssDescription[1];
} tSirSmeNeighborBssInd, *tpSirSmeNeighborBssInd;

/* / Definition for MIC failure indication */
/* / MAC ---> */
/* / MAC reports this each time a MIC failure occures on Rx TKIP packet */
typedef struct sSirSmeMicFailureInd {
	uint16_t messageType;   /* eWNI_SME_MIC_FAILURE_IND */
	uint16_t length;
	uint8_t sessionId;
	struct cdf_mac_addr bssId;
	tSirMicFailureInfo info;
} tSirSmeMicFailureInd, *tpSirSmeMicFailureInd;

typedef struct sSirSmeMissedBeaconInd {
	uint16_t messageType;   /* eWNI_SME_MISSED_BEACON_IND */
	uint16_t length;
	uint8_t bssIdx;
} tSirSmeMissedBeaconInd, *tpSirSmeMissedBeaconInd;

/* / Definition for Set Context request */
/* / ---> MAC */
typedef struct sSirSmeSetContextReq {
	uint16_t messageType;   /* eWNI_SME_SET_CONTEXT_REQ */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId; /* Transaction ID for cmd */
	tSirMacAddr peerMacAddr;
	tSirMacAddr bssId;      /* BSSID */
	tSirKeyMaterial keyMaterial;
} tSirSmeSetContextReq, *tpSirSmeSetContextReq;

/* / Definition for Set Context response */
/* / MAC ---> */
typedef struct sSirSmeSetContextRsp {
	uint16_t messageType;   /* eWNI_SME_SET_CONTEXT_RSP */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId; /* Transaction ID for cmd */
	tSirResultCodes statusCode;
	tSirMacAddr peerMacAddr;
} tSirSmeSetContextRsp, *tpSirSmeSetContextRsp;

/* / Statistic definitions */
/* ============================================================= */
/* Per STA statistic structure; This same struct will be used for Aggregate */
/* STA stats as well. */

/* Clear radio stats and clear per sta stats */
typedef enum {
	eANI_CLEAR_ALL_STATS,   /* Clears all stats */
	eANI_CLEAR_RX_STATS,    /* Clears RX stats of the radio interface */
	eANI_CLEAR_TX_STATS,    /* Clears TX stats of the radio interface */
	eANI_CLEAR_RADIO_STATS, /* Clears all the radio stats */
	eANI_CLEAR_PER_STA_STATS,       /* Clears Per STA stats */
	eANI_CLEAR_AGGR_PER_STA_STATS,  /* Clears aggregate stats */

	/* Used to distinguish between per sta to security stats. */
	/* Used only by AP, FW just returns the same param as it received. */
	eANI_LINK_STATS,        /* Get Per STA stats */
	eANI_SECURITY_STATS,    /* Get Per STA security stats */

	eANI_CLEAR_STAT_TYPES_END
} tAniStatSubTypes;

typedef struct sAniTxCtrs {
	/* add the rate counters here */
	uint32_t tx1Mbps;
	uint32_t tx2Mbps;
	uint32_t tx5_5Mbps;
	uint32_t tx6Mbps;
	uint32_t tx9Mbps;
	uint32_t tx11Mbps;
	uint32_t tx12Mbps;
	uint32_t tx18Mbps;
	uint32_t tx24Mbps;
	uint32_t tx36Mbps;
	uint32_t tx48Mbps;
	uint32_t tx54Mbps;
	uint32_t tx72Mbps;
	uint32_t tx96Mbps;
	uint32_t tx108Mbps;

	/* tx path radio counts */
	uint32_t txFragHi;
	uint32_t txFragLo;
	uint32_t txFrameHi;
	uint32_t txFrameLo;
	uint32_t txMulticastFrameHi;
	uint32_t txMulticastFrameLo;
	uint32_t txFailedHi;
	uint32_t txFailedLo;
	uint32_t multipleRetryHi;
	uint32_t multipleRetryLo;
	uint32_t singleRetryHi;
	uint32_t singleRetryLo;
	uint32_t ackFailureHi;
	uint32_t ackFailureLo;
	uint32_t xmitBeacons;
} tAniTxCtrs, *tpAniTxCtrs;

typedef struct sAniRxCtrs {
	/* receive frame rate counters */
	uint32_t rx1Mbps;
	uint32_t rx2Mbps;
	uint32_t rx5_5Mbps;
	uint32_t rx6Mbps;
	uint32_t rx9Mbps;
	uint32_t rx11Mbps;
	uint32_t rx12Mbps;
	uint32_t rx18Mbps;
	uint32_t rx24Mbps;
	uint32_t rx36Mbps;
	uint32_t rx48Mbps;
	uint32_t rx54Mbps;
	uint32_t rx72Mbps;
	uint32_t rx96Mbps;
	uint32_t rx108Mbps;

	/* receive size counters; 'Lte' = Less than or equal to */
	uint32_t rxLte64;
	uint32_t rxLte128Gt64;
	uint32_t rxLte256Gt128;
	uint32_t rxLte512Gt256;
	uint32_t rxLte1kGt512;
	uint32_t rxLte1518Gt1k;
	uint32_t rxLte2kGt1518;
	uint32_t rxLte4kGt2k;

	/* rx radio stats */
	uint32_t rxFrag;
	uint32_t rxFrame;
	uint32_t fcsError;
	uint32_t rxMulticast;
	uint32_t duplicate;
	uint32_t rtsSuccess;
	uint32_t rtsFailed;
	uint32_t wepUndecryptables;
	uint32_t drops;
	uint32_t aesFormatErrorUcastCnts;
	uint32_t aesReplaysUcast;
	uint32_t aesDecryptErrUcast;
} tAniRxCtrs, *tpAniRxCtrs;

/* Get Radio Stats request structure */
/* This structure shall be used for both Radio stats and Aggregate stats */
/* A valid request must contain entire structure with/without valid fields. */
/* Based on the request type, the valid fields will be checked. */
typedef struct sAniGetStatsReq {
	/* Common for all types are requests */
	uint16_t msgType;       /* message type is same as the request type */
	uint16_t msgLen;        /* length of the entire request */
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId;
	tSirMacAddr bssId;      /* BSSID */
	/* only used for clear stats and per sta stats clear */
	tAniStatSubTypes stat;  /* Clears the stats of the described types. */
	uint32_t staId;         /* Per STA stats request must contain valid */
	/* values */
	tSirMacAddr macAddr;
} tAniGetStatsReq, *tpAniGetStatsReq;

/* *************************************************************** */

/*******************PE Statistics*************************/

/*
 * tpAniGetPEStatsReq is tied to
 * for SME ==> PE eWNI_SME_GET_STATISTICS_REQ msgId  and
 * for PE ==> HAL SIR_HAL_GET_STATISTICS_REQ msgId
 */
typedef struct sAniGetPEStatsReq {
	/* Common for all types are requests */
	uint16_t msgType;       /* message type is same as the request type */
	uint16_t msgLen;        /* length of the entire request */
	uint32_t staId;         /* Per STA stats request must contain valid */
	/* categories of stats requested. look at ePEStatsMask */
	uint32_t statsMask;
	uint8_t sessionId;
} tAniGetPEStatsReq, *tpAniGetPEStatsReq;

/*
 * tpAniGetPEStatsRsp is tied to
 * for PE ==> SME eWNI_SME_GET_STATISTICS_RSP msgId  and
 * for HAL ==> PE SIR_HAL_GET_STATISTICS_RSP msgId
 */
typedef struct sAniGetPEStatsRsp {
	/* Common for all types are responses */
	uint16_t msgType;       /* message type is same as the request type */
	/* length of the entire request, includes the pStatsBuf length too */
	uint16_t msgLen;
	uint8_t sessionId;
	uint32_t rc;            /* success/failure */
	uint32_t staId;         /* Per STA stats request must contain valid */
	/* categories of stats requested. look at ePEStatsMask */
	uint32_t statsMask;
	/* void                  *pStatsBuf; */
	/*
	 * The Stats buffer starts here and can be an aggregate of more than one
	 * statistics structure depending on statsMask. The void pointer
	 * "pStatsBuf" is commented out intentionally and the src code that uses
	 * this structure should take that into account.
	 */
} tAniGetPEStatsRsp, *tpAniGetPEStatsRsp;

typedef struct sAniGetRssiReq {
	/* Common for all types are requests */
	uint16_t msgType;       /* message type is same as the request type */
	uint16_t msgLen;        /* length of the entire request */
	uint8_t sessionId;
	uint8_t staId;
	int8_t lastRSSI;        /* in case of error, return last RSSI */
	void *rssiCallback;
	void *pDevContext;      /* device context */
	void *p_cds_context;    /* cds context */

} tAniGetRssiReq, *tpAniGetRssiReq;

typedef struct sAniGetSnrReq {
	/* Common for all types are requests */
	uint16_t msgType;       /* message type is same as the request type */
	uint16_t msgLen;        /* length of the entire request */
	uint8_t sessionId;
	uint8_t staId;
	void *snrCallback;
	void *pDevContext;      /* device context */
	int8_t snr;
} tAniGetSnrReq, *tpAniGetSnrReq;

#if defined(FEATURE_WLAN_ESE) || defined(FEATURE_WLAN_ESE_UPLOAD)
typedef struct sSirTsmIE {
	uint8_t tsid;
	uint8_t state;
	uint16_t msmt_interval;
} tSirTsmIE, *tpSirTsmIE;
typedef struct sSirSmeTsmIEInd {
	tSirTsmIE tsmIe;
	uint8_t sessionId;
} tSirSmeTsmIEInd, *tpSirSmeTsmIEInd;
typedef struct sAniTrafStrmMetrics {
	uint16_t UplinkPktQueueDly;
	uint16_t UplinkPktQueueDlyHist[4];
	uint32_t UplinkPktTxDly;
	uint16_t UplinkPktLoss;
	uint16_t UplinkPktCount;
	uint8_t RoamingCount;
	uint16_t RoamingDly;
} tAniTrafStrmMetrics, *tpAniTrafStrmMetrics;

typedef struct sAniGetTsmStatsReq {
	/* Common for all types are requests */
	uint16_t msgType;       /* message type is same as the request type */
	uint16_t msgLen;        /* length of the entire request */
	uint8_t staId;
	uint8_t tid;            /* traffic id */
	struct cdf_mac_addr bssId;
	void *tsmStatsCallback;
	void *pDevContext;      /* device context */
	void *p_cds_context;    /* cds context */
} tAniGetTsmStatsReq, *tpAniGetTsmStatsReq;

typedef struct sAniGetTsmStatsRsp {
	/* Common for all types are responses */
	uint16_t msgType;       /*
				 * message type is same as
				 * the request type
				 */
	uint16_t msgLen;        /*
				 * length of the entire request,
				 * includes the pStatsBuf length too
				 */
	uint8_t sessionId;
	uint32_t rc;            /* success/failure */
	uint32_t staId;         /*
				 * Per STA stats request must
				 * contain valid
				 */
	tAniTrafStrmMetrics tsmMetrics;
	void *tsmStatsReq;      /* tsm stats request backup */
} tAniGetTsmStatsRsp, *tpAniGetTsmStatsRsp;

typedef struct sSirEseBcnReportBssInfo {
	tBcnReportFields bcnReportFields;
	uint8_t ieLen;
	uint8_t *pBuf;
} tSirEseBcnReportBssInfo, *tpSirEseBcnReportBssInfo;

typedef struct sSirEseBcnReportRsp {
	uint16_t measurementToken;
	uint8_t flag;        /* Flag to report measurement done and more data */
	uint8_t numBss;
	tSirEseBcnReportBssInfo bcnRepBssInfo[SIR_BCN_REPORT_MAX_BSS_DESC];
} tSirEseBcnReportRsp, *tpSirEseBcnReportRsp;

#endif /* FEATURE_WLAN_ESE || FEATURE_WLAN_ESE_UPLOAD */

/* Change country code request MSG structure */
typedef struct sAniChangeCountryCodeReq {
	/* Common for all types are requests */
	uint16_t msgType;       /* message type is same as the request type */
	uint16_t msgLen;        /* length of the entire request */
	uint8_t countryCode[WNI_CFG_COUNTRY_CODE_LEN]; /* 3 char country code */
	tAniBool countryFromUserSpace;
	tAniBool sendRegHint;   /* true if we want to send hint to NL80211 */
	void *changeCCCallback;
	void *pDevContext;      /* device context */
	void *p_cds_context;    /* cds context */

} tAniChangeCountryCodeReq, *tpAniChangeCountryCodeReq;

/**
 * struct ani_scan_req - Scan request
 * @msg_type: Message type
 * @msg_len: Message Length
 * @session_id: SME session Id
 * @scan_param: scan request parameter
 * @callback: call back function for scan result
 * @ctx: Global context
 *
 * Scan request message structure
 */
struct ani_scan_req {
	/* message type is same as the request type */
	uint16_t msg_type;
	/* length of the entire request */
	uint16_t msg_len;
	uint16_t session_id;
	void *scan_param;
	void *callback;
	void *ctx;
};

/**
 * struct ani_roc_req - Remain on channel request
 * @msg_type: Message type
 * @msg_len: Message Length
 * @session_id: SME session Id
 * @channel: channel number
 * @callback: call back function for scan result
 * @duration: Roc duration
 * @is_p2pprobe_allowed : flag for p2p probe request
 * @ctx: Global context
 * @scan_id: Scan Identifier
 *
 * Remain on channel request message structure
 */
struct ani_roc_req {
	/* message type is same as the request type */
	uint16_t msg_type;
	/* length of the entire request */
	uint16_t msg_len;
	uint16_t session_id;
	uint8_t channel;
	uint32_t duration;
	uint8_t is_p2pprobe_allowed;
	void *callback;
	void *ctx;
	uint32_t scan_id;
};

/* generic country code change request MSG structure */
typedef struct sAniGenericChangeCountryCodeReq {
	/* Common for all types are requests */
	uint16_t msgType;       /* message type is same as the request type */
	uint16_t msgLen;        /* length of the entire request */
	uint8_t countryCode[WNI_CFG_COUNTRY_CODE_LEN];  /* 3 char country code */
	uint16_t domain_index;
} tAniGenericChangeCountryCodeReq, *tpAniGenericChangeCountryCodeReq;

/**
 * struct sAniDHCPStopInd - DHCP Stop indication message
 * @msgType: message type is same as the request type
 * @msgLen: length of the entire request
 * @device_mode: Mode of the device(ex:STA, AP)
 * @adapterMacAddr: MAC address of the adapter
 * @peerMacAddr: MAC address of the connected peer
 */
typedef struct sAniDHCPStopInd {
	uint16_t msgType;
	uint16_t msgLen;
	uint8_t device_mode;
	struct cdf_mac_addr adapterMacAddr;
	struct cdf_mac_addr peerMacAddr;
} tAniDHCPInd, *tpAniDHCPInd;

typedef enum eTxRateInfo {
	eHAL_TX_RATE_LEGACY = 0x1,      /* Legacy rates */
	eHAL_TX_RATE_HT20 = 0x2,        /* HT20 rates */
	eHAL_TX_RATE_HT40 = 0x4,        /* HT40 rates */
	eHAL_TX_RATE_SGI = 0x8, /* Rate with Short guard interval */
	eHAL_TX_RATE_LGI = 0x10,        /* Rate with Long guard interval */
	eHAL_TX_RATE_VHT20 = 0x20,      /* VHT 20 rates */
	eHAL_TX_RATE_VHT40 = 0x40,      /* VHT 40 rates */
	eHAL_TX_RATE_VHT80 = 0x80       /* VHT 80 rates */
} tTxrateinfoflags;

/**********************PE Statistics end*************************/

typedef struct sSirP2PNoaStart {
	uint32_t status;
	uint32_t bssIdx;
} tSirP2PNoaStart, *tpSirP2PNoaStart;

typedef struct sSirTdlsInd {
	uint16_t status;
	uint16_t assocId;
	uint16_t staIdx;
	uint16_t reasonCode;
} tSirTdlsInd, *tpSirTdlsInd;

typedef struct sSirP2PNoaAttr {
#ifdef ANI_BIG_BYTE_ENDIAN
	uint32_t index:8;
	uint32_t oppPsFlag:1;
	uint32_t ctWin:7;
	uint32_t rsvd1:16;
#else
	uint32_t rsvd1:16;
	uint32_t ctWin:7;
	uint32_t oppPsFlag:1;
	uint32_t index:8;
#endif

#ifdef ANI_BIG_BYTE_ENDIAN
	uint32_t uNoa1IntervalCnt:8;
	uint32_t rsvd2:24;
#else
	uint32_t rsvd2:24;
	uint32_t uNoa1IntervalCnt:8;
#endif
	uint32_t uNoa1Duration;
	uint32_t uNoa1Interval;
	uint32_t uNoa1StartTime;

#ifdef ANI_BIG_BYTE_ENDIAN
	uint32_t uNoa2IntervalCnt:8;
	uint32_t rsvd3:24;
#else
	uint32_t rsvd3:24;
	uint32_t uNoa2IntervalCnt:8;
#endif
	uint32_t uNoa2Duration;
	uint32_t uNoa2Interval;
	uint32_t uNoa2StartTime;
} tSirP2PNoaAttr, *tpSirP2PNoaAttr;

typedef struct sSirTclasInfo {
	tSirMacTclasIE tclas;
	uint8_t version;        /* applies only for classifier type ip */
	union {
		tSirMacTclasParamEthernet eth;
		tSirMacTclasParamIPv4 ipv4;
		tSirMacTclasParamIPv6 ipv6;
		tSirMacTclasParam8021dq t8021dq;
	} cdf_packed tclasParams;
} cdf_packed tSirTclasInfo;

#if defined(FEATURE_WLAN_ESE) || defined(FEATURE_WLAN_ESE_UPLOAD)
#define TSRS_11AG_RATE_6MBPS   0xC
#define TSRS_11B_RATE_5_5MBPS  0xB
typedef struct sSirMacESETSRSIE {
	uint8_t tsid;
	uint8_t rates[8];
} tSirMacESETSRSIE;
typedef struct sSirMacESETSMIE {
	uint8_t tsid;
	uint8_t state;
	uint16_t msmt_interval;
} tSirMacESETSMIE;
typedef struct sTSMStats {
	uint8_t tid;
	struct cdf_mac_addr bssid;
	tTrafStrmMetrics tsmMetrics;
} tTSMStats, *tpTSMStats;
typedef struct sEseTSMContext {
	uint8_t tid;
	tSirMacESETSMIE tsmInfo;
	tTrafStrmMetrics tsmMetrics;
} tEseTSMContext, *tpEseTSMContext;
typedef struct sEsePEContext {
	tEseTSMContext tsm;
} tEsePEContext, *tpEsePEContext;
#endif /* FEATURE_WLAN_ESE && FEATURE_WLAN_ESE_UPLOAD */

typedef struct sSirAddtsReqInfo {
	uint8_t dialogToken;
	tSirMacTspecIE tspec;

	uint8_t numTclas;       /* number of Tclas elements */
	tSirTclasInfo tclasInfo[SIR_MAC_TCLASIE_MAXNUM];
	uint8_t tclasProc;
#if defined(FEATURE_WLAN_ESE)
	tSirMacESETSRSIE tsrsIE;
	uint8_t tsrsPresent:1;
#endif
	uint8_t wmeTspecPresent:1;
	uint8_t wsmTspecPresent:1;
	uint8_t lleTspecPresent:1;
	uint8_t tclasProcPresent:1;
} tSirAddtsReqInfo, *tpSirAddtsReqInfo;

typedef struct sSirAddtsRspInfo {
	uint8_t dialogToken;
	tSirMacStatusCodes status;
	tSirMacTsDelayIE delay;

	tSirMacTspecIE tspec;
	uint8_t numTclas;       /* number of Tclas elements */
	tSirTclasInfo tclasInfo[SIR_MAC_TCLASIE_MAXNUM];
	uint8_t tclasProc;
	tSirMacScheduleIE schedule;
#if defined(FEATURE_WLAN_ESE) || defined(FEATURE_WLAN_ESE_UPLOAD)
	tSirMacESETSMIE tsmIE;
	uint8_t tsmPresent:1;
#endif
	uint8_t wmeTspecPresent:1;
	uint8_t wsmTspecPresent:1;
	uint8_t lleTspecPresent:1;
	uint8_t tclasProcPresent:1;
	uint8_t schedulePresent:1;
} tSirAddtsRspInfo, *tpSirAddtsRspInfo;

typedef struct sSirDeltsReqInfo {
	tSirMacTSInfo tsinfo;
	tSirMacTspecIE tspec;
	uint8_t wmeTspecPresent:1;
	uint8_t wsmTspecPresent:1;
	uint8_t lleTspecPresent:1;
} tSirDeltsReqInfo, *tpSirDeltsReqInfo;

/* / Add a tspec as defined */
typedef struct sSirAddtsReq {
	uint16_t messageType;   /* eWNI_SME_ADDTS_REQ */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId;
	tSirMacAddr bssId;      /* BSSID */
	uint32_t timeout;       /* in ms */
	uint8_t rspReqd;
	tSirAddtsReqInfo req;
} tSirAddtsReq, *tpSirAddtsReq;

typedef struct sSirAddtsRsp {
	uint16_t messageType;   /* eWNI_SME_ADDTS_RSP */
	uint16_t length;
	uint8_t sessionId;      /* sme sessionId  Added for BT-AMP support */
	uint16_t transactionId; /* sme transaction Id - for BT-AMP Support */
	uint32_t rc;            /* return code */
	tSirAddtsRspInfo rsp;
} tSirAddtsRsp, *tpSirAddtsRsp;

typedef struct sSirDeltsReq {
	uint16_t messageType;   /* eWNI_SME_DELTS_REQ */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId;
	tSirMacAddr bssId;      /* BSSID */
	uint16_t aid;           /* use 0 if macAddr is being specified */
	tSirMacAddr macAddr;    /* only on AP to specify the STA */
	uint8_t rspReqd;
	tSirDeltsReqInfo req;
} tSirDeltsReq, *tpSirDeltsReq;

typedef struct sSirDeltsRsp {
	uint16_t messageType;   /* eWNI_SME_DELTS_RSP */
	uint16_t length;
	uint8_t sessionId;      /* sme sessionId  Added for BT-AMP support */
	uint16_t transactionId; /* sme transaction Id - for BT-AMP Support */
	uint32_t rc;
	uint16_t aid;           /* use 0 if macAddr is being specified */
	tSirMacAddr macAddr;    /* only on AP to specify the STA */
	tSirDeltsReqInfo rsp;
} tSirDeltsRsp, *tpSirDeltsRsp;

#if defined(FEATURE_WLAN_ESE) && defined(FEATURE_WLAN_ESE_UPLOAD)
typedef struct sSirPlmReq {
	uint16_t diag_token;    /* Dialog token */
	uint16_t meas_token;    /* measurement token */
	uint16_t numBursts;     /* total number of bursts */
	uint16_t burstInt;      /* burst interval in seconds */
	uint16_t measDuration;  /* in TU's,STA goes off-ch */
	/* no of times the STA should cycle through PLM ch list */
	uint8_t burstLen;
	tPowerdBm desiredTxPwr; /* desired tx power */
	struct cdf_mac_addr mac_addr;    /* MC dest addr */
	/* no of channels */
	uint8_t plmNumCh;
	/* channel numbers */
	uint8_t plmChList[WNI_CFG_VALID_CHANNEL_LIST_LEN];
	uint8_t sessionId;
	bool enable;
} tSirPlmReq, *tpSirPlmReq;
#endif

#if defined WLAN_FEATURE_VOWIFI_11R || defined FEATURE_WLAN_ESE || defined(FEATURE_WLAN_LFR)

#define SIR_QOS_NUM_AC_MAX 4

typedef struct sSirAggrQosReqInfo {
	uint16_t tspecIdx;
	tSirAddtsReqInfo aggrAddTsInfo[SIR_QOS_NUM_AC_MAX];
} tSirAggrQosReqInfo, *tpSirAggrQosReqInfo;

typedef struct sSirAggrQosReq {
	uint16_t messageType;   /* eWNI_SME_ADDTS_REQ */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId;
	tSirMacAddr bssId;      /* BSSID */
	uint32_t timeout;       /* in ms */
	uint8_t rspReqd;
	tSirAggrQosReqInfo aggrInfo;
} tSirAggrQosReq, *tpSirAggrQosReq;

typedef struct sSirAggrQosRspInfo {
	uint16_t tspecIdx;
	tSirAddtsRspInfo aggrRsp[SIR_QOS_NUM_AC_MAX];
} tSirAggrQosRspInfo, *tpSirAggrQosRspInfo;

typedef struct sSirAggrQosRsp {
	uint16_t messageType;
	uint16_t length;
	uint8_t sessionId;
	tSirAggrQosRspInfo aggrInfo;
} tSirAggrQosRsp, *tpSirAggrQosRsp;

#endif /*WLAN_FEATURE_VOWIFI_11R || FEATURE_WLAN_ESE */

typedef struct sSirQosMapSet {
	uint8_t present;
	uint8_t num_dscp_exceptions;
	uint8_t dscp_exceptions[21][2];
	uint8_t dscp_range[8][2];
} tSirQosMapSet, *tpSirQosMapSet;

typedef struct sSmeIbssPeerInd {
	uint16_t mesgType;
	uint16_t mesgLen;
	uint8_t sessionId;

	tSirMacAddr peerAddr;
	uint16_t staId;

	/*
	 * The DPU signatures will be sent eventually to TL to help it determine
	 * the association to which a packet belongs to
	 */
	/* Unicast DPU signature */
	uint8_t ucastSig;

	/*Broadcast DPU signature */
	uint8_t bcastSig;

	/* Beacon will be appended for new Peer indication. */
} tSmeIbssPeerInd, *tpSmeIbssPeerInd;

typedef struct sSirIbssPeerInactivityInd {
	uint8_t bssIdx;
	uint8_t staIdx;
	tSirMacAddr peerAddr;
} tSirIbssPeerInactivityInd, *tpSirIbssPeerInactivityInd;

typedef struct sLimScanChn {
	uint16_t numTimeScan;   /* how many time this channel is scan */
	uint8_t channelId;
} tLimScanChn;

typedef struct sSmeGetScanChnRsp {
	/* Message Type */
	uint16_t mesgType;
	/* Message Length */
	uint16_t mesgLen;
	uint8_t sessionId;
	uint8_t numChn;
	tLimScanChn scanChn[1];
} tSmeGetScanChnRsp, *tpSmeGetScanChnRsp;

typedef struct sLimScanChnInfo {
	uint8_t numChnInfo;     /* number of channels in scanChn */
	tLimScanChn scanChn[SIR_MAX_SUPPORTED_CHANNEL_LIST];
} tLimScanChnInfo;

typedef struct sSirSmeGetAssocSTAsReq {
	uint16_t messageType;   /* eWNI_SME_GET_ASSOC_STAS_REQ */
	uint16_t length;
	tSirMacAddr bssId;      /* BSSID */
	uint16_t modId;
	void *pUsrContext;
	void *pSapEventCallback;
	/* Pointer to allocated mem passed in wlansap_get_assoc_stations API */
	void *pAssocStasArray;
} tSirSmeGetAssocSTAsReq, *tpSirSmeGetAssocSTAsReq;

typedef struct sSmeMaxAssocInd {
	uint16_t mesgType;      /* eWNI_SME_MAX_ASSOC_EXCEEDED */
	uint16_t mesgLen;
	uint8_t sessionId;
	/* the new peer that got rejected max assoc limit reached */
	tSirMacAddr peerMac;
} tSmeMaxAssocInd, *tpSmeMaxAssocInd;

typedef struct sSmeCsaOffloadInd {
	uint16_t mesgType;      /* eWNI_SME_CSA_OFFLOAD_EVENT */
	uint16_t mesgLen;
	tSirMacAddr bssId;      /* BSSID */
} tSmeCsaOffloadInd, *tpSmeCsaOffloadInd;

/* WOW related structures */
#define SIR_WOWL_BCAST_PATTERN_MAX_SIZE 146

/**
 * struct wow_add_pattern - wow pattern add structure
 * @pattern_id: pattern id
 * @pattern_byte_offset: pattern byte offset from beginning of the 802.11
 *			 packet to start of the wake-up pattern
 * @pattern_size: pattern size
 * @pattern: pattern byte stream
 * @pattern_mask_size: pattern mask size
 * @pattern_mask: pattern mask
 * @session_id: session id
 */
struct wow_add_pattern {
	uint8_t pattern_id;
	uint8_t pattern_byte_offset;
	uint8_t pattern_size;
	uint8_t pattern[SIR_WOWL_BCAST_PATTERN_MAX_SIZE];
	uint8_t pattern_mask_size;
	uint8_t pattern_mask[SIR_WOWL_BCAST_PATTERN_MAX_SIZE];
	uint8_t session_id;
};

/**
 * struct wow_delete_patern - wow pattern delete structure
 * @pattern_id: pattern id of wake up pattern to be deleted
 * @session_id: session id
 */
struct wow_delete_pattern {
	uint8_t pattern_id;
	uint8_t session_id;
};

/* SME->PE: Enter WOWLAN parameters */
typedef struct sSirSmeWowlEnterParams {
	uint8_t sessionId;

	/* Enables/disables magic packet filtering */
	uint8_t ucMagicPktEnable;

	/* Magic pattern */
	tSirMacAddr magicPtrn;

	/* Enables/disables packet pattern filtering */
	uint8_t ucPatternFilteringEnable;

#ifdef WLAN_WAKEUP_EVENTS
	/*
	 * This configuration directs the WoW packet filtering to look at EAP-ID
	 * requests embedded in EAPOL frames and use this as a wake source.
	 */
	uint8_t ucWoWEAPIDRequestEnable;

	/*
	 * This configuration directs the WoW packet filtering to look for
	 * EAPOL-4WAY requests and use this as a wake source.
	 */
	uint8_t ucWoWEAPOL4WayEnable;

	/*
	 * This configuration allows a host wakeup on an network scan
	 * offload match.
	 */
	uint8_t ucWowNetScanOffloadMatch;

	/* This configuration allows a host wakeup on any GTK rekeying error.
	 */
	uint8_t ucWowGTKRekeyError;

	/* This configuration allows a host wakeup on BSS connection loss.
	 */
	uint8_t ucWoWBSSConnLoss;
#endif /* WLAN_WAKEUP_EVENTS */

	tSirMacAddr bssId;
} tSirSmeWowlEnterParams, *tpSirSmeWowlEnterParams;

/* PE<->HAL: Enter WOWLAN parameters */
typedef struct sSirHalWowlEnterParams {
	uint8_t sessionId;

	/* Enables/disables magic packet filtering */
	uint8_t ucMagicPktEnable;

	/* Magic pattern */
	tSirMacAddr magicPtrn;

	/* Enables/disables packet pattern filtering in firmware.
	   Enabling this flag enables broadcast pattern matching
	   in Firmware. If unicast pattern matching is also desired,
	   ucUcastPatternFilteringEnable flag must be set tot true
	   as well
	 */
	uint8_t ucPatternFilteringEnable;

	/* Enables/disables unicast packet pattern filtering.
	   This flag specifies whether we want to do pattern match
	   on unicast packets as well and not just broadcast packets.
	   This flag has no effect if the ucPatternFilteringEnable
	   (main controlling flag) is set to false
	 */
	uint8_t ucUcastPatternFilteringEnable;

	/* This configuration is valid only when magicPktEnable=1.
	 * It requests hardware to wake up when it receives the
	 * Channel Switch Action Frame.
	 */
	uint8_t ucWowChnlSwitchRcv;

	/* This configuration is valid only when magicPktEnable=1.
	 * It requests hardware to wake up when it receives the
	 * Deauthentication Frame.
	 */
	uint8_t ucWowDeauthRcv;

	/* This configuration is valid only when magicPktEnable=1.
	 * It requests hardware to wake up when it receives the
	 * Disassociation Frame.
	 */
	uint8_t ucWowDisassocRcv;

	/* This configuration is valid only when magicPktEnable=1.
	 * It requests hardware to wake up when it has missed
	 * consecutive beacons. This is a hardware register
	 * configuration (NOT a firmware configuration).
	 */
	uint8_t ucWowMaxMissedBeacons;

	/* This configuration is valid only when magicPktEnable=1.
	 * This is a timeout value in units of microsec. It requests
	 * hardware to unconditionally wake up after it has stayed
	 * in WoWLAN mode for some time. Set 0 to disable this feature.
	 */
	uint8_t ucWowMaxSleepUsec;

#ifdef WLAN_WAKEUP_EVENTS
	/* This config directs the WoW pkt filtering to look for EAP-ID
	 * requests embedded in EAPOL frames and use this as a wake source.
	 */
	uint8_t ucWoWEAPIDRequestEnable;

	/* This config directs the WoW pkt filtering to look for EAPOL-4WAY
	 * requests and use this as a wake source.
	 */
	uint8_t ucWoWEAPOL4WayEnable;

	/* This config allows a host wakeup on an network scan offload match.
	 */
	uint8_t ucWowNetScanOffloadMatch;

	/* This configuration allows a host wakeup on any GTK rekeying error.
	 */
	uint8_t ucWowGTKRekeyError;

	/* This configuration allows a host wakeup on BSS connection loss.
	 */
	uint8_t ucWoWBSSConnLoss;
#endif /* WLAN_WAKEUP_EVENTS */

	/* Status code to be filled by HAL when it sends
	 * SIR_HAL_WOWL_ENTER_RSP to PE.
	 */
	CDF_STATUS status;

	/*BSSID to find the current session
	 */
	uint8_t bssIdx;
} tSirHalWowlEnterParams, *tpSirHalWowlEnterParams;

/* SME->PE: Exit WOWLAN parameters */
typedef struct sSirSmeWowlExitParams {
	uint8_t sessionId;

} tSirSmeWowlExitParams, *tpSirSmeWowlExitParams;

/* PE<->HAL: Exit WOWLAN parameters */
typedef struct sSirHalWowlExitParams {
	uint8_t sessionId;

	/* Status code to be filled by HAL when it sends
	 * SIR_HAL_WOWL_EXIT_RSP to PE.
	 */
	CDF_STATUS status;

	/*BSSIDX to find the current session
	 */
	uint8_t bssIdx;
} tSirHalWowlExitParams, *tpSirHalWowlExitParams;

#define SIR_MAX_NAME_SIZE 64
#define SIR_MAX_TEXT_SIZE 32

typedef struct sSirName {
	uint8_t num_name;
	uint8_t name[SIR_MAX_NAME_SIZE];
} tSirName;

typedef struct sSirText {
	uint8_t num_text;
	uint8_t text[SIR_MAX_TEXT_SIZE];
} tSirText;

#define SIR_WPS_PROBRSP_VER_PRESENT    0x00000001
#define SIR_WPS_PROBRSP_STATE_PRESENT    0x00000002
#define SIR_WPS_PROBRSP_APSETUPLOCK_PRESENT    0x00000004
#define SIR_WPS_PROBRSP_SELECTEDREGISTRA_PRESENT    0x00000008
#define SIR_WPS_PROBRSP_DEVICEPASSWORDID_PRESENT    0x00000010
#define SIR_WPS_PROBRSP_SELECTEDREGISTRACFGMETHOD_PRESENT    0x00000020
#define SIR_WPS_PROBRSP_RESPONSETYPE_PRESENT    0x00000040
#define SIR_WPS_PROBRSP_UUIDE_PRESENT    0x00000080
#define SIR_WPS_PROBRSP_MANUFACTURE_PRESENT    0x00000100
#define SIR_WPS_PROBRSP_MODELNAME_PRESENT    0x00000200
#define SIR_WPS_PROBRSP_MODELNUMBER_PRESENT    0x00000400
#define SIR_WPS_PROBRSP_SERIALNUMBER_PRESENT    0x00000800
#define SIR_WPS_PROBRSP_PRIMARYDEVICETYPE_PRESENT    0x00001000
#define SIR_WPS_PROBRSP_DEVICENAME_PRESENT    0x00002000
#define SIR_WPS_PROBRSP_CONFIGMETHODS_PRESENT    0x00004000
#define SIR_WPS_PROBRSP_RF_BANDS_PRESENT    0x00008000

typedef struct sSirWPSProbeRspIE {
	uint32_t FieldPresent;
	uint32_t Version;       /* Version. 0x10 = version 1.0, 0x11 = etc. */
	uint32_t wpsState;      /* 1 = unconfigured, 2 = configured. */
	bool APSetupLocked;     /* Must be included if value is true */
	/*
	 * BOOL:  indicates if the user has recently activated a Registrar to
	 * add an Enrollee.
	 */
	bool SelectedRegistra;
	uint16_t DevicePasswordID;      /* Device Password ID */
	/* Selected Registrar config method */
	uint16_t SelectedRegistraCfgMethod;
	uint8_t ResponseType;   /* Response type */
	uint8_t UUID_E[16];     /* Unique identifier of the AP. */
	tSirName Manufacture;
	tSirText ModelName;
	tSirText ModelNumber;
	tSirText SerialNumber;
	/* Device Category ID: 1Computer, 2Input Device, ... */
	uint32_t PrimaryDeviceCategory;
	/* Vendor specific OUI for Device Sub Category */
	uint8_t PrimaryDeviceOUI[4];
	/*
	   Device Sub Category ID: 1-PC, 2-Server if Device Category ID
	 * is computer
	 */
	uint32_t DeviceSubCategory;
	tSirText DeviceName;
	uint16_t ConfigMethod;  /* Configuaration method */
	uint8_t RFBand;         /* RF bands available on the AP */
} tSirWPSProbeRspIE;

#define SIR_WPS_BEACON_VER_PRESENT    0x00000001
#define SIR_WPS_BEACON_STATE_PRESENT    0x00000002
#define SIR_WPS_BEACON_APSETUPLOCK_PRESENT    0x00000004
#define SIR_WPS_BEACON_SELECTEDREGISTRA_PRESENT    0x00000008
#define SIR_WPS_BEACON_DEVICEPASSWORDID_PRESENT    0x00000010
#define SIR_WPS_BEACON_SELECTEDREGISTRACFGMETHOD_PRESENT    0x00000020
#define SIR_WPS_BEACON_UUIDE_PRESENT    0x00000080
#define SIR_WPS_BEACON_RF_BANDS_PRESENT    0x00000100
#define SIR_WPS_UUID_LEN 16

typedef struct sSirWPSBeaconIE {
	uint32_t FieldPresent;
	uint32_t Version;       /* Version. 0x10 = version 1.0, 0x11 = etc. */
	uint32_t wpsState;      /* 1 = unconfigured, 2 = configured. */
	bool APSetupLocked;     /* Must be included if value is true */
	/*
	 * BOOL:  indicates if the user has recently activated a Registrar to
	 * add an Enrollee.
	 */
	bool SelectedRegistra;
	uint16_t DevicePasswordID;      /* Device Password ID */
	/* Selected Registrar config method */
	uint16_t SelectedRegistraCfgMethod;
	uint8_t UUID_E[SIR_WPS_UUID_LEN];     /* Unique identifier of the AP. */
	uint8_t RFBand;         /* RF bands available on the AP */
} tSirWPSBeaconIE;

#define SIR_WPS_ASSOCRSP_VER_PRESENT    0x00000001
#define SIR_WPS_ASSOCRSP_RESPONSETYPE_PRESENT    0x00000002

typedef struct sSirWPSAssocRspIE {
	uint32_t FieldPresent;
	uint32_t Version;
	uint8_t ResposeType;
} tSirWPSAssocRspIE;

typedef struct sSirAPWPSIEs {
	tSirWPSProbeRspIE SirWPSProbeRspIE;     /*WPS Set Probe Respose IE */
	tSirWPSBeaconIE SirWPSBeaconIE; /*WPS Set Beacon IE */
	tSirWPSAssocRspIE SirWPSAssocRspIE;     /*WPS Set Assoc Response IE */
} tSirAPWPSIEs, *tpSiriAPWPSIEs;

typedef struct sSirUpdateAPWPSIEsReq {
	uint16_t messageType;   /* eWNI_SME_UPDATE_APWPSIE_REQ */
	uint16_t length;
	uint16_t transactionId; /* Transaction ID for cmd */
	tSirMacAddr bssId;      /* BSSID */
	uint8_t sessionId;      /* Session ID */
	tSirAPWPSIEs APWPSIEs;
} tSirUpdateAPWPSIEsReq, *tpSirUpdateAPWPSIEsReq;

typedef struct sSirUpdateParams {
	uint16_t messageType;
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint8_t ssidHidden;     /* Hide SSID */
} tSirUpdateParams, *tpSirUpdateParams;

/* Beacon Interval */
typedef struct sSirChangeBIParams {
	uint16_t messageType;
	uint16_t length;
	uint16_t beaconInterval;        /* Beacon Interval */
	tSirMacAddr bssId;
	uint8_t sessionId;      /* Session ID */
} tSirChangeBIParams, *tpSirChangeBIParams;

#ifdef QCA_HT_2040_COEX
typedef struct sSirSetHT2040Mode {
	uint16_t messageType;
	uint16_t length;
	uint8_t cbMode;
	bool obssEnabled;
	tSirMacAddr bssId;
	uint8_t sessionId;      /* Session ID */
} tSirSetHT2040Mode, *tpSirSetHT2040Mode;
#endif

#define SIR_WPS_PBC_WALK_TIME   120     /* 120 Second */

typedef struct sSirWPSPBCSession {
	struct sSirWPSPBCSession *next;
	tSirMacAddr addr;
	uint8_t uuid_e[SIR_WPS_UUID_LEN];
	uint32_t timestamp;
} tSirWPSPBCSession;

typedef struct sSirSmeGetWPSPBCSessionsReq {
	uint16_t messageType;   /* eWNI_SME_GET_WPSPBC_SESSION_REQ */
	uint16_t length;
	void *pUsrContext;
	void *pSapEventCallback;
	tSirMacAddr bssId;      /* BSSID */
	/* MAC Address of STA in WPS Session to be removed */
	tSirMacAddr pRemoveMac;
} tSirSmeGetWPSPBCSessionsReq, *tpSirSmeGetWPSPBCSessionsReq;

typedef struct sSirWPSPBCProbeReq {
	tSirMacAddr peerMacAddr;
	uint16_t probeReqIELen;
	uint8_t probeReqIE[512];
} tSirWPSPBCProbeReq, *tpSirWPSPBCProbeReq;

/* probereq from peer, when wsc is enabled */
typedef struct sSirSmeProbeReqInd {
	uint16_t messageType;   /*  eWNI_SME_WPS_PBC_PROBE_REQ_IND */
	uint16_t length;
	uint8_t sessionId;
	tSirMacAddr bssId;
	tSirWPSPBCProbeReq WPSPBCProbeReq;
} tSirSmeProbeReqInd, *tpSirSmeProbeReqInd;

typedef struct sSirUpdateAPWPARSNIEsReq {
	uint16_t messageType;   /* eWNI_SME_SET_APWPARSNIEs_REQ */
	uint16_t length;
	uint16_t transactionId; /* Transaction ID for cmd */
	tSirMacAddr bssId;      /* BSSID */
	uint8_t sessionId;      /* Session ID */
	tSirRSNie APWPARSNIEs;
} tSirUpdateAPWPARSNIEsReq, *tpSirUpdateAPWPARSNIEsReq;

#define SIR_ROAM_MAX_CHANNELS            80
#define SIR_ROAM_SCAN_MAX_PB_REQ_SIZE    450
/* Occupied channel list remains static */
#define CHANNEL_LIST_STATIC                   1
/* Occupied channel list can be learnt after init */
#define CHANNEL_LIST_DYNAMIC_INIT             2
/* Occupied channel list can be learnt after flush */
#define CHANNEL_LIST_DYNAMIC_FLUSH            3
/* Occupied channel list can be learnt after update */
#define CHANNEL_LIST_DYNAMIC_UPDATE           4
#define SIR_ROAM_SCAN_24G_DEFAULT_CH     1
#define SIR_ROAM_SCAN_5G_DEFAULT_CH      36
#define SIR_ROAM_SCAN_RESERVED_BYTES     61

#ifdef WLAN_FEATURE_ROAM_OFFLOAD
#define SIR_ROAM_SCAN_PSK_SIZE    32
#define SIR_ROAM_R0KH_ID_MAX_LEN  48
#endif
/* SME -> HAL - This is the host offload request. */
#define SIR_IPV4_ARP_REPLY_OFFLOAD                  0
#define SIR_IPV6_NEIGHBOR_DISCOVERY_OFFLOAD         1
#define SIR_IPV6_NS_OFFLOAD                         2
#define SIR_OFFLOAD_DISABLE                         0
#define SIR_OFFLOAD_ENABLE                          1
#define SIR_OFFLOAD_BCAST_FILTER_ENABLE             0x2
#define SIR_OFFLOAD_MCAST_FILTER_ENABLE             0x4
#define SIR_OFFLOAD_ARP_AND_BCAST_FILTER_ENABLE     (SIR_OFFLOAD_ENABLE|SIR_OFFLOAD_BCAST_FILTER_ENABLE)
#define SIR_OFFLOAD_NS_AND_MCAST_FILTER_ENABLE      (SIR_OFFLOAD_ENABLE|SIR_OFFLOAD_MCAST_FILTER_ENABLE)

#ifdef WLAN_NS_OFFLOAD
typedef struct sSirNsOffloadReq {
	uint8_t srcIPv6Addr[16];
	uint8_t selfIPv6Addr[SIR_MAC_NUM_TARGET_IPV6_NS_OFFLOAD_NA][SIR_MAC_IPV6_ADDR_LEN];
	uint8_t targetIPv6Addr[SIR_MAC_NUM_TARGET_IPV6_NS_OFFLOAD_NA][SIR_MAC_IPV6_ADDR_LEN];
	tSirMacAddr selfMacAddr;
	uint8_t srcIPv6AddrValid;
	uint8_t targetIPv6AddrValid[SIR_MAC_NUM_TARGET_IPV6_NS_OFFLOAD_NA];
	uint8_t slotIdx;
} tSirNsOffloadReq, *tpSirNsOffloadReq;
#endif /* WLAN_NS_OFFLOAD */

typedef struct sSirHostOffloadReq {
	uint8_t offloadType;
	uint8_t enableOrDisable;
	uint32_t num_ns_offload_count;
	union {
		uint8_t hostIpv4Addr[4];
		uint8_t hostIpv6Addr[16];
	} params;
#ifdef WLAN_NS_OFFLOAD
	tSirNsOffloadReq nsOffloadInfo;
#endif /* WLAN_NS_OFFLOAD */
	tSirMacAddr bssId;
} tSirHostOffloadReq, *tpSirHostOffloadReq;

/* Packet Types. */
#define SIR_KEEP_ALIVE_NULL_PKT              1
#define SIR_KEEP_ALIVE_UNSOLICIT_ARP_RSP     2

/* Keep Alive request. */
typedef struct sSirKeepAliveReq {
	uint8_t packetType;
	uint32_t timePeriod;
	tSirIpv4Addr hostIpv4Addr;
	tSirIpv4Addr destIpv4Addr;
	tSirMacAddr destMacAddr;
	tSirMacAddr bssId;
	uint8_t sessionId;
} tSirKeepAliveReq, *tpSirKeepAliveReq;

typedef struct sSirSmeMgmtFrameInd {
	uint16_t mesgType;
	uint16_t mesgLen;
	uint32_t rxChan;
	uint8_t sessionId;
	uint8_t frameType;
	int8_t rxRssi;
	uint8_t frameBuf[1];    /* variable */
} tSirSmeMgmtFrameInd, *tpSirSmeMgmtFrameInd;

#ifdef WLAN_FEATURE_11W
typedef struct sSirSmeUnprotMgmtFrameInd {
	uint8_t sessionId;
	uint8_t frameType;
	uint8_t frameLen;
	uint8_t frameBuf[1];    /* variable */
} tSirSmeUnprotMgmtFrameInd, *tpSirSmeUnprotMgmtFrameInd;
#endif

#define SIR_IS_FULL_POWER_REASON_DISCONNECTED(eReason) \
	((eSME_LINK_DISCONNECTED_BY_HDD == (eReason)) || \
	 (eSME_LINK_DISCONNECTED_BY_OTHER == (eReason)))
#define SIR_IS_FULL_POWER_NEEDED_BY_HDD(eReason) \
	((eSME_LINK_DISCONNECTED_BY_HDD == (eReason)) || \
	 (eSME_FULL_PWR_NEEDED_BY_HDD == (eReason)))

/* P2P Power Save Related */
typedef struct sSirNoAParam {
	uint8_t ctWindow:7;
	uint8_t OppPS:1;
	uint8_t count;
	uint32_t duration;
	uint32_t interval;
	uint32_t singleNoADuration;
	uint8_t psSelection;
} tSirNoAParam, *tpSirNoAParam;

typedef struct sSirWlanSuspendParam {
	uint8_t configuredMcstBcstFilterSetting;
	uint8_t sessionId;
	uint8_t connectedState;
} tSirWlanSuspendParam, *tpSirWlanSuspendParam;

typedef struct sSirWlanResumeParam {
	uint8_t configuredMcstBcstFilterSetting;
} tSirWlanResumeParam, *tpSirWlanResumeParam;

#ifdef WLAN_FEATURE_EXTWOW_SUPPORT

typedef enum ext_wow_type {
	EXT_WOW_TYPE_APP_TYPE1, /* wow type: only enable wakeup for app type1 */
	EXT_WOW_TYPE_APP_TYPE2, /* wow type: only enable wakeup for app type2 */
	EXT_WOW_TYPE_APP_TYPE1_2,  /* wow type: enable wakeup for app type1&2 */
} EXT_WOW_TYPE;

typedef struct {
	uint8_t vdev_id;
	EXT_WOW_TYPE type;
	uint32_t wakeup_pin_num;
} tSirExtWoWParams, *tpSirExtWoWParams;

typedef struct {
	uint8_t vdev_id;
	tSirMacAddr wakee_mac_addr;
	uint8_t identification_id[8];
	uint8_t password[16];
	uint32_t id_length;
	uint32_t pass_length;
} tSirAppType1Params, *tpSirAppType1Params;

typedef struct {
	uint8_t vdev_id;

	uint8_t rc4_key[16];
	uint32_t rc4_key_len;

	/** ip header parameter */
	uint32_t ip_id;         /* NC id */
	uint32_t ip_device_ip;  /* NC IP address */
	uint32_t ip_server_ip;  /* Push server IP address */

	/** tcp header parameter */
	uint16_t tcp_src_port;  /* NC TCP port */
	uint16_t tcp_dst_port;  /* Push server TCP port */
	uint32_t tcp_seq;
	uint32_t tcp_ack_seq;

	uint32_t keepalive_init;        /* Initial ping interval */
	uint32_t keepalive_min; /* Minimum ping interval */
	uint32_t keepalive_max; /* Maximum ping interval */
	uint32_t keepalive_inc; /* Increment of ping interval */

	tSirMacAddr gateway_mac;
	uint32_t tcp_tx_timeout_val;
	uint32_t tcp_rx_timeout_val;
} tSirAppType2Params, *tpSirAppType2Params;
#endif

typedef struct sSirWlanSetRxpFilters {
	uint8_t configuredMcstBcstFilterSetting;
	uint8_t setMcstBcstFilter;
} tSirWlanSetRxpFilters, *tpSirWlanSetRxpFilters;


#define ANI_MAX_IBSS_ROUTE_TABLE_ENTRY   100

typedef struct sAniDestIpNextHopMacPair {
	uint8_t destIpv4Addr[CDF_IPV4_ADDR_SIZE];
	uint8_t nextHopMacAddr[CDF_MAC_ADDR_SIZE];
} tAniDestIpNextHopMacPair;

typedef struct sAniIbssRouteTable {
	uint8_t sessionId;
	uint16_t numEntries;
	tAniDestIpNextHopMacPair destIpNextHopPair[1];
} tAniIbssRouteTable;

#ifdef FEATURE_WLAN_SCAN_PNO
/* */
/* PNO Messages */
/* */


/* Set PNO */
#define SIR_PNO_MAX_NETW_CHANNELS  26
#define SIR_PNO_MAX_NETW_CHANNELS_EX  60
#define SIR_PNO_MAX_SUPP_NETWORKS  16

/*
 * size based of dot11 declaration without extra IEs as we will not carry those
 * for PNO
 */
#define SIR_PNO_MAX_PB_REQ_SIZE    450

#define SIR_PNO_24G_DEFAULT_CH     1
#define SIR_PNO_5G_DEFAULT_CH      36

typedef enum {
	SIR_PNO_MODE_IMMEDIATE,
	SIR_PNO_MODE_ON_SUSPEND,
	SIR_PNO_MODE_ON_RESUME,
	SIR_PNO_MODE_MAX
} eSirPNOMode;

typedef struct {
	tSirMacSSid ssId;
	uint32_t authentication;
	uint32_t encryption;
	uint32_t bcastNetwType;
	uint8_t ucChannelCount;
	uint8_t aChannels[SIR_PNO_MAX_NETW_CHANNELS_EX];
	int32_t rssiThreshold;
} tSirNetworkType;

/**
 * struct sSirPNOScanReq - PNO Scan request structure
 * @enable: flag to enable or disable
 * @modePNO: PNO Mode
 * @ucNetworksCount: Number of networks
 * @aNetworks: Preferred network list
 * @sessionId: Session identifier
 * @fast_scan_period: Fast Scan period
 * @slow_scan_period: Slow scan period
 * @fast_scan_max_cycles: Fast scan max cycles
 * @us24GProbeTemplateLen: 2.4G probe template length
 * @p24GProbeTemplate: 2.4G probe template
 * @us5GProbeTemplateLen: 5G probe template length
 * @p5GProbeTemplate: 5G probe template
 */
typedef struct sSirPNOScanReq {
	uint8_t enable;
	eSirPNOMode modePNO;
	uint8_t ucNetworksCount;
	tSirNetworkType aNetworks[SIR_PNO_MAX_SUPP_NETWORKS];
	uint8_t sessionId;
	uint32_t fast_scan_period;
	uint32_t slow_scan_period;
	uint8_t fast_scan_max_cycles;

	uint32_t        active_min_time;
	uint32_t        active_max_time;
	uint32_t        passive_min_time;
	uint32_t        passive_max_time;

	uint16_t us24GProbeTemplateLen;
	uint8_t p24GProbeTemplate[SIR_PNO_MAX_PB_REQ_SIZE];
	uint16_t us5GProbeTemplateLen;
	uint8_t p5GProbeTemplate[SIR_PNO_MAX_PB_REQ_SIZE];
#ifdef FEATURE_WLAN_SCAN_PNO
	bool pno_channel_prediction;
	uint8_t top_k_num_of_channels;
	uint8_t stationary_thresh;
	uint32_t channel_prediction_full_scan;
#endif
} tSirPNOScanReq, *tpSirPNOScanReq;

/* Preferred Network Found Indication */
typedef struct {
	uint16_t mesgType;
	uint16_t mesgLen;
	/* Network that was found with the highest RSSI */
	tSirMacSSid ssId;
	/* Indicates the RSSI */
	uint8_t rssi;
	/* Length of the beacon or probe response
	 * corresponding to the candidate found by PNO */
	uint32_t frameLength;
	uint8_t sessionId;
	/* Index to memory location where the contents of
	 * beacon or probe response frame will be copied */
	uint8_t data[1];
} tSirPrefNetworkFoundInd, *tpSirPrefNetworkFoundInd;
#endif /* FEATURE_WLAN_SCAN_PNO */

#ifdef WLAN_FEATURE_ROAM_OFFLOAD
typedef struct {
	uint8_t acvo_uapsd:1;
	uint8_t acvi_uapsd:1;
	uint8_t acbk_uapsd:1;
	uint8_t acbe_uapsd:1;
	uint8_t reserved:4;
} tSirAcUapsd, *tpSirAcUapsd;
#endif

typedef struct {
	tSirMacSSid ssId;
	uint8_t currAPbssid[CDF_MAC_ADDR_SIZE];
	uint32_t authentication;
	uint8_t encryption;
	uint8_t mcencryption;
	uint8_t ChannelCount;
	uint8_t ChannelCache[SIR_ROAM_MAX_CHANNELS];
#ifdef WLAN_FEATURE_11W
	bool mfp_enabled;
#endif

} tSirRoamNetworkType;

typedef struct SirMobilityDomainInfo {
	uint8_t mdiePresent;
	uint16_t mobilityDomain;
} tSirMobilityDomainInfo;

typedef enum {
	SIR_ROAMING_DFS_CHANNEL_DISABLED = 0,
	SIR_ROAMING_DFS_CHANNEL_ENABLED_NORMAL = 1,
	SIR_ROAMING_DFS_CHANNEL_ENABLED_ACTIVE = 2
} eSirDFSRoamScanMode;
#define MAX_SSID_ALLOWED_LIST 4
#define MAX_BSSID_AVOID_LIST  16
#define MAX_BSSID_FAVORED     16
/**
 * struct roam_ext_params - Structure holding roaming parameters
 * @num_bssid_avoid_list:       The number of BSSID's that we should
 *                              avoid connecting to. It is like a
 *                              blacklist of BSSID's.
 * @num_ssid_allowed_list:      The number of SSID profiles that are
 *                              in the Whitelist. When roaming, we
 *                              consider the BSSID's with this SSID
 *                              also for roaming apart from the connected one's
 * @num_bssid_favored:          Number of BSSID's which have a preference over
 *                              others
 * @ssid_allowed_list:          Whitelist SSID's
 * @bssid_avoid_list:           Blacklist SSID's
 * @bssid_favored:              Favorable BSSID's
 * @bssid_favored_factor:       RSSI to be added to this BSSID to prefer it
 * @raise_rssi_thresh_5g:       The RSSI threshold below which the
 *                              raise_factor_5g (boost factor) should be
 *                              applied.
 * @drop_rssi_thresh_5g:        The RSSI threshold beyond which the
 *                              drop_factor_5g (penalty factor) should be
 *                              applied
 * @raise_rssi_type_5g:         Algorithm to apply the boost factor
 * @raise_factor_5g:            Boost factor
 * @drop_rssi_type_5g:          Algorithm to apply the penalty factor
 * @drop_factor_5g:             Penalty factor
 * @max_raise_rssi_5g:          Maximum amount of Boost that can added
 * @max_drop_rssi_5g:           Maximum amount of penalty that can be subtracted
 * @good_rssi_threshold:        The Lookup UP threshold beyond which roaming
 *                              scan should be performed.
 * @rssi_diff:                  RSSI difference for the AP to be better over the
 *                              current AP to avoid ping pong effects
 * @good_rssi_roam:             Lazy Roam
 * @is_5g_pref_enabled:         5GHz BSSID preference feature enable/disable.
 *
 * This structure holds all the key parameters related to
 * initial connection and also roaming connections.
 * */
struct roam_ext_params {
	uint8_t num_bssid_avoid_list;
	uint8_t num_ssid_allowed_list;
	uint8_t num_bssid_favored;
	tSirMacSSid ssid_allowed_list[MAX_SSID_ALLOWED_LIST];
	tSirMacAddr bssid_avoid_list[MAX_BSSID_AVOID_LIST];
	tSirMacAddr bssid_favored[MAX_BSSID_FAVORED];
	uint8_t bssid_favored_factor[MAX_BSSID_FAVORED];
	int raise_rssi_thresh_5g;
	int drop_rssi_thresh_5g;
	uint8_t raise_rssi_type_5g;
	uint8_t raise_factor_5g;
	uint8_t drop_rssi_type_5g;
	uint8_t drop_factor_5g;
	int max_raise_rssi_5g;
	int max_drop_rssi_5g;
	int alert_rssi_threshold;
	int rssi_diff;
	int good_rssi_roam;
	bool is_5g_pref_enabled;
};

typedef struct sSirRoamOffloadScanReq {
	bool RoamScanOffloadEnabled;
	bool MAWCEnabled;
	int8_t LookupThreshold;
	uint8_t delay_before_vdev_stop;
	uint8_t OpportunisticScanThresholdDiff;
	uint8_t RoamRescanRssiDiff;
	uint8_t RoamRssiDiff;
	uint8_t ChannelCacheType;
	uint8_t Command;
	uint8_t reason;
	uint16_t NeighborScanTimerPeriod;
	uint16_t NeighborRoamScanRefreshPeriod;
	uint16_t NeighborScanChannelMinTime;
	uint16_t NeighborScanChannelMaxTime;
	uint16_t EmptyRefreshScanPeriod;
	uint8_t ValidChannelCount;
	uint8_t ValidChannelList[SIR_ROAM_MAX_CHANNELS];
	bool IsESEAssoc;
	uint16_t us24GProbeTemplateLen;
	uint8_t p24GProbeTemplate[SIR_ROAM_SCAN_MAX_PB_REQ_SIZE];
	uint16_t us5GProbeTemplateLen;
	uint8_t p5GProbeTemplate[SIR_ROAM_SCAN_MAX_PB_REQ_SIZE];
	uint8_t ReservedBytes[SIR_ROAM_SCAN_RESERVED_BYTES];
	/*ReservedBytes is to add any further params in future
	   without changing the interface params on Host
	   and firmware.The firmware right now checks
	   if the size of this structure matches and then
	   proceeds with the processing of the command.
	   So, in future, if there is any need to add
	   more params, pick the memory from reserved
	   bytes and keep deducting the reserved bytes
	   by the amount of bytes picked. */
	uint8_t nProbes;
	uint16_t HomeAwayTime;
	tSirRoamNetworkType ConnectedNetwork;
	tSirMobilityDomainInfo MDID;
	uint8_t sessionId;
	uint8_t RoamBmissFirstBcnt;
	uint8_t RoamBmissFinalBcnt;
	uint8_t RoamBeaconRssiWeight;
	eSirDFSRoamScanMode allowDFSChannelRoam;
#ifdef WLAN_FEATURE_ROAM_OFFLOAD
	uint8_t RoamOffloadEnabled;
	uint8_t PSK_PMK[SIR_ROAM_SCAN_PSK_SIZE];
	uint32_t pmk_len;
	uint8_t Prefer5GHz;
	uint8_t RoamRssiCatGap;
	uint8_t Select5GHzMargin;
	uint8_t KRK[SIR_KRK_KEY_LEN];
	uint8_t BTK[SIR_BTK_KEY_LEN];
	uint32_t ReassocFailureTimeout;
	tSirAcUapsd AcUapsd;
	uint8_t R0KH_ID[SIR_ROAM_R0KH_ID_MAX_LEN];
	uint32_t R0KH_ID_Length;
	uint8_t RoamKeyMgmtOffloadEnabled;
#endif
	struct roam_ext_params roam_params;
	uint8_t  middle_of_roaming;
	uint32_t hi_rssi_scan_max_count;
	uint32_t hi_rssi_scan_rssi_delta;
	uint32_t hi_rssi_scan_delay;
	int32_t hi_rssi_scan_rssi_ub;
	uint8_t early_stop_scan_enable;
	int8_t early_stop_scan_min_threshold;
	int8_t early_stop_scan_max_threshold;
} tSirRoamOffloadScanReq, *tpSirRoamOffloadScanReq;

typedef struct sSirRoamOffloadScanRsp {
	uint8_t sessionId;
	uint32_t reason;
} tSirRoamOffloadScanRsp, *tpSirRoamOffloadScanRsp;


#ifdef WLAN_FEATURE_PACKET_FILTERING
/*---------------------------------------------------------------------------
   Packet Filtering Parameters
   ---------------------------------------------------------------------------*/
#define    SIR_IPV4_ADDR_LEN                 4
#define    SIR_MAC_ADDR_LEN                  6
#define    SIR_MAX_FILTER_TEST_DATA_LEN       8
#define    SIR_MAX_NUM_MULTICAST_ADDRESS    240
#define    SIR_MAX_NUM_FILTERS               20
#define    SIR_MAX_NUM_TESTS_PER_FILTER      10

/* */
/* Receive Filter Parameters */
/* */
typedef enum {
	SIR_RCV_FILTER_TYPE_INVALID,
	SIR_RCV_FILTER_TYPE_FILTER_PKT,
	SIR_RCV_FILTER_TYPE_BUFFER_PKT,
	SIR_RCV_FILTER_TYPE_MAX_ENUM_SIZE
} eSirReceivePacketFilterType;

typedef enum {
	SIR_FILTER_HDR_TYPE_INVALID,
	SIR_FILTER_HDR_TYPE_MAC,
	SIR_FILTER_HDR_TYPE_ARP,
	SIR_FILTER_HDR_TYPE_IPV4,
	SIR_FILTER_HDR_TYPE_IPV6,
	SIR_FILTER_HDR_TYPE_UDP,
	SIR_FILTER_HDR_TYPE_MAX
} eSirRcvPktFltProtocolType;

typedef enum {
	SIR_FILTER_CMP_TYPE_INVALID,
	SIR_FILTER_CMP_TYPE_EQUAL,
	SIR_FILTER_CMP_TYPE_MASK_EQUAL,
	SIR_FILTER_CMP_TYPE_NOT_EQUAL,
	SIR_FILTER_CMP_TYPE_MASK_NOT_EQUAL,
	SIR_FILTER_CMP_TYPE_MAX
} eSirRcvPktFltCmpFlagType;

typedef struct sSirRcvPktFilterFieldParams {
	eSirRcvPktFltProtocolType protocolLayer;
	eSirRcvPktFltCmpFlagType cmpFlag;
	/* Length of the data to compare */
	uint16_t dataLength;
	/* from start of the respective frame header */
	uint8_t dataOffset;
	/* Reserved field */
	uint8_t reserved;
	/* Data to compare */
	uint8_t compareData[SIR_MAX_FILTER_TEST_DATA_LEN];
	/* Mask to be applied on the received packet data before compare */
	uint8_t dataMask[SIR_MAX_FILTER_TEST_DATA_LEN];
} tSirRcvPktFilterFieldParams, *tpSirRcvPktFilterFieldParams;

typedef struct sSirRcvPktFilterCfg {
	uint8_t filterId;
	eSirReceivePacketFilterType filterType;
	uint32_t numFieldParams;
	uint32_t coalesceTime;
	struct cdf_mac_addr self_macaddr;
	struct cdf_mac_addr bssid;      /* Bssid of the connected AP */
	tSirRcvPktFilterFieldParams paramsData[SIR_MAX_NUM_TESTS_PER_FILTER];
} tSirRcvPktFilterCfgType, *tpSirRcvPktFilterCfgType;

/* */
/* Filter Packet Match Count Parameters */
/* */
typedef struct sSirRcvFltPktMatchCnt {
	uint8_t filterId;
	uint32_t matchCnt;
} tSirRcvFltPktMatchCnt, tpSirRcvFltPktMatchCnt;

typedef struct sSirRcvFltPktMatchRsp {
	uint16_t mesgType;
	uint16_t mesgLen;

	/* Success or Failure */
	uint32_t status;
	tSirRcvFltPktMatchCnt filterMatchCnt[SIR_MAX_NUM_FILTERS];
	struct cdf_mac_addr bssid;
} tSirRcvFltPktMatchRsp, *tpSirRcvFltPktMatchRsp;

/* */
/* Receive Filter Clear Parameters */
/* */
typedef struct sSirRcvFltPktClearParam {
	uint32_t status;        /* only valid for response message */
	uint8_t filterId;
	struct cdf_mac_addr self_macaddr;
	struct cdf_mac_addr bssid;
} tSirRcvFltPktClearParam, *tpSirRcvFltPktClearParam;

/* */
/* Multicast Address List Parameters */
/* */
typedef struct sSirRcvFltMcAddrList {
	uint32_t ulMulticastAddrCnt;
	struct cdf_mac_addr multicastAddr[SIR_MAX_NUM_MULTICAST_ADDRESS];
	struct cdf_mac_addr self_macaddr;
	struct cdf_mac_addr bssid;
	uint8_t action;
} tSirRcvFltMcAddrList, *tpSirRcvFltMcAddrList;
#endif /* WLAN_FEATURE_PACKET_FILTERING */

/* */
/* Generic version information */
/* */
typedef struct {
	uint8_t revision;
	uint8_t version;
	uint8_t minor;
	uint8_t major;
} tSirVersionType;

#ifdef WLAN_FEATURE_GTK_OFFLOAD
/*---------------------------------------------------------------------------
* WMA_GTK_OFFLOAD_REQ
*--------------------------------------------------------------------------*/
typedef struct {
	uint32_t ulFlags;       /* optional flags */
	uint8_t aKCK[16];       /* Key confirmation key */
	uint8_t aKEK[16];       /* key encryption key */
	uint64_t ullKeyReplayCounter;   /* replay counter */
	struct cdf_mac_addr bssid;
} tSirGtkOffloadParams, *tpSirGtkOffloadParams;

/*---------------------------------------------------------------------------
* WMA_GTK_OFFLOAD_GETINFO_REQ
*--------------------------------------------------------------------------*/
typedef struct {
	uint16_t mesgType;
	uint16_t mesgLen;

	uint32_t ulStatus;      /* success or failure */
	uint64_t ullKeyReplayCounter;   /* current replay counter value */
	uint32_t ulTotalRekeyCount;     /* total rekey attempts */
	uint32_t ulGTKRekeyCount;       /* successful GTK rekeys */
	uint32_t ulIGTKRekeyCount;      /* successful iGTK rekeys */
	struct cdf_mac_addr bssid;
} tSirGtkOffloadGetInfoRspParams, *tpSirGtkOffloadGetInfoRspParams;
#endif /* WLAN_FEATURE_GTK_OFFLOAD */

/**
 * struct sir_wifi_start_log - Structure to store the params sent to start/
 * stop logging
 * @name:          Attribute which indicates the type of logging like per packet
 *                 statistics, connectivity etc.
 * @verbose_level: Verbose level which can be 0,1,2,3
 * @flag:          Flag field for future use
 */
struct sir_wifi_start_log {
	uint32_t ring_id;
	uint32_t verbose_level;
	uint32_t flag;
};

/**
 * enum hw_mode_ss_config - Possible spatial stream configuration
 * @SS_0x0: Unused Tx and Rx of MAC
 * @SS_1x1: 1 Tx SS and 1 Rx SS
 * @SS_2x2: 2 Tx SS and 2 Rx SS
 * @SS_3x3: 3 Tx SS and 3 Rx SS
 * @SS_4x4: 4 Tx SS and 4 Rx SS
 *
 * Note: Right now only 1x1 and 2x2 are being supported. Other modes should
 * be added when supported. Asymmetric configuration like 1x2, 2x1 are also
 * not supported now. But, they are still valid. Right now, Tx/Rx SS support is
 * 4 bits long. So, we can go upto 15x15
 */
enum hw_mode_ss_config {
	HW_MODE_SS_0x0,
	HW_MODE_SS_1x1,
	HW_MODE_SS_2x2,
	HW_MODE_SS_3x3,
	HW_MODE_SS_4x4,
};

/**
 * enum hw_mode_bandwidth - bandwidth of wifi channel.
 *
 * @HW_MODE_5_MHZ: 5 Mhz bandwidth
 * @HW_MODE_10_MHZ: 10 Mhz bandwidth
 * @HW_MODE_20_MHZ: 20 Mhz bandwidth
 * @HW_MODE_40_MHZ: 40 Mhz bandwidth
 * @HW_MODE_80_MHZ: 80 Mhz bandwidth
 * @HW_MODE_80_PLUS_80_MHZ: 80 Mhz plus 80 Mhz bandwidth
 * @HW_MODE_160_MHZ: 160 Mhz bandwidth
 * @HW_MODE_MAX_BANDWIDTH: Max place holder
 *
 * These are generic IDs that identify the various roles
 * in the software system
 */
enum hw_mode_bandwidth {
	HW_MODE_BW_NONE,
	HW_MODE_5_MHZ,
	HW_MODE_10_MHZ,
	HW_MODE_20_MHZ,
	HW_MODE_40_MHZ,
	HW_MODE_80_MHZ,
	HW_MODE_80_PLUS_80_MHZ,
	HW_MODE_160_MHZ,
	HW_MODE_MAX_BANDWIDTH
};

/**
 * enum hw_mode_dbs_capab - DBS HW mode capability
 * @HW_MODE_DBS_NONE: Non DBS capable
 * @HW_MODE_DBS: DFS capable
 */
enum hw_mode_dbs_capab {
	HW_MODE_DBS_NONE,
	HW_MODE_DBS,
};

/**
 * enum hw_mode_agile_dfs_capab - Agile DFS HW mode capability
 * @HW_MODE_AGILE_DFS_NONE: Non Agile DFS capable
 * @HW_MODE_AGILE_DFS: Agile DFS capable
 */
enum hw_mode_agile_dfs_capab {
	HW_MODE_AGILE_DFS_NONE,
	HW_MODE_AGILE_DFS,
};

/**
 * enum set_hw_mode_status - Status of set HW mode command
 * @SET_HW_MODE_STATUS_OK: command successful
 * @SET_HW_MODE_STATUS_EINVAL: Requested invalid hw_mode
 * @SET_HW_MODE_STATUS_ECANCELED: HW mode change cancelled
 * @SET_HW_MODE_STATUS_ENOTSUP: HW mode not supported
 * @SET_HW_MODE_STATUS_EHARDWARE: HW mode change prevented by hardware
 * @SET_HW_MODE_STATUS_EPENDING: HW mode change is pending
 * @SET_HW_MODE_STATUS_ECOEX: HW mode change conflict with Coex
 */
enum set_hw_mode_status {
	SET_HW_MODE_STATUS_OK,
	SET_HW_MODE_STATUS_EINVAL,
	SET_HW_MODE_STATUS_ECANCELED,
	SET_HW_MODE_STATUS_ENOTSUP,
	SET_HW_MODE_STATUS_EHARDWARE,
	SET_HW_MODE_STATUS_EPENDING,
	SET_HW_MODE_STATUS_ECOEX,
};

/**
 * struct sir_pcl_list - Format of PCL
 * @pcl_list: List of preferred channels
 * @pcl_len: Number of channels in the PCL
 */
struct sir_pcl_list {
	uint8_t pcl_list[128];
	uint32_t pcl_len;
};

/**
 * struct sir_hw_mode_params - HW mode params
 * @mac0_tx_ss: MAC0 Tx spatial stream
 * @mac0_rx_ss: MAC0 Rx spatial stream
 * @mac1_tx_ss: MAC1 Tx spatial stream
 * @mac1_rx_ss: MAC1 Rx spatial stream
 * @mac0_bw: MAC0 bandwidth
 * @mac1_bw: MAC1 bandwidth
 * @dbs_cap: DBS capabality
 * @agile_dfs_cap: Agile DFS capabality
 */
struct sir_hw_mode_params {
	uint8_t mac0_tx_ss;
	uint8_t mac0_rx_ss;
	uint8_t mac1_tx_ss;
	uint8_t mac1_rx_ss;
	uint8_t mac0_bw;
	uint8_t mac1_bw;
	uint8_t dbs_cap;
	uint8_t agile_dfs_cap;
};

/**
 * struct sir_vdev_mac_map - vdev id-mac id map
 * @vdev_id: VDEV id
 * @mac_id: MAC id
 */
struct sir_vdev_mac_map {
	uint32_t vdev_id;
	uint32_t mac_id;
};

/**
 * struct sir_set_hw_mode_resp - HW mode response
 * @status: Status
 * @cfgd_hw_mode_index: Configured HW mode index
 * @num_vdev_mac_entries: Number of vdev-mac id entries
 * @vdev_mac_map: vdev id-mac id map
 */
struct sir_set_hw_mode_resp {
	uint32_t status;
	uint32_t cfgd_hw_mode_index;
	uint32_t num_vdev_mac_entries;
	struct sir_vdev_mac_map vdev_mac_map[MAX_VDEV_SUPPORTED];
};

/**
 * struct sir_hw_mode_trans_ind - HW mode transition indication
 * @old_hw_mode_index: Index of old HW mode
 * @new_hw_mode_index: Index of new HW mode
 * @num_vdev_mac_entries: Number of vdev-mac id entries
 * @vdev_mac_map: vdev id-mac id map
 */
struct sir_hw_mode_trans_ind {
	uint32_t old_hw_mode_index;
	uint32_t new_hw_mode_index;
	uint32_t num_vdev_mac_entries;
	struct sir_vdev_mac_map vdev_mac_map[MAX_VDEV_SUPPORTED];
};

/**
 * struct sir_dual_mac_config_resp - Dual MAC config response
 * @status: Status of setting the dual mac configuration
 */
struct sir_dual_mac_config_resp {
	uint32_t status;
};

#ifdef WLAN_WAKEUP_EVENTS
/*---------------------------------------------------------------------------
   tSirWakeReasonInd
   ---------------------------------------------------------------------------*/
typedef struct {
	uint16_t mesgType;
	uint16_t mesgLen;
	uint32_t ulReason;      /* see tWakeReasonType */
	uint32_t ulReasonArg;   /* argument specific to the reason type */
	/* length of optional data stored in this message, in case
	 * HAL truncates the data (i.e. data packets) this length
	 * will be less than the actual length
	 */
	uint32_t ulStoredDataLen;
	uint32_t ulActualDataLen;       /* actual length of data */
	/* variable length start of data (length == storedDataLen)
	 * see specific wake type
	 */
	uint8_t aDataStart[1];
} tSirWakeReasonInd, *tpSirWakeReasonInd;
#endif /* WLAN_WAKEUP_EVENTS */

/*---------------------------------------------------------------------------
   sAniSetTmLevelReq
   ---------------------------------------------------------------------------*/
typedef struct sAniSetTmLevelReq {
	uint16_t tmMode;
	uint16_t newTmLevel;
} tAniSetTmLevelReq, *tpAniSetTmLevelReq;

#ifdef FEATURE_WLAN_TDLS
/* TDLS Request struct SME-->PE */
typedef struct sSirTdlsSendMgmtReq {
	uint16_t messageType;   /* eWNI_SME_TDLS_DISCOVERY_START_REQ */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId; /* Transaction ID for cmd */
	uint8_t reqType;
	uint8_t dialog;
	uint16_t statusCode;
	uint8_t responder;
	uint32_t peerCapability;
	/* For multi-session, for PE to locate peSession ID */
	tSirMacAddr bssid;
	tSirMacAddr peerMac;
	/* Variable lenght. Dont add any field after this. */
	uint8_t addIe[1];
} tSirTdlsSendMgmtReq, *tpSirSmeTdlsSendMgmtReq;

typedef enum TdlsAddOper {
	TDLS_OPER_NONE,
	TDLS_OPER_ADD,
	TDLS_OPER_UPDATE
} eTdlsAddOper;

/* TDLS Request struct SME-->PE */
typedef struct sSirTdlsAddStaReq {
	uint16_t messageType;   /* eWNI_SME_TDLS_DISCOVERY_START_REQ */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId; /* Transaction ID for cmd */
	/* For multi-session, for PE to locate peSession ID */
	struct cdf_mac_addr bssid;
	eTdlsAddOper tdlsAddOper;
	struct cdf_mac_addr peermac;
	uint16_t capability;
	uint8_t extn_capability[SIR_MAC_MAX_EXTN_CAP];
	uint8_t supported_rates_length;
	uint8_t supported_rates[SIR_MAC_MAX_SUPP_RATES];
	uint8_t htcap_present;
	tSirHTCap htCap;
	uint8_t vhtcap_present;
	tSirVHTCap vhtCap;
	uint8_t uapsd_queues;
	uint8_t max_sp;
} tSirTdlsAddStaReq, *tpSirSmeTdlsAddStaReq;

/* TDLS Response struct PE-->SME */
typedef struct sSirTdlsAddStaRsp {
	uint16_t messageType;
	uint16_t length;
	tSirResultCodes statusCode;
	struct cdf_mac_addr peermac;
	uint8_t sessionId;      /* Session ID */
	uint16_t staId;
	uint16_t staType;
	uint8_t ucastSig;
	uint8_t bcastSig;
	eTdlsAddOper tdlsAddOper;
} tSirTdlsAddStaRsp;

/* TDLS Request struct SME-->PE */
typedef struct {
	uint16_t messageType;   /* eWNI_SME_TDLS_LINK_ESTABLISH_REQ */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId; /* Transaction ID for cmd */
	uint8_t uapsdQueues;    /* Peer's uapsd Queues Information */
	uint8_t maxSp;          /* Peer's Supported Maximum Service Period */
	uint8_t isBufSta;       /* Does Peer Support as Buffer Station. */
	/* Does Peer Support as TDLS Off Channel. */
	uint8_t isOffChannelSupported;
	uint8_t isResponder;    /* Is Peer a responder. */
	/* For multi-session, for PE to locate peSession ID */
	struct cdf_mac_addr bssid;
	struct cdf_mac_addr peermac;
	uint8_t supportedChannelsLen;
	uint8_t supportedChannels[SIR_MAC_MAX_SUPP_CHANNELS];
	uint8_t supportedOperClassesLen;
	uint8_t supportedOperClasses[SIR_MAC_MAX_SUPP_OPER_CLASSES];
} tSirTdlsLinkEstablishReq, *tpSirTdlsLinkEstablishReq;

/* TDLS Request struct SME-->PE */
typedef struct {
	uint16_t messageType;   /* eWNI_SME_TDLS_LINK_ESTABLISH_RSP */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId; /* Transaction ID for cmd */
	tSirResultCodes statusCode;
	struct cdf_mac_addr peermac;
} tSirTdlsLinkEstablishReqRsp, *tpSirTdlsLinkEstablishReqRsp;

/* TDLS Request struct SME-->PE */
typedef struct sSirTdlsDelStaReq {
	uint16_t messageType;   /* eWNI_SME_TDLS_DISCOVERY_START_REQ */
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint16_t transactionId; /* Transaction ID for cmd */
	/* For multi-session, for PE to locate peSession ID */
	struct cdf_mac_addr bssid;
	struct cdf_mac_addr peermac;
} tSirTdlsDelStaReq, *tpSirSmeTdlsDelStaReq;
/* TDLS Response struct PE-->SME */
typedef struct sSirTdlsDelStaRsp {
	uint16_t messageType;
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	tSirResultCodes statusCode;
	struct cdf_mac_addr peermac;
	uint16_t staId;
} tSirTdlsDelStaRsp, *tpSirTdlsDelStaRsp;
/* TDLS Delete Indication struct PE-->SME */
typedef struct sSirTdlsDelStaInd {
	uint16_t messageType;
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	struct cdf_mac_addr peermac;
	uint16_t staId;
	uint16_t reasonCode;
} tSirTdlsDelStaInd, *tpSirTdlsDelStaInd;
typedef struct sSirTdlsDelAllPeerInd {
	uint16_t messageType;
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
} tSirTdlsDelAllPeerInd, *tpSirTdlsDelAllPeerInd;
typedef struct sSirMgmtTxCompletionInd {
	uint16_t messageType;
	uint16_t length;
	uint8_t sessionId;      /* Session ID */
	uint32_t txCompleteStatus;
} tSirMgmtTxCompletionInd, *tpSirMgmtTxCompletionInd;

typedef struct sSirTdlsEventnotify {
	uint8_t sessionId;
	struct cdf_mac_addr peermac;
	uint16_t messageType;
	uint32_t peer_reason;
} tSirTdlsEventnotify;
#endif /* FEATURE_WLAN_TDLS */

typedef struct sSirActiveModeSetBcnFilterReq {
	uint16_t messageType;
	uint16_t length;
	uint8_t seesionId;
} tSirSetActiveModeSetBncFilterReq, *tpSirSetActiveModeSetBncFilterReq;

/* Reset AP Caps Changed */
typedef struct sSirResetAPCapsChange {
	uint16_t messageType;
	uint16_t length;
	struct cdf_mac_addr bssId;
} tSirResetAPCapsChange, *tpSirResetAPCapsChange;

/* / Definition for Candidate found indication from FW */
typedef struct sSirSmeCandidateFoundInd {
	uint16_t messageType;   /* eWNI_SME_CANDIDATE_FOUND_IND */
	uint16_t length;
	uint8_t sessionId;      /* Session Identifier */
} tSirSmeCandidateFoundInd, *tpSirSmeCandidateFoundInd;

#ifdef WLAN_FEATURE_11W
typedef struct sSirWlanExcludeUnencryptParam {
	bool excludeUnencrypt;
	struct cdf_mac_addr bssid;
} tSirWlanExcludeUnencryptParam, *tpSirWlanExcludeUnencryptParam;
#endif

typedef enum {
	P2P_SCAN_TYPE_SEARCH = 1,       /* P2P Search */
	P2P_SCAN_TYPE_LISTEN    /* P2P Listen */
} tSirP2pScanType;

typedef struct sAniHandoffReq {
	/* Common for all types are requests */
	uint16_t msgType;       /* message type is same as the request type */
	uint16_t msgLen;        /* length of the entire request */
	uint8_t sessionId;
	uint8_t bssid[CDF_MAC_ADDR_SIZE];
	uint8_t channel;
	uint8_t handoff_src;
} tAniHandoffReq, *tpAniHandoffReq;

typedef struct sSirScanOffloadReq {
	uint8_t sessionId;
	struct cdf_mac_addr bssId;
	uint8_t numSsid;
	tSirMacSSid ssId[SIR_SCAN_MAX_NUM_SSID];
	uint8_t hiddenSsid;
	struct cdf_mac_addr selfMacAddr;
	tSirBssType bssType;
	uint8_t dot11mode;
	tSirScanType scanType;
	uint32_t minChannelTime;
	uint32_t maxChannelTime;
	uint32_t scan_id;
	/* in units of milliseconds, ignored when not connected */
	uint32_t restTime;
	tSirP2pScanType p2pScanType;
	uint16_t uIEFieldLen;
	uint16_t uIEFieldOffset;
	tSirChannelList channelList;
	/*-----------------------------
	  sSirScanOffloadReq....
	  -----------------------------
	  uIEFieldLen
	  -----------------------------
	  uIEFieldOffset               ----+
	  -----------------------------    |
	  channelList.numChannels          |
	  -----------------------------    |
	  ... variable size up to          |
	  channelNumber[numChannels-1]     |
	  This can be zero, if             |
	  numChannel is zero.              |
	  ----------------------------- <--+
	  ... variable size uIEField
	  up to uIEFieldLen (can be 0)
	  -----------------------------*/
} tSirScanOffloadReq, *tpSirScanOffloadReq;

typedef enum sSirScanEventType {
	SCAN_EVENT_STARTED = 0x1,       /* Scan command accepted by FW */
	SCAN_EVENT_COMPLETED = 0x2,     /* Scan has been completed by FW */
	SCAN_EVENT_BSS_CHANNEL = 0x4,   /* FW is going to move to HOME ch */
	SCAN_EVENT_FOREIGN_CHANNEL = 0x8,   /* FW going to move to FORIEGN ch */
	SCAN_EVENT_DEQUEUED = 0x10,     /* scan request got dequeued */
	SCAN_EVENT_PREEMPTED = 0x20,    /* preempted by high priority scan */
	SCAN_EVENT_START_FAILED = 0x40, /* scan start failed */
	SCAN_EVENT_RESTARTED = 0x80,    /*scan restarted */
	SCAN_EVENT_MAX = 0x8000
} tSirScanEventType;

typedef struct sSirScanOffloadEvent {
	tSirScanEventType event;
	tSirResultCodes reasonCode;
	uint32_t chanFreq;
	uint32_t requestor;
	uint32_t scanId;
	tSirP2pScanType p2pScanType;
	uint8_t sessionId;
} tSirScanOffloadEvent, *tpSirScanOffloadEvent;

/**
 * struct sSirUpdateChanParam - channel parameters
 * @chanId: ID of the channel
 * @pwr: power level
 * @dfsSet: is dfs supported or not
 * @half_rate: is the channel operating at 10MHz
 * @quarter_rate: is the channel operating at 5MHz
 */
typedef struct sSirUpdateChanParam {
	uint8_t chanId;
	uint8_t pwr;
	bool dfsSet;
	bool half_rate;
	bool quarter_rate;
} tSirUpdateChanParam, *tpSirUpdateChanParam;

typedef struct sSirUpdateChan {
	uint8_t numChan;
	tSirUpdateChanParam chanParam[1];
} tSirUpdateChanList, *tpSirUpdateChanList;

typedef enum eSirAddonPsReq {
	eSIR_ADDON_NOTHING,
	eSIR_ADDON_ENABLE_UAPSD,
	eSIR_ADDON_DISABLE_UAPSD
} tSirAddonPsReq;

#ifdef FEATURE_WLAN_LPHB
#define SIR_LPHB_FILTER_LEN   64

typedef enum {
	LPHB_SET_EN_PARAMS_INDID,
	LPHB_SET_TCP_PARAMS_INDID,
	LPHB_SET_TCP_PKT_FILTER_INDID,
	LPHB_SET_UDP_PARAMS_INDID,
	LPHB_SET_UDP_PKT_FILTER_INDID,
	LPHB_SET_NETWORK_INFO_INDID,
} LPHBIndType;

typedef struct sSirLPHBEnableStruct {
	uint8_t enable;
	uint8_t item;
	uint8_t session;
} tSirLPHBEnableStruct;

typedef struct sSirLPHBTcpParamStruct {
	uint32_t srv_ip;
	uint32_t dev_ip;
	uint16_t src_port;
	uint16_t dst_port;
	uint16_t timeout;
	uint8_t session;
	struct cdf_mac_addr gateway_mac;
	uint16_t timePeriodSec; /* in seconds */
	uint32_t tcpSn;
} tSirLPHBTcpParamStruct;

typedef struct sSirLPHBTcpFilterStruct {
	uint16_t length;
	uint8_t offset;
	uint8_t session;
	uint8_t filter[SIR_LPHB_FILTER_LEN];
} tSirLPHBTcpFilterStruct;

typedef struct sSirLPHBUdpParamStruct {
	uint32_t srv_ip;
	uint32_t dev_ip;
	uint16_t src_port;
	uint16_t dst_port;
	uint16_t interval;
	uint16_t timeout;
	uint8_t session;
	struct cdf_mac_addr gateway_mac;
} tSirLPHBUdpParamStruct;

typedef struct sSirLPHBUdpFilterStruct {
	uint16_t length;
	uint8_t offset;
	uint8_t session;
	uint8_t filter[SIR_LPHB_FILTER_LEN];
} tSirLPHBUdpFilterStruct;

typedef struct sSirLPHBReq {
	uint16_t cmd;
	uint16_t dummy;
	union {
		tSirLPHBEnableStruct lphbEnableReq;
		tSirLPHBTcpParamStruct lphbTcpParamReq;
		tSirLPHBTcpFilterStruct lphbTcpFilterReq;
		tSirLPHBUdpParamStruct lphbUdpParamReq;
		tSirLPHBUdpFilterStruct lphbUdpFilterReq;
	} params;
} tSirLPHBReq;

typedef struct sSirLPHBInd {
	uint8_t sessionIdx;
	uint8_t protocolType;   /*TCP or UDP */
	uint8_t eventReason;
} tSirLPHBInd;
#endif /* FEATURE_WLAN_LPHB */

#ifdef FEATURE_WLAN_CH_AVOID
typedef struct sSirChAvoidUpdateReq {
	uint32_t reserved_param;
} tSirChAvoidUpdateReq;
#endif /* FEATURE_WLAN_CH_AVOID */

typedef struct sSirLinkSpeedInfo {
	/* MAC Address for the peer */
	struct cdf_mac_addr peer_macaddr;
	uint32_t estLinkSpeed;  /* Linkspeed from firmware */
} tSirLinkSpeedInfo, *tpSirLinkSpeedInfo;

typedef struct sSirAddPeriodicTxPtrn {
	/* MAC Address for the adapter */
	struct cdf_mac_addr mac_address;
	uint8_t ucPtrnId;       /* Pattern ID */
	uint16_t ucPtrnSize;    /* Pattern size */
	uint32_t usPtrnIntervalMs;      /* In msec */
	uint8_t ucPattern[PERIODIC_TX_PTRN_MAX_SIZE];   /* Pattern buffer */
} tSirAddPeriodicTxPtrn, *tpSirAddPeriodicTxPtrn;

typedef struct sSirDelPeriodicTxPtrn {
	/* MAC Address for the adapter */
	struct cdf_mac_addr mac_address;
	/* Bitmap of pattern IDs that need to be deleted */
	uint32_t ucPatternIdBitmap;
	uint8_t ucPtrnId;       /* Pattern ID */
} tSirDelPeriodicTxPtrn, *tpSirDelPeriodicTxPtrn;

typedef struct {
	uint16_t mesgType;
	uint16_t mesgLen;
	bool suspended;
} tSirReadyToSuspendInd, *tpSirReadyToSuspendInd;
#ifdef WLAN_FEATURE_EXTWOW_SUPPORT
typedef struct {
	uint16_t mesgType;
	uint16_t mesgLen;
	bool status;
} tSirReadyToExtWoWInd, *tpSirReadyToExtWoWInd;
#endif
typedef struct sSirRateUpdateInd {
	uint8_t nss;            /* 0: 1x1, 1: 2x2 */
	struct cdf_mac_addr bssid;
	tCDF_CON_MODE dev_mode;
	int32_t bcastDataRate;  /* bcast rate unit Mbpsx10, -1:not used */
	/*
	 * 0 implies RA, positive value implies fixed rate, -1 implies ignore
	 * this param.
	 */
	int32_t ucastDataRate;

	/* TX flag to differentiate between HT20, HT40 etc */
	tTxrateinfoflags ucastDataRateTxFlag;

	/*
	 * 0 implies MCAST RA, positive value implies fixed rate,
	 * -1 implies ignore this param
	 */
	int32_t reliableMcastDataRate;  /* unit Mbpsx10 */

	/* TX flag to differentiate between HT20, HT40 etc */
	tTxrateinfoflags reliableMcastDataRateTxFlag;

	/*
	 * MCAST(or BCAST) fixed data rate in 2.4 GHz, unit Mbpsx10,
	 * 0 implies ignore
	 */
	uint32_t mcastDataRate24GHz;

	/* TX flag to differentiate between HT20, HT40 etc */
	tTxrateinfoflags mcastDataRate24GHzTxFlag;

	/*
	 * MCAST(or BCAST) fixed data rate in 5 GHz,
	 * unit Mbpsx10, 0 implies ignore
	 */
	uint32_t mcastDataRate5GHz;

	/* TX flag to differentiate between HT20, HT40 etc */
	tTxrateinfoflags mcastDataRate5GHzTxFlag;

} tSirRateUpdateInd, *tpSirRateUpdateInd;

#if defined(FEATURE_WLAN_CH_AVOID) || defined(FEATURE_WLAN_FORCE_SAP_SCC)
#define SIR_CH_AVOID_MAX_RANGE   4

typedef struct sSirChAvoidFreqType {
	uint32_t start_freq;
	uint32_t end_freq;
} tSirChAvoidFreqType;

typedef struct sSirChAvoidIndType {
	uint32_t avoid_range_count;
	tSirChAvoidFreqType avoid_freq_range[SIR_CH_AVOID_MAX_RANGE];
} tSirChAvoidIndType;
#endif /* FEATURE_WLAN_CH_AVOID || FEATURE_WLAN_FORCE_SAP_SCC */

#define SIR_DFS_MAX_20M_SUB_CH 8

typedef struct sSirSmeDfsChannelList {
	uint32_t nchannels;
	/* Ch num including bonded channels on which the RADAR is present */
	uint8_t channels[SIR_DFS_MAX_20M_SUB_CH];
} tSirSmeDfsChannelList, *tpSirSmeDfsChannelList;

typedef struct sSirSmeDfsEventInd {
	uint32_t sessionId;
	tSirSmeDfsChannelList chan_list;
	uint32_t dfs_radar_status;
	int use_nol;
} tSirSmeDfsEventInd, *tpSirSmeDfsEventInd;

typedef struct sSirChanChangeRequest {
	uint16_t messageType;
	uint16_t messageLen;
	uint8_t targetChannel;
	uint8_t cbMode;
	uint8_t channel_width;
	uint8_t center_freq_seg_0;
	uint8_t center_freq_seg_1;
	uint8_t bssid[CDF_MAC_ADDR_SIZE];
	uint32_t dot11mode;
	tSirMacRateSet operational_rateset;
	tSirMacRateSet extended_rateset;
} tSirChanChangeRequest, *tpSirChanChangeRequest;

typedef struct sSirChanChangeResponse {
	uint8_t sessionId;
	uint8_t newChannelNumber;
	uint8_t channelChangeStatus;
	ePhyChanBondState secondaryChannelOffset;
} tSirChanChangeResponse, *tpSirChanChangeResponse;

typedef struct sSirStartBeaconIndication {
	uint16_t messageType;
	uint16_t messageLen;
	uint8_t beaconStartStatus;
	uint8_t bssid[CDF_MAC_ADDR_SIZE];
} tSirStartBeaconIndication, *tpSirStartBeaconIndication;

/* additional IE type */
typedef enum tUpdateIEsType {
	eUPDATE_IE_NONE,
	eUPDATE_IE_PROBE_BCN,
	eUPDATE_IE_PROBE_RESP,
	eUPDATE_IE_ASSOC_RESP,

	/* Add type above this line */
	/* this is used to reset all buffer */
	eUPDATE_IE_ALL,
	eUPDATE_IE_MAX
} eUpdateIEsType;

/* Modify particular IE in addition IE for prob resp Bcn */
typedef struct sSirModifyIE {
	struct cdf_mac_addr bssid;
	uint16_t smeSessionId;
	bool notify;
	uint8_t ieID;
	uint8_t ieIDLen;        /*ie length as per spec */
	uint16_t ieBufferlength;
	uint8_t *pIEBuffer;

} tSirModifyIE, *tpSirModifyIE;

/* Message format for Update IE message sent to PE  */
typedef struct sSirModifyIEsInd {
	uint16_t msgType;
	uint16_t msgLen;
	tSirModifyIE modifyIE;
	eUpdateIEsType updateType;
} tSirModifyIEsInd, *tpSirModifyIEsInd;

/* Message format for Update IE message sent to PE  */
typedef struct sSirUpdateIE {
	struct cdf_mac_addr bssid;
	uint16_t smeSessionId;
	bool append;
	bool notify;
	uint16_t ieBufferlength;
	uint8_t *pAdditionIEBuffer;
} tSirUpdateIE, *tpSirUpdateIE;

/* Message format for Update IE message sent to PE  */
typedef struct sSirUpdateIEsInd {
	uint16_t msgType;
	uint16_t msgLen;
	tSirUpdateIE updateIE;
	eUpdateIEsType updateType;
} tSirUpdateIEsInd, *tpSirUpdateIEsInd;

/* Message format for requesting channel switch announcement to lower layers */
typedef struct sSirDfsCsaIeRequest {
	uint16_t msgType;
	uint16_t msgLen;
	uint8_t targetChannel;
	uint8_t csaIeRequired;
	uint8_t bssid[CDF_MAC_ADDR_SIZE];
	uint8_t ch_bandwidth;
} tSirDfsCsaIeRequest, *tpSirDfsCsaIeRequest;

/* Indication from lower layer indicating the completion of first beacon send
 * after the beacon template update
 */
typedef struct sSirFirstBeaconTxCompleteInd {
	uint16_t messageType;   /* eWNI_SME_MISSED_BEACON_IND */
	uint16_t length;
	uint8_t bssIdx;
} tSirFirstBeaconTxCompleteInd, *tpSirFirstBeaconTxCompleteInd;

typedef struct sSirSmeCSAIeTxCompleteRsp {
	uint8_t sessionId;
	uint8_t chanSwIeTxStatus;
} tSirSmeCSAIeTxCompleteRsp, *tpSirSmeCSAIeTxCompleteRsp;

/* Thermal Mitigation*/

typedef struct {
	uint16_t minTempThreshold;
	uint16_t maxTempThreshold;
} t_thermal_level_info, *tp_thermal_level_info;

typedef enum {
	WLAN_WMA_THERMAL_LEVEL_0,
	WLAN_WMA_THERMAL_LEVEL_1,
	WLAN_WMA_THERMAL_LEVEL_2,
	WLAN_WMA_THERMAL_LEVEL_3,
	WLAN_WMA_MAX_THERMAL_LEVELS
} t_thermal_level;

typedef struct {
	/* Array of thermal levels */
	t_thermal_level_info thermalLevels[WLAN_WMA_MAX_THERMAL_LEVELS];
	uint8_t thermalCurrLevel;
	uint8_t thermalMgmtEnabled;
	uint32_t throttlePeriod;
} t_thermal_mgmt, *tp_thermal_mgmt;

typedef struct sSirTxPowerLimit {
	/* Thermal limits for 2g and 5g */
	uint32_t txPower2g;
	uint32_t txPower5g;
} tSirTxPowerLimit;

/* notify MODEM power state to FW */
typedef struct {
	uint32_t param;
} tSirModemPowerStateInd, *tpSirModemPowerStateInd;

#ifdef WLAN_FEATURE_STATS_EXT
typedef struct {
	uint32_t vdev_id;
	uint32_t event_data_len;
	uint8_t event_data[];
} tSirStatsExtEvent, *tpSirStatsExtEvent;
#endif

#ifdef WLAN_FEATURE_NAN
typedef struct {
	uint32_t event_data_len;
	uint8_t event_data[];
} tSirNanEvent, *tpSirNanEvent;
#endif
#ifdef WLAN_FEATURE_ROAM_OFFLOAD
typedef struct sSirSmeRoamOffloadSynchInd {
	uint16_t messageType;   /*eWNI_SME_ROAM_OFFLOAD_SYNCH_IND */
	uint16_t length;
	uint16_t beaconProbeRespOffset;
	uint16_t beaconProbeRespLength;
	uint16_t reassocRespOffset;
	uint16_t reassocRespLength;
	uint8_t isBeacon;
	uint8_t roamedVdevId;
	tSirMacAddr bssId;
	int8_t txMgmtPower;
	uint32_t authStatus;
	uint8_t rssi;
	uint8_t roamReason;
	uint32_t chan_freq;
	uint8_t kck[SIR_KCK_KEY_LEN];
	uint8_t kek[SIR_KEK_KEY_LEN];
	uint8_t replay_ctr[SIR_REPLAY_CTR_LEN];
	tpSirBssDescription  bss_desc_ptr;
} roam_offload_synch_ind;

typedef struct sSirSmeRoamOffloadSynchCnf {
	uint8_t sessionId;
} tSirSmeRoamOffloadSynchCnf, *tpSirSmeRoamOffloadSynchCnf;

typedef struct sSirSmeHOFailureInd {
	uint8_t sessionId;
} tSirSmeHOFailureInd, *tpSirSmeHOFailureInd;

struct roam_offload_synch_fail {
	uint8_t session_id;
};

#endif

#ifdef FEATURE_WLAN_EXTSCAN

/**
 * typedef enum wifi_scan_flags - wifi scan flags
 * @WIFI_SCAN_FLAG_INTERRUPTED: Indicates that scan results are not complete
 *				because probes were not sent on some channels
 */
typedef enum {
	WIFI_SCAN_FLAG_INTERRUPTED = 1,
} wifi_scan_flags;

typedef enum {
	WIFI_BAND_UNSPECIFIED,
	WIFI_BAND_BG = 1,       /* 2.4 GHz */
	WIFI_BAND_A = 2,        /* 5 GHz without DFS */
	WIFI_BAND_ABG = 3,      /* 2.4 GHz + 5 GHz; no DFS */
	WIFI_BAND_A_DFS_ONLY = 4,       /* 5 GHz DFS only */
	/* 5 is reserved */
	WIFI_BAND_A_WITH_DFS = 6,       /* 5 GHz with DFS */
	WIFI_BAND_ABG_WITH_DFS = 7,     /* 2.4 GHz + 5 GHz with DFS */

	/* Keep it last */
	WIFI_BAND_MAX
} tWifiBand;

/* wifi scan related events */
typedef enum {
	WIFI_SCAN_BUFFER_FULL,
	WIFI_SCAN_COMPLETE,
} tWifiScanEventType;

/**
 * enum extscan_configuration_flags - extscan config flags
 * @EXTSCAN_LP_EXTENDED_BATCHING: extended batching
 */
enum extscan_configuration_flags {
	EXTSCAN_LP_EXTENDED_BATCHING = 0x00000001,
};

typedef struct {
	struct cdf_mac_addr bssid;

	/* Low threshold */
	int32_t low;

	/* High threshold */
	int32_t high;
} tSirAPThresholdParam, *tpSirAPThresholdParam;

typedef struct {
	uint32_t requestId;
	uint8_t sessionId;
} tSirGetExtScanCapabilitiesReqParams, *tpSirGetExtScanCapabilitiesReqParams;

/**
 * struct ext_scan_capabilities_response - extscan capabilities response data
 * @requestId: request identifier
 * @status:    status
 * @max_scan_cache_size: total space allocated for scan (in bytes)
 * @max_scan_buckets: maximum number of channel buckets
 * @max_ap_cache_per_scan: maximum number of APs that can be stored per scan
 * @max_rssi_sample_size: number of RSSI samples used for averaging RSSI
 * @ax_scan_reporting_threshold: max possible report_threshold
 * @max_hotlist_bssids: maximum number of entries for hotlist APs
 * @max_significant_wifi_change_aps: maximum number of entries for
 *				significant wifi change APs
 * @max_bssid_history_entries: number of BSSID/RSSI entries that device can hold
 * @max_hotlist_ssids: maximum number of entries for hotlist SSIDs
 * @max_number_epno_networks: max number of epno entries
 * @max_number_epno_networks_by_ssid: max number of epno entries
 *			if ssid is specified, that is, epno entries for
 *			which an exact match is required,
 *			or entries corresponding to hidden ssids
 * @max_number_of_white_listed_ssid: max number of white listed SSIDs
 */
struct ext_scan_capabilities_response {
	uint32_t requestId;
	uint32_t status;

	uint32_t max_scan_cache_size;
	uint32_t max_scan_buckets;
	uint32_t max_ap_cache_per_scan;
	uint32_t max_rssi_sample_size;
	uint32_t max_scan_reporting_threshold;

	uint32_t max_hotlist_bssids;
	uint32_t max_significant_wifi_change_aps;

	uint32_t max_bssid_history_entries;
	uint32_t max_hotlist_ssids;
	uint32_t max_number_epno_networks;
	uint32_t max_number_epno_networks_by_ssid;
	uint32_t max_number_of_white_listed_ssid;
};

typedef struct {
	uint32_t requestId;
	uint8_t sessionId;

	/*
	 * 1 - return cached results and flush it
	 * 0 - return cached results and do not flush
	 */
	bool flush;
} tSirExtScanGetCachedResultsReqParams, *tpSirExtScanGetCachedResultsReqParams;

typedef struct {
	uint32_t requestId;
	uint32_t status;
} tSirExtScanGetCachedResultsRspParams, *tpSirExtScanGetCachedResultsRspParams;

typedef struct {
	/* Time of discovery */
	uint64_t ts;

	/* Null terminated SSID */
	uint8_t ssid[SIR_MAC_MAX_SSID_LENGTH + 1];

	struct cdf_mac_addr bssid;

	/* Frequency in MHz */
	uint32_t channel;

	/* RSSI in dBm */
	int32_t rssi;

	/* RTT in nanoseconds */
	uint32_t rtt;

	/* Standard deviation in rtt */
	uint32_t rtt_sd;

	/* Period advertised in the beacon */
	uint16_t beaconPeriod;

	/* Capabilities advertised in the beacon */
	uint16_t capability;

	uint16_t ieLength;

	uint8_t ieData[];
} tSirWifiScanResult, *tpSirWifiScanResult;

/**
 * struct extscan_hotlist_match - extscan hotlist match
 * @requestId: request identifier
 * @numOfAps: number of bssids retrieved by the scan
 * @moreData: 0 - for last fragment
 *	      1 - still more fragment(s) coming
 * @ap: wifi scan result
 */
struct extscan_hotlist_match {
	uint32_t    requestId;
	bool        moreData;
	bool        ap_found;
	uint32_t    numOfAps;
	tSirWifiScanResult   ap[];
};

/**
 * struct extscan_cached_scan_result - extscan cached scan result
 * @scan_id: a unique identifier for the scan unit
 * @flags: a bitmask with additional information about scan
 * @num_results: number of bssids retrieved by the scan
 * @ap: wifi scan bssid results info
 */
struct extscan_cached_scan_result {
	uint32_t    scan_id;
	uint32_t    flags;
	uint32_t    num_results;
	tSirWifiScanResult *ap;
};

/**
 * struct tSirWifiScanResultEvent - wifi scan result event
 * @requestId: request identifier
 * @ap_found: flag to indicate ap found or not
 *		true: AP was found
 *		false: AP was lost
 * @numOfAps: number of aps
 * @moreData: more data
 * @ap: bssid information
 *
 */
typedef struct {
	uint32_t     requestId;
	bool         ap_found;
	uint32_t     numOfAps;
	bool         moreData;
	tSirWifiScanResult   ap[];
} tSirWifiScanResultEvent, *tpSirWifiScanResultEvent;

/**
 * struct extscan_cached_scan_results - extscan cached scan results
 * @request_id: request identifier
 * @more_data: 0 - for last fragment
 *	       1 - still more fragment(s) coming
 * @num_scan_ids: number of scan ids
 * @result: wifi scan result
 */
struct extscan_cached_scan_results {
	uint32_t    request_id;
	bool        more_data;
	uint32_t    num_scan_ids;
	struct extscan_cached_scan_result  *result;
};


/**
 * struct tSirWifiFullScanResultEvent - extscan full scan event
 * @request_id: request identifier
 * @moreData: 0 - for last fragment
 *             1 - still more fragment(s) coming
 * @ap: bssid info
 *
 * Reported when each probe response is received, if reportEvents
 * enabled in tSirWifiScanCmdReqParams
 */
typedef struct {
	uint32_t            requestId;
	bool                moreData;
	tSirWifiScanResult  ap;
} tSirWifiFullScanResultEvent, *tpSirWifiFullScanResultEvent;

/**
 * struct pno_match_found - epno match found
 * @request_id: request identifier
 * @moreData: 0 - for last fragment
     * 1 - still more fragment(s) coming
 * @num_results: number of bssids, driver sends this event to upper layer
 *		 for every beacon, hence %num_results is always set to 1.
 * @ap: bssid info
 *
 * Reported when each beacon probe response is received with
 * epno match found tag.
     */
struct pno_match_found {
	uint32_t            request_id;
	bool                more_data;
	uint32_t            num_results;
	tSirWifiScanResult  ap[];
};

typedef struct {
	/* Frequency in MHz */
	uint32_t channel;

	uint32_t dwellTimeMs;

	/* 0 => active
	   1 => passive scan; ignored for DFS */
	bool passive;

	uint8_t chnlClass;
} tSirWifiScanChannelSpec, *tpSirWifiScanChannelSpec;

/**
 * struct tSirWifiScanBucketSpec - wifi scan bucket spec
 * @bucket: bucket identifier
 * @band: wifi band
 * @period: Desired period, in millisecond; if this is too
 *		low, the firmware should choose to generate results as fast as
 *		it can instead of failing the command byte
 *		for exponential backoff bucket this is the min_period
 * @reportEvents: 0 => normal reporting (reporting rssi history
 *		only, when rssi history buffer is % full)
 *		1 => same as 0 + report a scan completion event after scanning
 *		this bucket
 *		2 => same as 1 + forward scan results
 *		(beacons/probe responses + IEs) in real time to HAL
 * @max_period: if max_period is non zero or different than period,
 *		then this bucket is an exponential backoff bucket and
 *		the scan period will grow exponentially as per formula:
 *		actual_period(N) = period ^ (N/(step_count+1)) to a
 *		maximum period of max_period
 * @exponent: for exponential back off bucket: multiplier:
 *		new_period = old_period * exponent
 * @step_count: for exponential back off bucket, number of scans performed
 *		at a given period and until the exponent is applied
 * @numChannels: channels to scan; these may include DFS channels
 *		Note that a given channel may appear in multiple buckets
 * @min_dwell_time_active: per bucket minimum active dwell time
 * @max_dwell_time_active: per bucket maximum active dwell time
 * @min_dwell_time_passive: per bucket minimum passive dwell time
 * @max_dwell_time_passive: per bucket maximum passive dwell time
 * @channels: Channel list
 */
typedef struct {
	uint8_t         bucket;
	tWifiBand       band;
	uint32_t        period;
	uint32_t        reportEvents;
	uint32_t        max_period;
	uint32_t        exponent;
	uint32_t        step_count;
	uint32_t        numChannels;

	uint32_t        min_dwell_time_active;
	uint32_t        max_dwell_time_active;
	uint32_t        min_dwell_time_passive;
	uint32_t        max_dwell_time_passive;
	tSirWifiScanChannelSpec channels[WLAN_EXTSCAN_MAX_CHANNELS];
} tSirWifiScanBucketSpec, *tpSirWifiScanBucketSpec;

/**
 * struct tSirWifiScanCmdReqParams - wifi scan command request params
 * @basePeriod: base timer period
 * @maxAPperScan: max ap per scan
 * @report_threshold_percent: report threshold
 *	in %, when buffer is this much full, wake up host
 * @report_threshold_num_scans: report threshold number of scans
 *	in number of scans, wake up host after these many scans
 * @requestId: request id
 * @sessionId: session id
 * @numBuckets: number of buckets
 * @min_dwell_time_active: per bucket minimum active dwell time
 * @max_dwell_time_active: per bucket maximum active dwell time
 * @min_dwell_time_passive: per bucket minimum passive dwell time
 * @max_dwell_time_passive: per bucket maximum passive dwell time
 * @configuration_flags: configuration flags
 * @buckets: buckets array
 */
typedef struct {
	uint32_t basePeriod;
	uint32_t maxAPperScan;

	uint32_t report_threshold_percent;
	uint32_t report_threshold_num_scans;

	uint32_t requestId;
	uint8_t  sessionId;
	uint32_t numBuckets;

	uint32_t min_dwell_time_active;
	uint32_t max_dwell_time_active;
	uint32_t min_dwell_time_passive;
	uint32_t max_dwell_time_passive;
	uint32_t configuration_flags;
	tSirWifiScanBucketSpec buckets[WLAN_EXTSCAN_MAX_BUCKETS];
} tSirWifiScanCmdReqParams, *tpSirWifiScanCmdReqParams;

/**
 * struct sir_extscan_generic_response -
 *	Generic ExtScan Response structure
 * @request_id: ID of the request
 * @status: operation status returned by firmware
 */
struct sir_extscan_generic_response {
	uint32_t request_id;
	uint32_t status;
};

typedef struct {
	uint32_t requestId;
	uint8_t sessionId;
} tSirExtScanStopReqParams, *tpSirExtScanStopReqParams;

/**
 * struct tSirExtScanSetBssidHotListReqParams - set hotlist request
 * @requestId: request identifier
 * @sessionId: session identifier
 * @lost_ap_sample_size: number of samples to confirm AP loss
 * @numAp: Number of hotlist APs
 * @ap: hotlist APs
 */
typedef struct {
	uint32_t  requestId;
	uint8_t   sessionId;

	uint32_t  lost_ap_sample_size;
	uint32_t  numAp;
	tSirAPThresholdParam ap[WLAN_EXTSCAN_MAX_HOTLIST_APS];
} tSirExtScanSetBssidHotListReqParams, *tpSirExtScanSetBssidHotListReqParams;

typedef struct {
	uint32_t requestId;
	uint8_t sessionId;
} tSirExtScanResetBssidHotlistReqParams,
*tpSirExtScanResetBssidHotlistReqParams;

/**
 * struct sir_ssid_hotlist_param - param for SSID Hotlist
 * @ssid: SSID which is being hotlisted
 * @band: Band in which the given SSID should be scanned
 * @rssi_low: Low bound on RSSI
 * @rssi_high: High bound on RSSI
 */
struct sir_ssid_hotlist_param {
	tSirMacSSid ssid;
	uint8_t band;
	int32_t rssi_low;
	int32_t rssi_high;
};

/**
 * struct sir_set_ssid_hotlist_request - set SSID hotlist request struct
 * @request_id: ID of the request
 * @session_id: ID of the session
 * @lost_ssid_sample_size: Number of consecutive scans in which the SSID
 *	must not be seen in order to consider the SSID "lost"
 * @ssid_count: Number of valid entries in the @ssids array
 * @ssids: Array that defines the SSIDs that are in the hotlist
 */
struct sir_set_ssid_hotlist_request {
	uint32_t request_id;
	uint8_t session_id;
	uint32_t lost_ssid_sample_size;
	uint32_t ssid_count;
	struct sir_ssid_hotlist_param ssids[WLAN_EXTSCAN_MAX_HOTLIST_SSIDS];
};

typedef struct {
	uint32_t requestId;
	uint8_t sessionId;

	/* Number of samples for averaging RSSI */
	uint32_t rssiSampleSize;

	/* Number of missed samples to confirm AP loss */
	uint32_t lostApSampleSize;

	/* Number of APs breaching threshold required for firmware
	 * to generate event
	 */
	uint32_t minBreaching;

	uint32_t numAp;
	tSirAPThresholdParam ap[WLAN_EXTSCAN_MAX_SIGNIFICANT_CHANGE_APS];
} tSirExtScanSetSigChangeReqParams, *tpSirExtScanSetSigChangeReqParams;

typedef struct {
	struct cdf_mac_addr bssid;
	uint32_t channel;
	uint32_t numOfRssi;

	/* Rssi history in db */
	int32_t rssi[];
} tSirWifiSignificantChange, *tpSirWifiSignificantChange;

typedef struct {
	uint32_t requestId;

	bool moreData;
	uint32_t numResults;
	tSirWifiSignificantChange ap[];
} tSirWifiSignificantChangeEvent, *tpSirWifiSignificantChangeEvent;

typedef struct {
	uint32_t requestId;
	uint8_t sessionId;
} tSirExtScanResetSignificantChangeReqParams,
*tpSirExtScanResetSignificantChangeReqParams;

typedef struct {
	uint32_t requestId;
	uint32_t numResultsAvailable;
} tSirExtScanResultsAvailableIndParams, *tpSirExtScanResultsAvailableIndParams;

typedef struct {
	uint32_t requestId;
	uint32_t status;
	uint8_t scanEventType;
} tSirExtScanOnScanEventIndParams, *tpSirExtScanOnScanEventIndParams;

/**
 * struct wifi_epno_network - enhanced pno network block
 * @ssid: ssid
 * @rssi_threshold: threshold for considering this SSID as found, required
 *		    granularity for this threshold is 4dBm to 8dBm
 * @flags: WIFI_PNO_FLAG_XXX
 * @auth_bit_field: auth bit field for matching WPA IE
 */
struct wifi_epno_network {
	tSirMacSSid  ssid;
	int8_t       rssi_threshold;
	uint8_t      flags;
	uint8_t      auth_bit_field;
};

/**
 * struct wifi_epno_params - enhanced pno network params
 * @num_networks: number of ssids
 * @networks: PNO networks
 */
struct wifi_epno_params {
	uint32_t    request_id;
	uint32_t    session_id;
	uint32_t    num_networks;
	struct wifi_epno_network networks[];
};

#define SIR_PASSPOINT_REALM_LEN 256
#define SIR_PASSPOINT_ROAMING_CONSORTIUM_ID_NUM 16
#define SIR_PASSPOINT_PLMN_LEN 3
/**
 * struct wifi_passpoint_network - passpoint network block
 * @id: identifier of this network block
 * @realm: null terminated UTF8 encoded realm, 0 if unspecified
 * @roaming_consortium_ids: roaming consortium ids to match, 0s if unspecified
 * @plmn: mcc/mnc combination as per rules, 0s if unspecified
 */
struct wifi_passpoint_network {
	uint32_t id;
	uint8_t  realm[SIR_PASSPOINT_REALM_LEN];
	int64_t  roaming_consortium_ids[SIR_PASSPOINT_ROAMING_CONSORTIUM_ID_NUM];
	uint8_t  plmn[SIR_PASSPOINT_PLMN_LEN];
};

/**
 * struct wifi_passpoint_req - passpoint request
 * @request_id: request identifier
 * @num_networks: number of networks
 * @networks: passpoint networks
 */
struct wifi_passpoint_req {
	uint32_t request_id;
	uint32_t session_id;
	uint32_t num_networks;
	struct wifi_passpoint_network networks[];
};

/**
 * struct wifi_passpoint_match - wifi passpoint network match
 * @id: network block identifier for the matched network
 * @anqp_len: length of ANQP blob
 * @ap: scan result, with channel and beacon information
 * @anqp: ANQP data, in the information_element format
 */
struct wifi_passpoint_match {
	uint32_t  request_id;
	uint32_t  id;
	uint32_t  anqp_len;
	tSirWifiScanResult ap;
	uint8_t   anqp[];
};
#endif /* FEATURE_WLAN_EXTSCAN */

#ifdef FEATURE_WLAN_AUTO_SHUTDOWN
typedef struct {
	uint32_t timer_val;
} tSirAutoShutdownCmdParams;

typedef struct {
	uint32_t shutdown_reason;
} tSirAutoShutdownEvtParams;
#endif

#ifdef WLAN_FEATURE_LINK_LAYER_STATS

typedef struct {
	uint32_t reqId;
	uint8_t staId;
	uint32_t mpduSizeThreshold;
	uint32_t aggressiveStatisticsGathering;
} tSirLLStatsSetReq, *tpSirLLStatsSetReq;

typedef struct {
	uint32_t reqId;
	uint8_t staId;
	uint32_t paramIdMask;
} tSirLLStatsGetReq, *tpSirLLStatsGetReq;

typedef struct {
	uint32_t reqId;
	uint8_t staId;
	uint32_t statsClearReqMask;
	uint8_t stopReq;
} tSirLLStatsClearReq, *tpSirLLStatsClearReq;

typedef struct {
	uint8_t oui[WIFI_SCANNING_MAC_OUI_LENGTH];
} tSirScanMacOui, *tpSirScanMacOui;

enum {
	SIR_AP_RX_DATA_OFFLOAD             = 0x00,
	SIR_STA_RX_DATA_OFFLOAD            = 0x01,
};

struct sir_ipa_offload_enable_disable {
	uint32_t offload_type;
	uint32_t vdev_id;
	uint32_t enable;
};

/*---------------------------------------------------------------------------
   WLAN_HAL_LL_NOTIFY_STATS
   ---------------------------------------------------------------------------*/

/******************************LINK LAYER Statistics**********************/

typedef int tSirWifiRadio;
typedef int tSirWifiChannel;
typedef int tSirwifiTxRate;

/* channel operating width */
typedef enum {
	WIFI_CHAN_WIDTH_20 = 0,
	WIFI_CHAN_WIDTH_40 = 1,
	WIFI_CHAN_WIDTH_80 = 2,
	WIFI_CHAN_WIDTH_160 = 3,
	WIFI_CHAN_WIDTH_80P80 = 4,
	WIFI_CHAN_WIDTH_5 = 5,
	WIFI_CHAN_WIDTH_10 = 6,
} tSirWifiChannelWidth;

typedef enum {
	WIFI_DISCONNECTED = 0,
	WIFI_AUTHENTICATING = 1,
	WIFI_ASSOCIATING = 2,
	WIFI_ASSOCIATED = 3,
	WIFI_EAPOL_STARTED = 4, /* if done by firmware/driver */
	WIFI_EAPOL_COMPLETED = 5,       /* if done by firmware/driver */
} tSirWifiConnectionState;

typedef enum {
	WIFI_ROAMING_IDLE = 0,
	WIFI_ROAMING_ACTIVE = 1,
} tSirWifiRoamState;

typedef enum {
	WIFI_INTERFACE_STA = 0,
	WIFI_INTERFACE_SOFTAP = 1,
	WIFI_INTERFACE_IBSS = 2,
	WIFI_INTERFACE_P2P_CLIENT = 3,
	WIFI_INTERFACE_P2P_GO = 4,
	WIFI_INTERFACE_NAN = 5,
	WIFI_INTERFACE_MESH = 6,
} tSirWifiInterfaceMode;

/* set for QOS association */
#define WIFI_CAPABILITY_QOS          0x00000001
/* set for protected assoc (802.11 beacon frame control protected bit set) */
#define WIFI_CAPABILITY_PROTECTED    0x00000002
/* set if 802.11 Extended Capabilities element interworking bit is set */
#define WIFI_CAPABILITY_INTERWORKING 0x00000004
/* set for HS20 association */
#define WIFI_CAPABILITY_HS20         0x00000008
/* set is 802.11 Extended Capabilities element UTF-8 SSID bit is set */
#define WIFI_CAPABILITY_SSID_UTF8    0x00000010
/* set is 802.11 Country Element is present */
#define WIFI_CAPABILITY_COUNTRY      0x00000020

typedef struct {
	/* tSirWifiInterfaceMode */
	/* interface mode */
	uint8_t mode;
	/* interface mac address (self) */
	struct cdf_mac_addr macAddr;
	/* tSirWifiConnectionState */
	/* connection state (valid for STA, CLI only) */
	uint8_t state;
	/* tSirWifiRoamState */
	/* roaming state */
	uint32_t roaming;
	/* WIFI_CAPABILITY_XXX (self) */
	uint32_t capabilities;
	/* null terminated SSID */
	uint8_t ssid[33];
	/* bssid */
	struct cdf_mac_addr bssid;
	/* country string advertised by AP */
	uint8_t apCountryStr[WNI_CFG_COUNTRY_CODE_LEN];
	/* country string for this association */
	uint8_t countryStr[WNI_CFG_COUNTRY_CODE_LEN];
} tSirWifiInterfaceInfo, *tpSirWifiInterfaceInfo;

/* channel information */
typedef struct {
	/* channel width (20, 40, 80, 80+80, 160) */
	tSirWifiChannelWidth width;
	/* primary 20 MHz channel */
	tSirWifiChannel centerFreq;
	/* center frequency (MHz) first segment */
	tSirWifiChannel centerFreq0;
	/* center frequency (MHz) second segment */
	tSirWifiChannel centerFreq1;
} tSirWifiChannelInfo, *tpSirWifiChannelInfo;

/* wifi rate info */
typedef struct {
	/* 0: OFDM, 1:CCK, 2:HT 3:VHT 4..7 reserved */
	uint32_t preamble:3;
	/* 0:1x1, 1:2x2, 3:3x3, 4:4x4 */
	uint32_t nss:2;
	/* 0:20MHz, 1:40Mhz, 2:80Mhz, 3:160Mhz */
	uint32_t bw:3;
	/* OFDM/CCK rate code would be as per ieee std in units of 0.5mbps */
	/* HT/VHT it would be mcs index */
	uint32_t rateMcsIdx:8;
	/* reserved */
	uint32_t reserved:16;
	/* units of 100 Kbps */
	uint32_t bitrate;
} tSirWifiRate, *tpSirWifiRate;

/* channel statistics */
typedef struct {
	/* channel */
	tSirWifiChannelInfo channel;
	/* msecs the radio is awake (32 bits number accruing over time) */
	uint32_t onTime;
	/* msecs the CCA register is busy (32 bits number accruing over time) */
	uint32_t ccaBusyTime;
} tSirWifiChannelStats, *tpSirWifiChannelStats;

/* radio statistics */
typedef struct {
	/* wifi radio (if multiple radio supported) */
	tSirWifiRadio radio;
	/* msecs the radio is awake (32 bits number accruing over time) */
	uint32_t onTime;
	/* msecs the radio is transmitting
	 * (32 bits number accruing over time)
	 */
	uint32_t txTime;
	/* msecs the radio is in active receive
	   *(32 bits number accruing over time)
	 */
	uint32_t rxTime;
	/* msecs the radio is awake due to all scan
	 * (32 bits number accruing over time)
	 */
	uint32_t onTimeScan;
	/* msecs the radio is awake due to NAN
	 * (32 bits number accruing over time)
	 */
	uint32_t onTimeNbd;
	/* msecs the radio is awake due to Gscan
	 * (32 bits number accruing over time)
	 */
	uint32_t onTimeGscan;
	/* msecs the radio is awake due to roam?scan
	 * (32 bits number accruing over time)
	 */
	uint32_t onTimeRoamScan;
	/* msecs the radio is awake due to PNO scan
	 * (32 bits number accruing over time)
	 */
	uint32_t onTimePnoScan;
	/* msecs the radio is awake due to HS2.0 scans and GAS exchange
	 * (32 bits number accruing over time)
	 */
	uint32_t onTimeHs20;
	/* number of channels */
	uint32_t numChannels;
	/* channel statistics tSirWifiChannelStats */
	tSirWifiChannelStats channels[0];
} tSirWifiRadioStat, *tpSirWifiRadioStat;

/* per rate statistics */
typedef struct {
	/* rate information */
	tSirWifiRate rate;
	/* number of successfully transmitted data pkts (ACK rcvd) */
	uint32_t txMpdu;
	/* number of received data pkts */
	uint32_t rxMpdu;
	/* number of data packet losses (no ACK) */
	uint32_t mpduLost;
	/* total number of data pkt retries * */
	uint32_t retries;
	/* number of short data pkt retries */
	uint32_t retriesShort;
	/* number of long data pkt retries */
	uint32_t retriesLong;
} tSirWifiRateStat, *tpSirWifiRateStat;

/* access categories */
typedef enum {
	WIFI_AC_VO = 0,
	WIFI_AC_VI = 1,
	WIFI_AC_BE = 2,
	WIFI_AC_BK = 3,
	WIFI_AC_MAX = 4,
} tSirWifiTrafficAc;

/* wifi peer type */
typedef enum {
	WIFI_PEER_STA,
	WIFI_PEER_AP,
	WIFI_PEER_P2P_GO,
	WIFI_PEER_P2P_CLIENT,
	WIFI_PEER_NAN,
	WIFI_PEER_TDLS,
	WIFI_PEER_INVALID,
} tSirWifiPeerType;

/* per peer statistics */
typedef struct {
	/* peer type (AP, TDLS, GO etc.) */
	tSirWifiPeerType type;
	/* mac address */
	struct cdf_mac_addr peerMacAddress;
	/* peer WIFI_CAPABILITY_XXX */
	uint32_t capabilities;
	/* number of rates */
	uint32_t numRate;
	/* per rate statistics, number of entries  = num_rate */
	tSirWifiRateStat rateStats[0];
} tSirWifiPeerInfo, *tpSirWifiPeerInfo;

/* per access category statistics */
typedef struct {
	/* tSirWifiTrafficAc */
	/* access category (VI, VO, BE, BK) */
	uint32_t ac;
	/* number of successfully transmitted unicast data pkts (ACK rcvd) */
	uint32_t txMpdu;
	/* number of received unicast mpdus */
	uint32_t rxMpdu;
	/* number of succesfully transmitted multicast data packets */
	/* STA case: implies ACK received from AP for the unicast */
	/* packet in which mcast pkt was sent */
	uint32_t txMcast;
	/* number of received multicast data packets */
	uint32_t rxMcast;
	/* number of received unicast a-mpdus */
	uint32_t rxAmpdu;
	/* number of transmitted unicast a-mpdus */
	uint32_t txAmpdu;
	/* number of data pkt losses (no ACK) */
	uint32_t mpduLost;
	/* total number of data pkt retries */
	uint32_t retries;
	/* number of short data pkt retries */
	uint32_t retriesShort;
	/* number of long data pkt retries */
	uint32_t retriesLong;
	/* data pkt min contention time (usecs) */
	uint32_t contentionTimeMin;
	/* data pkt max contention time (usecs) */
	uint32_t contentionTimeMax;
	/* data pkt avg contention time (usecs) */
	uint32_t contentionTimeAvg;
	/* num of data pkts used for contention statistics */
	uint32_t contentionNumSamples;
} tSirWifiWmmAcStat, *tpSirWifiWmmAcStat;

/* Interface statistics - corresponding to 2nd most
 * LSB in wifi statistics bitmap  for getting statistics
 */
typedef struct {
	/* current state of the interface */
	tSirWifiInterfaceInfo info;
	/* access point beacon received count from connected AP */
	uint32_t beaconRx;
	/* access point mgmt frames received count from */
	/* connected AP (including Beacon) */
	uint32_t mgmtRx;
	/* action frames received count */
	uint32_t mgmtActionRx;
	/* action frames transmit count */
	uint32_t mgmtActionTx;
	/* access Point Beacon and Management frames RSSI (averaged) */
	uint32_t rssiMgmt;
	/* access Point Data Frames RSSI (averaged) from connected AP */
	uint32_t rssiData;
	/* access Point ACK RSSI (averaged) from connected AP */
	uint32_t rssiAck;
	/* number of peers */
	uint32_t num_peers;
	/*
	 * Indicates how many peer_stats events will be sent depending on the
	 * num_peers.
	 */
	uint32_t num_peer_events;
	/* number of ac */
	uint32_t num_ac;
	/* Roaming Stat */
	uint32_t roam_state;
	/*
	 * Average Beacon spread offset is the averaged time delay between TBTT
	 * and beacon TSF. Upper 32 bits of averaged 64 bit beacon spread offset
	 */
	uint32_t avg_bcn_spread_offset_high;
	/* Lower 32 bits of averaged 64 bit beacon spread offset */
	uint32_t avg_bcn_spread_offset_low;
	/*
	 * Takes value of 1 if AP leaks packets after sending an ACK for PM=1
	 * otherwise 0
	 */
	uint32_t is_leaky_ap;
	/*
	 * Average number of frames received from AP after receiving the ACK
	 * for a frame with PM = 1
	 */
	uint32_t avg_rx_frms_leaked;
	/*
	 *  Rx leak watch window currently in force to minimize data loss
	 *  because of leaky AP. Rx leak window is the
	 *  time driver waits before shutting down the radio or switching
	 *  the channel and after receiving an ACK for
	 *  a data frame with PM bit set.
	 */
	uint32_t rx_leak_window;
	/* per ac data packet statistics */
	tSirWifiWmmAcStat AccessclassStats[WIFI_AC_MAX];
} tSirWifiIfaceStat, *tpSirWifiIfaceStat;

/* Peer statistics - corresponding to 3rd most LSB in
 * wifi statistics bitmap  for getting statistics
 */
typedef struct {
	/* number of peers */
	uint32_t numPeers;
	/* per peer statistics */
	tSirWifiPeerInfo peerInfo[0];
} tSirWifiPeerStat, *tpSirWifiPeerStat;

/* wifi statistics bitmap  for getting statistics */
#define WMI_LINK_STATS_RADIO          0x00000001
#define WMI_LINK_STATS_IFACE          0x00000002
#define WMI_LINK_STATS_ALL_PEER       0x00000004
#define WMI_LINK_STATS_PER_PEER       0x00000008

/* wifi statistics bitmap  for clearing statistics */
/* all radio statistics */
#define WIFI_STATS_RADIO              0x00000001
/* cca_busy_time (within radio statistics) */
#define WIFI_STATS_RADIO_CCA          0x00000002
/* all channel statistics (within radio statistics) */
#define WIFI_STATS_RADIO_CHANNELS     0x00000004
/* all scan statistics (within radio statistics) */
#define WIFI_STATS_RADIO_SCAN         0x00000008
/* all interface statistics */
#define WIFI_STATS_IFACE              0x00000010
/* all tx rate statistics (within interface statistics) */
#define WIFI_STATS_IFACE_TXRATE       0x00000020
/* all ac statistics (within interface statistics) */
#define WIFI_STATS_IFACE_AC           0x00000040
/* all contention (min, max, avg) statistics (within ac statistics) */
#define WIFI_STATS_IFACE_CONTENTION   0x00000080

typedef struct {
	uint32_t paramId;
	uint8_t ifaceId;
	uint32_t rspId;
	uint32_t moreResultToFollow;
	union {
		uint32_t num_peers;
		uint32_t num_radio;
	};

	uint32_t peer_event_number;
	/* Variable  length field - Do not add anything after this */
	uint8_t results[0];
} tSirLLStatsResults, *tpSirLLStatsResults;

#endif /* WLAN_FEATURE_LINK_LAYER_STATS */

typedef struct sAniGetLinkStatus {
	uint16_t msgType;       /* message type is same as the request type */
	uint16_t msgLen;        /* length of the entire request */
	uint8_t linkStatus;
	uint8_t sessionId;
} tAniGetLinkStatus, *tpAniGetLinkStatus;

#ifdef DHCP_SERVER_OFFLOAD
typedef struct {
	uint32_t vdev_id;
	uint32_t dhcpSrvOffloadEnabled;
	uint32_t dhcpClientNum;
	uint32_t dhcpSrvIP;
} tSirDhcpSrvOffloadInfo, *tpSirDhcpSrvOffloadInfo;
#endif /* DHCP_SERVER_OFFLOAD */
#ifdef WLAN_FEATURE_GPIO_LED_FLASHING
typedef struct {
	uint32_t reqId;
	/* pattern identifier. 0: disconnected 1: connected */
	uint32_t pattern_id;
	uint32_t led_x0;        /* led flashing parameter0 */
	uint32_t led_x1;        /* led flashing parameter1 */
} tSirLedFlashingReq, *tpSirLedFlashingReq;
#endif
/* find the size of given member within a structure */
#ifndef member_size
#define member_size(type, member) (sizeof(((type *)0)->member))
#endif

#define RTT_INVALID                     0x00
#define RTT_TIMING_MEAS_CAPABILITY      0x01
#define RTT_FINE_TIME_MEAS_INITIATOR_CAPABILITY 0x02
#define RTT_FINE_TIME_MEAS_RESPONDER_CAPABILITY 0x03

/**
 * enum fine_time_meas_mask - bit mask to identify device's
 *                            fine timing measurement capability
 * @FINE_TIME_MEAS_STA_INITIATOR - STA role, Initiator capability is supported
 * @FINE_TIME_MEAS_STA_RESPONDER - STA role, Responder capability is supported
 * @FINE_TIME_MEAS_P2PCLI_INITIATOR - P2P-CLI supports initiator capability
 * @FINE_TIME_MEAS_P2PCLI_RESPONDER - P2P-CLI supports responder capability
 * @FINE_TIME_MEAS_P2PGO_INITIATOR - P2P-GO supports initiator capability
 * @FINE_TIME_MEAS_P2PGO_RESPONDER - P2P-GO supports responder capability
 * @FINE_TIME_MEAS_SAP_INITIATOR - SAP role, Initiator capability is supported
 * @FINE_TIME_MEAS_SAP_RESPONDER - SAP role, Responder capability is supported
 */
enum fine_time_meas_mask {
	FINE_TIME_MEAS_STA_INITIATOR    = (1 << (0)),
	FINE_TIME_MEAS_STA_RESPONDER    = (1 << (1)),
	FINE_TIME_MEAS_P2PCLI_INITIATOR = (1 << (2)),
	FINE_TIME_MEAS_P2PCLI_RESPONDER = (1 << (3)),
	FINE_TIME_MEAS_P2PGO_INITIATOR  = (1 << (4)),
	FINE_TIME_MEAS_P2PGO_RESPONDER  = (1 << (5)),
	FINE_TIME_MEAS_SAP_INITIATOR    = (1 << (6)),
	FINE_TIME_MEAS_SAP_RESPONDER    = (1 << (7)),
};

/* number of neighbor reports that we can handle in Neighbor Report Response */
#define MAX_SUPPORTED_NEIGHBOR_RPT 15

/**
 * struct sir_stats_avg_factor
 * @vdev_id: session id
 * @stats_avg_factor: average factor
 */
struct sir_stats_avg_factor {
	uint8_t vdev_id;
	uint16_t stats_avg_factor;
};

/**
 * struct sir_guard_time_request
 * @vdev_id: session id
 * @guard_time: guard time
 */
struct sir_guard_time_request {
	uint8_t vdev_id;
	uint32_t guard_time;
};

/* Max number of rates allowed in Supported Rates IE */
#define MAX_NUM_SUPPORTED_RATES (8)

/*
 * struct rssi_monitor_req - rssi monitoring
 * @request_id: request id
 * @session_id: session id
 * @min_rssi: minimum rssi
 * @max_rssi: maximum rssi
 * @control: flag to indicate start or stop
 */
struct rssi_monitor_req {
	uint32_t request_id;
	uint32_t session_id;
	int8_t   min_rssi;
	int8_t   max_rssi;
	bool     control;
};

/**
 * struct rssi_breach_event - rssi breached event structure
 * @request_id: request id
 * @session_id: session id
 * @curr_rssi: current rssi
 * @curr_bssid: current bssid
 */
struct rssi_breach_event {
	uint32_t     request_id;
	uint32_t     session_id;
	int8_t       curr_rssi;
	struct cdf_mac_addr  curr_bssid;
};

#define MAX_NUM_FW_SEGMENTS 4

/**
 * struct fw_dump_seg_req - individual segment details
 * @seg_id - segment id.
 * @seg_start_addr_lo - lower address of the segment.
 * @seg_start_addr_hi - higher address of the segment.
 * @seg_length - length of the segment.
 * @dst_addr_lo - lower address of the destination buffer.
 * @dst_addr_hi - higher address of the destination buffer.
 *
 * This structure carries the information to firmware about the
 * individual segments. This structure is part of firmware memory
 * dump request.
 */
struct fw_dump_seg_req {
	uint8_t seg_id;
	uint32_t seg_start_addr_lo;
	uint32_t seg_start_addr_hi;
	uint32_t seg_length;
	uint32_t dst_addr_lo;
	uint32_t dst_addr_hi;
};

/**
 * struct fw_dump_req - firmware memory dump request details.
 * @request_id - request id.
 * @num_seg - requested number of segments.
 * @fw_dump_seg_req - individual segment information.
 *
 * This structure carries information about the firmware
 * memory dump request.
 */
struct fw_dump_req {
	uint32_t request_id;
	uint32_t num_seg;
	struct fw_dump_seg_req segment[MAX_NUM_FW_SEGMENTS];
};

/**
 * struct fw_dump_rsp - firmware dump response details.
 * @request_id - request id.
 * @dump_complete - copy completion status.
 *
 * This structure is used to store the firmware dump copy complete
 * response from the firmware.
 */
struct fw_dump_rsp {
	uint32_t request_id;
	uint32_t dump_complete;
};

/**
 * struct vdev_ie_info - IE info
 * @vdev_id - vdev for which the IE is being sent
 * @ie_id - ID of the IE
 * @length - length of the IE data
 * @data - IE data
 *
 * This structure is used to store the IE information.
 */
struct vdev_ie_info {
	uint32_t vdev_id;
	uint32_t ie_id;
	uint32_t length;
	uint8_t *data;
};

/**
 * struct send_extcap_ie - used to pass send_extcap_ie msg from SME to PE
 * @type - MSG type
 * @length - length of the message
 * @seesion_id - session_id for which the message is intended for
 *
 * This structure is used to pass send_extcap_ie msg from SME to PE
 */
struct send_extcap_ie {
	uint16_t msg_type; /* eWNI_SME_SET_IE_REQ */
	uint16_t length;
	uint8_t session_id;
};

typedef void (*hw_mode_cb)(uint32_t status, uint32_t cfgd_hw_mode_index,
		uint32_t num_vdev_mac_entries,
		struct sir_vdev_mac_map *vdev_mac_map);
typedef void (*hw_mode_transition_cb)(uint32_t old_hw_mode_index,
		uint32_t new_hw_mode_index,
		uint32_t num_vdev_mac_entries,
		struct sir_vdev_mac_map *vdev_mac_map);
typedef void (*dual_mac_cb)(uint32_t status, uint32_t scan_config,
		uint32_t fw_mode_config);

/**
 * struct sir_nss_update_request
 * @msgType: nss update msg type
 * @msgLen: length of the msg
 * @new_nss: new spatial stream value
 * @vdev_id: session id
 */
struct sir_nss_update_request {
	uint16_t msgType;
	uint16_t msgLen;
	uint8_t  new_nss;
	uint32_t vdev_id;
};

/**
 * struct sir_beacon_tx_complete_rsp
 *
 * @session_id: session for which beacon update happened
 * @tx_status: status of the beacon tx from FW
 */
struct sir_beacon_tx_complete_rsp {
	uint8_t session_id;
	uint8_t tx_status;
};

typedef void (*nss_update_cb)(void *context, uint8_t tx_status, uint8_t vdev_id,
		uint8_t next_action);

/**
 * OCB structures
 */

#define NUM_AC			(4)
#define OCB_CHANNEL_MAX	(5)

struct sir_qos_params {
	uint8_t aifsn;
	uint8_t cwmin;
	uint8_t cwmax;
};

/**
 * struct sir_ocb_set_config_response
 * @status: response status
 */
struct sir_ocb_set_config_response {
	uint8_t status;
};

/** Callback for the dcc_stats_event */
typedef void (*dcc_stats_event_callback_t)(void *hdd_ctx, uint32_t vdev_id,
	uint32_t num_channels, uint32_t stats_per_channel_array_len,
	const void *stats_per_channel_array);

/**
 * struct sir_ocb_config_channel
 * @chan_freq: frequency of the channel
 * @bandwidth: bandwidth of the channel, either 10 or 20 MHz
 * @mac_address: MAC address assigned to this channel
 * @qos_params: QoS parameters
 * @max_pwr: maximum transmit power of the channel (dBm)
 * @min_pwr: minimum transmit power of the channel (dBm)
 * @reg_pwr: maximum transmit power specified by the regulatory domain (dBm)
 * @antenna_max: maximum antenna gain specified by the regulatory domain (dB)
 */
struct sir_ocb_config_channel {
	uint32_t chan_freq;
	uint32_t bandwidth;
	struct cdf_mac_addr mac_address;
	struct sir_qos_params qos_params[MAX_NUM_AC];
	uint32_t max_pwr;
	uint32_t min_pwr;
	uint8_t reg_pwr;
	uint8_t antenna_max;
	uint16_t flags;
};

/**
 * OCB_CHANNEL_FLAG_NO_RX_HDR - Don't add the RX stats header to packets
 *      received on this channel.
 */
#define OCB_CHANNEL_FLAG_DISABLE_RX_STATS_HDR	(1 << 0)

/**
 * struct sir_ocb_config_sched
 * @chan_freq: frequency of the channel
 * @total_duration: duration of the schedule
 * @guard_interval: guard interval on the start of the schedule
 */
struct sir_ocb_config_sched {
	uint32_t chan_freq;
	uint32_t total_duration;
	uint32_t guard_interval;
};

/**
 * struct sir_ocb_config
 * @session_id: session id
 * @channel_count: number of channels
 * @schedule_size: size of the channel schedule
 * @flags: reserved
 * @channels: array of OCB channels
 * @schedule: array of OCB schedule elements
 * @dcc_ndl_chan_list_len: size of the ndl_chan array
 * @dcc_ndl_chan_list: array of dcc channel info
 * @dcc_ndl_active_state_list_len: size of the active state array
 * @dcc_ndl_active_state_list: array of active states
 * @adapter: the OCB adapter
 * @dcc_stats_callback: callback for the response event
 */
struct sir_ocb_config {
	uint8_t session_id;
	uint32_t channel_count;
	uint32_t schedule_size;
	uint32_t flags;
	struct sir_ocb_config_channel *channels;
	struct sir_ocb_config_sched *schedule;
	uint32_t dcc_ndl_chan_list_len;
	void *dcc_ndl_chan_list;
	uint32_t dcc_ndl_active_state_list_len;
	void *dcc_ndl_active_state_list;
};

/* The size of the utc time in bytes. */
#define SIZE_UTC_TIME (10)
/* The size of the utc time error in bytes. */
#define SIZE_UTC_TIME_ERROR (5)

/**
 * struct sir_ocb_utc
 * @vdev_id: session id
 * @utc_time: number of nanoseconds from Jan 1st 1958
 * @time_error: the error in the UTC time. All 1's for unknown
 */
struct sir_ocb_utc {
	uint32_t vdev_id;
	uint8_t utc_time[SIZE_UTC_TIME];
	uint8_t time_error[SIZE_UTC_TIME_ERROR];
};

/**
 * struct sir_ocb_timing_advert
 * @vdev_id: session id
 * @chan_freq: frequency on which to advertise
 * @repeat_rate: the number of times it will send TA in 5 seconds
 * @timestamp_offset: offset of the timestamp field in the TA frame
 * @time_value_offset: offset of the time_value field in the TA frame
 * @template_length: size in bytes of the TA frame
 * @template_value: the TA frame
 */
struct sir_ocb_timing_advert {
	uint32_t vdev_id;
	uint32_t chan_freq;
	uint32_t repeat_rate;
	uint32_t timestamp_offset;
	uint32_t time_value_offset;
	uint32_t template_length;
	uint8_t *template_value;
};

/**
 * struct sir_ocb_get_tsf_timer_response
 * @vdev_id: session id
 * @timer_high: higher 32-bits of the timer
 * @timer_low: lower 32-bits of the timer
 */
struct sir_ocb_get_tsf_timer_response {
	uint32_t vdev_id;
	uint32_t timer_high;
	uint32_t timer_low;
};

/**
 * struct sir_ocb_get_tsf_timer
 * @vdev_id: session id
 */
struct sir_ocb_get_tsf_timer {
	uint32_t vdev_id;
};

/**
 * struct sir_dcc_get_stats_response
 * @vdev_id: session id
 * @num_channels: number of dcc channels
 * @channel_stats_array_len: size in bytes of the stats array
 * @channel_stats_array: the stats array
 */
struct sir_dcc_get_stats_response {
	uint32_t vdev_id;
	uint32_t num_channels;
	uint32_t channel_stats_array_len;
	void *channel_stats_array;
};

/**
 * struct sir_dcc_get_stats
 * @vdev_id: session id
 * @channel_count: number of dcc channels
 * @request_array_len: size in bytes of the request array
 * @request_array: the request array
 */
struct sir_dcc_get_stats {
	uint32_t vdev_id;
	uint32_t channel_count;
	uint32_t request_array_len;
	void *request_array;
};

/**
 * struct sir_dcc_clear_stats
 * @vdev_id: session id
 * @dcc_stats_bitmap: bitmap of clear option
 */
struct sir_dcc_clear_stats {
	uint32_t vdev_id;
	uint32_t dcc_stats_bitmap;
};

/**
 * struct sir_dcc_update_ndl_response
 * @vdev_id: session id
 * @status: response status
 */
struct sir_dcc_update_ndl_response {
	uint32_t vdev_id;
	uint32_t status;
};

/**
 * struct sir_dcc_update_ndl
 * @vdev_id: session id
 * @channel_count: number of channels to be updated
 * @dcc_ndl_chan_list_len: size in bytes of the ndl_chan array
 * @dcc_ndl_chan_list: the ndl_chan array
 * @dcc_ndl_active_state_list_len: size in bytes of the active_state array
 * @dcc_ndl_active_state_list: the active state array
 */
struct sir_dcc_update_ndl {
	uint32_t vdev_id;
	uint32_t channel_count;
	uint32_t dcc_ndl_chan_list_len;
	void *dcc_ndl_chan_list;
	uint32_t dcc_ndl_active_state_list_len;
	void *dcc_ndl_active_state_list;
};

/**
 * enum powersave_qpower_mode: QPOWER modes
 * @QPOWER_DISABLED: Qpower is disabled
 * @QPOWER_ENABLED: Qpower is enabled
 * @QPOWER_DUTY_CYCLING: Qpower is enabled with duty cycling
 */
enum powersave_qpower_mode {
	QPOWER_DISABLED = 0,
	QPOWER_ENABLED = 1,
	QPOWER_DUTY_CYCLING = 2
};

/**
 * enum powersave_qpower_mode: powersave_mode
 * @PS_NOT_SUPPORTED: Power save is not supported
 * @PS_LEGACY_NODEEPSLEEP: Legacy power save enabled and deep sleep disabled
 * @PS_QPOWER_NODEEPSLEEP: QPOWER enabled and deep sleep disabled
 * @PS_LEGACY_DEEPSLEEP: Legacy power save enabled and deep sleep enabled
 * @PS_QPOWER_DEEPSLEEP: QPOWER enabled and deep sleep enabled
 * @PS_DUTY_CYCLING_QPOWER: QPOWER enabled in duty cycling mode
 */
enum powersave_mode {
	PS_NOT_SUPPORTED = 0,
	PS_LEGACY_NODEEPSLEEP = 1,
	PS_QPOWER_NODEEPSLEEP = 2,
	PS_LEGACY_DEEPSLEEP = 3,
	PS_QPOWER_DEEPSLEEP = 4,
	PS_DUTY_CYCLING_QPOWER = 5
};

/**
 * struct sir_sme_ext_change_chan_req - channel change request
 * @message_type: message id
 * @length: msg length
 * @new_channel: new channel
 * @session_id: session id
 */
struct sir_sme_ext_cng_chan_req {
	uint16_t  message_type; /* eWNI_SME_EXT_CHANGE_CHANNEL */
	uint16_t  length;
	uint32_t  new_channel;
	uint8_t   session_id;
};

/**
 * struct sir_sme_ext_change_chan_ind.
 * @session_id: session id
 * @new_channel: new channel to change
 */
struct sir_sme_ext_cng_chan_ind {
	uint8_t  session_id;
	uint8_t  new_channel;
};

#endif /* __SIR_API_H */
