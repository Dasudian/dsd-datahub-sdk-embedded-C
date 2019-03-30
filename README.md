[![Build Status](https://travis-ci.org/dsd/dsd-iot-device-sdk-embedded-C.svg?branch=master)](https://travis-ci.org/dsd/dsd-iot-device-sdk-embedded-C)

<a href="https://scan.coverity.com/projects/dsd-iot-device-sdk-embedded-c">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/15543/badge.svg"/>
</a>

## Enrich data with wisdom and foresee the future with insights

## Overview

The DSD IoT DataHub SDK for embedded C is a collection of C source files which can be used in embedded applications to securely connect to the [DSD IoT platform](https://dasudian.com/cn/service_iot.html). It includes transport clients **MQTT**, **TLS** implementations and examples for their use. It also supports DSD IoT specific features such as **Thing Shadow**. It is distributed in source form and intended to be built into customer firmware along with application code, other libraries and RTOS. For additional information about porting the Device SDK for embedded C onto additional platforms please refer to the [PortingGuide](https://github.com/Dasudian/dsd-datahub-sdk-embedded-C/blob/master/PortingGuide.md).

快速启动基于本SDK的智能边缘计算节点，请参考本文最后一章[Sample APIs](## Sample APIs)

## APIs
<table class="doxtable">
<tr>
<th align="left">Description </th><th align="left">Definition </th></tr>
<tr>
<td align="left">Initializing the connection parameters </td><td align="left"><code>IoT_Client_Connect_Params dsd_init_conn_params(char *username, char *password);</code> </td></tr>
<td align="left">Initializing the shadow connection parameters </td><td align="left"><code>IoT_Client_Connect_Params dsd_init_shadow_conn_params(char *username, char *password, char *clientID);</code> </td></tr>
<td align="left">Initializing the client </td><td align="left"><code>void dsd_iot_mqtt_init(MQTTClient_t *pClient);</code> </td></tr>
<tr>
<td align="left">Connect </td><td align="left"><code>IoT_Error_t dsd_iot_mqtt_connect(MQTTConnectParams *pParams);</code> </td></tr>
<tr>
<td align="left">Subscribe </td><td align="left"><code>IoT_Error_t dsd_iot_mqtt_subscribe(MQTTSubscribeParams *pParams);</code> </td></tr>
<tr>
<td align="left">Unsubscribe </td><td align="left"><code>IoT_Error_t dsd_iot_mqtt_unsubscribe(char *pTopic);</code> </td></tr>
<tr>
<td align="left">Yield </td><td align="left"><code>IoT_Error_t dsd_iot_mqtt_yield(int timeout);</code> </td></tr>
<tr>
<td align="left">Publish </td><td align="left"><code>IoT_Error_t dsd_iot_mqtt_publish(MQTTPublishParams *pParams);</code> </td></tr>
<tr>
<td align="left">Disconnect </td><td align="left"><code>IoT_Error_t dsd_iot_mqtt_disconnect(void);</code> </td></tr>
</table>

## Features
The Device SDK simplifies access to the Pub/Sub functionality of the DSD IoT Datahub broker via MQTT and provide APIs to interact with Thing Shadows. The SDK has been tested to work with the DSD IoT platform to ensure best interoperability of a device with the DSD IoT platform.

### MQTT Connection
The Device SDK provides functionality to create and maintain a mutually authenticated TLS connection over which it runs MQTT. This connection is used for any further publish operations and allow for subscribing to MQTT topics which will call a configurable callback function when these topics are received.

### Thing Shadow
The Device SDK implements the specific protocol for Thing Shadows to retrieve, update and delete Thing Shadows adhering to the protocol that is implemented to ensure correct versioning and support for client tokens. It abstracts the necessary MQTT topic subscriptions by automatically subscribing to and unsubscribing from the reserved topics as needed for each API call. Inbound state change requests are automatically signalled via a configurable callback.

### Jobs
The Device SDK implements features to facilitate use of the DSD Jobs service. The Jobs service can be used for device management tasks such as updating program files, rotating device certificates, or running other maintenance tasks such are restoring device settings or restarting devices.

## Design Goals of this SDK
The embedded C SDK was specifically designed for resource constrained devices (running on micro-controllers and RTOS).

Primary aspects are:
 * Flexibility in picking and choosing functionality (reduce memory footprint)
 * Static memory only (no malloc's)
 * Configurable resource usage(JSON tokens, MQTT subscription handlers, etc.)
 * Can be ported to a different RTOS, uses wrappers for OS specific functions

## Collection of Metrics
DSD collects usage metrics indicating which language and version of the SDK is being used. This allows us to prioritize our resources towards addressing issues faster in SDKs that see the most and is an important data point. However, we do understand that not all customers would want to report this data by default. In that case, the sending of usage metrics can be easily disabled by the user by setting the `DISABLE_METRICS` flag to true in the
`dsd_iot_config.h` for each application.

## How to get started ?
Ensure you understand the DSD IoT platform and create the necessary certificates and policies. For more information on the DSD IoT platform please visit the [DSD IoT developer guide](http://docs.dsd.dasudian.com/iot/latest/developerguide/iot-security-identity.html).

In order to quickly get started with the DSD IoT platform, we have ported the SDK for POSIX type Operating Systems like Ubuntu, OS X and RHEL. The SDK is configured for the mbedTLS library and can be built out of the box with *GCC* using *make utility*. You'll need to download mbedTLS from the official ARMmbed repository. We recommend that you pick the latest version in order to have up-to-date security fixes.

<h2>Sample APIs</h2>
<p>Connecting to the DSD IoT MQTT platform</p>
<div class="fragment"><div class="line"><a name="l00001"></a><span class="lineno">    1</span>&#160;DSD_IoT_Client client;</div>
<div class="line"><a name="l00002"></a><span class="lineno">    2</span>&#160;rc = dsd_iot_mqtt_init(&amp;client, &amp;iotInitParams);</div>
<div class="line"><a name="l00003"></a><span class="lineno">    3</span>&#160;rc = dsd_iot_mqtt_connect(&amp;client, &amp;iotConnectParams);</div>
</div><!-- fragment --><p>Subscribe to a topic</p>
<div class="fragment"><div class="line"><a name="l00001"></a><span class="lineno">    1</span>&#160;DSD_IoT_Client client;</div>
<div class="line"><a name="l00002"></a><span class="lineno">    2</span>&#160;rc = dsd_iot_mqtt_subscribe(&amp;client, &quot;sdkTest/sub&quot;, 11, QOS0, iot_subscribe_callback_handler, NULL);</div>
</div><!-- fragment --><p>Update Thing Shadow from a device</p>
<div class="fragment"><div class="line"><a name="l00001"></a><span class="lineno">    1</span>&#160;rc = dsd_iot_shadow_update(&amp;mqttClient, DSD_IOT_MY_THING_NAME, pJsonDocumentBuffer, ShadowUpdateStatusCallback,</div>
<div class="line"><a name="l00002"></a><span class="lineno">    2</span>&#160;                            pCallbackContext, TIMEOUT_4SEC, persistenSubscription);</div>
</div><!-- fragment --> </div></div>

## Installation
This section explains the individual steps to retrieve the necessary files and be able to build your first application using the DSD IoT device SDK for embedded C.

Steps:

 * Create a directory to hold your application e.g. (/home/<user>/dsd_iot/my_app)
 * Change directory to this new directory
 * Download the SDK to device and place in the newly created directory
 * Expand the tarball (tar -xf <tarball.tar>).  This will create the below directories:
    * `certs` - TLS certificates directory
    * `docs` - SDK API and file documentation. This folder is not present on GitHub. You can access the documentation [here](doc/index.html)
    * `external_libs` - The mbedTLS and jsmn source files
    * `include` - The DSD IoT SDK header files
    * `platform` - Platform specific files for timer, TLS and threading layers
    * `samples` - The sample applications
    * `src` - The DSD IoT SDK source files
    * `tests` - Contains tests for verifying that the SDK is functioning as expected. More information can be found [here](https://github.com/dsd/dsd-iot-device-sdk-embedded-c/blob/master/tests/README.md)
 * View further information on how to use the SDK in the Readme file for samples that can be found [here](https://github.com/dsd/dsd-iot-device-sdk-embedded-c/blob/master/samples/README.md)

Also, for a guided example on getting started with the Thing Shadow, visit the DSD IoT Console's [Interactive Guide](https://console.dsd.dasudian.com/iot).

## Porting to different platforms
As Embedded devices run on different Real Time Operating Systems and TCP/IP stacks, it is one of the important design goals for the Device SDK to keep it portable. Please refer to the [porting guide](https://github.com/Dasudian/dsd-datahub-sdk-embedded-C/blob/master/PortingGuide.md) to get more information on how to make this SDK run on your devices (i.e. micro-controllers).

### Changes in the `jsonStruct` data structure:
The member `dataLength` has been added to struct `jsonStruct`, which is declared in [include/dsd_iot_shadow_json_data.h](include/dsd_iot_shadow_json_data.h#L60).

```c
struct jsonStruct {
    const char *         pKey;
    void *               pData;
    size_t               dataLength;
    JsonPrimitiveType    type;
    JsonStructCallback_t cb;
};
```

The size of the buffer `pData` must now be specified by `dataLength`. **Failure to do so may result in undefined behavior**. Below are examples of the code changes required to use the new jsonStruct.

With a primitive data type, such as `int32_t`:

```c
...
jsonStruct_t exampleJsonStruct;
int32_t value = 0L;

/* Set the members of exampleJsonStruct. */
exampleJsonStruct.pKey = "...";
exampleJsonStruct.pData = &value;
exampleJsonStruct.type = SHADOW_JSON_INT32;
exampleJsonStruct.cb = exampleCallback;

/* Register a delta callback using example JsonStruct. */
dsd_iot_shadow_register_delta(&mqttClient, &exampleJsonStruct);
"..."
```

Version 1.0.0 will require the following code:

```c
...
jsonStruct_t exampleJsonStruct;
int32_t value = 0L;

/* Set the members of exampleJsonStruct. */
exampleJsonStruct.pKey = "...";
exampleJsonStruct.pData = &value;
exampleJsonStruct.dataLength = sizeof(int32_t); /* sizeof(value) also OK.*/
exampleJsonStruct.type = SHADOW_JSON_INT32;
exampleJsonStruct.cb = exampleCallback;

/* Register a delta callback using example JsonStruct. */
dsd_iot_shadow_register_delta(&mqttClient, &exampleJsonStruct);
...
```

With a string, version 1.0.0 will require the following code:

```c
...
jsonStruct_t exampleJsonStruct;
char stringBuffer[SIZE_OF_BUFFER];
/* Set the members of exampleJsonStruct. */
exampleJsonStruct.pKey = "...";
exampleJsonStruct.pData = stringBuffer;
exampleJsonStruct.dataLength = SIZE_OF_BUFFER;
exampleJsonStruct.type = SHADOW_JSON_STRING;
exampleJsonStruct.cb = exampleCallback;
/* Register a delta callback using example JsonStruct. */
dsd_iot_shadow_register_delta(&mqttClient, &exampleJsonStruct);
...
```

### Changes in parseStringValue:
The function `parseStringValue`, declared in [include/dsd_iot_json_utils.h](include/dsd_iot_json_utils.h#L179) and implemented in [src/dsd_iot_json_utils.c](src/dsd_iot_json_utils.c#L184), now requires the inclusion of a buffer length. Its new function signature is:

```c
IoT_Error_t parseStringValue(char *buf, size_t bufLen, const char *jsonString, jsmntok_t *token);
```

Below is an example of code changes required to use the new parseStringValue.

Version 1.0.0 will require the following code:

```c
...
char* jsonString = "...";
jsmntok_t jsmnTokens[NUMBER_OF_JSMN_TOKENS];
char stringBuffer[SIZE_OF_BUFFER];
parseStringValue(stringBuffer, SIZE_OF_BUFFER, jsonString, jsmnTokens);
...
```

### Changes to functions intended for internal usage:
Version 1.0.0 changes the signature of four functions intended for internal usage. The new signatures explicitly carry the information for the size of the buffer or JSON document passed as a parameter to the functions. Users of the SDK may need to change their code and recompile to ingest the changes. We report the old and new signatures below.

#### Old signatures:

```c
bool extractClientToken(const char *pJsonDocument, char *pExtractedClientToken);

static void emptyJsonWithClientToken(char *pBuffer);

bool isJsonValidAndParse(const char *pJsonDocument, void *pJsonHandler, int32_t *pTokenCount);

bool isReceivedJsonValid(const char *pJsonDocument);
```

#### New signatures:

```c
bool extractClientToken(const char *pJsonDocument, size_t jsonSize, char *pExtractedClientToken, size_t clientTokenSize);

static void emptyJsonWithClientToken(char *pBuffer, size_t bufferSize);

bool isJsonValidAndParse(const char *pJsonDocument, size_t jsonSize, void *pJsonHandler, int32_t *pTokenCount);

bool isReceivedJsonValid(const char *pJsonDocument, size_t jsonSize);
```

## Resources
[API Documentation](doc/index.html)

[MQTT 3.1.1 Spec](http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/csprd02/mqtt-v3.1.1-csprd02.html)

## Support
If you have any technical questions about DSD IoT Device SDK, use the [DSD IoT forum](https://dasudian.com/cn/service_iot.html).
For any other questions on DSD IoT, contact [DSD Support](mailto:support@dasudian.com).

## Sample APIs
Connecting to the DSD IoT MQTT platform

```
#define DSD_IOT_MQTT_HOST              "datahub.dsdiot.com" ///< Customer specific MQTT HOST. Make sure with Dasudian Tech team
#define DSD_IOT_MQTT_PORT              8883 ///< default port for MQTT/S
#define DSD_IOT_MQTT_CLIENT_ID         "{customer_prefix}|{deviceType|{deviceID}" ///< MQTT client ID should be unique for every device, asking Dasudian team for the customer prefix
#define DSD_IOT_MY_THING_NAME          "PLC-S7-200" ///< Thing Name of the Shadow this device is associated with
#define DSD_IOT_MY_USER_NAME           "{username}" ///Username associated with the connection credential, ask Dasudian Team for the info
#define DSD_IOT_MY_PASSWORD            "{password}" ///Password associated with the connection credential, ask Dasudian Team for the info
#define DSD_IOT_ROOT_CA_FILENAME       "rootCA.crt" ///< Root CA file name
#define DSD_IOT_CERTIFICATE_FILENAME   "cert.pem" ///< device signed certificate file name
#define DSD_IOT_PRIVATE_KEY_FILENAME   "privkey.pem" ///< Device private key filename

IoT_Client_Init_Params iotInitParams = iotClientInitParamsDefault;
IoT_Client_Connect_Params iotConnectParams = dsd_init_conn_params(DSD_IOT_MY_USER_NAME, DSD_IOT_MY_PASSWORD);
DSD_IoT_Client client;

rc = dsd_iot_mqtt_init(&client, &iotInitParams);
rc = dsd_iot_mqtt_connect(&client, &iotConnectParams);
```


Subscribe to a topic

```
DSD_IoT_Client client;
rc = dsd_iot_mqtt_subscribe(&client, "sdkTest/sub", 11, QOS0, iot_subscribe_callback_handler, NULL);
```


Update Thing Shadow from a device

```
rc = dsd_iot_shadow_update(&mqttClient, DSD_IOT_MY_THING_NAME, pJsonDocumentBuffer, ShadowUpdateStatusCallback,
                            pCallbackContext, TIMEOUT_4SEC, persistenSubscription);
```
