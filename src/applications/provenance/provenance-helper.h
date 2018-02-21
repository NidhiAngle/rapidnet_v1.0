#ifndef PROVENANCE_HELPER_H
#define PROVENANCE_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "provenance.h"

namespace ns3 {
namespace rapidnet {
namespace provenance {

class Provenance;

class ProvenanceHelper: public RapidNetApplicationHelper
{
public:
  ProvenanceHelper ()
  {
    m_factory.SetTypeId (Provenance::GetTypeId ());
  }
  virtual ~ProvenanceHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Provenance> ();
  }
};

} // namespace provenance
} // namespace rapidnet
} // namespace ns3

#endif // PROVENANCE_HELPER_H

