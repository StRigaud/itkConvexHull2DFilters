
// C++ STL
#include <iostream>

// ITK
#include "itkConnectedComponentImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLabelGeometryImageFilter.h"
#include "itkViewImage.h"

// Custom
#include "itkLabelImageToConvexHullFilter.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Missing Parameters " << std::endl;
        std::cerr << "Usage: " << argv[0];
        std::cerr << " input output" << std::endl;
        return EXIT_FAILURE;
    }

    const unsigned int Dimension = 2;
    using PixelType = unsigned char;
    using ImageType = itk::Image<PixelType, Dimension>;
    using ImageReaderType = itk::ImageFileReader<ImageType>;
    using ImageWriteType = itk::ImageFileWriter<ImageType>;
    using GeometryFilterType = itk::LabelGeometryImageFilter<ImageType>;
    using ConvHullFilterType = itk::LabelImageToConvexHullFilter<ImageType, GeometryFilterType>;

    ImageReaderType::Pointer reader = ImageReaderType::New();
    reader->SetFileName(argv[1]);
    try
    {
        reader->Update();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    // ImageType::Pointer outImage = reader->GetOutput();

    GeometryFilterType::Pointer featuresFilter = GeometryFilterType::New();
    featuresFilter->SetInput(reader->GetOutput());
    featuresFilter->CalculatePixelIndicesOn();
    try
    {
        featuresFilter->Update();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    ConvHullFilterType::Pointer hullFilter = ConvHullFilterType::New();
    hullFilter->SetInput(reader->GetOutput());
    hullFilter->SetLabelFilter(featuresFilter);
    try
    {
        hullFilter->Update();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    ImageWriteType::Pointer writer = ImageWriteType::New();
    writer->SetFileName(argv[2]);
    writer->SetInput(hullFilter->GetOutput());
    try
    {
        writer->Update();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
