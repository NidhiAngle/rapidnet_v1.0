/* A RapidNet application. Generated by RapidNet compiler. */

#include "sdn-load-balancing.h"
#include <cstdlib>
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "ns3/type-ids.h"
#include "ns3/rapidnet-types.h"
#include "ns3/rapidnet-utils.h"
#include "ns3/assignor.h"
#include "ns3/selector.h"
#include "ns3/rapidnet-functions.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::sdnloadbalancing;

const string SdnLoadBalancing::DESIGNATED = "designated";
const string SdnLoadBalancing::HASHED = "hashed";
const string SdnLoadBalancing::INITPACKET = "initPacket";
const string SdnLoadBalancing::PACKET = "packet";
const string SdnLoadBalancing::RECVPACKET = "recvPacket";
const string SdnLoadBalancing::SERVERMAPPING = "serverMapping";

NS_LOG_COMPONENT_DEFINE ("SdnLoadBalancing");
NS_OBJECT_ENSURE_REGISTERED (SdnLoadBalancing);

TypeId
SdnLoadBalancing::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::sdnloadbalancing::SdnLoadBalancing")
    .SetParent<RapidNetApplicationBase> ()
    .AddConstructor<SdnLoadBalancing> ()
    ;
  return tid;
}

SdnLoadBalancing::SdnLoadBalancing()
{
  NS_LOG_FUNCTION_NOARGS ();
}

SdnLoadBalancing::~SdnLoadBalancing()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
SdnLoadBalancing::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::DoDispose ();
}

void
SdnLoadBalancing::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::StartApplication ();
  RAPIDNET_LOG_INFO("SdnLoadBalancing Application Started");
}

void
SdnLoadBalancing::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::StopApplication ();
  RAPIDNET_LOG_INFO("SdnLoadBalancing Application Stopped");
}

void
SdnLoadBalancing::InitDatabase ()
{
  //RapidNetApplicationBase::InitDatabase ();

  AddRelationWithKeys (DESIGNATED, attrdeflist (
    attrdef ("designated_attr1", IPV4),
    attrdef ("designated_attr2", IPV4)));

  AddRelationWithKeys (INITPACKET, attrdeflist (
    attrdef ("initPacket_attr1", IPV4),
    attrdef ("initPacket_attr2", IPV4),
    attrdef ("initPacket_attr3", IPV4)));

  AddRelationWithKeys (SERVERMAPPING, attrdeflist (
    attrdef ("serverMapping_attr1", IPV4),
    attrdef ("serverMapping_attr2", IPV4),
    attrdef ("serverMapping_attr3", INT32)));

}

void
SdnLoadBalancing::DemuxRecv (Ptr<Tuple> tuple)
{
  RapidNetApplicationBase::DemuxRecv (tuple);

  if (IsInsertEvent (tuple, INITPACKET))
    {
      R1Eca0Ins (tuple);
    }
  if (IsRecvEvent (tuple, PACKET))
    {
      R2_eca (tuple);
    }
  if (IsRecvEvent (tuple, HASHED))
    {
      R3_eca (tuple);
    }
  if (IsRecvEvent (tuple, PACKET))
    {
      R4_eca (tuple);
    }
}

void
SdnLoadBalancing::R1Eca0Ins (Ptr<Tuple> initPacket)
{
  RAPIDNET_LOG_INFO ("R1Eca0Ins triggered");

  Ptr<Tuple> result = initPacket;

  result = result->Project (
    PACKET,
    strlist ("initPacket_attr3",
      "initPacket_attr1",
      "initPacket_attr2",
      "initPacket_attr3"),
    strlist ("packet_attr1",
      "packet_attr2",
      "packet_attr3",
      RN_DEST));

  Send (result);
}

void
SdnLoadBalancing::R2_eca (Ptr<Tuple> packet)
{
  RAPIDNET_LOG_INFO ("R2_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (DESIGNATED)->Join (
    packet,
    strlist ("designated_attr1"),
    strlist ("packet_attr1"));

  result->Assign (Assignor::New ("Value",
    FHashIP::New (
      VarExpr::New ("packet_attr2"))));

  result->Assign (Assignor::New ("ServerNum",
    Operation::New (RN_PLUS,
      ValueExpr::New (Int32Value::New (1)),
      FModulo::New (
        VarExpr::New ("Value"),
        ValueExpr::New (Int32Value::New (5))))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("designated_attr2"),
      VarExpr::New ("packet_attr3"))));

  result = result->Project (
    HASHED,
    strlist ("packet_attr1",
      "packet_attr2",
      "ServerNum"),
    strlist ("hashed_attr1",
      "hashed_attr2",
      "hashed_attr3"));

  SendLocal (result);
}

void
SdnLoadBalancing::R3_eca (Ptr<Tuple> hashed)
{
  RAPIDNET_LOG_INFO ("R3_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (SERVERMAPPING)->Join (
    hashed,
    strlist ("serverMapping_attr1", "serverMapping_attr3"),
    strlist ("hashed_attr1", "hashed_attr3"));

  result = result->Project (
    RECVPACKET,
    strlist ("serverMapping_attr2",
      "hashed_attr2",
      "serverMapping_attr2"),
    strlist ("recvPacket_attr1",
      "recvPacket_attr2",
      RN_DEST));

  Send (result);
}

void
SdnLoadBalancing::R4_eca (Ptr<Tuple> packet)
{
  RAPIDNET_LOG_INFO ("R4_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (DESIGNATED)->Join (
    packet,
    strlist ("designated_attr1"),
    strlist ("packet_attr1"));

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("packet_attr3"),
      VarExpr::New ("designated_attr2"))));

  result = result->Project (
    RECVPACKET,
    strlist ("packet_attr3",
      "packet_attr2",
      "packet_attr3"),
    strlist ("recvPacket_attr1",
      "recvPacket_attr2",
      RN_DEST));

  Send (result);
}
