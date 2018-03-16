#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/mlprovenance-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"

#define clf(node,id) \
  tuple (Mlprovenance::CLF, \
    attr ("clf_attr1", Ipv4Value, node),\
    attr ("clf_attr2", Int32Value, id))

#define image(node,id, img) \
  tuple (Mlprovenance::IMAGE, \
    attr ("image_attr1", Ipv4Value, node),\
    attr ("image_attr2", Int32Value, id), \
    attr ("image_attr3", StrValue, img))

#define associationrule(node, item, rule, probability)\
  tuple (Mlprovenance::ASSOCIATIONRULE, \
    attr ("associationrule_attr1", Ipv4Value, node),\
    attr ("associationrule_attr2", StrValue, item),\
    attr ("associationrule_attr3", StrValue, rule),\
    attr ("associationrule_attr4", RealValue, probability))

#define insertclf(node, id) \
  app(node)->Insert (clf (addr(node), id))

#define insertimage(node,id, img) \
  app(node)->Insert (image (addr(node),id, img));

#define insertrule(node,item,rule,probability) \
  app(node)->Insert (associationrule(addr(node),item,rule,probability));
 
using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::mlprovenance;

ApplicationContainer apps;

void Print ()
{
  PrintRelation (apps, Mlprovenance::IMAGE);
  PrintRelation (apps, Mlprovenance::PREDICTION);
  PrintRelation (apps, Mlprovenance::RESULT);
}

// Create a chain topology
void
UpdateTables1 ()
{
  insertclf(1, 1);
  insertimage (1,1,"/home/iped/nfs/diaper.jpg");
}

void
UpdateTables2 ()
{
  insertrule(1,"diaper","diaper->beer",0.45);
}

int
main (int argc, char *argv[])
{
  LogComponentEnable("Mlprovenance", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  apps = InitRapidNetApps (1, Create<MlprovenanceHelper> ());
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (11.0));

  /* schedule (0.0, UpdateTables); */
  schedule (1.0, UpdateTables1);
  schedule (2.0, UpdateTables2);
  schedule (10.0, Print);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}

