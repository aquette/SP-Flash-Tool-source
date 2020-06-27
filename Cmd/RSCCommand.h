#ifndef RSCCOMMAND_H
#define RSCCOMMAND_H

#include "ICommand.h"

namespace APCore
{

class RSCCommand : public ICommand
{
public:
    RSCCommand(APKey key);
    virtual ~RSCCommand();

    virtual void exec(const QSharedPointer<APCore::Connection> &conn);

    void setRSCIndex(unsigned int index)
    {
        m_rsc_index = index;
    }

    void setRSCProjectName(std::string proj_name)
    {
        m_rsc_proj_name = proj_name;
    }

    void setRSCOperatorName(std::string op_name)
    {
        m_rsc_operator_name = op_name;
    }

	void setRSCFilePath(std::string rsc_path)
    {
        m_rsc_file_path = rsc_path;
    }

private:
    unsigned int m_rsc_index;
    std::string m_rsc_proj_name;
    std::string m_rsc_operator_name;
    std::string m_rsc_file_path;
};

}

#endif // RSCCOMMAND_H
