#ifndef _TRAN_IMP_H_
#define _TRAN_IMP_H_

#include <map>
#include <string>
#include <vector>
#include "servant/Application.h"

/////////////////////////////////////////////////////////////////////////
class TranImp : public tars::Servant {

public:
    /**
     * 构造函数
     */
    TranImp();

public:
    /**
     * 对象初始化
     */
    virtual void initialize();

    /**
     * 处理客户端的主动请求
     * @param current 
     * @param response 
     * @return int 
     */
    virtual int doRequest(tars::CurrentPtr current, std::vector<char> &response);

    /**
     * @param resp 
     * @return int 
     */
    virtual int doResponse(tars::ReqMessagePtr resp);

    /**
     * 对象销毁
     */
    virtual void destroy();

protected:
    void async_call(tars::CurrentPtr current, std::vector<char> &response);
protected:
	tars::ServantPrx _servantPrx;

};
///////////////////////////////////////////////////////////////////////////////
#endif
