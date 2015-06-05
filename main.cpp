// STD includes
#include <iostream>
#include <string>

// ExampleVTKReader includes
#include "ExampleVTKReader.h"

void printUsage(void)
{
  std::cout << "\nExampleVTKReader - Render VTK objects in the VRUI context" << std::endl;
  std::cout << "\nUSAGE:\n\t./ExampleVTKReader [-f <string>] [-h]" << std::endl;
  std::cout << "\nWhere:" << std::endl;
  std::cout << "\t-f <string>, -fileName <string>" << std::endl;
  std::cout << "\tName of VTK file to load using VTK.\n" << std::endl;
  std::cout << "\t-r <digit>, -renderMode <digit>" << std::endl;
  std::cout << "\tRender mode to request for vtkSmartVolumeMapper.\n" << std::endl;
  std::cout << "\t-s <digit>, -sampling <digit>" << std::endl;
  std::cout << "\tSampling to downsample Volume.\n" << std::endl;
  std::cout << "\t-h, -help" << std::endl;
  std::cout << "\tDisplay this usage information and exit." << std::endl;
  std::cout << "\nAdditionally, all the commandline switches the VRUI " <<
    "accepts can be passed to ExampleVTKReader.\nFor example, -rootSection," <<
    " -vruiVerbose, -vruiHelp, etc.\n" << std::endl;
}

/* Create and execute an application object: */
/*
 * main - The application main method.
 *
 * parameter argc - int
 * parameter argv - char**
 *
 */
int main(int argc, char* argv[])
{
  try
    {
    std::string name;
    int renderMode = -1;
    int sampling = 4;
    bool verbose = false;
    if(argc > 1)
      {
      /* Parse the command-line arguments */
      for(int i = 1; i < argc; ++i)
        {
        if(strcmp(argv[i], "-f")==0 || strcmp(argv[i], "-filename")==0)
          {
          name.assign(argv[i+1]);
          ++i;
          }
        if(strcmp(argv[i], "-r")==0 || strcmp(argv[i], "-renderMode")==0)
          {
          renderMode = atoi(argv[i+1]);
          ++i;
          }
        if(strcmp(argv[i], "-s")==0 || strcmp(argv[i], "-sampling")==0)
          {
          sampling = atoi(argv[i+1]);
          ++i;
          }
        if(strcmp(argv[i],"-h")==0 || strcmp(argv[i], "-help")==0)
          {
          printUsage();
          return 0;
          }
        if(strcmp(argv[i],"-vruiVerbose")==0)
          {
          verbose = true;
          }
        }
      }

    ExampleVTKReader application(argc, argv, sampling);
    if(!name.empty())
      {
      application.setFileName(name.c_str());
      application.setVerbose(verbose);
      }
    if(renderMode != -1)
      {
      application.setRequestedRenderMode(renderMode);
      }
    application.run();
    return 0;
    }
  catch (std::runtime_error e)
    {
    std::cerr << "Error: Exception " << e.what() << "!" << std::endl;
    return 1;
    }
}
