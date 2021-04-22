#include "DLOnlyCommand.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/CoreHandle.h"
#include "../Resource/Handle/DLHandle.h"
#include "../Resource/Handle/RBHandle.h"

#include "../Flow/storhelper.h"
#include "../Flow/dlproxy.h"
#include "../Flow/defaultrom.h"
#include "../Utility/constString.h"
#include "../Public/RomInfoFactory.h"
#include "../Host/Inc/RuntimeMemory.h"
#include "../Logger/Log.h"
#include "../Err/Exception.h"
#include "../Err/FlashToolErrorCodeDef.h"
#include "../BootRom/flashtool_handle.h"
#include <algorithm>

namespace APCore
{

DLOnlyCommand::DLOnlyCommand(APKey key):
    ICommand(key),
    dl_setting_(NULL),
    key_(key)
{
}

DLOnlyCommand::~DLOnlyCommand()
{

}

bool DLOnlyCommand::CheckPMTLayoutChange(const QSharedPointer<Connection> &conn)
{
    bool ret = false;

    APCore::CoreHandle *core_handle = __POOL_OF_HANDLE(key_);
    if(core_handle == NULL)
    {
        THROW_APP_EXCEPTION(FT_INVALIDE_SESSION_KEY, "");
    }

    APCore::RBHandle *rb_handle = GET_RB_HANDLE(key_);

    CStorHelper storage(conn.data());

    CDLProxy proxy(conn, rb_handle, storage);

    std::list<ROM_INFO> rom_list;

    RomInfoFactory info_factory(key_);
    info_factory.GetRomList(rom_list);


    char scat_version[64]={0};
    DL_GetScatterVersion(GET_DL_HANDLE_T(key_), scat_version);
    LOGD("scatter version: %s", scat_version);

    SCATTER_Head_Info scatter_header_info;
    DL_GetScatterInfo(GET_DL_HANDLE_T(key_), &scatter_header_info);

    bool process_combo_check_done = false;
    bool is_pmt_changed = false;
    for(std::list<ROM_INFO>::const_iterator rom_it = rom_list.begin();
        rom_it != rom_list.end(); ++rom_it)
    {
        const PART_INFO *pmt =
                proxy.read_pmt(rom_it->name);
        if (NULL == pmt)
            continue;

        if(std::string(scat_version) == "V1.2.0" || std::string(scat_version) == "V2.2.0" ) //process combo_partsize_check part first
        {
            //only support two part with attr "combo_partisize_check"
            if(rom_it->combo_partsize_check && !process_combo_check_done)
            {
                Q_ASSERT(rom_it != rom_list.end());
                std::list<ROM_INFO>::const_iterator next_rom = rom_it;
                std::advance(next_rom,1);
                Q_ASSERT(next_rom->combo_partsize_check == _TRUE);

                U64 combo_begin_addr = rom_it->begin_addr;
                U64 combo_end_addr = next_rom->end_addr;

                U64 combo_pmt_begin_addr = pmt->begin_addr;
                const PART_INFO *next_pmt = proxy.read_pmt(next_rom->name);
                U64 combo_pmt_end_addr = next_pmt->begin_addr + next_pmt->image_length;

                if(combo_begin_addr != combo_pmt_begin_addr || combo_end_addr != combo_pmt_end_addr)
                {
                    ret = true;
                    LOGD("combo PMT chaned!");
                    LOGE("combo_begin_addr: 0x%llx, combo_end_addr: 0x%llx", combo_begin_addr, combo_end_addr);
                    LOGE("combo_pmt_begin_addr: 0x%llx, combo_pmt_end_addr: 0x%llx", combo_pmt_begin_addr, combo_pmt_end_addr);
                    break;
                }

                //skip next rom, for here already process it
                process_combo_check_done = true;
                ++rom_it;
                continue;
            }
        }

        std::string image_name = std::string(rom_it->name);
        std::transform(image_name.begin(), image_name.end(), image_name.begin(), tolower);
        if (image_name == "userdata" && 
		scatter_header_info.skip_pmt_operate && !scatter_header_info.resize_check)
        {
            is_pmt_changed = is_pmt_start_addr_changed(pmt, *rom_it);
        } 
        else
        {    
            is_pmt_changed = pmt_is_changed(pmt, *rom_it);
        }

        if (is_pmt_changed)
        {

            if((strstr(rom_it->name, "BMTPOOL")||strstr("BMTPOOL", rom_it->name))
                    && ((rom_it->begin_addr&0xFFFF)== 0))
            {
                continue;
            }
            else
            {
                LOGD("PMT changed for <%s>: addr<0x%llx>-->addr<0x%llx>, len<0x%llx>-->len<0x%llx>",
                        rom_it->name, pmt->begin_addr, rom_it->begin_addr, pmt->image_length, rom_it->partition_size);
                ret = true;
                break;
            }
        }

    }

    return ret;
}

void DLOnlyCommand::exec(const QSharedPointer<Connection> &conn)
{
    conn->ConnectDA();

    if(CheckPMTLayoutChange(conn) == true)
    {
        fw_throw_error(ERROR_ROM_MUST_ENABLE);
    }

    if(NULL != dl_setting_)
        dl_setting_->CreateCommand(conn->ap_key())->exec(conn);
}
}
