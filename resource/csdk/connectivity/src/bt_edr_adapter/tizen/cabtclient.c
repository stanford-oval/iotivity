/******************************************************************
 *
 * Copyright 2014 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/**
 * @file    cabtclient.c
 * @brief   This    file provides the APIs to establish RFCOMM connection with remote bluetooth device
 */

#include "cabtclient.h"
#include "caadapterutils.h"
#include "cabtutils.h"
#include "logger.h"

CAResult_t CABTClientConnect(const char *remoteAddress, const char *serviceUUID)
{
    OIC_LOG_V(DEBUG, BLUETOOTH_ADAPTER_TAG, "IN");

    int err = BT_ERROR_NONE;

    VERIFY_NON_NULL(remoteAddress, BLUETOOTH_ADAPTER_TAG, "Remote address is null");
    VERIFY_NON_NULL(serviceUUID, BLUETOOTH_ADAPTER_TAG, "Service UUID is null");

    if (0 >= strlen(remoteAddress))
    {
        OIC_LOG_V(ERROR, BLUETOOTH_ADAPTER_TAG, "Invalid input: Empty remote address");
        return  CA_STATUS_INVALID_PARAM;
    }

    if (0 >= strlen(serviceUUID))
    {
        OIC_LOG_V(ERROR, BLUETOOTH_ADAPTER_TAG, "Invalid input: Empty service uuid");
        return  CA_STATUS_INVALID_PARAM;
    }

    if (BT_ERROR_NONE != (err = bt_socket_connect_rfcomm(remoteAddress, serviceUUID)))
    {
        OIC_LOG_V(ERROR, BLUETOOTH_ADAPTER_TAG,
                  "Failed to connect!, address [%s] uuid [%s] error num [%x]",
                  remoteAddress, serviceUUID, err);
        return CA_STATUS_FAILED;
    }

    OIC_LOG_V(DEBUG, BLUETOOTH_ADAPTER_TAG, "OUT");
    return CA_STATUS_OK;
}

