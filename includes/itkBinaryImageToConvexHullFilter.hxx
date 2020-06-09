#ifndef __itkBinaryImageToConvexHullFilter_hxx
#define __itkBinaryImageToConvexHullFilter_hxx

#include "itkBinaryImageToConvexHullFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIteratorWithIndex.h"

#include "itkViewImage.h"

namespace itk
{

template <class TImage> BinaryImageToConvexHullFilter<TImage>::BinaryImageToConvexHullFilter()
{
    this->m_LabelValue = NumericTraits<PixelType>::Zero;
    this->m_LabelIndices.clear();
    this->m_HullIndices.clear();
}

template <class TImage>
std::vector<typename TImage::IndexType> BinaryImageToConvexHullFilter<TImage>::GetHullIndices() const
{
    return this->m_HullIndices;
}

template <class TImage>
void BinaryImageToConvexHullFilter<TImage>::SetHullIndices(std::vector<typename TImage::IndexType> &val)
{
    this->m_HullIndices = val;
}

template <class TImage>
std::vector<typename TImage::IndexType> BinaryImageToConvexHullFilter<TImage>::GetLabelIndices() const
{
    return this->m_LabelIndices;
}

template <class TImage>
void BinaryImageToConvexHullFilter<TImage>::SetLabelIndices(std::vector<typename TImage::IndexType> &val)
{
    this->m_LabelIndices = val;
}

template <class TImage>
typename TImage::IndexType::IndexValueType BinaryImageToConvexHullFilter<TImage>::LineDistance(
    typename TImage::IndexType &A, typename TImage::IndexType &B, typename TImage::IndexType &Q)
{
    return (Q[1] - A[1]) * (B[0] - A[0]) - (B[1] - A[1]) * (Q[0] - A[0]);
}

template <class TImage>
typename TImage::IndexType::IndexValueType BinaryImageToConvexHullFilter<TImage>::WhichSide(
    typename TImage::IndexType::IndexValueType value)
{
    // 1 : left ; -1 : right ; 0 : on the line
    return (NumericTraits<IndexValueType>::Zero < value) - (value < NumericTraits<IndexValueType>::Zero);
}

template <class TImage> void BinaryImageToConvexHullFilter<TImage>::GetIndices()
{
    itkDebugMacro(<< "Computing list of Label Indices.");
    PixelType zero = NumericTraits<PixelType>::Zero;
    ImageRegionConstIteratorWithIndex<ImageType> it(this->GetOutput(), this->GetOutput()->GetRequestedRegion());
    for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
        if (it.Get() != zero)
        {
            if (this->m_LabelValue == zero || it.Get() == this->m_LabelValue)
            {
                this->m_LabelIndices.push_back(it.GetIndex());
            }
        }
    }
    if (this->m_LabelValue == 0)
    {
        m_LabelValue = this->GetOutput()->GetPixel(this->m_LabelIndices.front());
    }
}

template <class TImage>
void BinaryImageToConvexHullFilter<TImage>::FindHull(std::vector<typename TImage::IndexType> &subIndices,
                                                     typename TImage::IndexType &P, typename TImage::IndexType &Q)
{
    // Test if current subset is not empty.
    if (!subIndices.empty())
    {
        // find the farthest point from the line (maximize PQM area).
        IndexValueType maxDistance = itk::NumericTraits<IndexValueType>::Zero;
        auto pIndice = subIndices.begin();
        for (auto ite = subIndices.begin(); ite != subIndices.end(); ++ite)
        {
            IndexValueType T = std::abs(this->LineDistance(P, Q, *ite));
            if (T > maxDistance)
            {
                maxDistance = T;
                pIndice = ite;
            }
        }
        IndexType C = *pIndice;

        // split point list into two subsets outside of the PQC triangle.
        LabelIndicesType pcRightSideIndices;
        LabelIndicesType cqRightSideIndices;
        for (auto ite = subIndices.begin(); ite != subIndices.end(); ++ite)
        {
            IndexType M = *ite;
            IndexValueType T1 = this->LineDistance(P, C, M);
            IndexValueType T2 = this->LineDistance(C, Q, M);
            if (this->WhichSide(T1) == -1)
            {
                pcRightSideIndices.push_back(M);
            }
            else if (this->WhichSide(T2) == -1)
            {
                cqRightSideIndices.push_back(M);
            }
            else
            {
                /* inside the hull */
            }
        }
        this->FindHull(pcRightSideIndices, P, C);
        this->FindHull(cqRightSideIndices, C, Q);
    }
    else
    {
        this->m_HullIndices.push_back(P);
        this->m_HullIndices.push_back(Q);
    }
}

template <class TImage> void BinaryImageToConvexHullFilter<TImage>::QuickHull()
{
    itkDebugMacro(<< "Starting QuickHull algorithm.");
    // Get minimum and maximum x point indices.
    auto pMinIndex = this->m_LabelIndices.begin();
    auto pMaxIndex = this->m_LabelIndices.end();
    IndexValueType min_x = itk::NumericTraits<IndexValueType>::max();
    IndexValueType max_x = itk::NumericTraits<IndexValueType>::Zero;
    for (auto ite = this->m_LabelIndices.begin(); ite != this->m_LabelIndices.end(); ++ite)
    {
        if ((*ite)[0] < min_x)
        {
            min_x = (*ite)[0];
            pMinIndex = ite;
        }
        else if ((*ite)[0] > max_x)
        {
            max_x = (*ite)[0];
            pMaxIndex = ite;
        }
    }
    IndexType A = *pMinIndex;
    IndexType B = *pMaxIndex;
    // split point list into two subset righ and left of AB line
    LabelIndicesType abRightSideIndices;
    LabelIndicesType baRightSideIndices;
    for (auto ite = this->m_LabelIndices.begin(); ite != this->m_LabelIndices.end(); ++ite)
    {
        IndexType M = *ite;
        IndexValueType T = this->LineDistance(A, B, M);
        IndexValueType side = this->WhichSide(T);
        if (side == -1)
        {
            abRightSideIndices.push_back(M);
        }
        else if (side == 1)
        {
            baRightSideIndices.push_back(M);
        }
    }
    // Find hull on both subsets
    this->FindHull(abRightSideIndices, A, B);
    this->FindHull(baRightSideIndices, B, A);
    // Remove possible doubles
    this->m_HullIndices.erase(std::unique(this->m_HullIndices.begin(), this->m_HullIndices.end()),
                              this->m_HullIndices.end());
}

template <class TImage> void BinaryImageToConvexHullFilter<TImage>::DrawHull()
{
    itkDebugMacro(<< "Draw hull on region.");
    // Initialise parameters
    int hullSize = this->m_HullIndices.size();
    if (m_LabelValue == 0)
    {
        m_LabelValue = itk::NumericTraits<typename TImage::PixelType>::max();
    }
    // Iterate on region pixel
    itk::ImageRegionIteratorWithIndex<ImageType> it(this->GetOutput(), this->GetOutput()->GetRequestedRegion());
    for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
        IndexType Q = it.GetIndex();
        bool flag = true;
        int side[hullSize];
        // Check if current pixel is always on the same side of the hull edges
        for (size_t i = 0; i < hullSize; i++)
        {
            IndexType A = this->m_HullIndices.at(i);
            IndexType B = this->m_HullIndices.at((i + 1) % (hullSize - 1));
            IndexValueType T = this->LineDistance(A, B, Q);
            side[i] = this->WhichSide(T);
            if (i > 0 && side[i] != side[i - 1])
            {
                flag = false;
                break;
            }
        }
        // Draw if pixel inside hull
        if (flag)
        {
            it.Set(m_LabelValue);
        }
    }
}

template <class TImage> void BinaryImageToConvexHullFilter<TImage>::GenerateData()
{
    // take a copy of the input image and initialise the output with it.
    ImagePointer input = ImageType::New();
    input->Graft(const_cast<ImageType *>(this->GetInput()));
    this->GraftOutput(input);
    // Pipeline steps
    if (this->m_LabelIndices.empty())
    {
        this->GetIndices();
    }
    if (this->m_HullIndices.empty())
    {
        this->QuickHull();
    }
    if (!this->m_HullIndices.empty())
    {
        this->DrawHull();
    }
}

} // namespace itk

#endif // __itkBinaryImageToConvexHullFilter_hxx