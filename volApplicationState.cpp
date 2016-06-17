#include "volApplicationState.h"

#include "volGeometry.h"
#include "volOutline.h"
#include "volReader.h"
#include "volVolume.h"

#include <algorithm>

volApplicationState::volApplicationState()
  : Superclass(),
    m_geometry(new volGeometry),
    m_outline(new volOutline),
    m_reader(new volReader),
    m_volume(new volVolume)
{
  std::fill(m_colorMap.begin(), m_colorMap.end(), 0.);

  m_objects.push_back(m_geometry);
  m_objects.push_back(m_outline);
  m_objects.push_back(m_volume);
}

volApplicationState::~volApplicationState()
{
  delete m_geometry;
  delete m_outline;
  delete m_reader;
  delete m_volume;
}
