#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/provenance-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"

#define image(node,id, img) \
  tuple (Provenance::IMAGE, \
    attr ("image_attr1", Int32Value, node), \
    attr ("image_attr2", Int32Value, id), \
    attr ("image_attr3", StrValue, img))

#define insertimage(node,id, img) \
  app(node)->Insert (image (node,id, img));

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::provenance;

ApplicationContainer apps;

void Print ()
{
  PrintRelation (apps, Provenance::IMAGE);
  PrintRelation (apps, Provenance::PREDICTION);
}

// Create a chain topology
void
UpdateImage ()
{
  insertimage (1,1, "/home/vnidhi/diaper.jpg");
}

int
main (int argc, char *argv[])
{
  LogComponentEnable("Provenance", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  apps = InitRapidNetApps (5, Create<ProvenanceHelper> ());
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (10.0));

  schedule (0.0, UpdateImage);
  schedule (10.0, Print);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}

