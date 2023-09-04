/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */

#ifndef  _TC_LOOP_QUEUE_H_
#define  _TC_LOOP_QUEUE_H_

#include <vector>
#include <atomic>
#include <stdlib.h>
#include <string.h>

namespace tars
{

using namespace std;

/////////////////////////////////////////////////
/**
 * @file tc_loop_queue.h 
 * @brief 循环队列,大小固定 . 
 * @brief Circular queue, fixed size.
 *  
 */
/////////////////////////////////////////////////

template<typename T>
class TC_LoopQueue
{
public:
    typedef vector<T> queue_type;

    TC_LoopQueue(size_t iSize)
        : _iBegin(0)
        , _iEnd(0)
    {
        //做个保护 最多不能超过 1000000
        //Make a protection. No more than 1000000.
        // assert(iSize<1000000);
        _iCapacitySub = iSize;
        _iCapacity = iSize + 1;
        _p=(T*)malloc(_iCapacity*sizeof(T));
        //_p= new T[_iCapacity];
    }
    ~TC_LoopQueue()
    {
        free(_p);
        //delete _p;
    }

    bool push_back(const T &t,bool & bEmpty, size_t & iBegin, size_t & iEnd)
    {
        bEmpty = false;
        //uint32_t iEnd = _iEnd;
        iBegin = _iBegin.load(std::memory_order_relaxed);
        iEnd = _iEnd.load(std::memory_order_acquire);
        if((iEnd > iBegin && iEnd - iBegin < 2) ||
                ( iBegin > iEnd && iBegin - iEnd > (_iCapacity-2) ) )
        {
            return false;
        }
        else
        { 
            memcpy(_p+iBegin,&t,sizeof(T));
            //*(_p+_iBegin) = t;

            if(iEnd == iBegin)
                bEmpty = true;

            if(iBegin == _iCapacitySub)
                iBegin = 0;
            else
                iBegin++;
            
            _iBegin.store(iBegin, std::memory_order_release);

            if(!bEmpty && 1 == size(iBegin, iEnd))
                bEmpty = true;

            return true;
        }
    }

    bool push_back(const T &t,bool & bEmpty)
    {
        bEmpty = false;
        size_t iBegin = _iBegin.load(std::memory_order_relaxed);
        size_t iEnd = _iEnd.load(std::memory_order_acquire);
        if((iEnd > iBegin && iEnd - iBegin < 2) ||
                ( iBegin > iEnd && iBegin - iEnd > (_iCapacity-2) ) )
        {
            return false;
        }
        else
        { 
            memcpy(_p+iBegin,&t,sizeof(T));
            //*(_p+_iBegin) = t;

            if(iEnd == iBegin)
                bEmpty = true;

            if(iBegin == _iCapacitySub)
                iBegin = 0;
            else
                iBegin++;
            
            _iBegin.store(iBegin, std::memory_order_release);

            if(!bEmpty && 1 == size(iBegin, iEnd))
                bEmpty = true;
#if 0
            if(1 == size())
                bEmpty = true;
#endif

            return true;
        }
    }

    bool push_back(const T &t)
    {
        bool bEmpty;
        return push_back(t,bEmpty);
    }

    bool push_back(const queue_type &vt)
    {
        size_t iBegin = _iBegin.load(std::memory_order_relaxed);
        size_t iEnd = _iEnd.load(std::memory_order_acquire);
        if(vt.size()>(_iCapacity-1) ||
                (iEnd>iBegin && (iEnd-iBegin)<(vt.size()+1)) ||
                ( iBegin>iEnd && (iBegin-iEnd)>(_iCapacity-vt.size()-1) ) )
        {
            return false;
        }
        else
        { 
            for(size_t i=0;i<vt.size();i++)
            {
                memcpy(_p+iBegin,&vt[i],sizeof(T));
                //*(_p+_iBegin) = vt[i];
                if(iBegin == _iCapacitySub)
                    iBegin = 0;
                else
                    iBegin++;
            }
            _iBegin.store(iBegin, std::memory_order_release);
            return true;
        }
    }

    bool pop_front(T &t)
    {
        size_t iEnd = _iEnd.load(std::memory_order_relaxed);
        size_t iBegin = _iBegin.load(std::memory_order_acquire);
        if(iEnd==iBegin)
        {
            return false;
        }
        memcpy(&t,_p+iEnd,sizeof(T));
        //t = *(_p+_iEnd);
        if(iEnd == _iCapacitySub)
            iEnd = 0;
        else
            iEnd++;
        _iEnd.store(iEnd, std::memory_order_release);    
        return true;
    }

    bool pop_front()
    {
        size_t iEnd = _iEnd.load(std::memory_order_relaxed);
        size_t iBegin = _iBegin.load(std::memory_order_acquire);
        if(iEnd==iBegin)
        {
            return false;
        }
        if(iEnd == _iCapacitySub)
            iEnd = 0;
        else
            iEnd++;
        _iEnd.store(iEnd, std::memory_order_release);    
        return true;
    }

    bool get_front(T &t)
    {
        size_t iEnd = _iEnd.load(std::memory_order_relaxed);
        size_t iBegin = _iBegin.load(std::memory_order_relaxed);
        if(iEnd==iBegin)
        {
            return false;
        }
        memcpy(&t,_p+iEnd,sizeof(T));
        //t = *(_p+_iEnd);
        return true;
    }

    bool empty()
    {
        return _iEnd.load(std::memory_order_relaxed) == _iBegin.load(std::memory_order_relaxed);
    }

	size_t size()
    {
		size_t iBegin=_iBegin.load(std::memory_order_relaxed);
		size_t iEnd=_iEnd.load(std::memory_order_relaxed);
        return size(iBegin, iEnd);
    }

    size_t size(size_t iBegin, size_t iEnd)
    {
        if(iBegin<iEnd)
            return iBegin+_iCapacity-iEnd;
        return iBegin-iEnd;
    }

	size_t getCapacity()
    {
        return _iCapacity;
    }

private:
    // cache_line_size - sizeof(size_t)
    static const size_t padding_size = 64 - sizeof(size_t);
	std::atomic<size_t> _iBegin;
    char padding1[padding_size]; /* force _iBegin and _iEnd to different cache lines */
	std::atomic<size_t> _iEnd;
    T * _p;
    size_t _iCapacity;
	size_t _iCapacitySub;
};

}

#endif   /* ----- #ifndef _TC_LOOP_QUEUE_H_ ----- */

