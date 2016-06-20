#include "volSlices.h"

#include "volApplicationState.h"
#include "volContextState.h"
#include "volContours.h"
#include "volReader.h"

#include <vtkActor.h>
#include <vtkCutter.h>
#include <vtkDataObject.h>
#include <vtkExternalOpenGLRenderer.h>
#include <vtkLookupTable.h>
#include <vtkPlane.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

//------------------------------------------------------------------------------
volSlices::volSlices()
{
}

//------------------------------------------------------------------------------
volSlices::~volSlices()
{
}

//------------------------------------------------------------------------------
void volSlices::setSliceVisible(size_t dim, bool vis)
{
  assert(dim >= 0 && dim <= 3);
  this->objectState<ObjectState>().sliceVisible[dim] = vis;
}

//------------------------------------------------------------------------------
bool volSlices::sliceVisible(size_t dim) const
{
  assert(dim >= 0 && dim <= 3);
  return this->objectState<ObjectState>().sliceVisible[dim];
}

//------------------------------------------------------------------------------
void volSlices::setContourSliceVisible(size_t dim, bool vis)
{
  assert(dim >= 0 && dim <= 3);
  this->objectState<ObjectState>().contourVisible[dim] = vis;
}

//------------------------------------------------------------------------------
bool volSlices::contourSliceVisible(size_t dim) const
{
  assert(dim >= 0 && dim <= 3);
  return this->objectState<ObjectState>().contourVisible[dim];
}

//------------------------------------------------------------------------------
void volSlices::setSliceLocation(size_t dim, size_t sliceIdx)
{
  assert(dim >= 0 && dim <= 3);
  this->objectState<ObjectState>().sliceLocations[dim] = sliceIdx;
}

//------------------------------------------------------------------------------
size_t volSlices::sliceLocation(size_t dim) const
{
  assert(dim >= 0 && dim <= 3);
  return this->objectState<ObjectState>().sliceLocations[dim];
}

//------------------------------------------------------------------------------
void volSlices::setContourSliceLocation(size_t dim, size_t sliceIdx)
{
  assert(dim >= 0 && dim <= 3);
  this->objectState<ObjectState>().contourLocations[dim] = sliceIdx;
}

//------------------------------------------------------------------------------
size_t volSlices::contourSliceLocation(size_t dim) const
{
  assert(dim >= 0 && dim <= 3);
  return this->objectState<ObjectState>().contourLocations[dim];
}

//------------------------------------------------------------------------------
std::string volSlices::progressLabel() const
{
  return "Slices";
}

//------------------------------------------------------------------------------
volSlices::Superclass::ObjectState *volSlices::createObjectState() const
{
  return new ObjectState;
}

//------------------------------------------------------------------------------
volSlices::Superclass::DataPipeline *
volSlices::createDataPipeline(Superclass::LevelOfDetail lod) const
{
  switch (lod)
    {
    case LevelOfDetail::HiRes:
    case LevelOfDetail::LoRes:
      return new DataPipeline(lod);
    default:
    case LevelOfDetail::Hint:
      return nullptr;
    }
}

//------------------------------------------------------------------------------
volSlices::Superclass::RenderPipeline *
volSlices::createRenderPipeline(Superclass::LevelOfDetail lod) const
{
  switch (lod)
    {
    case LevelOfDetail::HiRes:
    case LevelOfDetail::LoRes:
      return new RenderPipeline;
    default:
    case LevelOfDetail::Hint:
      return nullptr;
    }
}

//------------------------------------------------------------------------------
volSlices::Superclass::LODData *
volSlices::createLODData(Superclass::LevelOfDetail lod) const
{
  switch (lod)
    {
    case LevelOfDetail::HiRes:
    case LevelOfDetail::LoRes:
      return new LODData;
    default:
    case LevelOfDetail::Hint:
      return nullptr;
    }
}

//------------------------------------------------------------------------------
volSlices::ObjectState::ObjectState()
{
  this->color->SetNumberOfColors(256);
  this->color->Build();

  std::fill(this->sliceLocations.begin(), this->sliceLocations.end(), 0);
  std::fill(this->contourLocations.begin(), this->contourLocations.end(), 0);
  std::fill(this->sliceVisible.begin(), this->sliceVisible.end(), false);
  std::fill(this->contourVisible.begin(), this->contourVisible.end(), false);

  for (size_t i = 0; i < 3; ++i)
    {
    std::array<double, 3> normal{0., 0., 0.};
    normal[i] = 1.;
    this->slicePlanes[i]->SetNormal(normal.data());
    this->contourPlanes[i]->SetNormal(normal.data());
    }
}

//------------------------------------------------------------------------------
void volSlices::ObjectState::update(const vvApplicationState &appState)
{
  const volApplicationState &state =
      static_cast<const volApplicationState&>(appState);
  std::array<double, 3> spacing = state.reader().spacing();
  std::array<double, 3> center;
  state.reader().bounds().GetCenter(center.data());
  const double *min = state.reader().bounds().GetMinPoint();

  for (size_t i = 0; i < 3; ++i)
    {
    std::array<double, 3> origin = center;

    origin[i] = min[i] + spacing[i] * this->sliceLocations[i];
    this->slicePlanes[i]->SetOrigin(origin.data());

    origin[i] = min[i] + spacing[i] * this->contourLocations[i];
    this->contourPlanes[i]->SetOrigin(origin.data());
    }

  for (vtkIdType i = 0; i < 256; ++i)
    {
    this->color->SetTableValue(i,
                               state.sliceColorMap()[4*i + 0],
                               state.sliceColorMap()[4*i + 1],
                               state.sliceColorMap()[4*i + 2],
                               1.);
    }
}

//------------------------------------------------------------------------------
volSlices::DataPipeline::DataPipeline(vvLODAsyncGLObject::LevelOfDetail l)
  : lod(l)
{
}

//------------------------------------------------------------------------------
void volSlices::DataPipeline::configure(
    const Superclass::ObjectState &objStateIn,
    const vvApplicationState &appState)
{
  const volApplicationState &state =
      static_cast<const volApplicationState&>(appState);
  const ObjectState &objState =
      static_cast<const ObjectState&>(objStateIn);

  vtkDataObject *inputDO = nullptr;

  switch (this->lod)
    {
    case LevelOfDetail::HiRes:
      inputDO = state.reader().dataObject();
      break;
    case LevelOfDetail::LoRes:
      inputDO = state.reader().reducedDataObject();
      break;

    default:
      std::cerr << "Unsupported level of detail for data pipeline.\n";
      return;
    }

  vtkDataObject *contours = state.contours().contourData(this->lod);

  for (size_t i = 0; i < 3; ++i)
    {
    this->sliceCutters[i]->SetCutFunction(objState.slicePlanes[i].Get());
    this->sliceCutters[i]->SetInputDataObject(inputDO);

    this->contourCutters[i]->SetCutFunction(objState.contourPlanes[i].Get());
    this->contourCutters[i]->SetInputDataObject(contours);
    }
}

//------------------------------------------------------------------------------
bool volSlices::DataPipeline::needsUpdate(
    const Superclass::ObjectState &objStateIn,
    const Superclass::LODData &resultIn) const
{
  const ObjectState &state =
      static_cast<const ObjectState&>(objStateIn);
  const LODData &result =
      static_cast<const LODData&>(resultIn);

  for (size_t i = 0; i < 3; ++i)
    {
    if (state.sliceVisible[i])
      {
      if (result.slices[i].Get() == nullptr ||
          result.slices[i]->GetMTime() < state.slicePlanes[i]->GetMTime() ||
          result.slices[i]->GetMTime() < this->sliceCutters[i]->GetMTime())
        {
        return true;
        }
      }

    if (state.contourVisible[i])
      {
      if (result.contours[i].Get() == nullptr ||
          result.contours[i]->GetMTime() < state.contourPlanes[i]->GetMTime() ||
          result.contours[i]->GetMTime() < this->contourCutters[i]->GetMTime())
        {
        return true;
        }
      }
    }

  return false;
}

//------------------------------------------------------------------------------
void volSlices::DataPipeline::execute()
{
  for (size_t i = 0; i < 3; ++i)
    {
    this->sliceCutters[i]->Update();
    this->contourCutters[i]->Update();
    }
}

//------------------------------------------------------------------------------
void volSlices::DataPipeline::exportResult(
    Superclass::LODData &resultIn) const
{
  LODData &result = static_cast<LODData&>(resultIn);
  for (size_t i = 0; i < 3; ++i)
    {
    vtkDataObject *data = this->sliceCutters[i]->GetOutputDataObject(0);
    if (data)
      {
      result.slices[i].TakeReference(data->NewInstance());
      result.slices[i]->ShallowCopy(data);
      }
    else
      {
      result.slices[i] = nullptr;
      }

    data = this->contourCutters[i]->GetOutputDataObject(0);
    if (data)
      {
      result.contours[i].TakeReference(data->NewInstance());
      result.contours[i]->ShallowCopy(data);
      }
    else
      {
      result.contours[i] = nullptr;
      }
    }
}

//------------------------------------------------------------------------------
void volSlices::RenderPipeline::init(
    const Superclass::ObjectState &objState, vvContextState &contextState)
{
  const ObjectState &state = static_cast<const ObjectState&>(objState);

  for (size_t i = 0; i < 3; ++i)
    {
    this->sliceMappers[i]->SetLookupTable(state.color.Get());
    this->sliceMappers[i]->SetColorModeToMapScalars();
    this->sliceActors[i]->SetMapper(this->sliceMappers[i].Get());
    contextState.renderer().AddActor(this->sliceActors[i].Get());

    std::array<double, 3> color;
    color[i] = 1.;
    this->contourMappers[i]->SetScalarVisibility(0);
    this->contourActors[i]->SetMapper(this->contourMappers[i].Get());
    this->contourActors[i]->GetProperty()->SetColor(color.data());
    this->contourActors[i]->GetProperty()->SetLineWidth(3);
    contextState.renderer().AddActor(this->contourActors[i].Get());
    }
}

//------------------------------------------------------------------------------
void volSlices::RenderPipeline::update(
    const Superclass::ObjectState &objState,
    const vvApplicationState &appStateIn,
    const vvContextState &,
    const Superclass::LODData &result)
{
  const ObjectState &state = static_cast<const ObjectState&>(objState);
  const LODData &data = static_cast<const LODData&>(result);
  const volApplicationState &appState =
      static_cast<const volApplicationState&>(appStateIn);

  std::array<double, 2> scalarRange = appState.reader().scalarRange();

  for (size_t i = 0; i < 3; ++i)
    {
    this->sliceMappers[i]->SetScalarRange(scalarRange.data());
    this->sliceMappers[i]->SetInputDataObject(data.slices[i].Get());
    this->sliceActors[i]->SetVisibility(state.sliceVisible[i] ? 1 : 0);

    this->contourMappers[i]->SetInputDataObject(data.contours[i].Get());
    this->contourActors[i]->SetVisibility(state.contourVisible[i] ? 1 : 0);
    }

}

//------------------------------------------------------------------------------
void volSlices::RenderPipeline::disable()
{
  for (size_t i = 0; i < 3; ++i)
    {
    this->sliceActors[i]->SetVisibility(0);
    this->contourActors[i]->SetVisibility(0);
    }
}
