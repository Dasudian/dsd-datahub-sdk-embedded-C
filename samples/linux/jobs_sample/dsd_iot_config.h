/*
 * Copyright 2010-2018 Dasudian.com, Inc. or its affiliates. All Rights Reserved.
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
 * @file dsd_iot_config.h
 * @brief DSD IoT specific configuration file
 */

#ifndef SRC_JOBS_IOT_JOB_CONFIG_H_
#define SRC_JOBS_IOT_JOB_CONFIG_H_

// Get from console
// =================================================
#define DSD_IOT_MQTT_HOST              "datahub.dsdiot.com" ///< Customer specific datahub MQTT HOST. Make sure with Dasudian Tech team>
#define DSD_IOT_MQTT_PORT              8883 ///< default port for MQTT/S
#define DSD_IOT_MQTT_CLIENT_ID         "{customer_prefix}|{deviceType|{deviceID}" ///< MQTT client ID should be unique for every device, ask Dasudian team for the customer prefix
#define DSD_IOT_MY_THING_NAME          "PLC-S7-200" ///< Thing Name of the Shadow this device is associated with
#define DSD_IOT_MY_USER_NAME           "{username}" ///Username associated with the connection credential, ask Dasudian Team for the info
#define DSD_IOT_MY_PASSWORD            "{password}" ///Password associated with the connection credential, ask Dasudian Team for the info
#define DSD_IOT_ROOT_CA_FILENAME       "rootCA.crt" ///< Root CA file name
#define DSD_IOT_CERTIFICATE_FILENAME   "cert.pem" ///< device signed certificate file name
#define DSD_IOT_PRIVATE_KEY_FILENAME   "privkey.pem" ///< Device private key filename

// MQTT PubSub
#ifndef DISABLE_IOT_JOBS
#define DSD_IOT_MQTT_RX_BUF_LEN 512 ///< Any message that comes into the device should be less than this buffer size. If a received message is bigger than this buffer size the message will be dropped.
#else
#define DSD_IOT_MQTT_RX_BUF_LEN 2048
#endif
#define DSD_IOT_MQTT_TX_BUF_LEN 512 ///< Any time a message is sent out through the MQTT layer. The message is copied into this buffer anytime a publish is done. This will also be used in the case of Thing Shadow
#define DSD_IOT_MQTT_NUM_SUBSCRIBE_HANDLERS 5 ///< Maximum number of topic filters the MQTT client can handle at any given time. This should be increased appropriately when using Thing Shadow

// Shadow and Job common configs
#define MAX_SIZE_OF_UNIQUE_CLIENT_ID_BYTES 80  ///< Maximum size of the Unique Client Id. For More info on the Client Id refer \ref response "Acknowledgments"
#define MAX_SIZE_CLIENT_ID_WITH_SEQUENCE MAX_SIZE_OF_UNIQUE_CLIENT_ID_BYTES + 10 ///< This is size of the extra sequence number that will be appended to the Unique client Id
#define MAX_SIZE_CLIENT_TOKEN_CLIENT_SEQUENCE MAX_SIZE_CLIENT_ID_WITH_SEQUENCE + 20 ///< This is size of the the total clientToken key and value pair in the JSON
#define MAX_SIZE_OF_THING_NAME 30 ///< The Thing Name should not be bigger than this value. Modify this if the Thing Name needs to be bigger

// Thing Shadow specific configs
#define SHADOW_MAX_SIZE_OF_RX_BUFFER 512 ///< Maximum size of the SHADOW buffer to store the received Shadow message
#define MAX_ACKS_TO_COMEIN_AT_ANY_GIVEN_TIME 10 ///< At Any given time we will wait for this many responses. This will correlate to the rate at which the shadow actions are requested
#define MAX_THINGNAME_HANDLED_AT_ANY_GIVEN_TIME 10 ///< We could perform shadow action on any thing Name and this is maximum Thing Names we can act on at any given time
#define MAX_JSON_TOKEN_EXPECTED 120 ///< These are the max tokens that is expected to be in the Shadow JSON document. Include the metadata that gets published
#define MAX_SHADOW_TOPIC_LENGTH_WITHOUT_THINGNAME 60 ///< All shadow actions have to be published or subscribed to a topic which is of the format $dsd/things/{thingName}/shadow/update/accepted. This refers to the size of the topic without the Thing Name
#define MAX_SHADOW_TOPIC_LENGTH_BYTES MAX_SHADOW_TOPIC_LENGTH_WITHOUT_THINGNAME + MAX_SIZE_OF_THING_NAME ///< This size includes the length of topic with Thing Name

// Job specific configs
#ifndef DISABLE_IOT_JOBS
#define MAX_SIZE_OF_JOB_ID 64
#define MAX_JOB_JSON_TOKEN_EXPECTED 120
#define MAX_SIZE_OF_JOB_REQUEST DSD_IOT_MQTT_TX_BUF_LEN

#define MAX_JOB_TOPIC_LENGTH_WITHOUT_JOB_ID_OR_THING_NAME 40
#define MAX_JOB_TOPIC_LENGTH_BYTES MAX_JOB_TOPIC_LENGTH_WITHOUT_JOB_ID_OR_THING_NAME + MAX_SIZE_OF_THING_NAME + MAX_SIZE_OF_JOB_ID + 2
#endif

// Auto Reconnect specific config
#define DSD_IOT_MQTT_MIN_RECONNECT_WAIT_INTERVAL 1000 ///< Minimum time before the First reconnect attempt is made as part of the exponential back-off algorithm
#define DSD_IOT_MQTT_MAX_RECONNECT_WAIT_INTERVAL 128000 ///< Maximum time interval after which exponential back-off will stop attempting to reconnect.

#define DISABLE_METRICS false ///< Disable the collection of metrics by setting this to true

#endif /* SRC_JOBS_IOT_JOB_CONFIG_H_ */