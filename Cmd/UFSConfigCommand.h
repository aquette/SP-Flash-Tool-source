#ifndef __UFSCONFIG_COMMAND_H__
#define __UFSCONFIG_COMMAND_H__

#include "ICommand.h"
#include "../Conn/Connection.h"

namespace APCore
{

class UFSConfigCommand:public ICommand
{
public:
    UFSConfigCommand(APKey key);
    ~UFSConfigCommand();

    virtual void exec(const QSharedPointer<Connection> &conn);
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
