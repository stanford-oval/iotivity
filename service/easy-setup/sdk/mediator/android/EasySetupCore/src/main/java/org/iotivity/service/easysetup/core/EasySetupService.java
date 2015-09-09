/**
 * ***************************************************************
 * <p/>
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 * <p/>
 * <p/>
 * <p/>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p/>
 * http://www.apache.org/licenses/LICENSE-2.0
 * <p/>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * <p/>
 * ****************************************************************
 */

package org.iotivity.service.easysetup.core;

import android.content.Context;

import org.iotivity.service.easysetup.impl.EnrolleeDeviceFactory;
import org.iotivity.service.easysetup.mediator.EasySetupManager;

import java.io.IOException;
import java.util.ArrayList;

/**
 * This is facade class, a single point of contact for Application.
 * It contains set of APIs to do easy setup of the enrolling device.
 * ON-BOARDING - This is a step to establish connectivity between the device & Mediator device.
 * PROVISION   - This is a step where the netowork's detail & credentials are given to the
 * enrolling device.
 */
public class EasySetupService {

    private static EasySetupService sInstance;

    private final EasySetupStatus mCallback;

    private ArrayList<EnrolleeDevice> mEnrolleeDeviceList;

    private final ProvisioningCallback mProvisioningCallback;

    private static Context mContext;

    public EnrolleeDeviceFactory mDeviceFactory;

    private EasySetupService(EasySetupStatus callback) {
        mCallback = callback;
        mProvisioningCallback = new ProvisioningCallbackImpl(mCallback);
        mEnrolleeDeviceList = new ArrayList<EnrolleeDevice>();
        mContext = null;
        mDeviceFactory = null;

        //Native Api call to initialize the OIC stack
        EasySetupManager.getInstance().initEasySetup();
    }

    /**
     * Gives a singleton instance of Easy setup service and initialize the service
     *
     * @param callback Application needs to provide this callback to receive the status of easy
     *                 setup process.
     */

    public synchronized static EasySetupService getInstance(Context context, EasySetupStatus
            callback) {
        if (sInstance == null) {
            sInstance = new EasySetupService(callback);
            mContext = context;
        }
        return sInstance;
    }

    /**
     * Reset the Easy setup Service
     */

    public void finish() {
        //Native Api call to reset OIC stack
        EasySetupManager.getInstance().terminateEasySetup();
    }

    /**
     * Starts Easy setup process for the enrolling device.
     *
     * @param enrolledevice Device to be enrolled in network
     * @throws IOException Throws exception in case of any connection error.
     */

    public synchronized void startSetup(final EnrolleeDevice enrolledevice) throws IOException {

        mEnrolleeDeviceList.add(enrolledevice);

        // Starts the provisioning directly if the device is already on boarded on the network.
        if (enrolledevice.onBoarded()) {
            enrolledevice.startProvisioning(mProvisioningCallback);
            return;
        }

        enrolledevice.startOnBoarding(new OnBoardingCallback() {

            @Override
            public void onFinished(OnBoardingConnection connection) {
                if (connection.isConnected()) {
                    try {
                        Thread.sleep(15000);//Sleep for allowing thin device to start the services
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    // Start provisioning here
                    enrolledevice.setConnection(connection);
                    enrolledevice.startProvisioning(mProvisioningCallback);
                } else {
                    enrolledevice.mState = EnrolleeState.DEVICE_PROVISIONING_FAILED_STATE;
                    mProvisioningCallback.onFinished(enrolledevice);
                }

            }

        });

    }

    /**
     * Stops on-going Easy setup process for enrolling device.
     *
     * @param enrolleedevice Device to be enrolled in network
     */
    public synchronized void stopSetup(EnrolleeDevice enrolleedevice) {
        enrolleedevice.stopOnBoardingProcess();
        mEnrolleeDeviceList.remove(enrolleedevice);

        //Native Api call to stop on-going enrolling process for the enrolling device
        EasySetupManager.getInstance().stopEnrolleeProvisioning(enrolleedevice.mOnBoardingConfig.getConnType().getValue());
    }

    public synchronized void getEnrolleeDevice(OnBoardingConfig connType) {
        mDeviceFactory = EnrolleeDeviceFactory.newInstance(mContext);
    }

    class ProvisioningCallbackImpl extends ProvisioningCallback {

        private final EasySetupStatus mCallback;

        ProvisioningCallbackImpl(EasySetupStatus callback) {
            mCallback = callback;
        }

        @Override
        public void onFinished(EnrolleeDevice enrolledevice) {
            //if(mEnrolleeDeviceList.contains(enrolledevice)) {
            mCallback.onFinished(enrolledevice);
            //}
        }

    }


}