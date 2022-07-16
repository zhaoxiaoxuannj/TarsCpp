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

#ifndef __TARS2CPP_H_
#define __TARS2CPP_H_

#include "parse.h"

#include <cassert>
#include <string>

using namespace tars;

/**
 * 根据tars生成c++文件
 * 包括结构的编解码以及生成Proxy和Servant
 */
class Tars2Cpp
{
public:

    Tars2Cpp();

    /**
     * 生成
     * @param file
     */
    void createFile(const std::string &file);//, const std::vector<std::string> &vsCoder);

    /**
     * 设置生成文件的目录
     *
     */
    void setBaseDir(const std::string & sPath) { _baseDir = sPath; }

    /**
     * 设置是否需要打包默认值
     */
    void setCheckDefault(bool bCheck) { _checkDefault = bCheck; }

    /**
     * 根据命令选项设置是否需要生成sql支持
     */
    void setSqlSupport(bool bSqlSupport) { _bSqlSupport  = bSqlSupport;}

    /**
     * 根据命令选项设置是否需要生成xml支持
     */
    void setXmlSupport(bool bXmlSupport, const std::vector<std::string>& vXmlIntf)
    {
        _bXmlSupport = bXmlSupport;
        _vXmlIntf = vXmlIntf;
    }

    /**
     * 根据命令选项设置是否需要生成json支持
     */
    void setJsonSupport(bool bJsonSupport) { _bJsonSupport = bJsonSupport; }

    /**
     * 根据命令选项设置是否需要生成json支持
     */
    void setJsonSupport(const std::vector<std::string>& vJsonIntf) { _vJsonIntf = vJsonIntf; }

    /**
     * 设置是否只生成struct
     */
    void setOnlyStruct(bool bOnlyStruct) { _onlyStruct = bOnlyStruct; }

    /**
     * setUnknownField for UnknownField support
     */
    // void setUnknownField(bool bUnkownField) { _unknownField = bUnkownField; }

    /**
     * added for master info
     */
    void setTarsMaster(bool bTarsMaster) { _tarsMaster = bTarsMaster; }

    /**
    * 调用链追踪
    * @param bTrace
    */
    void setTrace(bool bTrace) { _bTrace = bTrace; }

    //下面是编解码的源码生成
protected:
    /**
     * 生成xml
     * @param pPtr
     *
     * @return std::string
     */
	std::string writeToXml(const TypeIdPtr &pPtr) const;

    /**
     * 生成xml
     * @param pPtr
     *
     * @return std::string
     */
	std::string readFromXml(const TypeIdPtr &pPtr, bool bIsRequire = true) const;

    /**
     * 生成sql
     * @param pPtr
     *
     * @return std::string
     */
	std::string writeToSql(const TypeIdPtr &pPtr) const;

    /**
     * 生成sql
     * @param pPtr
     *
     * @return std::string
     */
	std::string readFromSql(const TypeIdPtr &pPtr, bool bIsRequire = true) const;

    /**
     * 生成json
     * @param pPtr
     *
     * @return std::string
     */
    std::string writeToJson(const TypeIdPtr& pPtr) const;

    /**
     * 生成json
     * @param pPtr
     *
     * @return std::string
     */
    std::string readFromJson(const TypeIdPtr& pPtr, bool bIsRequire = true) const;

    /**
     * 生成某类型的解码源码
     * @param pPtr
     *
     * @return std::string
     */
    std::string writeTo(const TypeIdPtr &pPtr) const;

    /**
     * 生成某类型的编码源码
     * @param pPtr
     *
     * @return std::string
     */
    std::string readFrom(const TypeIdPtr &pPtr, bool bIsRequire = true) const;

    /**
     * 生成某类型的编码源码
     * @param pPtr
     *
     * @return std::string
     */
    std::string promiseReadFrom(const TypeIdPtr &pPtr, bool bIsRequire = true) const;

    /**
     *
     * @param pPtr
     *
     * @return std::string
     */
//    std::string readUnknown(const TypeIdPtr &pPtr) const;
    /**
     *
     * @param pPtr
     *
     * @return std::string
     */
    // std::string writeUnknown()const;
    /**
     *
     * @param pPtr
     *
     * @return std::string
     */
    std::string display(const TypeIdPtr &pPtr) const;

    /**
     *
     * @param pPtr
     *
     * @return std::string
     */
    std::string displaySimple(const TypeIdPtr &pPtr, bool bSep) const;

   /**
     *
     * @param pPtr
     *
     * @return std::string
     */
    std::string generateCollection(const TypeIdPtr &pPtr, bool bSep) const;

    //下面是类型描述的源码生成
protected:
    /**
     * 生成某类型的字符串描述源码
     * @param pPtr
     *
     * @return std::string
     */
    std::string tostr(const TypePtr &pPtr) const;

    /**
     * 生成内建类型的字符串源码
     * @param pPtr
     *
     * @return std::string
     */
    std::string tostrBuiltin(const BuiltinPtr &pPtr) const;
    /**
     * 生成vector的字符串描述
     * @param pPtr
     *
     * @return std::string
     */
    std::string tostrVector(const VectorPtr &pPtr) const;

    /**
     * 生成map的字符串描述
     * @param pPtr
     *
     * @return std::string
     */
    std::string tostrMap(const MapPtr &pPtr) const;

    /**
     * 生成某种结构的符串描述
     * @param pPtr
     *
     * @return std::string
     */
    std::string tostrStruct(const StructPtr &pPtr) const;

    /**
     * 生成某种枚举的符串描述
     * @param pPtr
     *
     * @return std::string
     */
    std::string tostrEnum(const EnumPtr &pPtr) const;

    /**
     * 获取定长数组的坐标
     * @param pPtr
     *
     * @return std::string
     */

    std::string toStrSuffix(const TypeIdPtr &pPtr) const;

    /**
     * 获取定长数组的坐标
     * @param pPtr
     *
     * @return int
     */
    int getSuffix(const TypeIdPtr &pPtr) const;

    /**
     * 生成unsigned转换为signed的符串描述
     * @param pPtr
     *
     * @return std::string
     */
    //string unsigned2signedStr(const BuiltinPtr &pPtr) const;

    /**
     * 生成类型变量的解码源码
     * @param pPtr
     *
     * @return std::string
     */
    std::string decode(const TypeIdPtr &pPtr) const;

    /**
     * 生成类型变量的编码源码
     * @param pPtr
     *
     * @return std::string
     */
    std::string encode(const TypeIdPtr &pPtr) const;

    //以下是h文件的具体生成
protected:
    /**
     * 结构的md5
     * @param pPtr
     *
     * @return std::string
     */
    std::string MD5(const StructPtr &pPtr) const;

    /**
     * 生成结构的头文件内容
     * @param pPtr
     *
     * @return std::string
     */
    std::string generateH(const StructPtr &pPtr, const std::string& namespaceId) const;

    /**
     * 生成容器的头文件源码
     * @param pPtr
     *
     * @return std::string
     */
    std::string generateH(const ContainerPtr &pPtr) const;

    /**
     * 生成参数声明的头文件内容
     * @param pPtr
     *
     * @return std::string
     */
    std::string generateH(const ParamDeclPtr &pPtr) const;

    /**
     *
     * @param pPtr
     *
     * @return std::string
     */
    std::string generateOutH(const ParamDeclPtr &pPtr) const;

    /**
     *
     * @param pPtr
     * @param cn
     *
     * @return std::string
     */
    std::string generateAsyncResponseCpp(const OperationPtr &pPtr, const std::string &cn) const;

    /**
     * 生成参数声明的内容
     * @param pPtr
     *
     * @return std::string
     */
    std::string generateParamDecl(const ParamDeclPtr &pPtr) const;

    /**
     * 生成操作函数调用分发的源码
     * @param pPtr
     * @param cn
     *
     * @return std::string
     */
    std::string generateDispatchAsync(const OperationPtr &pPtr, const std::string &cn) const;

    /**
     * 生成操作函数调用分发的源码
     * @param pPtr
     * @param cn
     *
     * @return std::string
     */
    std::string generateDispatchCoroAsync(const OperationPtr &pPtr, const std::string &cn) const;

    /**
     * 生成servant操作函数调用的源码
     * @param pPtr
     * @param cn
     *
     * @return std::string
     */
    std::string generateServantDispatch(const OperationPtr& pPtr, const std::string& cn) const;

    /**
     * 生成操作的servant的头文件源码
     * @param pPtr
     * @param bVirtual
     *
     * @return std::string
     */
    std::string generateHAsync(const OperationPtr &pPtr) const;

    /**
     * 生成操作的servant的头文件源码
     * @param pPtr
     * @param bVirtual
     *
     * @return std::string
     */
    std::string generateHAsync(const OperationPtr &pPtr, const std::string& interfaceId) const;

    /**
     * 生成操作的servant的头文件源码
     * @param pPtr
     * @param bVirtual
     *
     * @return std::string
     */
    std::string generateHPromiseAsync(const InterfacePtr &pInter, const OperationPtr &pPtr) const;

    /**
     * 生成操作函数调用分发的源码
     * @param pPtr
     * @param cn
     *
     * @return std::string
     */
    std::string generateDispatchPromiseAsync(const OperationPtr &pPtr, const std::string &cn) const;

    /**
     * 生成操作的servant的头文件源码
     * @param pPtr
     * @param bVirtual
     *
     * @return std::string
     */
    std::string generateH(const OperationPtr &pPtr, bool bVirtual, const std::string& interfaceId) const;

    /**
     * 生成接口的头文件源码
     * @param pPtr
     *
     * @return std::string
     */
    std::string generateH(const InterfacePtr &pPtr, const NamespacePtr &nPtr) const;

    /**
     * 生成枚举的头文件源码
     * @param pPtr
     *
     * @return std::string
     */
    std::string generateH(const EnumPtr &pPtr) const;

    /**
     * 生成常量头文件源码
     * @param pPtr
     *
     * @return std::string
     */
    std::string generateH(const ConstPtr &pPtr) const;

    /**
     * 生成名字空间的头文件源码
     * @param pPtr
     *
     * @return std::string
     */
    std::string generateH(const NamespacePtr &pPtr) const;

    /**
     * 生成名字空间的头文件源码
     * @param pPtr
     *
     * @return std::string
     */
    void generateH(const ContextPtr &pPtr) const;

    /**
     *
     * 根据名字查找结构
     * @param pPtr
     * @param id
     *
     * @return StructPtr
     */
    StructPtr findStruct(const ContextPtr &pPtr,const std::string &id);

    // /**
    //  *
    //  * 生成接口编解码代码
    //  * @param pPtr
    //  * @param interface
    //  */
    // void generateCoder(const ContextPtr &pPtr,const std::string &interface) const;

    // std::string generateCoder(const NamespacePtr &pPtr,const std::string & sInterface) const;

    // std::string generateCoder(const InterfacePtr &pPtr) const;

    // std::string generateCoder(const OperationPtr &pPtr) const;

    std::string generateInitValue(const TypeIdPtr &pPtr) const;

    bool isPromiseDispatchInitValue(const TypeIdPtr &pPtr) const;

private:
    std::string _baseDir;

    bool _checkDefault;

    bool _onlyStruct;

    bool _bSqlSupport;

    bool _bXmlSupport;

    bool _bJsonSupport;

    std::vector<std::string>  _vJsonIntf;
    std::vector<std::string>  _vXmlIntf;


    std::string _namespace ;

    // bool _unknownField;

    bool _tarsMaster;

    bool _bTrace;
};

#endif
