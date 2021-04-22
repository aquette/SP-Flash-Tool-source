#include "UFSConfigSetting.h"
#include "../Cmd/UFSConfigCommand.h"
#include "../XMLParser/XMLNode.h"

namespace APCore
{
UFSConfigSetting::UFSConfigSetting():
    m_tw_no_reduction(true),
    m_tw_size_gb(0),
    m_hpb_size_gb(0)
{

}

void UFSConfigSetting::LoadXML(const XML::Node &node)
{
    Q_UNUSED(node);
}
void UFSConfigSetting::SaveXML(XML::Node &node) const
{
    Q_UNUSED(node);
}

QSharedPointer<APCore::ICommand> UFSConfigSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::UFSConfigCommand> cmd(new APCore::UFSConfigCommand(key));
    cmd->set_tw_no_reduction(m_tw_no_reduction);
    cmd->set_tw_size_gb(m_tw_size_gb);
    cmd->set_hpb_size_gb(m_hpb_size_gb);
    cmd->set_lu3_size_mb(m_lu3_size_mb);
    cmd->set_lu3_type(m_lu3_type);

    return cmd;
}

}
