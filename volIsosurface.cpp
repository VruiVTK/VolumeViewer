#include "volIsosurface.h"

#include "volApplicationState.h"
#include "volContextState.h"
#include "volReader.h"

#include <vtkActor.h>
#include <vtkDataObject.h>
#include <vtkContourFilter.h>
#include <vtkExternalOpenGLRenderer.h>
#include <vtkLookupTable.h>
#include <vtkPolyDataMapper.h>

//------------------------------------------------------------------------------
volIsosurface::volIsosurface()
{
}

//------------------------------------------------------------------------------
volIsosurface::~volIsosurface()
{
}

//------------------------------------------------------------------------------
bool volIsosurface::visible() const
{
  return this->objectState<IsosurfaceState>().visible;
}

//------------------------------------------------------------------------------
void volIsosurface::setVisible(bool vis)
{
  this->objectState<IsosurfaceState>().visible = vis;
}

//------------------------------------------------------------------------------
double volIsosurface::contourValue() const
{
  return this->objectState<IsosurfaceState>().contourValue;
}

//------------------------------------------------------------------------------
void volIsosurface::setContourValue(double val)
{
  this->objectState<IsosurfaceState>().contourValue = val;
}

//------------------------------------------------------------------------------
std::string volIsosurface::progressLabel() const
{
  return "Isosurface";
}

//------------------------------------------------------------------------------
vvLODAsyncGLObject::ObjectState *volIsosurface::createObjectState() const
{
  return new IsosurfaceState;
}

//------------------------------------------------------------------------------
vvLODAsyncGLObject::DataPipeline *
volIsosurface::createDataPipeline(LevelOfDetail lod) const
{
  switch (lod)
    {
    case LevelOfDetail::LoRes:
    case LevelOfDetail::HiRes:
      return new IsosurfaceDataPipeline(lod);

    default:
      return nullptr;
    }
}

//------------------------------------------------------------------------------
vvLODAsyncGLObject::RenderPipeline *
volIsosurface::createRenderPipeline(LevelOfDetail lod) const
{
  switch (lod)
    {
    case LevelOfDetail::LoRes:
    case LevelOfDetail::HiRes:
      return new IsosurfaceRenderPipeline;

    default:
      return nullptr;
    }
}

//------------------------------------------------------------------------------
vvLODAsyncGLObject::LODData *
volIsosurface::createLODData(LevelOfDetail lod) const
{
  switch (lod)
    {
    case LevelOfDetail::LoRes:
    case LevelOfDetail::HiRes:
      return new IsosurfaceLODData;

    default:
      return nullptr;
    }
}

//------------------------------------------------------------------------------
volIsosurface::IsosurfaceState::IsosurfaceState()
{
  this->color->SetNumberOfColors(256);
  this->color->Build();
}

//------------------------------------------------------------------------------
void volIsosurface::IsosurfaceState::update(const vvApplicationState &appState)
{
  const volApplicationState &state =
      static_cast<const volApplicationState&>(appState);

  if (this->color->GetNumberOfTableValues() == 0 ||
      this->color->GetMTime() < state.isosurfaceColorMapTimeStamp())
    {
    for (vtkIdType i = 0; i < 256; ++i)
      {
      this->color->SetTableValue(i,
                                 state.isosurfaceColorMap()[4*i + 0],
                                 state.isosurfaceColorMap()[4*i + 1],
                                 state.isosurfaceColorMap()[4*i + 2],
                                 1.);
      }
    }
}

//------------------------------------------------------------------------------
volIsosurface::IsosurfaceDataPipeline::IsosurfaceDataPipeline(LevelOfDetail l)
  : lod(l)
{
}

//------------------------------------------------------------------------------
void volIsosurface::IsosurfaceDataPipeline::configure(
    const ObjectState &objState, const vvApplicationState &appStateIn)
{
  const IsosurfaceState &state = static_cast<const IsosurfaceState&>(objState);
  const volApplicationState &appState =
      static_cast<const volApplicationState&>(appStateIn);

  vtkDataObject *dataObject = nullptr;
  switch (this->lod)
    {
    case LevelOfDetail::LoRes:
      dataObject = appState.reader().reducedDataObject();
      break;

    case LevelOfDetail::HiRes:
      dataObject = appState.reader().dataObject();
      break;

    default:
      std::cerr << "Invalid level of detail for IsosurfaceDataPipeline.\n";
    }

  this->contour->SetInputDataObject(dataObject);
  this->contour->SetValue(0, state.contourValue);
}

//------------------------------------------------------------------------------
bool volIsosurface::IsosurfaceDataPipeline::needsUpdate(
    const ObjectState &objState, const LODData &result) const
{
  const IsosurfaceState &state = static_cast<const IsosurfaceState&>(objState);
  const IsosurfaceLODData &data = static_cast<const IsosurfaceLODData&>(result);

  return
      state.visible &&
      this->contour->GetInputDataObject(0, 0) != nullptr &&
      (!data.contour ||
       this->contour->GetMTime() > data.contour->GetMTime());

}

//------------------------------------------------------------------------------
void volIsosurface::IsosurfaceDataPipeline::execute()
{
  this->contour->Update();
}

//------------------------------------------------------------------------------
void volIsosurface::IsosurfaceDataPipeline::exportResult(LODData &result) const
{
  IsosurfaceLODData &data = static_cast<IsosurfaceLODData&>(result);

  vtkDataObject *dataObject = this->contour->GetOutputDataObject(0);
  data.contour.TakeReference(dataObject->NewInstance());
  data.contour->ShallowCopy(dataObject);
}

//------------------------------------------------------------------------------
void volIsosurface::IsosurfaceRenderPipeline::init(
    const ObjectState &objState, vvContextState &contextState)
{
  const IsosurfaceState &state = static_cast<const IsosurfaceState&>(objState);
  this->mapper->SetLookupTable(state.color.Get());
  this->mapper->SetColorModeToMapScalars();
  this->actor->SetMapper(this->mapper.Get());
  contextState.renderer().AddActor(this->actor.Get());
}

//------------------------------------------------------------------------------
void volIsosurface::IsosurfaceRenderPipeline::update(
    const ObjectState &objState, const vvApplicationState &appStateIn,
    const vvContextState &, const LODData &result)
{
  const IsosurfaceState &state = static_cast<const IsosurfaceState&>(objState);
  const IsosurfaceLODData &data = static_cast<const IsosurfaceLODData&>(result);
  const volApplicationState &appState =
      static_cast<const volApplicationState&>(appStateIn);

  std::array<double, 2> scalarRange = appState.reader().scalarRange();

  this->mapper->SetScalarRange(scalarRange.data());
  this->mapper->SetInputDataObject(data.contour);
  this->actor->SetVisibility(state.visible ? 1 : 0);
}

//------------------------------------------------------------------------------
void volIsosurface::IsosurfaceRenderPipeline::disable()
{
  this->actor->SetVisibility(0);
}
