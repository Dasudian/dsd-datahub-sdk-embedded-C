/*
 * Copyright 2010-2015 Dasudian.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://dsd.dasudian.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

/**
 * @file dsd_iot_shadow.c
 * @brief Shadow client API definitions
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "../include/dsd_iot_mqtt_client_interface.h"
#include "../include/dsd_iot_shadow_interface.h"
#include "../include/dsd_iot_error.h"
#include "../include/dsd_iot_log.h"
#include "../include/dsd_iot_shadow_actions.h"
#include "../include/dsd_iot_shadow_json.h"
#include "../include/dsd_iot_shadow_key.h"
#include "../include/dsd_iot_shadow_records.h"

const ShadowInitParameters_t ShadowInitParametersDefault = {(char *) DSD_IOT_MQTT_HOST, DSD_IOT_MQTT_PORT, NULL, NULL,
															NULL, false, NULL};

const ShadowConnectParameters_t ShadowConnectParametersDefault = {(char *) DSD_IOT_MY_THING_NAME,
								  (char *) DSD_IOT_MQTT_CLIENT_ID, 0, NULL};

IoT_Client_Connect_Params dsd_init_shadow_conn_params(char *username, char *password, char *clientID) {
	IoT_Client_Connect_Params a = { {'M', 'Q', 'T', 'C'}, MQTT_3_1_1, clientID, (uint16_t) strlen(clientID), 60, true, false, \
        IoT_MQTT_Will_Options_Initializer, username, (uint16_t) strlen(username), password,(uint16_t) strlen(password) };
	return a;
}

static char deleteAcceptedTopic[MAX_SHADOW_TOPIC_LENGTH_BYTES];

void dsd_iot_shadow_reset_last_received_version(void) {
	shadowJsonVersionNum = 0;
}

uint32_t dsd_iot_shadow_get_last_received_version(void) {
	return shadowJsonVersionNum;
}

void dsd_iot_shadow_enable_discard_old_delta_msgs(void) {
	shadowDiscardOldDeltaFlag = true;
}

void dsd_iot_shadow_disable_discard_old_delta_msgs(void) {
	shadowDiscardOldDeltaFlag = false;
}

IoT_Error_t dsd_iot_shadow_free(DSD_IoT_Client *pClient)
{
    IoT_Error_t rc;

    if (NULL == pClient) {
        FUNC_EXIT_RC(NULL_VALUE_ERROR);
    }

    rc = dsd_iot_mqtt_free(pClient);

    FUNC_EXIT_RC(rc);
}

IoT_Error_t dsd_iot_shadow_init(DSD_IoT_Client *pClient, ShadowInitParameters_t *pParams, IoT_Client_Connect_Params *pConnParams) {
	IoT_Client_Init_Params mqttInitParams = IoT_Client_Init_Params_initializer;
	IoT_Error_t rc;

	FUNC_ENTRY;

	if(NULL == pClient || NULL == pParams) {
		FUNC_EXIT_RC(NULL_VALUE_ERROR);
	}

	mqttInitParams.enableAutoReconnect = pParams->enableAutoReconnect;
	mqttInitParams.pHostURL = pParams->pHost;
	mqttInitParams.port = pParams->port;
	mqttInitParams.pRootCALocation = pParams->pRootCA;
	mqttInitParams.pDeviceCertLocation = pParams->pClientCRT;
	mqttInitParams.pDevicePrivateKeyLocation = pParams->pClientKey;
	mqttInitParams.mqttPacketTimeout_ms = 5000;
	mqttInitParams.mqttCommandTimeout_ms = 20000;
	mqttInitParams.tlsHandshakeTimeout_ms = 5000;
	mqttInitParams.isSSLHostnameVerify = true;
	mqttInitParams.disconnectHandler = pParams->disconnectHandler;

	rc = dsd_iot_mqtt_init(pClient, &mqttInitParams, pConnParams);
	if(SUCCESS != rc) {
		FUNC_EXIT_RC(rc);
	}

	resetClientTokenSequenceNum();
	dsd_iot_shadow_reset_last_received_version();
	initDeltaTokens();

	FUNC_EXIT_RC(SUCCESS);
}

IoT_Error_t dsd_iot_shadow_connect(DSD_IoT_Client *pClient, ShadowConnectParameters_t *pParams, IoT_Client_Connect_Params *pConnParams) {
	IoT_Error_t rc = SUCCESS;
	uint16_t deleteAcceptedTopicLen;

	FUNC_ENTRY;

	if(NULL == pClient || NULL == pParams || NULL == pParams->pMqttClientId) {
		FUNC_EXIT_RC(NULL_VALUE_ERROR);
	}

	snprintf(myThingName, MAX_SIZE_OF_THING_NAME, "%s", pParams->pMyThingName);
	snprintf(mqttClientID, MAX_SIZE_OF_UNIQUE_CLIENT_ID_BYTES, "%s", pParams->pMqttClientId);


	rc = dsd_iot_mqtt_connect(pClient, pConnParams);

	if(SUCCESS != rc) {
		FUNC_EXIT_RC(rc);
	}

	initializeRecords(pClient);

	if(NULL != pParams->deleteActionHandler) {
		snprintf(deleteAcceptedTopic, MAX_SHADOW_TOPIC_LENGTH_BYTES,
				 "$dsd/things/%s/shadow/delete/accepted", myThingName);
		deleteAcceptedTopicLen = (uint16_t) strlen(deleteAcceptedTopic);
		rc = dsd_iot_mqtt_subscribe(pClient, deleteAcceptedTopic, deleteAcceptedTopicLen, QOS1,
									pParams->deleteActionHandler, (void *) myThingName);
	}

	FUNC_EXIT_RC(rc);
}

IoT_Error_t dsd_iot_shadow_register_delta(DSD_IoT_Client *pMqttClient, jsonStruct_t *pStruct) {
	if(NULL == pMqttClient || NULL == pStruct) {
		return NULL_VALUE_ERROR;
	}

	if(!dsd_iot_mqtt_is_client_connected(pMqttClient)) {
		return MQTT_CONNECTION_ERROR;
	}

	return registerJsonTokenOnDelta(pStruct);
}

IoT_Error_t dsd_iot_shadow_yield(DSD_IoT_Client *pClient, uint32_t timeout) {
	if(NULL == pClient) {
		return NULL_VALUE_ERROR;
	}

	HandleExpiredResponseCallbacks();
	return dsd_iot_mqtt_yield(pClient, timeout);
}

IoT_Error_t dsd_iot_shadow_disconnect(DSD_IoT_Client *pClient) {
	return dsd_iot_mqtt_disconnect(pClient);
}

IoT_Error_t dsd_iot_shadow_update(DSD_IoT_Client *pClient, const char *pThingName, char *pJsonString,
								  fpActionCallback_t callback, void *pContextData, uint8_t timeout_seconds,
								  bool isPersistentSubscribe) {
	IoT_Error_t rc;

	if(NULL == pClient) {
		FUNC_EXIT_RC(NULL_VALUE_ERROR);
	}

	if(!dsd_iot_mqtt_is_client_connected(pClient)) {
		FUNC_EXIT_RC(MQTT_CONNECTION_ERROR);
	}

	rc = dsd_iot_shadow_internal_action(pThingName, SHADOW_UPDATE, pJsonString, strlen(pJsonString), callback, pContextData,
										timeout_seconds, isPersistentSubscribe);

	FUNC_EXIT_RC(rc);
}

IoT_Error_t dsd_iot_shadow_delete(DSD_IoT_Client *pClient, const char *pThingName, fpActionCallback_t callback,
								  void *pContextData, uint8_t timeout_seconds, bool isPersistentSubscribe) {
	char deleteRequestJsonBuf[MAX_SIZE_CLIENT_TOKEN_CLIENT_SEQUENCE];
	IoT_Error_t rc;

	FUNC_ENTRY;

	if(NULL == pClient) {
		FUNC_EXIT_RC(NULL_VALUE_ERROR);
	}

	if(!dsd_iot_mqtt_is_client_connected(pClient)) {
		FUNC_EXIT_RC(MQTT_CONNECTION_ERROR);
	}

	rc = dsd_iot_shadow_internal_delete_request_json(deleteRequestJsonBuf, MAX_SIZE_CLIENT_TOKEN_CLIENT_SEQUENCE );
    if ( SUCCESS != rc ) {
        FUNC_EXIT_RC( rc );
    }

	rc = dsd_iot_shadow_internal_action(pThingName, SHADOW_DELETE, deleteRequestJsonBuf, MAX_SIZE_CLIENT_TOKEN_CLIENT_SEQUENCE, callback, pContextData,
										timeout_seconds, isPersistentSubscribe);

	FUNC_EXIT_RC(rc);
}

IoT_Error_t dsd_iot_shadow_get(DSD_IoT_Client *pClient, const char *pThingName, fpActionCallback_t callback,
							   void *pContextData, uint8_t timeout_seconds, bool isPersistentSubscribe) {
	char getRequestJsonBuf[MAX_SIZE_CLIENT_TOKEN_CLIENT_SEQUENCE];
	IoT_Error_t rc;

	FUNC_ENTRY;

	if(NULL == pClient) {
		FUNC_EXIT_RC(NULL_VALUE_ERROR);
	}

	if(!dsd_iot_mqtt_is_client_connected(pClient)) {
		FUNC_EXIT_RC(MQTT_CONNECTION_ERROR);
	}

    rc = dsd_iot_shadow_internal_get_request_json(getRequestJsonBuf, MAX_SIZE_CLIENT_TOKEN_CLIENT_SEQUENCE );
    if (SUCCESS != rc) {
        FUNC_EXIT_RC(rc);
    }

	rc = dsd_iot_shadow_internal_action(pThingName, SHADOW_GET, getRequestJsonBuf, MAX_SIZE_CLIENT_TOKEN_CLIENT_SEQUENCE, callback, pContextData,
										timeout_seconds, isPersistentSubscribe);
	FUNC_EXIT_RC(rc);
}

IoT_Error_t dsd_iot_shadow_set_autoreconnect_status(DSD_IoT_Client *pClient, bool newStatus) {
	return dsd_iot_mqtt_autoreconnect_set_status(pClient, newStatus);
}

#ifdef __cplusplus
}
#endif

