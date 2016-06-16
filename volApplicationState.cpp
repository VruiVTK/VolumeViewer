#include "volApplicationState.h"

#include "volGeometry.h"
#include "volOutline.h"
#include "volReader.h"

volApplicationState::volApplicationState()
  : Superclass(),
    m_geometry(new volGeometry),
    m_outline(new volOutline),
    m_reader(new volReader)
{
  m_objects.push_back(m_geometry);
  m_objects.push_back(m_outline);
}

volApplicationState::~volApplicationState()
{
  delete m_geometry;
  delete m_outline;
  delete m_reader;
}
