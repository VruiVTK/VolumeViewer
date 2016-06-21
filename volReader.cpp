#include "volReader.h"

#include <vtkExtractVOI.h>
#include <vtkImageData.h>
#include <vtkPassThrough.h>
#include <vtkTrivialProducer.h>
#include <vtkXMLImageDataReader.h>


//------------------------------------------------------------------------------
volReader::volReader()
  : m_sampleRate(4)
{
  m_reducer->IncludeBoundaryOn();
}

//------------------------------------------------------------------------------
volReader::~volReader()
{
}

//------------------------------------------------------------------------------
vtkImageData *volReader::typedDataObject() const
{
  return static_cast<vtkImageData*>(m_dataObject.Get());
}

//------------------------------------------------------------------------------
vtkImageData *volReader::typedReducedDataObject() const
{
  return static_cast<vtkImageData*>(m_reducedData.Get());
}

//------------------------------------------------------------------------------
std::array<int, 3> volReader::dimensions() const
{
  std::array<int, 3> result{0, 0, 0};
  if (m_dataObject)
    {
    this->typedDataObject()->GetDimensions(result.data());
    }
  return result;
}

//------------------------------------------------------------------------------
std::array<double, 2> volReader::scalarRange() const
{
  std::array<double, 2> result{0., 0.};
  if (m_dataObject)
    {
    this->typedDataObject()->GetScalarRange(result.data());
    }
  return result;
}

//------------------------------------------------------------------------------
std::array<double, 3> volReader::spacing() const
{
  std::array<double, 3> result{0., 0., 0.};
  if (m_dataObject)
    {
    this->typedDataObject()->GetSpacing(result.data());
    }
  return result;
}

//------------------------------------------------------------------------------
void volReader::syncReaderState()
{
  if (m_fileName.empty())
    {
    // Render a simple cube. The call to AllocateScalars always modifies the
    // scalar array (even when it's a no-op), so only set the image data params
    // the first time around:
    if (m_imageData->GetNumberOfPoints() == 0)
      {
      m_imageData->SetDimensions(3, 3, 3);
      m_imageData->SetOrigin(-1., -1., -1);
      m_imageData->SetSpacing(1., 1., 1.);
      m_imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
      }
    m_imageDataSource->SetOutput(m_imageData.Get());
    m_selector->SetInputConnection(m_imageDataSource->GetOutputPort());
    }
  else
    {
    m_reader->SetFileName(m_fileName.c_str());
    m_selector->SetInputConnection(m_reader->GetOutputPort());
    }
}

//------------------------------------------------------------------------------
bool volReader::dataNeedsUpdate()
{
  if (m_dataObject)
    {
    if (m_dataObject->GetMTime() < m_selector->GetMTime())
      { // Selector has changed
      return true;
      }

    if (m_fileName.empty() &&
        m_dataObject->GetMTime() < m_imageDataSource->GetMTime())
      { // Default cube has not been generated
      return true;
      }

    if (!m_fileName.empty() &&
        m_dataObject->GetMTime() < m_reader->GetMTime())
      { // File needs to be read
      return true;
      }

    // Everything's up to date.
    return false;
    }

  // No data object.
  return true;
}

//------------------------------------------------------------------------------
void volReader::executeReaderInformation()
{
  m_selector->UpdateInformation();
}

//------------------------------------------------------------------------------
void volReader::executeReaderData()
{
  m_selector->Update();
}

//------------------------------------------------------------------------------
void volReader::updateInformationCache()
{
  // no-op. We don't use information passes here.
}

//------------------------------------------------------------------------------
void volReader::updateDataCache()
{
  m_dataObject.TakeReference(m_selector->GetOutput()->NewInstance());
  m_dataObject->ShallowCopy(m_selector->GetOutput());

  std::array<double, 6> bounds;
  this->typedDataObject()->GetBounds(bounds.data());
  m_bounds.SetBounds(bounds.data());
}

//------------------------------------------------------------------------------
void volReader::syncReducerState()
{
  std::array<int, 3> dims(this->dimensions());

  m_reducer->SetInputConnection(m_selector->GetOutputPort());
  m_reducer->SetSampleRate(m_sampleRate, m_sampleRate, m_sampleRate);
  m_reducer->SetVOI(0, dims[0] - 1, 0, dims[1] - 1, 0, dims[2] - 1);
}

//------------------------------------------------------------------------------
bool volReader::reducerNeedsUpdate()
{
  if (!m_dataObject)
    {
    return false;
    }

  if (!m_reducedData)
    {
    return true;
    }

  return
      m_reducedData->GetMTime() < m_dataObject->GetMTime() ||
      m_reducedData->GetMTime() < m_reducer->GetMTime();
}

//------------------------------------------------------------------------------
void volReader::executeReducer()
{
  m_reducer->Update();
}

//------------------------------------------------------------------------------
void volReader::updateReducedData()
{
  m_reducedData = m_reducer->GetOutput()->NewInstance();
  m_reducedData->ShallowCopy(m_reducer->GetOutput());
}

//------------------------------------------------------------------------------
int volReader::sampleRate() const
{
  return m_sampleRate;
}

//------------------------------------------------------------------------------
void volReader::setSampleRate(int rate)
{
  m_sampleRate = rate;
}
