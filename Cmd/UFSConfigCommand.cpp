#include "UFSConfigCommand.h"

#include "../Err/Exception.h"
#include "../Logger/Log.h"
#include "../Utility/FileUtils.h"
#include "../UI/src/ICallback.h"
#include "../Utility/Utils.h"

namespace APCore
{

UFSConfigCommand::UFSConfigCommand(APKey key):
    ICommand(key),
    m_tw_no_reduction(true),
    m_tw_size_gb(0),
    m_hpb_size_gb(0),
    m_lu3_size_mb(0),
    m_lu3_type(0)
{}

UFSConfigCommand::~UFSConfigCommand()
{}

void UFSConfigCommand::exec(const QSharedPointer<Connection> &conn)
{
    conn->ConnectDA();

    LOG("executing UFS Config Command...");

    UFS_Config cfg;
    cfg.force_provision = 1;
    cfg.tw_size_gb = m_tw_size_gb;
    cfg.tw_no_red = m_tw_no_reduction;
    cfg.hpb_size_gb = m_hpb_size_gb;
    cfg.lu3_size_mb = m_lu3_size_mb;
    cfg.lu3_type = m_lu3_type;

    LOG("force_provison: %d", cfg.force_provision);
    LOG("tw_no_reduction: %d", cfg.tw_no_red);
    LOG("tw_size_gb: 0x%x", cfg.tw_size_gb);
    LOG("hpb_size_gb: 0x%x", cfg.hpb_size_gb);
    LOG("lu3_size_mb: 0x%x", cfg.lu3_size_mb);
    LOG("lu3_type: %d", cfg.lu3_type);

    int ret = FlashTool_SetUFSConfig(conn->FTHandle(), &cfg);
    if( S_DONE != ret )
    {
         LOG("ERROR: FlashTool_SetUFSConfig() failed, error code: %s(%d)!", StatusToString(ret), ret);
         THROW_BROM_EXCEPTION(ret, 0);
    }
}

}
