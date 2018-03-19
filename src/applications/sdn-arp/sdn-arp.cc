/* A RapidNet application. Generated by RapidNet compiler. */

#include "sdn-arp.h"
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
using namespace ns3::rapidnet::sdnarp;


NS_LOG_COMPONENT_DEFINE ("SdnArp");
NS_OBJECT_ENSURE_REGISTERED (SdnArp);

TypeId
SdnArp::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::sdnarp::SdnArp")
    .SetParent<RapidNetApplicationBase> ()
    .AddConstructor<SdnArp> ()
    ;
  return tid;
}

SdnArp::SdnArp()
{
  NS_LOG_FUNCTION_NOARGS ();
}

SdnArp::~SdnArp()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
SdnArp::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::DoDispose ();
}

void
SdnArp::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::StartApplication ();
  RAPIDNET_LOG_INFO("SdnArp Application Started");
}

void
SdnArp::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::StopApplication ();
  RAPIDNET_LOG_INFO("SdnArp Application Stopped");
}

void
SdnArp::InitDatabase ()
{
  //RapidNetApplicationBase::InitDatabase ();

}

void
SdnArp::DemuxRecv (Ptr<Tuple> tuple)
{
  RapidNetApplicationBase::DemuxRecv (tuple);

}
