/* A RapidNet application. Generated by RapidNet compiler. */

#ifndef PKTFWDNORMDISTHLISTRMITMAUXTABLESQUERY_H
#define PKTFWDNORMDISTHLISTRMITMAUXTABLESQUERY_H

#include <string>
#include <iostream>
#include "ns3/ptr.h"
#include "ns3/event-id.h"
#include "ns3/ipv4-address.h"
#include "ns3/rapidnet-header.h"
#include "ns3/relation-base.h"
#include "ns3/database.h"
#include "ns3/rapidnet-application-base.h"
#include "ns3/aggregator.h"
#include "ns3/aggwrap.h"

using namespace std;
using namespace ns3;

namespace ns3 {

class Socket;
class Packet;

namespace rapidnet {
namespace pktfwdnormdisthlistrmitmauxtablesquery {

class PktfwdNormDistHlistRmitmAuxtablesQuery : public RapidNetApplicationBase
{
public:
  static const string EPACKET;
  static const string EPACKETTEMP;
  static const string ERECVPACKET;
  static const string ERECVPACKETTEMP;
  static const string FLOWENTRY;
  static const string INITPACKET;
  static const string LINK;
  static const string PACKET;
  static const string PROV;
  static const string PROVQUERY;
  static const string PROVRESULT;
  static const string PROVSTR;
  static const string RQUERY;
  static const string RRETURN;
  static const string RRETURNSIG;
  static const string RECORDRULE;
  static const string RECVPACKET;
  static const string RULEEXEC;
  static const string RULEINFO;
  static const string SLOWCHANGEQUERY;
  static const string TQUERY;
  static const string TRETURN;
  static const string TUPLEITR;

  static TypeId GetTypeId (void);

  PktfwdNormDistHlistRmitmAuxtablesQuery ();

  virtual ~PktfwdNormDistHlistRmitmAuxtablesQuery ();

protected:

  virtual void DoDispose (void);

  virtual void StartApplication (void);

  virtual void StopApplication (void);

  virtual void InitDatabase (void);

  virtual void DemuxRecv (Ptr<Tuple> tuple);

  virtual void Rr01Eca0Ins (Ptr<Tuple> recordRule);

  virtual void Rr01Eca0Del (Ptr<Tuple> recordRule);

  virtual void Rr02Eca0Ins (Ptr<Tuple> recordRule);

  virtual void Rr02Eca0Del (Ptr<Tuple> recordRule);

  virtual void Rr03Eca0Ins (Ptr<Tuple> recordRule);

  virtual void Rr03Eca0Del (Ptr<Tuple> recordRule);

  virtual void Prov_r00Eca0Ins (Ptr<Tuple> initPacket);

  virtual void Prov_r00Eca0Del (Ptr<Tuple> initPacket);

  virtual void Prov_r01Eca0Ins (Ptr<Tuple> link);

  virtual void Prov_r01Eca0Del (Ptr<Tuple> link);

  virtual void Prov_r03Eca0Ins (Ptr<Tuple> flowEntry);

  virtual void Prov_r03Eca0Del (Ptr<Tuple> flowEntry);

  virtual void Prov_rs1_1_eca (Ptr<Tuple> epacket);

  virtual void Prov_rs1_2_eca (Ptr<Tuple> epacketTemp);

  virtual void Prov_rs1_3_eca (Ptr<Tuple> epacketTemp);

  virtual void Prov_rs1_4_eca (Ptr<Tuple> epacket);

  virtual void Prov_rh1_1Eca0Ins (Ptr<Tuple> initPacket);

  virtual void Prov_rh1_1Eca1Ins (Ptr<Tuple> flowEntry);

  virtual void Prov_rh1_1Eca2Ins (Ptr<Tuple> link);

  virtual void Prov_rh2_1_eca (Ptr<Tuple> epacket);

  virtual void Prov_rh2_2_eca (Ptr<Tuple> erecvPacketTemp);

  virtual void Prov_rh2_3_eca (Ptr<Tuple> erecvPacketTemp);

  virtual void Prov_rh2_5_eca (Ptr<Tuple> erecvPacket);

  virtual void Prov_rh2_4_eca (Ptr<Tuple> erecvPacket);

  virtual void Rqini_eca (Ptr<Tuple> provQuery);

  virtual void Rqrec_eca (Ptr<Tuple> rQuery);

  virtual void Rqsc1_eca (Ptr<Tuple> rQuery);

  virtual void Rqsc2Eca0Ins (Ptr<Tuple> slowChangeQuery);

  virtual void Rqsc3_eca (Ptr<Tuple> tupleItr);

  virtual void Rqsc4_eca (Ptr<Tuple> tupleItr);

  virtual void Rqsc5_eca (Ptr<Tuple> tQuery);

  virtual void Rqrt1_eca (Ptr<Tuple> rQuery);

  virtual void Rqrt2_eca (Ptr<Tuple> rQuery);

  virtual void Rqrt3_eca (Ptr<Tuple> tReturn);

  virtual void Rqrt4_eca (Ptr<Tuple> rReturn);

  virtual void Rqrt5Eca0Ins (Ptr<Tuple> provResult);

  virtual void Rqrt6_eca (Ptr<Tuple> rReturnSig);

};

} // namespace pktfwdnormdisthlistrmitmauxtablesquery
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMDISTHLISTRMITMAUXTABLESQUERY_H
