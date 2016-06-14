#include "volApplicationState.h"

#include "volGeometry.h"
#include "volReader.h"

volApplicationState::volApplicationState()
  : Superclass(),
    m_geometry(new volGeometry),
    m_reader(new volReader)
{
  m_objects.push_back(m_geometry);
}

volApplicationState::~volApplicationState()
{
  delete m_geometry;
  delete m_reader;
}
