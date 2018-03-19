/* A RapidNet application. Generated by RapidNet compiler. */

#include "pktfwd-norm-prov-comp-strawman.h"
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
using namespace ns3::rapidnet::pktfwdnormprovcompstrawman;

const string PktfwdNormProvCompStrawman::DEVICE = "device";
const string PktfwdNormProvCompStrawman::ERECVPACKETCOUNT = "eRecvPacketCount";
const string PktfwdNormProvCompStrawman::EPACKETCOUNT = "epacketCount";
const string PktfwdNormProvCompStrawman::EPACKETTEMP = "epacketTemp";
const string PktfwdNormProvCompStrawman::ERECVPACKET = "erecvPacket";
const string PktfwdNormProvCompStrawman::ERECVPACKETTEMP = "erecvPacketTemp";
const string PktfwdNormProvCompStrawman::FLOWENTRY = "flowEntry";
const string PktfwdNormProvCompStrawman::INITPACKET = "initPacket";
const string PktfwdNormProvCompStrawman::LINK = "link";
const string PktfwdNormProvCompStrawman::LINKHR = "linkhr";
const string PktfwdNormProvCompStrawman::PACKET = "packet";
const string PktfwdNormProvCompStrawman::RECVAUXPKT = "recvAuxPkt";
const string PktfwdNormProvCompStrawman::RECVPACKET = "recvPacket";
const string PktfwdNormProvCompStrawman::RULEEXEC = "ruleExec";

NS_LOG_COMPONENT_DEFINE ("PktfwdNormProvCompStrawman");
NS_OBJECT_ENSURE_REGISTERED (PktfwdNormProvCompStrawman);

TypeId
PktfwdNormProvCompStrawman::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::pktfwdnormprovcompstrawman::PktfwdNormProvCompStrawman")
    .SetParent<RapidNetApplicationBase> ()
    .AddConstructor<PktfwdNormProvCompStrawman> ()
    ;
  return tid;
}

PktfwdNormProvCompStrawman::PktfwdNormProvCompStrawman()
{
  NS_LOG_FUNCTION_NOARGS ();
}

PktfwdNormProvCompStrawman::~PktfwdNormProvCompStrawman()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
PktfwdNormProvCompStrawman::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::DoDispose ();
}

void
PktfwdNormProvCompStrawman::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::StartApplication ();
  RAPIDNET_LOG_INFO("PktfwdNormProvCompStrawman Application Started");
}

void
PktfwdNormProvCompStrawman::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::StopApplication ();
  RAPIDNET_LOG_INFO("PktfwdNormProvCompStrawman Application Stopped");
}

void
PktfwdNormProvCompStrawman::InitDatabase ()
{
  //RapidNetApplicationBase::InitDatabase ();

  AddRelationWithKeys (DEVICE, attrdeflist (
    attrdef ("device_attr2", INT32)));

  AddRelationWithKeys (FLOWENTRY, attrdeflist (
    attrdef ("flowEntry_attr2", IPV4)));

  AddRelationWithKeys (INITPACKET, attrdeflist (
    attrdef ("initPacket_attr2", IPV4),
    attrdef ("initPacket_attr3", IPV4),
    attrdef ("initPacket_attr4", STR)));

  AddRelationWithKeys (LINK, attrdeflist (
    attrdef ("link_attr2", IPV4)));

  AddRelationWithKeys (LINKHR, attrdeflist (
    attrdef ("linkhr_attr2", IPV4)));

  AddRelationWithKeys (RECVAUXPKT, attrdeflist (
    attrdef ("recvAuxPkt_attr2", ID)));

  AddRelationWithKeys (RECVPACKET, attrdeflist (
    attrdef ("recvPacket_attr2", IPV4),
    attrdef ("recvPacket_attr3", IPV4),
    attrdef ("recvPacket_attr4", STR)));

  AddRelationWithKeys (RULEEXEC, attrdeflist (
    attrdef ("ruleExec_attr2", ID)));

}

void
PktfwdNormProvCompStrawman::DemuxRecv (Ptr<Tuple> tuple)
{
  RapidNetApplicationBase::DemuxRecv (tuple);

  if (IsRecvEvent (tuple, PACKET))
    {
      Prov_rs1_1_eca (tuple);
    }
  if (IsRecvEvent (tuple, EPACKETTEMP))
    {
      Prov_rs1_2_eca (tuple);
    }
  if (IsRecvEvent (tuple, EPACKETCOUNT))
    {
      Prov_rs1_3_eca (tuple);
    }
  if (IsRecvEvent (tuple, EPACKETTEMP))
    {
      Prov_rs1_4_eca (tuple);
    }
  if (IsInsertEvent (tuple, DEVICE))
    {
      Prov_rh1_1Eca0Ins (tuple);
    }
  if (IsInsertEvent (tuple, INITPACKET))
    {
      Prov_rh1_1Eca1Ins (tuple);
    }
  if (IsInsertEvent (tuple, LINKHR))
    {
      Prov_rh1_1Eca2Ins (tuple);
    }
  if (IsRecvEvent (tuple, PACKET))
    {
      Prov_rh2_1_eca (tuple);
    }
  if (IsRecvEvent (tuple, ERECVPACKETTEMP))
    {
      Prov_rh2_2_eca (tuple);
    }
  if (IsRecvEvent (tuple, ERECVPACKETCOUNT))
    {
      Prov_rh2_3_eca (tuple);
    }
  if (IsRecvEvent (tuple, ERECVPACKETTEMP))
    {
      Prov_rh2_4_eca (tuple);
    }
  if (IsRecvEvent (tuple, ERECVPACKET))
    {
      Prov_rh2_5_eca (tuple);
    }
  if (IsRecvEvent (tuple, ERECVPACKET))
    {
      Prov_rh2_6_eca (tuple);
    }
}

void
PktfwdNormProvCompStrawman::Prov_rs1_1_eca (Ptr<Tuple> packet)
{
  RAPIDNET_LOG_INFO ("Prov_rs1_1_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (DEVICE)->Join (
    packet,
    strlist ("device_attr1"),
    strlist ("packet_attr1"));

  result = GetRelation (FLOWENTRY)->Join (
    result,
    strlist ("flowEntry_attr2", "flowEntry_attr1"),
    strlist ("packet_attr3", "packet_attr1"));

  result = GetRelation (LINK)->Join (
    result,
    strlist ("link_attr2", "link_attr1"),
    strlist ("flowEntry_attr3", "packet_attr1"));

  result->Assign (Assignor::New ("PID1",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("device")),
          VarExpr::New ("packet_attr1")),
        VarExpr::New ("device_attr2")))));

  result->Assign (Assignor::New ("List",
    FAppend::New (
      VarExpr::New ("PID1"))));

  result->Assign (Assignor::New ("PID3",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            ValueExpr::New (StrValue::New ("flowEntry")),
            VarExpr::New ("packet_attr1")),
          VarExpr::New ("packet_attr3")),
        VarExpr::New ("flowEntry_attr3")))));

  result->Assign (Assignor::New ("List3",
    FAppend::New (
      VarExpr::New ("PID3"))));

  result->Assign (Assignor::New ("List",
    FConcat::New (
      VarExpr::New ("List"),
      VarExpr::New ("List3"))));

  result->Assign (Assignor::New ("PID4",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("link")),
          VarExpr::New ("packet_attr1")),
        VarExpr::New ("flowEntry_attr3")))));

  result->Assign (Assignor::New ("List4",
    FAppend::New (
      VarExpr::New ("PID4"))));

  result->Assign (Assignor::New ("List",
    FConcat::New (
      VarExpr::New ("List"),
      VarExpr::New ("List4"))));

  result->Assign (Assignor::New ("RLOC",
    VarExpr::New ("packet_attr1")));

  result->Assign (Assignor::New ("R",
    ValueExpr::New (StrValue::New ("rs1"))));

  result->Assign (Assignor::New ("RID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          VarExpr::New ("R"),
          VarExpr::New ("RLOC")),
        VarExpr::New ("List")))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("device_attr2"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Project (
    EPACKETTEMP,
    strlist ("RLOC",
      "flowEntry_attr3",
      "packet_attr2",
      "packet_attr3",
      "packet_attr4",
      "RID",
      "R",
      "List",
      "packet_attr5",
      "RLOC"),
    strlist ("epacketTemp_attr1",
      "epacketTemp_attr2",
      "epacketTemp_attr3",
      "epacketTemp_attr4",
      "epacketTemp_attr5",
      "epacketTemp_attr6",
      "epacketTemp_attr7",
      "epacketTemp_attr8",
      "epacketTemp_attr9",
      RN_DEST));

  Send (result);
}

void
PktfwdNormProvCompStrawman::Prov_rs1_2_eca (Ptr<Tuple> epacketTemp)
{
  RAPIDNET_LOG_INFO ("Prov_rs1_2_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (RULEEXEC)->Join (
    epacketTemp,
    strlist ("ruleExec_attr4", "ruleExec_attr3", "ruleExec_attr2", "ruleExec_attr1"),
    strlist ("epacketTemp_attr8", "epacketTemp_attr7", "epacketTemp_attr6", "epacketTemp_attr1"));

  result = AggWrapCount::New ()->Compute (result, epacketTemp);

  result = result->Project (
    EPACKETCOUNT,
    strlist ("epacketTemp_attr1",
      "epacketTemp_attr6",
      "epacketTemp_attr7",
      "epacketTemp_attr8",
      "count"),
    strlist ("epacketCount_attr1",
      "epacketCount_attr2",
      "epacketCount_attr3",
      "epacketCount_attr4",
      "epacketCount_attr5"));

  SendLocal (result);
}

void
PktfwdNormProvCompStrawman::Prov_rs1_3_eca (Ptr<Tuple> epacketCount)
{
  RAPIDNET_LOG_INFO ("Prov_rs1_3_eca triggered");

  Ptr<Tuple> result = epacketCount;

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("epacketCount_attr5"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Project (
    RULEEXEC,
    strlist ("epacketCount_attr1",
      "epacketCount_attr2",
      "epacketCount_attr3",
      "epacketCount_attr4"),
    strlist ("ruleExec_attr1",
      "ruleExec_attr2",
      "ruleExec_attr3",
      "ruleExec_attr4"));

  Insert (result);
}

void
PktfwdNormProvCompStrawman::Prov_rs1_4_eca (Ptr<Tuple> epacketTemp)
{
  RAPIDNET_LOG_INFO ("Prov_rs1_4_eca triggered");

  Ptr<Tuple> result = epacketTemp;

  result->Assign (Assignor::New ("Hash",
    FAppend::New (
      VarExpr::New ("epacketTemp_attr6"))));

  result->Assign (Assignor::New ("NewHashList",
    FConcat::New (
      VarExpr::New ("epacketTemp_attr9"),
      VarExpr::New ("Hash"))));

  result = result->Project (
    PACKET,
    strlist ("epacketTemp_attr2",
      "epacketTemp_attr3",
      "epacketTemp_attr4",
      "epacketTemp_attr5",
      "NewHashList",
      "epacketTemp_attr2"),
    strlist ("packet_attr1",
      "packet_attr2",
      "packet_attr3",
      "packet_attr4",
      "packet_attr5",
      RN_DEST));

  Send (result);
}

void
PktfwdNormProvCompStrawman::Prov_rh1_1Eca0Ins (Ptr<Tuple> device)
{
  RAPIDNET_LOG_INFO ("Prov_rh1_1Eca0Ins triggered");

  Ptr<RelationBase> result;

  result = GetRelation (INITPACKET)->Join (
    device,
    strlist ("initPacket_attr1"),
    strlist ("device_attr1"));

  result = GetRelation (LINKHR)->Join (
    result,
    strlist ("linkhr_attr1"),
    strlist ("device_attr1"));

  result->Assign (Assignor::New ("PIDev",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            Operation::New (RN_PLUS,
              ValueExpr::New (StrValue::New ("initPacket")),
              VarExpr::New ("device_attr1")),
            VarExpr::New ("initPacket_attr2")),
          VarExpr::New ("initPacket_attr3")),
        VarExpr::New ("initPacket_attr4")))));

  result->Assign (Assignor::New ("HashList",
    FAppend::New (
      VarExpr::New ("PIDev"))));

  result->Assign (Assignor::New ("PID1",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("device")),
          VarExpr::New ("device_attr1")),
        VarExpr::New ("device_attr2")))));

  result->Assign (Assignor::New ("List",
    FAppend::New (
      VarExpr::New ("PID1"))));

  result->Assign (Assignor::New ("PID2",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("linkhr")),
          VarExpr::New ("device_attr1")),
        VarExpr::New ("linkhr_attr2")))));

  result->Assign (Assignor::New ("List2",
    FAppend::New (
      VarExpr::New ("PID2"))));

  result->Assign (Assignor::New ("List",
    FConcat::New (
      VarExpr::New ("List"),
      VarExpr::New ("List2"))));

  result->Assign (Assignor::New ("RLOC",
    VarExpr::New ("device_attr1")));

  result->Assign (Assignor::New ("R",
    ValueExpr::New (StrValue::New ("rh1"))));

  result->Assign (Assignor::New ("RID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          VarExpr::New ("R"),
          VarExpr::New ("RLOC")),
        VarExpr::New ("List")))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("device_attr2"),
      ValueExpr::New (Int32Value::New (1)))));

  result = result->Project (
    EPACKETTEMP,
    strlist ("RLOC",
      "linkhr_attr2",
      "initPacket_attr2",
      "initPacket_attr3",
      "initPacket_attr4",
      "RID",
      "R",
      "List",
      "HashList",
      "RLOC"),
    strlist ("epacketTemp_attr1",
      "epacketTemp_attr2",
      "epacketTemp_attr3",
      "epacketTemp_attr4",
      "epacketTemp_attr5",
      "epacketTemp_attr6",
      "epacketTemp_attr7",
      "epacketTemp_attr8",
      "epacketTemp_attr9",
      RN_DEST));

  Send (result);
}

void
PktfwdNormProvCompStrawman::Prov_rh1_1Eca1Ins (Ptr<Tuple> initPacket)
{
  RAPIDNET_LOG_INFO ("Prov_rh1_1Eca1Ins triggered");

  Ptr<RelationBase> result;

  result = GetRelation (DEVICE)->Join (
    initPacket,
    strlist ("device_attr1"),
    strlist ("initPacket_attr1"));

  result = GetRelation (LINKHR)->Join (
    result,
    strlist ("linkhr_attr1"),
    strlist ("initPacket_attr1"));

  result->Assign (Assignor::New ("PIDev",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            Operation::New (RN_PLUS,
              ValueExpr::New (StrValue::New ("initPacket")),
              VarExpr::New ("initPacket_attr1")),
            VarExpr::New ("initPacket_attr2")),
          VarExpr::New ("initPacket_attr3")),
        VarExpr::New ("initPacket_attr4")))));

  result->Assign (Assignor::New ("HashList",
    FAppend::New (
      VarExpr::New ("PIDev"))));

  result->Assign (Assignor::New ("PID1",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("device")),
          VarExpr::New ("initPacket_attr1")),
        VarExpr::New ("device_attr2")))));

  result->Assign (Assignor::New ("List",
    FAppend::New (
      VarExpr::New ("PID1"))));

  result->Assign (Assignor::New ("PID2",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("linkhr")),
          VarExpr::New ("initPacket_attr1")),
        VarExpr::New ("linkhr_attr2")))));

  result->Assign (Assignor::New ("List2",
    FAppend::New (
      VarExpr::New ("PID2"))));

  result->Assign (Assignor::New ("List",
    FConcat::New (
      VarExpr::New ("List"),
      VarExpr::New ("List2"))));

  result->Assign (Assignor::New ("RLOC",
    VarExpr::New ("initPacket_attr1")));

  result->Assign (Assignor::New ("R",
    ValueExpr::New (StrValue::New ("rh1"))));

  result->Assign (Assignor::New ("RID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          VarExpr::New ("R"),
          VarExpr::New ("RLOC")),
        VarExpr::New ("List")))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("device_attr2"),
      ValueExpr::New (Int32Value::New (1)))));

  result = result->Project (
    EPACKETTEMP,
    strlist ("RLOC",
      "linkhr_attr2",
      "initPacket_attr2",
      "initPacket_attr3",
      "initPacket_attr4",
      "RID",
      "R",
      "List",
      "HashList",
      "RLOC"),
    strlist ("epacketTemp_attr1",
      "epacketTemp_attr2",
      "epacketTemp_attr3",
      "epacketTemp_attr4",
      "epacketTemp_attr5",
      "epacketTemp_attr6",
      "epacketTemp_attr7",
      "epacketTemp_attr8",
      "epacketTemp_attr9",
      RN_DEST));

  Send (result);
}

void
PktfwdNormProvCompStrawman::Prov_rh1_1Eca2Ins (Ptr<Tuple> linkhr)
{
  RAPIDNET_LOG_INFO ("Prov_rh1_1Eca2Ins triggered");

  Ptr<RelationBase> result;

  result = GetRelation (DEVICE)->Join (
    linkhr,
    strlist ("device_attr1"),
    strlist ("linkhr_attr1"));

  result = GetRelation (INITPACKET)->Join (
    result,
    strlist ("initPacket_attr1"),
    strlist ("linkhr_attr1"));

  result->Assign (Assignor::New ("PIDev",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            Operation::New (RN_PLUS,
              ValueExpr::New (StrValue::New ("initPacket")),
              VarExpr::New ("linkhr_attr1")),
            VarExpr::New ("initPacket_attr2")),
          VarExpr::New ("initPacket_attr3")),
        VarExpr::New ("initPacket_attr4")))));

  result->Assign (Assignor::New ("HashList",
    FAppend::New (
      VarExpr::New ("PIDev"))));

  result->Assign (Assignor::New ("PID1",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("device")),
          VarExpr::New ("linkhr_attr1")),
        VarExpr::New ("device_attr2")))));

  result->Assign (Assignor::New ("List",
    FAppend::New (
      VarExpr::New ("PID1"))));

  result->Assign (Assignor::New ("PID2",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("linkhr")),
          VarExpr::New ("linkhr_attr1")),
        VarExpr::New ("linkhr_attr2")))));

  result->Assign (Assignor::New ("List2",
    FAppend::New (
      VarExpr::New ("PID2"))));

  result->Assign (Assignor::New ("List",
    FConcat::New (
      VarExpr::New ("List"),
      VarExpr::New ("List2"))));

  result->Assign (Assignor::New ("RLOC",
    VarExpr::New ("linkhr_attr1")));

  result->Assign (Assignor::New ("R",
    ValueExpr::New (StrValue::New ("rh1"))));

  result->Assign (Assignor::New ("RID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          VarExpr::New ("R"),
          VarExpr::New ("RLOC")),
        VarExpr::New ("List")))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("device_attr2"),
      ValueExpr::New (Int32Value::New (1)))));

  result = result->Project (
    EPACKETTEMP,
    strlist ("RLOC",
      "linkhr_attr2",
      "initPacket_attr2",
      "initPacket_attr3",
      "initPacket_attr4",
      "RID",
      "R",
      "List",
      "HashList",
      "RLOC"),
    strlist ("epacketTemp_attr1",
      "epacketTemp_attr2",
      "epacketTemp_attr3",
      "epacketTemp_attr4",
      "epacketTemp_attr5",
      "epacketTemp_attr6",
      "epacketTemp_attr7",
      "epacketTemp_attr8",
      "epacketTemp_attr9",
      RN_DEST));

  Send (result);
}

void
PktfwdNormProvCompStrawman::Prov_rh2_1_eca (Ptr<Tuple> packet)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_1_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (DEVICE)->Join (
    packet,
    strlist ("device_attr1"),
    strlist ("packet_attr1"));

  result->Assign (Assignor::New ("PID1",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("device")),
          VarExpr::New ("packet_attr1")),
        VarExpr::New ("device_attr2")))));

  result->Assign (Assignor::New ("List",
    FAppend::New (
      VarExpr::New ("PID1"))));

  result->Assign (Assignor::New ("PID2",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            Operation::New (RN_PLUS,
              ValueExpr::New (StrValue::New ("packet")),
              VarExpr::New ("packet_attr1")),
            VarExpr::New ("packet_attr2")),
          VarExpr::New ("packet_attr3")),
        VarExpr::New ("packet_attr4")))));

  result->Assign (Assignor::New ("List2",
    FAppend::New (
      VarExpr::New ("PID2"))));

  result->Assign (Assignor::New ("List",
    FConcat::New (
      VarExpr::New ("List"),
      VarExpr::New ("List2"))));

  result->Assign (Assignor::New ("RLOC",
    VarExpr::New ("packet_attr1")));

  result->Assign (Assignor::New ("R",
    ValueExpr::New (StrValue::New ("rh2"))));

  result->Assign (Assignor::New ("RID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          VarExpr::New ("R"),
          VarExpr::New ("RLOC")),
        VarExpr::New ("List")))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("packet_attr3"),
      VarExpr::New ("packet_attr1"))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("device_attr2"),
      ValueExpr::New (Int32Value::New (1)))));

  result = result->Project (
    ERECVPACKETTEMP,
    strlist ("RLOC",
      "packet_attr1",
      "packet_attr2",
      "packet_attr3",
      "packet_attr4",
      "RID",
      "R",
      "List",
      "packet_attr5",
      "RLOC"),
    strlist ("erecvPacketTemp_attr1",
      "erecvPacketTemp_attr2",
      "erecvPacketTemp_attr3",
      "erecvPacketTemp_attr4",
      "erecvPacketTemp_attr5",
      "erecvPacketTemp_attr6",
      "erecvPacketTemp_attr7",
      "erecvPacketTemp_attr8",
      "erecvPacketTemp_attr9",
      RN_DEST));

  Send (result);
}

void
PktfwdNormProvCompStrawman::Prov_rh2_2_eca (Ptr<Tuple> erecvPacketTemp)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_2_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (RULEEXEC)->Join (
    erecvPacketTemp,
    strlist ("ruleExec_attr4", "ruleExec_attr3", "ruleExec_attr2", "ruleExec_attr1"),
    strlist ("erecvPacketTemp_attr8", "erecvPacketTemp_attr7", "erecvPacketTemp_attr6", "erecvPacketTemp_attr1"));

  result = AggWrapCount::New ()->Compute (result, erecvPacketTemp);

  result = result->Project (
    ERECVPACKETCOUNT,
    strlist ("erecvPacketTemp_attr1",
      "erecvPacketTemp_attr6",
      "erecvPacketTemp_attr7",
      "erecvPacketTemp_attr8",
      "count"),
    strlist ("eRecvPacketCount_attr1",
      "eRecvPacketCount_attr2",
      "eRecvPacketCount_attr3",
      "eRecvPacketCount_attr4",
      "eRecvPacketCount_attr5"));

  SendLocal (result);
}

void
PktfwdNormProvCompStrawman::Prov_rh2_3_eca (Ptr<Tuple> eRecvPacketCount)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_3_eca triggered");

  Ptr<Tuple> result = eRecvPacketCount;

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("eRecvPacketCount_attr5"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Project (
    RULEEXEC,
    strlist ("eRecvPacketCount_attr1",
      "eRecvPacketCount_attr2",
      "eRecvPacketCount_attr3",
      "eRecvPacketCount_attr4"),
    strlist ("ruleExec_attr1",
      "ruleExec_attr2",
      "ruleExec_attr3",
      "ruleExec_attr4"));

  Insert (result);
}

void
PktfwdNormProvCompStrawman::Prov_rh2_4_eca (Ptr<Tuple> erecvPacketTemp)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_4_eca triggered");

  Ptr<Tuple> result = erecvPacketTemp;

  result->Assign (Assignor::New ("Hash",
    FAppend::New (
      VarExpr::New ("erecvPacketTemp_attr6"))));

  result->Assign (Assignor::New ("NewHashList",
    FConcat::New (
      VarExpr::New ("erecvPacketTemp_attr9"),
      VarExpr::New ("Hash"))));

  result = result->Project (
    ERECVPACKET,
    strlist ("erecvPacketTemp_attr2",
      "erecvPacketTemp_attr3",
      "erecvPacketTemp_attr4",
      "erecvPacketTemp_attr5",
      "erecvPacketTemp_attr6",
      "erecvPacketTemp_attr1",
      "erecvPacketTemp_attr9",
      "erecvPacketTemp_attr2"),
    strlist ("erecvPacket_attr1",
      "erecvPacket_attr2",
      "erecvPacket_attr3",
      "erecvPacket_attr4",
      "erecvPacket_attr5",
      "erecvPacket_attr6",
      "erecvPacket_attr7",
      RN_DEST));

  Send (result);
}

void
PktfwdNormProvCompStrawman::Prov_rh2_5_eca (Ptr<Tuple> erecvPacket)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_5_eca triggered");

  Ptr<Tuple> result = erecvPacket;

  result = result->Project (
    RECVPACKET,
    strlist ("erecvPacket_attr1",
      "erecvPacket_attr2",
      "erecvPacket_attr3",
      "erecvPacket_attr4"),
    strlist ("recvPacket_attr1",
      "recvPacket_attr2",
      "recvPacket_attr3",
      "recvPacket_attr4"));

  Insert (result);
}

void
PktfwdNormProvCompStrawman::Prov_rh2_6_eca (Ptr<Tuple> erecvPacket)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_6_eca triggered");

  Ptr<Tuple> result = erecvPacket;

  result->Assign (Assignor::New ("PID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            Operation::New (RN_PLUS,
              ValueExpr::New (StrValue::New ("recvPacket")),
              VarExpr::New ("erecvPacket_attr1")),
            VarExpr::New ("erecvPacket_attr2")),
          VarExpr::New ("erecvPacket_attr3")),
        VarExpr::New ("erecvPacket_attr4")))));

  result = result->Project (
    RECVAUXPKT,
    strlist ("erecvPacket_attr1",
      "PID",
      "erecvPacket_attr7"),
    strlist ("recvAuxPkt_attr1",
      "recvAuxPkt_attr2",
      "recvAuxPkt_attr3"));

  Insert (result);
}

