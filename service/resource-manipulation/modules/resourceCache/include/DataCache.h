//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef RCM_DATACACHE_H_
#define RCM_DATACACHE_H_

#include <list>
#include <string>
#include <memory>

#include "CacheTypes.h"
#include "ExpiryTimer.h"

namespace OIC
{
    namespace Service
    {
        class DataCache
        {
        public:
            DataCache();
            ~DataCache();

            void initializeDataCache(PrimitiveResourcePtr pResource);

            CacheID addSubscriber(CacheCB func, REPORT_FREQUENCY rf, long repeatTime);
            CacheID deleteSubscriber(CacheID id);

            CACHE_STATE getCacheState() const;
            const ResourceAttributes getCachedData() const;
            const PrimitiveResourcePtr getPrimitiveResource() const;

            void requestGet();

        private:
            // resource instance
            PrimitiveResourcePtr sResource;
            std::shared_ptr<BaseResource> baseHandler;

            // cached data info
            ResourceAttributes attributes;
            CACHE_STATE state;

            // subscriber info
            std::unique_ptr<SubscriberInfo> subscriberList;

            ExpiryTimer networkTimer;
            ExpiryTimer pollingTimer;
            TimerID networkTimeOutHandle;
            TimerID pollingHandle;

            ObserveCB pObserveCB;
            GetCB pGetCB;
            TimerCB pTimerCB;
            TimerCB pPollingCB;

            // for requestCB from base
            void onObserve(const HeaderOptions& _hos,
                    const ResponseStatement& _rep, int _result, int _seq);
            void onGet(const HeaderOptions& _hos, const ResponseStatement& _rep, int _result);
            void *onTimeOut(const unsigned int timerID);
            void *onPollingOut(const unsigned int timerID);

            CacheID generateCacheID();
            SubscriberInfoPair findSubscriber(CacheID id);
            void notifyObservers(ResourceAttributes Att);
        };
    } // namespace Service
} // namespace OIC

#endif /* RCM_DATACACHE_H_ */