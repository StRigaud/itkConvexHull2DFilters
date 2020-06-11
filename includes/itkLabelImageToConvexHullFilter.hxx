#ifndef __itkLabelImageToConvexHullFilter_hxx
#define __itkLabelImageToConvexHullFilter_hxx

#include "itkLabelImageToConvexHullFilter.h"

#include "itkViewImage.h"

namespace itk
{

template <class TImage, class TFilter> LabelImageToConvexHullFilter<TImage, TFilter>::LabelImageToConvexHullFilter()
{
}

template <class TImage, class TFilter> void LabelImageToConvexHullFilter<TImage, TFilter>::GenerateData()
{
    // Copy input and graft it.
    ImagePointer input = ImageType::New();
    input->Graft(const_cast<ImageType *>(this->GetInput()));

    // Run geometry filter if not provided.
    if (m_LabelFilter == nullptr)
    {
        m_LabelFilter = LabelFilterType::New();
        m_LabelFilter->SetInput(input);
        m_LabelFilter->CalculatePixelIndicesOn();
        try
        {
            m_LabelFilter->Update();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    LabelsType allLabels = m_LabelFilter->GetLabels();
    typename LabelsType::iterator allLabelsIt;
    for (allLabelsIt = allLabels.begin(); allLabelsIt != allLabels.end(); allLabelsIt++)
    {
        if (*allLabelsIt != 0)
        {
            m_HullFilter = HullFilterType::New();
            m_HullFilter->SetInput(input);
            LabelPixelType labelValue = *allLabelsIt;
            m_HullFilter->SetLabelValue(labelValue);
            LabelRegionType labelRegion = m_LabelFilter->GetRegion(labelValue);
            m_HullFilter->GetOutput()->SetRequestedRegion(labelRegion);
            if (m_LabelFilter->GetCalculatePixelIndices())
            {
                LabelIndicesType labelindices = m_LabelFilter->GetPixelIndices(labelValue);
                m_HullFilter->SetLabelIndices(labelindices);
            }
            try
            {
                m_HullFilter->Update();
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
            input = m_HullFilter->GetOutput();
            input->DisconnectPipeline();
        }
    }
    this->GraftOutput(input);
}

} // namespace itk

#endif // __itkLabelImageToConvexHullFilter_hxx