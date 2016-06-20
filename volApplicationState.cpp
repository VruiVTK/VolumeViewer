#include "volApplicationState.h"

#include "volContours.h"
#include "volGeometry.h"
#include "volOutline.h"
#include "volReader.h"
#include "volSlices.h"
#include "volVolume.h"

#include <algorithm>

volApplicationState::volApplicationState()
  : Superclass(),
    m_contours(new volContours),
    m_geometry(new volGeometry),
    m_outline(new volOutline),
    m_reader(new volReader),
    m_slices(new volSlices),
    m_volume(new volVolume)
{
  std::fill(m_colorMap.begin(), m_colorMap.end(), 0.);
  std::fill(m_sliceColorMap.begin(), m_sliceColorMap.end(), 0.);

  m_objects.push_back(m_contours);
  m_objects.push_back(m_geometry);
  m_objects.push_back(m_outline);
  m_objects.push_back(m_slices);
  m_objects.push_back(m_volume);
}

volApplicationState::~volApplicationState()
{
  delete m_contours;
  delete m_geometry;
  delete m_outline;
  delete m_reader;
  delete m_slices;
  delete m_volume;
}
