#ifndef COMMANDSETTINGVALIDATOR_H
#define COMMANDSETTINGVALIDATOR_H

#include "Config.h"
#include "CommandLineArguments.h"

namespace ConsoleMode
{

class CommandSettingValidator
{
public:
    CommandSettingValidator(const Config &config, const CommandLineArguments& args);
    virtual ~CommandSettingValidator();

    bool Validate() const;

    bool hasRSCXMLFile() const;

private:
    bool ValidateRSCSetting() const;
    QString getRSCFileName() const;
    bool validRSCIndex() const;

private:
    const Config &m_config;
    const CommandLineArguments& m_args;
};
}

#endif // COMMANDSETTINGVALIDATOR_H

