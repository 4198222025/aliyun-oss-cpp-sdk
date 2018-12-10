/*
 * Copyright 2009-2017 Alibaba Cloud All rights reserved.
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
 */


#include <alibabacloud/oss/model/UploadPartCopyResult.h>
#include <alibabacloud/oss/model/Owner.h>
#include <tinyxml2/tinyxml2.h>
#include "../utils/Utils.h"
using namespace AlibabaCloud::OSS;
using namespace tinyxml2;

static const std::string EMPTY;

UploadPartCopyResult::UploadPartCopyResult() :
    OssResult()
{
}

UploadPartCopyResult::UploadPartCopyResult(
    const std::string& result):
    UploadPartCopyResult()
{
    *this = result;
    
}

UploadPartCopyResult::UploadPartCopyResult(
    const std::shared_ptr<std::iostream>& result,
    const HeaderCollection &header):
    UploadPartCopyResult()
{
    headers = header;
    std::istreambuf_iterator<char> isb(*result.get()), end;
    std::string str(isb, end);
    *this = str;
    if (header.find("x-oss-request-id") != header.end()) {
        requestId_ = header.at("x-oss-request-id");
    }
}

UploadPartCopyResult& UploadPartCopyResult::operator =(
    const std::string& result)
{
    XMLDocument doc;
    XMLError xml_err;
    if ((xml_err = doc.Parse(result.c_str(), result.size())) == XML_SUCCESS) {
        XMLElement* root =doc.RootElement();
        if (root && !std::strncmp("CopyPartResult", root->Name(), 14)) {
            XMLElement *node;

            node = root->FirstChildElement("LastModified");
            if (node && node->GetText()) {
                lastModified_ = node->GetText();
            }

            node = root->FirstChildElement("ETag");
            if (node && node->GetText()) {
                eTag_ = TrimQuotes(node->GetText());
            }
            parseDone_ = true;
		}
    }
    return *this;
}

const std::string& UploadPartCopyResult::CopySourceIfMatch() const
{
    auto it = headers.find("x-oss-copy-source-if-match");
    if(it != headers.end())
    {
        return it->first;
    }
    return EMPTY;
}

const std::string& UploadPartCopyResult::CopySourceIfNoneMatch() const
{
    auto it = headers.find("x-oss-copy-source-if-none-match");
    if(it != headers.end())
    {
        return it->first;
    }
    return EMPTY;
}

const std::string& UploadPartCopyResult::CopySourceIfUnmodifiedSince() const
{
    auto it = headers.find("x-oss-copy-source-if-unmodified-since");
    if(it != headers.end())
    {
        return it->first;
    }
    return EMPTY;
}

const std::string& UploadPartCopyResult::CopySourceIfModifiedSince() const
{
    auto it = headers.find("x-oss-copy-source-if-modified-since");
    if(it != headers.end())
    {
        return it->first;
    }
    return EMPTY;
}

const std::string& UploadPartCopyResult::LastModified() const
{
    return lastModified_;
}
const std::string& UploadPartCopyResult::ETag() const
{
    return eTag_;
}
