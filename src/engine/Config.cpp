#include <cstdio>
#include <iostream>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/error/en.h"

#include "engine/Config.h"
#include "engine/common/Common.h"

namespace ds
{
Config::Config()
{
    m_isLoaded = false;
}

bool Config::LoadFile(const std::string &filePath)
{
    bool result = false;

    rapidjson::Document tmp;
    // Clear previous document
    m_document.Swap(tmp);

    FILE *fp = fopen(filePath.c_str(), "r");

    // If we could open file
    if (fp != nullptr)
    {
        // Get the size of the file
        fseek(fp, 0, SEEK_END);
        long int fileSize = ftell(fp);

        fseek(fp, 0, SEEK_SET);

        // Read file in
        char *readBuffer = (char *)malloc(fileSize);
        rapidjson::FileReadStream fstream(fp, readBuffer, fileSize);

        // Attempt to parse file stream
        rapidjson::ParseResult parseResult = m_document.ParseStream(fstream);
        if (parseResult == false)
        {
            fprintf(stderr, "JSON parse error: %s (%zu)\n",
                    rapidjson::GetParseError_En(parseResult.Code()),
                    parseResult.Offset());
        }
        else
        {
            // Parsing was successful
            result = true;
        }

        // Close file and free memory
        fclose(fp);
        free(readBuffer);
    }

    // Update our status
    m_isLoaded = result;

    return result;
}

bool Config::IsLoaded() const
{
    return m_isLoaded;
}

rapidjson::Value::ConstMemberIterator
Config::GetDocumentMember(const std::string &key) const
{
    rapidjson::Value::ConstMemberIterator result = m_document.MemberEnd();

    // Get tokens
    std::vector<std::string> tokens = ds_com::TokenizeString('.', key);

    // Searching a document is different to searching a value, so do first
    // step manually.
    std::vector<std::string>::const_iterator tokenIt = tokens.begin();
    rapidjson::Value::ConstMemberIterator docIt =
        m_document.FindMember(tokenIt->c_str());

    // If we found a value
    if (docIt != m_document.MemberEnd())
    {
        // Analyze next token
        ++tokenIt;

        // If we have no more tokens to process
        if (tokenIt == tokens.end())
        {
            // We're done
            result = docIt;
        }
        // If we have more tokens to process
        else
        {
            // Does the document iterator currently point to an indexable
            // object?
            if (docIt->value.IsObject())
            {
                // Attempt to find next member
                rapidjson::Value::ConstMemberIterator valIt =
                    docIt->value.FindMember(tokenIt->c_str());
                // Iterator end for value just searched
                rapidjson::Value::ConstMemberIterator endIt =
                    docIt->value.MemberEnd();

                // For the rest of the tokens to process
                while (tokenIt != tokens.end())
                {
                    // Did we find the token?
                    if (valIt != endIt)
                    {
                        // If no more tokens to process, success
                        if (tokenIt + 1 == tokens.end())
                        {
                            tokenIt = tokens.end();
                            result = valIt;
                        }
                        // If more tokens to process
                        else
                        {
                            // Is the current value itr pointing to an object?
                            if (valIt->value.IsObject())
                            {
                                // Continue processing
                                ++tokenIt;
                                endIt = valIt->value.MemberEnd();
                                valIt =
                                    valIt->value.FindMember(tokenIt->c_str());
                            }
                            else
                            {
                                std::cerr << "Config::GetDocumentMember: "
                                             "Warning: value '";
                                for (std::vector<std::string>::const_iterator
                                         tokenPrint = tokens.begin();
                                     tokenPrint != tokenIt + 1; ++tokenPrint)
                                {
                                    std::cerr << *tokenPrint << ". ";
                                }
                                std::cerr << "' is not an object, it's members "
                                             "can't be accessed."
                                          << std::endl;
                                break;
                            }
                        }
                    }
                    else
                    {
                        std::cerr << "Config::GetDocumentMember: Warning: "
                                     "Could not find '";
                        std::vector<std::string>::const_iterator tokenPrint =
                            tokens.begin();
                        std::cerr << *tokenPrint;
                        ++tokenPrint;
                        for (; tokenPrint != tokenIt + 1; ++tokenPrint)
                        {
                            std::cerr << "." << *tokenPrint;
                        }
                        std::cerr << "' in config file." << std::endl;
                        break;
                    }
                }
            }
            // If not, error
            else
            {
                std::cerr
                    << "Config::GetDocumentMember: Warning: '" << *(tokenIt - 1)
                    << "' is not an object, it's members can't be accessed."
                    << std::endl;
            }
        }
    }
    else
    {
        std::cerr
            << "Config::GetDocumentMember: Warning: could not find member: '"
            << *(tokenIt) << "'" << std::endl;
    }

    return result;
}

bool Config::GetUnsignedInt(const std::string &key, unsigned int *uint) const
{
    bool result = false;

    rapidjson::Value::ConstMemberIterator valueIt = GetDocumentMember(key);

    if (IsLoaded())
    {
        if (valueIt != m_document.MemberEnd())
        {
            if (valueIt->value.IsUint())
            {
                *uint = valueIt->value.GetUint();
                result = true;
            }
            else
            {
                std::cerr << "Config::GetUnsignedInt: Warning: '" << key
                          << "' is not an unsigned int." << std::endl;
            }
        }
    }

    return result;
}

bool Config::GetBool(const std::string &key, bool *boolean) const
{
    bool result = false;

    if (IsLoaded())
    {
        rapidjson::Value::ConstMemberIterator valueIt = GetDocumentMember(key);

        if (valueIt != m_document.MemberEnd())
        {
            if (valueIt->value.IsBool())
            {
                *boolean = valueIt->value.GetBool();
                result = true;
            }
            else
            {
                std::cerr << "Config::GetBool: Warning: '" << key
                          << "' is not a bool." << std::endl;
            }
        }
    }

    return result;
}

bool Config::GetString(const std::string &key, std::string *string) const
{
    bool result = false;

    if (IsLoaded())
    {
        rapidjson::Value::ConstMemberIterator valueIt = GetDocumentMember(key);

        if (valueIt != m_document.MemberEnd())
        {
            if (valueIt->value.IsString())
            {
                *string = valueIt->value.GetString();
                result = true;
            }
            else
            {
                std::cerr << "Config::GetString: Warning: '" << key
                          << "' is not a string." << std::endl;
            }
        }
    }

    return result;
}

std::vector<std::string> Config::GetObjectKeys(const std::string &key) const
{
    std::vector<std::string> contents;

    if (IsLoaded())
    {
        rapidjson::Value::ConstMemberIterator valueIt = GetDocumentMember(key);

        if (valueIt != m_document.MemberEnd())
        {
            if (valueIt->value.IsObject())
            {
                rapidjson::Value::ConstObject object =
                    valueIt->value.GetObject();

                rapidjson::Value::ConstMemberIterator it = object.MemberBegin();
                for (; it != object.MemberEnd(); ++it)
                {
                    contents.push_back(it->name.GetString());
                }
            }
            else
            {
                std::cerr << "Config::GetString: Warning: '" << key
                          << "' is not an object." << std::endl;
            }
        }
    }

    return contents;
}
}
