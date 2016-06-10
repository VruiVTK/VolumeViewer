#ifndef VOLREADER_H
#define VOLREADER_H

#include <vvReader.h>

#include <vtkNew.h>

#include <array>

class vtkExtractVOI;
class vtkImageData;
class vtkPassThrough;
class vtkTrivialProducer;
class vtkXMLImageDataReader;

class volReader : public vvReader
{
public:
  volReader();
  ~volReader() override;

  vtkImageData* typedDataObject() const;
  vtkImageData* typedReducedDataObject() const;

  std::array<int, 3> dimensions() const;
  std::array<double, 3> spacing() const;
  std::array<double, 2> scalarRange() const;

  int sampleRate() const;
  void setSampleRate(int sampleRate);

private:
  void syncReaderState() override;
  bool dataNeedsUpdate() override;
  void executeReaderInformation() override;
  void executeReaderData() override;
  void updateInformationCache() override;
  void updateDataCache() override;

  void syncReducerState() override;
  bool reducerNeedsUpdate() override;
  void executeReducer() override;
  void updateReducedData() override;

private:
  // The actual file reader:
  vtkNew<vtkXMLImageDataReader> m_reader;

  // A simple image data shown when the filename is not set:
  vtkNew<vtkImageData> m_imageData;
  vtkNew<vtkTrivialProducer> m_imageDataSource;

  // Selects between the reader and the default image data:
  vtkNew<vtkPassThrough> m_selector;

  // The low-res data producer:
  vtkNew<vtkExtractVOI> m_reducer;

  // Downsample rate for data reducer.
  int m_sampleRate;
};

#endif // VOLREADER_H
