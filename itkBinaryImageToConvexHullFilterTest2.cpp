
// C++ STL
#include <iostream>

// ITK
#include "itkConnectedComponentImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLabelGeometryImageFilter.h"
#include "itkViewImage.h"

// Custom
#include "itkBinaryImageToConvexHullFilter.h"

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
    ImageType::Pointer outImage = reader->GetOutput();

    GeometryFilterType::Pointer featuresFilter = GeometryFilterType::New();
    featuresFilter->SetInput(outImage);
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
    GeometryFilterType::LabelsType allLabels = featuresFilter->GetLabels();

    GeometryFilterType::LabelsType::iterator allLabelsIt;
    for (allLabelsIt = allLabels.begin(); allLabelsIt != allLabels.end(); allLabelsIt++)
    {
        if (*allLabelsIt != 0)
        {
            GeometryFilterType::LabelPixelType labelValue = *allLabelsIt;
            GeometryFilterType::RegionType labelRegion = featuresFilter->GetRegion(labelValue);
            GeometryFilterType::LabelIndicesType labelindices = featuresFilter->GetPixelIndices(labelValue);
            ConvHullFilterType::Pointer hull = ConvHullFilterType::New();
            hull->SetInput(outImage);
            hull->GetOutput()->SetRequestedRegion(labelRegion);
            hull->SetLabelValue(labelValue);
            hull->SetLabelIndices(labelindices);
            try
            {
                hull->Update();
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
                return EXIT_FAILURE;
            }
            outImage = hull->GetOutput();
            outImage->DisconnectPipeline();
        }
    }

    ImageWriteType::Pointer writer = ImageWriteType::New();
    writer->SetFileName(argv[2]);
    writer->SetInput(outImage);
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
