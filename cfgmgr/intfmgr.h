#ifndef __INTFMGR__
#define __INTFMGR__

#include "dbconnector.h"
#include "producerstatetable.h"
#include "orch.h"
#include "netmsg.h"

#include <map>
#include <string>
#include <set>

namespace swss {

class IntfMgr : public Orch, public NetMsg
{
public:
    IntfMgr(DBConnector *cfgDb, DBConnector *appDb, DBConnector *stateDb, const std::vector<std::string> &tableNames);
    using Orch::doTask;
    virtual void onMsg(int nlmsg_type, struct nl_object *obj);

private:
    ProducerStateTable m_appIntfTableProducer;
    Table m_cfgIntfTable, m_cfgVlanIntfTable, m_cfgLagIntfTable, m_cfgLoopbackIntfTable;
    Table m_statePortTable, m_stateLagTable, m_stateVlanTable, m_stateVrfTable, m_stateIntfTable;
    Table m_cfgIntfTable, m_cfgVlanIntfTable, m_cfgLagIntfTable;

    std::set<std::string> m_subIntfList;
    std::set<std::string> m_loopbackIntfList;
    std::set<std::string> m_pendingReplayIntfList;

    void setIntfIp(const std::string &alias, const std::string &opCmd, const IpPrefix &ipPrefix);
    void setIntfVrf(const std::string &alias, const std::string &vrfName);
    void setIntfMac(const std::string &alias, const std::string &macAddr);
    bool setIntfMpls(const std::string &alias, const std::string &mpls);

    bool doIntfGeneralTask(const std::vector<std::string>& keys, std::vector<FieldValueTuple> data, const std::string& op);
    bool doIntfAddrTask(const std::vector<std::string>& keys, const std::vector<FieldValueTuple>& data, const std::string& op);
    void doTask(Consumer &consumer);

    bool isIntfStateOk(const std::string &alias);
    bool isIntfCreated(const std::string &alias);
    bool isIntfChangeVrf(const std::string &alias, const std::string &vrfName);
    int getIntfIpCount(const std::string &alias);
    void buildIntfReplayList(void);
    void setWarmReplayDoneState();

    void addLoopbackIntf(const std::string &alias);
    void delLoopbackIntf(const std::string &alias);
    void flushLoopbackIntfs(void);

    void addHostSubIntf(const std::string&intf, const std::string &subIntf, const std::string &vlan);
    void setHostSubIntfMtu(const std::string &subIntf, const std::string &mtu);
    void setHostSubIntfAdminStatus(const std::string &subIntf, const std::string &admin_status);
    void removeHostSubIntf(const std::string &subIntf);
    void setSubIntfStateOk(const std::string &alias);
    void removeSubIntfState(const std::string &alias);

    bool setIntfProxyArp(const std::string &alias, const std::string &proxy_arp);
    bool setIntfGratArp(const std::string &alias, const std::string &grat_arp);

    int getIntfIpv6DisableMode(const std::string &alias);
    bool ifManualIPv6AddrsConfigured(const std::string &alias);
    bool isIpv6LinkLocalModeEnabled(const std::string &alias);
    void setIntfIpv6Mode(const std::string &port, const std::string &mode);

    bool m_replayDone {false};
};

}

#endif
