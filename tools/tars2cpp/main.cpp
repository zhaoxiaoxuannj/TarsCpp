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

#include "util/tc_option.h"
#include "util/tc_file.h"
#include "util/tc_common.h"
#include "tars2cpp.h"

using namespace std;
using namespace tars;

void usage()
{
    std::cout << "Usage : tars2cpp [OPTION] tarsfile" << std::endl;
    // std::cout << "  --coder=Demo::interface1;Demo::interface2   create interface encode and decode api"  << std::endl;
    std::cout << "  --dir=DIRECTORY                             generate source file to DIRECTORY(create tars protocol file to DIRECTORY, default is current directory)"  << std::endl;
    std::cout << "  --check-default=<true,false>                optional field with default value not do package(default: true)"  << std::endl;
    std::cout << "  --unjson                                    not json interface"  << std::endl;
    std::cout << "  --os                                        only create struct(not create interface) "  << std::endl;
    std::cout << "  --include=\"dir1;dir2;dir3\"                set search path of tars protocol"  << std::endl;
    // std::cout << "  --unknown                                   create unkown field"  << std::endl;
    std::cout << "  --tarsMaster                                create get registry info interface"  << std::endl;
    std::cout << "  --currentPriority						   use current path first."  << std::endl;
    std::cout << "  --without-trace                             不需要调用链追踪逻辑"  << std::endl;
    std::cout << "  tars2cpp support type: bool byte short int long float double vector map"  << std::endl;
    exit(0);
}

void check(std::vector<std::string> &vTars)
{
    for(size_t i  = 0; i < vTars.size(); i++)
    {
        std::string ext  = tars::TC_File::extractFileExt(vTars[i]);
        if(ext == "tars")
        {
            if(!tars::TC_File::isFileExist(vTars[i]))
            {
                std::cerr << "file '" << vTars[i] << "' not exists"  << std::endl;
				usage();
                exit(0);
            }
        }
        else
        {
            std::cerr << "only support tars file."  << std::endl;
            exit(0);
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        usage();
    }

    tars::TC_Option option;
    option.decode(argc, argv);
    std::vector<std::string> vTars = option.getSingle();

    check(vTars);

    if(option.hasParam("help"))
    {
        usage();
    }

    // bool bCoder = option.hasParam("coder");
    // vector<std::string> vCoder;
    // if(bCoder)
    // {
    //     vCoder = tars::TC_Common::sepstr<std::string>(option.getValue("coder"), ";", false);
    //     if(vCoder.size() == 0)
    //     {
    //         usage();
    //         return 0;
    //     }
    // }

    Tars2Cpp t2c;

    if (option.hasParam("dir"))
    {
        t2c.setBaseDir(option.getValue("dir"));
    }
    else
    {
        t2c.setBaseDir(".");
    }

    t2c.setCheckDefault(tars::TC_Common::lower(option.getValue("check-default")) == "false"?false:true);

    t2c.setOnlyStruct(option.hasParam("os"));

    //默认支持json
    t2c.setJsonSupport(true);

    if (option.hasParam("unjson"))
    {
        t2c.setJsonSupport(false);
    }

    if (option.hasParam("sql"))
    {
        t2c.setSqlSupport(true);
        t2c.setJsonSupport(true);
    }

    // 调用链追踪
    if (option.hasParam("without-trace"))
    {
        t2c.setTrace(false);
    }
    else
    {
        t2c.setTrace(true);
    }

    if (option.hasParam("xml"))
    {
        vector<std::string> vXmlIntf;
        string sXml = tars::TC_Common::trim(option.getValue("xml"));
        sXml = tars::TC_Common::trimleft(tars::TC_Common::trimright(sXml, "]"), "[");
        if (!sXml.empty())
        {
            vXmlIntf = tars::TC_Common::sepstr<std::string>(sXml, ",", false);
        }
        t2c.setXmlSupport(true, vXmlIntf);
    }

    // if (option.hasParam("json"))
    // {
    //     t2c.setJsonSupport(true);
    //     string sJson = tars::TC_Common::trim(option.getValue("json"));
    //     sJson = tars::TC_Common::trimleft(tars::TC_Common::trimright(sJson, "]"), "[");
    //     if (!sJson.empty())
    //     {
    //         t2c.setJsonSupport(tars::TC_Common::sepstr<std::string>(sJson, ",", false));
    //     }
    // }

    t2c.setTarsMaster(option.hasParam("tarsMaster"));

    try
    {
        //增加include搜索路径
        g_parse->addIncludePath(option.getValue("include"));

        //是否可以以tars开头
        g_parse->setTars(option.hasParam("with-tars"));
        g_parse->setHeader(option.getValue("header"));
        g_parse->setCurrentPriority(option.hasParam("currentPriority"));

        // t2c.setUnknownField(option.hasParam("unknown"));
        for(size_t i = 0; i < vTars.size(); i++)
        {

            g_parse->parse(vTars[i]);
            t2c.createFile(vTars[i]);//, vCoder);
        }
    }
    catch(exception& e)
    {
	    cerr<<e.what()<<endl;
    }

    return 0;
}

