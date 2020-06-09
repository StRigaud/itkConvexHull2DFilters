
// C++ STL
#include <iostream>

// ITK
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkViewImage.h"

// Custom
#include "itkBinaryImageToConvexHullFilter.h"

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Missing Parameters " << std::endl;
        std::cerr << "Usage: " << argv[0];
        std::cerr << " input output [label] " << std::endl;
        return EXIT_FAILURE;
    }

    const unsigned int Dimension = 2;
    using PixelType = unsigned char;
    using ImageType = itk::Image<PixelType, Dimension>;
    using ImageReaderType = itk::ImageFileReader<ImageType>;
    using ImageWriteType = itk::ImageFileWriter<ImageType>;
    using ConvHullFilterType = itk::BinaryImageToConvexHullFilter<ImageType>;

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

    ConvHullFilterType::Pointer hull = ConvHullFilterType::New();
    hull->SetInput(reader->GetOutput());
    if (argc == 4)
    {
        hull->SetLabelValue(std::atoi(argv[3]));
    }
    try
    {
        hull->Update();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    ImageWriteType::Pointer writer = ImageWriteType::New();
    writer->SetFileName(argv[2]);
    writer->SetInput(hull->GetOutput());
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
