#ifndef __UFSCONFIGSETTING_H__
#define __UFSCONFIGSETTING_H__

#include "ISetting.h"

namespace APCore
{

class UFSConfigSetting: public ISetting
{
public:
    typedef void (__stdcall * void_callback)();
    UFSConfigSetting();

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    void set_tw_no_reduction(bool tw_no_reduction)
    {
        m_tw_no_reduction = tw_no_reduction;
    }

    void set_tw_size_gb(uint tw_size_gb)
    {
        m_tw_size_gb = tw_size_gb;
    }

    void set_hpb_size_gb(uint hpb_size_gb)
    {
        m_hpb_size_gb = hpb_size_gb;
    }

    void set_lu3_size_mb(uint lu3_size_mb)
    {
        m_lu3_size_mb = lu3_size_mb;
    }

    void set_lu3_type(uint lu3_type)
    {
        m_lu3_type = lu3_type;
    }

private:
    bool m_tw_no_reduction;
    uint m_tw_size_gb;
    uint m_hpb_size_gb;
    uint m_lu3_size_mb;
    uint m_lu3_type;
};

}
#endif
