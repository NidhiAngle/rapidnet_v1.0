/* A RapidNet application. Generated by RapidNet compiler. */

#ifndef PKTFWDNORMBDFAIRPROVCOMPSTRAWMAN_H
#define PKTFWDNORMBDFAIRPROVCOMPSTRAWMAN_H

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
namespace pktfwdnormbdfairprovcompstrawman {

class PktfwdNormBdfairProvCompStrawman : public RapidNetApplicationBase
{
public:
  static const string DEVICE;
  static const string ERECVPACKETCOUNT;
  static const string EPACKETCOUNT;
  static const string EPACKETTEMP;
  static const string ERECVPACKET;
  static const string ERECVPACKETTEMP;
  static const string FLOWENTRY;
  static const string INITPACKET;
  static const string LINK;
  static const string LINKHR;
  static const string PACKETP;
  static const string RECVAUXPKT;
  static const string RECVPACKET;
  static const string RULEEXEC;

  static TypeId GetTypeId (void);

  PktfwdNormBdfairProvCompStrawman ();

  virtual ~PktfwdNormBdfairProvCompStrawman ();

protected:

  virtual void DoDispose (void);

  virtual void StartApplication (void);

  virtual void StopApplication (void);

  virtual void InitDatabase (void);

  virtual void DemuxRecv (Ptr<Tuple> tuple);

  virtual void Prov_rs1_1_eca (Ptr<Tuple> packetP);

  virtual void Prov_rs1_2_eca (Ptr<Tuple> epacketTemp);

  virtual void Prov_rs1_3_eca (Ptr<Tuple> epacketCount);

  virtual void Prov_rs1_4_eca (Ptr<Tuple> epacketTemp);

  virtual void Prov_rh1_1Eca0Ins (Ptr<Tuple> device);

  virtual void Prov_rh1_1Eca1Ins (Ptr<Tuple> initPacket);

  virtual void Prov_rh1_1Eca2Ins (Ptr<Tuple> linkhr);

  virtual void Prov_rh2_1_eca (Ptr<Tuple> packetP);

  virtual void Prov_rh2_2_eca (Ptr<Tuple> erecvPacketTemp);

  virtual void Prov_rh2_3_eca (Ptr<Tuple> eRecvPacketCount);

  virtual void Prov_rh2_4_eca (Ptr<Tuple> erecvPacketTemp);

  virtual void Prov_rh2_5_eca (Ptr<Tuple> erecvPacket);

  virtual void Prov_rh2_6_eca (Ptr<Tuple> erecvPacket);

};

} // namespace pktfwdnormbdfairprovcompstrawman
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMBDFAIRPROVCOMPSTRAWMAN_H
