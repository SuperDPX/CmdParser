#ifndef CMD_PARSER_HPP
#define CMD_PARSER_HPP

#include <map>
#include <regex>
#include <string>
#include <iostream>

#define         OPT_NO_OPT                  0x01
#define         OPT_SHORT_ARG               0x02
#define         OPT_LONG_NO_SUFFIX          0x03
#define         OPT_LONG_SUFFIX             0x04


#define         REGEX_SHORT_ARG             "(-)([A-Za-z0-9]+)"
#define         REGEX_LONG_NO_SUFFIX        "-{1,2}([A-Za-z0-9]+)"
#define         REGEX_LONG_SUFFIX           "-{1,2}([A-Za-z0-9]+)=([^=]+)"


class CmdParser
{
private:
    std::map<std::string,std::string> m_args_map;
    int optionType(std::string);
    static std::regex reg_short_arg;
    static std::regex reg_long_no_suffix;
    static std::regex reg_long_suffix;
public:
    CmdParser(/* args */){}
    ~CmdParser(){}
public:
    void parse(int argc,char * const * argv);
    void printArgsMap();
    bool contains(std::string);
    std::string getOptionValue(std::string opt);
};

std::regex CmdParser::reg_short_arg = std::regex(REGEX_SHORT_ARG);
std::regex CmdParser::reg_long_no_suffix = std::regex(REGEX_LONG_NO_SUFFIX);
std::regex CmdParser::reg_long_suffix = std::regex(REGEX_LONG_SUFFIX);

int CmdParser::optionType(std::string arg)
{
    if(std::regex_match(arg,CmdParser::reg_short_arg))
        return OPT_SHORT_ARG;
    if(std::regex_match(arg,CmdParser::reg_long_no_suffix))
        return OPT_LONG_NO_SUFFIX;
    if(std::regex_match(arg,CmdParser::reg_long_suffix))
        return OPT_LONG_SUFFIX;
    
    return OPT_NO_OPT;

}


void CmdParser::parse(int argc,char * const * argv)
{
    for(int i = 1;i<argc;i++)
    {
        if(OPT_NO_OPT == optionType(argv[i])){
            continue;
        }
        if(OPT_SHORT_ARG == optionType(argv[i])){
            if(i + 1 < argc){
                if(OPT_NO_OPT == optionType(argv[i+1]))
                    this->m_args_map.emplace(std::regex_replace(std::string(argv[i]),CmdParser::reg_short_arg,"$2"),std::string(argv[i+1]));
                else
                    this->m_args_map.emplace(std::regex_replace(std::string(argv[i]),CmdParser::reg_short_arg,"$2"),std::string(""));
            }else{
                this->m_args_map.emplace(std::regex_replace(std::string(argv[i]),CmdParser::reg_short_arg,"$2"),std::string(""));
            }
            continue;
        }
        if(OPT_LONG_NO_SUFFIX == optionType(argv[i]))
        {
            if(i + 1 < argc){
                if(OPT_NO_OPT == optionType(argv[i+1]))
                    this->m_args_map.emplace(std::regex_replace(std::string(argv[i]),CmdParser::reg_long_no_suffix,"$1"),std::string(argv[i+1]));
                else
                    this->m_args_map.emplace(std::regex_replace(std::string(argv[i]),CmdParser::reg_long_no_suffix,"$1"),std::string(""));
            }else{
                this->m_args_map.emplace(std::regex_replace(std::string(argv[i]),CmdParser::reg_long_no_suffix,"$1"),std::string(""));
            }
            continue;
        }
        if(OPT_LONG_SUFFIX == optionType(argv[i]))
        {
            this->m_args_map.emplace(std::regex_replace(std::string(argv[i]),CmdParser::reg_long_suffix,"$1"),std::regex_replace(std::string(argv[i]),CmdParser::reg_long_suffix,"$2"));
            continue;
        }
    }
}
void CmdParser::printArgsMap()
{
    if(this->m_args_map.empty())
    {
        std::cout<<"Cmd has no arguments"<<std::endl;
    }
    auto it = this->m_args_map.begin();
    
    while(it != this->m_args_map.end())
    {
        std::cout<<"Option: "<<it->first<<" | Value: "<<it->second<<std::endl;
        it++;
    }
}

bool CmdParser::contains(std::string opt)
{
    auto it = this->m_args_map.find(opt);
    if(it == this->m_args_map.end())
    {
        return false;
    }else{
        return true;
    }
}
std::string CmdParser::getOptionValue(std::string opt)
{
    auto it = this->m_args_map.find(opt);
    if(it != this->m_args_map.end())
    {
        return it->second;
    }else{
        return "";
    }
}

#endif
