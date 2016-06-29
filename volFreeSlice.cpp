#include "volFreeSlice.h"

#include "volApplicationState.h"
#include "volContextState.h"
#include "volReader.h"

#include <vtkActor.h>
#include <vtkDataObject.h>
#include <vtkExternalOpenGLRenderer.h>
#include <vtkFlyingEdgesPlaneCutter.h>
#include <vtkLookupTable.h>
#include <vtkPlane.h>
#include <vtkPolyDataMapper.h>

//------------------------------------------------------------------------------
volFreeSlice::volFreeSlice()
{
}

//------------------------------------------------------------------------------
volFreeSlice::~volFreeSlice()
{
}

//------------------------------------------------------------------------------
bool volFreeSlice::visible() const
{
  return this->objectState<FreeSliceState>().visible;
}

//------------------------------------------------------------------------------
void volFreeSlice::setVisible(bool vis)
{
  this->objectState<FreeSliceState>().visible = vis;
}

//------------------------------------------------------------------------------
std::string volFreeSlice::progressLabel() const
{
  return "Free Slice";
}

//------------------------------------------------------------------------------
vvLODAsyncGLObject::ObjectState *volFreeSlice::createObjectState() const
{
  return new FreeSliceState;
}

//------------------------------------------------------------------------------
vvLODAsyncGLObject::DataPipeline *
volFreeSlice::createDataPipeline(LevelOfDetail lod) const
{
  switch (lod)
    {
    case LevelOfDetail::LoRes:
    case LevelOfDetail::HiRes:
      return new FreeSliceDataPipeline(lod);

    default:
      return nullptr;
    }
}

//------------------------------------------------------------------------------
vvLODAsyncGLObject::RenderPipeline *
volFreeSlice::createRenderPipeline(LevelOfDetail lod) const
{
  switch (lod)
    {
    case LevelOfDetail::LoRes:
    case LevelOfDetail::HiRes:
      return new FreeSliceRenderPipeline;

    default:
      return nullptr;
    }
}

//------------------------------------------------------------------------------
vvLODAsyncGLObject::LODData *
volFreeSlice::createLODData(LevelOfDetail lod) const
{
  switch (lod)
    {
    case LevelOfDetail::LoRes:
    case LevelOfDetail::HiRes:
      return new FreeSliceLODData;

    default:
      return nullptr;
    }
}

//------------------------------------------------------------------------------
void volFreeSlice::setNormal(const std::array<double, 3> &o)
{
  this->objectState<FreeSliceState>().normal = o;
}

//------------------------------------------------------------------------------
const std::array<double, 3> &volFreeSlice::normal() const
{
  return this->objectState<FreeSliceState>().normal;
}

//------------------------------------------------------------------------------
void volFreeSlice::setOrigin(const std::array<double, 3> &o)
{
  this->objectState<FreeSliceState>().origin = o;
}

//------------------------------------------------------------------------------
const std::array<double, 3> &volFreeSlice::origin() const
{
  return this->objectState<FreeSliceState>().origin;
}

//------------------------------------------------------------------------------
volFreeSlice::FreeSliceState::FreeSliceState()
{
  this->color->SetNumberOfColors(256);
  this->color->Build();
}

//------------------------------------------------------------------------------
void volFreeSlice::FreeSliceState::update(const vvApplicationState &appState)
{
  const volApplicationState &state =
      static_cast<const volApplicationState&>(appState);

  if (this->color->GetNumberOfTableValues() == 0 ||
      this->color->GetMTime() < state.sliceColorMapTimeStamp())
    {
    for (vtkIdType i = 0; i < 256; ++i)
      {
      this->color->SetTableValue(i,
                                 state.sliceColorMap()[4*i + 0],
                                 state.sliceColorMap()[4*i + 1],
                                 state.sliceColorMap()[4*i + 2],
                                 1.);
      }
    }
}

//------------------------------------------------------------------------------
volFreeSlice::FreeSliceDataPipeline::FreeSliceDataPipeline(LevelOfDetail l)
  : lod(l)
{
  this->cutter->SetPlane(this->plane.Get());
  this->cutter->ComputeNormalsOff();
  this->cutter->InterpolateAttributesOn();
}

//------------------------------------------------------------------------------
void volFreeSlice::FreeSliceDataPipeline::configure(
    const ObjectState &objState, const vvApplicationState &appStateIn)
{
  const FreeSliceState &state = static_cast<const FreeSliceState&>(objState);
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
      std::cerr << "Invalid level of detail for IsosurfaceDataPipeline.\n";
    }

  // const casts bc VTK is not const-correct
  this->plane->SetOrigin(const_cast<double*>(state.origin.data()));
  this->plane->SetNormal(const_cast<double*>(state.normal.data()));
  this->cutter->SetInputDataObject(input);
}

//------------------------------------------------------------------------------
bool volFreeSlice::FreeSliceDataPipeline::needsUpdate(
    const ObjectState &objState, const LODData &result) const
{
  const FreeSliceState &state = static_cast<const FreeSliceState&>(objState);
  const FreeSliceLODData &data = static_cast<const FreeSliceLODData&>(result);

  return
      state.visible &&
      this->cutter->GetInputDataObject(0, 0) != nullptr &&
      (!data.slice ||
       this->cutter->GetMTime() > data.slice->GetMTime());

}

//------------------------------------------------------------------------------
void volFreeSlice::FreeSliceDataPipeline::execute()
{
  this->cutter->Update();
}

//------------------------------------------------------------------------------
void volFreeSlice::FreeSliceDataPipeline::exportResult(LODData &result) const
{
  FreeSliceLODData &data = static_cast<FreeSliceLODData&>(result);

  vtkDataObject *dataObject = this->cutter->GetOutputDataObject(0);
  data.slice.TakeReference(dataObject->NewInstance());
  data.slice->ShallowCopy(dataObject);
}

//------------------------------------------------------------------------------
void volFreeSlice::FreeSliceRenderPipeline::init(const ObjectState &objState,
                                                 vvContextState &contextState)
{
  const FreeSliceState &state = static_cast<const FreeSliceState&>(objState);
  this->mapper->SetLookupTable(state.color.Get());
  this->mapper->SetColorModeToMapScalars();
  this->actor->SetMapper(this->mapper.Get());
  contextState.renderer().AddActor(this->actor.Get());
}

//------------------------------------------------------------------------------
void volFreeSlice::FreeSliceRenderPipeline::update(
    const ObjectState &objState, const vvApplicationState &appStateIn,
    const vvContextState &, const LODData &result)
{
  const FreeSliceState &state = static_cast<const FreeSliceState&>(objState);
  const FreeSliceLODData &data = static_cast<const FreeSliceLODData&>(result);
  const volApplicationState &appState =
      static_cast<const volApplicationState&>(appStateIn);

  std::array<double, 2> scalarRange = appState.reader().scalarRange();

  this->mapper->SetScalarRange(scalarRange.data());
  this->mapper->SetInputDataObject(data.slice);
  this->actor->SetVisibility(state.visible ? 1 : 0);
}

//------------------------------------------------------------------------------
void volFreeSlice::FreeSliceRenderPipeline::disable()
{
  this->actor->SetVisibility(0);
}
