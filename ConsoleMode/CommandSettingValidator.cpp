#include "CommandSettingValidator.h"
#include <QFileInfo>
#include <QDir>
#include "../Utility/version.h"

#define RSC_FILE_NAME "rsc.xml"

ConsoleMode::CommandSettingValidator::CommandSettingValidator(const ConsoleMode::Config &config,
                                                              const CommandLineArguments &args):
    m_config(config), m_args(args)
{

}

ConsoleMode::CommandSettingValidator::~CommandSettingValidator()
{

}

bool ConsoleMode::CommandSettingValidator::Validate() const
{
    return this->ValidateRSCSetting();
}

bool ConsoleMode::CommandSettingValidator::hasRSCXMLFile() const
{
    QString rsc_file = getRSCFileName();

    //check rsc.xml exist or not
    return QFileInfo(rsc_file).exists();
}

bool ConsoleMode::CommandSettingValidator::ValidateRSCSetting() const
{
    if (ToolInfo::IsCustomerVer())
    {
        //not support rsc mechanism in customer version
        //so quit if there has rsc cmd in config.xml(download/format/readback) or cmd arguments, no matter rsc.xml has or not.
        if (m_config.pclGetCommandSetting()->hasRSCCmdSetting())
        {
            LOGE("NOT support rsc mechanism in customer tool!");
            return false;
        }
        else
        {
            return true;
        }
    }

    if (!m_config.pclGetCommandSetting()->hasDownloadCmd()) {
        if (m_config.pclGetCommandSetting()->hasRSCCmdSetting()) {
            LOGI("rsc commnad exist, but ignore it for it's not a download command!");
        }
        return true;
    }

    //RSC only for internal flashtool    
    if (this->hasRSCXMLFile()) {
        if (m_config.pclGetCommandSetting()->hasRSCCmdSetting())
        {
            LOGI("rsc file and rsc commnad both exist!");
            if (this->validRSCIndex())
            {
                return true;
            }
            else
            {
                LOGI("invalid rsc_index in rsc commnad!");
                return false;
            }
        }
        else
        {
            LOGE("rsc file exist, but has no rsc command!");
            return false;
        }
    } else {
        if (m_config.pclGetCommandSetting()->hasRSCCmdSetting())
        {
            LOGW("rsc file NOT exist, and has rsc command, but ignore it!");
        }
        else
        {
            LOGI("rsc file NOT exist, and has no rsc command!!");
        }
        return true;
    }
}

QString ConsoleMode::CommandSettingValidator::getRSCFileName() const
{
    QString sScatterFile = QString::fromStdString(m_config.pclGetGeneralSetting()->pclGetGeneralArg()->scatter_file);
    QString rsc_dir = QFileInfo(sScatterFile).absolutePath();
    QString rsc_file = rsc_dir + QDir::separator().toLatin1() + RSC_FILE_NAME;
    LOG("rsc file: %s", rsc_file.toStdString().c_str());
    return rsc_file;
}

bool ConsoleMode::CommandSettingValidator::validRSCIndex() const
{
    unsigned int rsc_count = 0;
    QString rsc_file = getRSCFileName();
    unsigned int ret = FlashTool_GetRSCCnt(rsc_file.toStdString().c_str(), &rsc_count);
    if(STATUS_OK != ret)
    {
        char err_msg[512] = {0};
        FlashTool_GetLastErrorMessage(NULL, err_msg);
        LOGE("FlashTool_GetRSCCnt() failed! ret: %s(0x%x), error_msg: %s", StatusToString(ret), ret, err_msg);
        return false;
    }
    QSharedPointer<APCore::RSCSetting> rsc_setting = m_config.pclGetCommandSetting()->getRSCSetting();
    if (rsc_setting.isNull())
    {
        return false;
    }
    unsigned int rsc_index = rsc_setting->getRSCIndex();
    return 0 <= rsc_index && rsc_index < rsc_count;
}
