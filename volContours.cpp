#include "volContours.h"

#include "volApplicationState.h"
#include "volContextState.h"
#include "volReader.h"

#include <vtkActor.h>
#include <vtkDataObject.h>
#include <vtkFlyingEdges3D.h>
#include <vtkExternalOpenGLRenderer.h>
#include <vtkPolyDataMapper.h>

//------------------------------------------------------------------------------
volContours::volContours()
{
}

//------------------------------------------------------------------------------
volContours::~volContours()
{
}

//------------------------------------------------------------------------------
bool volContours::visible() const
{
  return this->objectState<ContourState>().visible;
}

//------------------------------------------------------------------------------
void volContours::setVisible(bool vis)
{
  this->objectState<ContourState>().visible = vis;
}

//------------------------------------------------------------------------------
const std::vector<double> &volContours::contourValues() const
{
  return this->objectState<ContourState>().contourValues;
}

//------------------------------------------------------------------------------
void volContours::setContourValues(const std::vector<double> &vals)
{
  this->objectState<ContourState>().contourValues = vals;
}

//------------------------------------------------------------------------------
vtkDataObject *volContours::contourData(LevelOfDetail lod) const
{
  const ContourLODData *data =
      static_cast<const ContourLODData*>(this->lodData(lod));

  // const_cast because VTK is not const correct:
  return data ? const_cast<vtkDataObject*>(data->contours.Get())
              : nullptr;
}

//------------------------------------------------------------------------------
std::string volContours::progressLabel() const
{
  return "Contours";
}

//------------------------------------------------------------------------------
vvLODAsyncGLObject::ObjectState *volContours::createObjectState() const
{
  return new ContourState;
}

//------------------------------------------------------------------------------
vvLODAsyncGLObject::DataPipeline *
volContours::createDataPipeline(vvLODAsyncGLObject::LevelOfDetail lod) const
{
  switch (lod)
    {
    case LevelOfDetail::LoRes:
    case LevelOfDetail::HiRes:
      return new ContourDataPipeline(lod);

    default:
      return nullptr;
    }
}

//------------------------------------------------------------------------------
vvLODAsyncGLObject::RenderPipeline *
volContours::createRenderPipeline(vvLODAsyncGLObject::LevelOfDetail lod) const
{
  switch (lod)
    {
    case LevelOfDetail::LoRes:
    case LevelOfDetail::HiRes:
      return new ContourRenderPipeline;

    default:
      return nullptr;
    }
}

//------------------------------------------------------------------------------
vvLODAsyncGLObject::LODData *
volContours::createLODData(vvLODAsyncGLObject::LevelOfDetail lod) const
{
  switch (lod)
    {
    case LevelOfDetail::LoRes:
    case LevelOfDetail::HiRes:
      return new ContourLODData;

    default:
      return nullptr;
    }
}

//------------------------------------------------------------------------------
void volContours::ContourState::update(const vvApplicationState &)
{
}

//------------------------------------------------------------------------------
volContours::ContourDataPipeline::ContourDataPipeline(LevelOfDetail l)
  : lod(l)
{
  this->contour->ComputeNormalsOn();
  this->contour->ComputeGradientsOff();
}

//------------------------------------------------------------------------------
void volContours::ContourDataPipeline::configure(
    const ObjectState &objState,  const vvApplicationState &appStateIn)
{
  const ContourState &state = static_cast<const ContourState&>(objState);
  const volApplicationState &appState =
      static_cast<const volApplicationState&>(appStateIn);

  vtkDataObject *input = nullptr;
  switch (this->lod)
    {
    case LevelOfDetail::LoRes:
      // Disable this LOD when forcing low res:
      if (!appState.forceLowResolution())
        {
        input = appState.reader().reducedDataObject();
        }
      break;

    case LevelOfDetail::HiRes:
      if (appState.forceLowResolution())
        {
        input = appState.reader().reducedDataObject();
        }
      else
        {
        input = appState.reader().dataObject();
        }
      break;

    default:
      std::cerr << "Invalid level of detail for ContourDataPipeline.\n";
      return;
    }

  this->contour->SetInputDataObject(input);
  this->contour->SetNumberOfContours(state.contourValues.size());
  for (size_t i = 0; i < state.contourValues.size(); ++i)
    {
    this->contour->SetValue(static_cast<int>(i), state.contourValues[i]);
    }
}

//------------------------------------------------------------------------------
bool volContours::ContourDataPipeline::needsUpdate(const ObjectState &,
                                                   const LODData &result) const
{
  const ContourLODData &data = static_cast<const ContourLODData&>(result);

  // Note that we don't check visibility -- this is intentional, as the contours
  // need to be up-to-date for volSlice, since slice uses the contoured data.
  return
      this->contour->GetInputDataObject(0, 0) &&
      (!data.contours ||
       data.contours->GetMTime() < this->contour->GetMTime());
}

//------------------------------------------------------------------------------
void volContours::ContourDataPipeline::execute()
{
  this->contour->Update();
}

//------------------------------------------------------------------------------
void volContours::ContourDataPipeline::exportResult(LODData &result) const
{
  ContourLODData &data = static_cast<ContourLODData&>(result);

  vtkDataObject *output = this->contour->GetOutputDataObject(0);
  assert(output);
  data.contours.TakeReference(output->NewInstance());
  data.contours->ShallowCopy(output);
}

//------------------------------------------------------------------------------
void volContours::ContourRenderPipeline::init(const ObjectState &,
                                              vvContextState &contextState)
{
  this->mapper->SetScalarVisibility(0);
  this->actor->SetMapper(this->mapper.Get());
  contextState.renderer().AddActor(this->actor.Get());
}

//------------------------------------------------------------------------------
void volContours::ContourRenderPipeline::update(
    const ObjectState &objState, const vvApplicationState &appState,
    const vvContextState &contextState, const LODData &result)
{
  const ContourState &state = static_cast<const ContourState&>(objState);
  const ContourLODData &data = static_cast<const ContourLODData&>(result);

  this->mapper->SetInputDataObject(data.contours);
  this->actor->SetVisibility(state.visible ? 1 : 0);
}

//------------------------------------------------------------------------------
void volContours::ContourRenderPipeline::disable()
{
  this->actor->SetVisibility(0);
}
