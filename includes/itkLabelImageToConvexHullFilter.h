#ifndef __itkLabelImageToConvexHullFilter_h
#define __itkLabelImageToConvexHullFilter_h

#include "itkBinaryImageToConvexHullFilter.h"
#include "itkImageToImageFilter.h"
#include "itkLabelGeometryImageFilter.h"

namespace itk
{

/** \class LabelImageToConvexHullFilter
 * \brief Compute the convex hull mask of an image (2D).
 *
 * The filter use a 2D QuickHull implementation to the pixel
 * indices of the hull of the binary object in the image and
 * draw its corresponding convex hull mask.
 *
 * The filter works on image region and/or provided label
 * to process labeled image.
 *
 * \author Stephane U. Rigaud (stephane.rigaud@pasteur.fr)
 */
template <class TImage, class TFilter = LabelGeometryImageFilter<TImage>>
class LabelImageToConvexHullFilter : public ImageToImageFilter<TImage, TImage>
{
  public:
    ITK_DISALLOW_COPY_AND_ASSIGN(LabelImageToConvexHullFilter);

    /** Standard class typedef. */
    using Self = LabelImageToConvexHullFilter;
    using Superclass = ImageToImageFilter<TImage, TImage>;
    using Pointer = SmartPointer<Self>;
    using ConstPointer = SmartPointer<const Self>;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(LabelImageToConvexHullFilter, ImageToImageFilter);

    /** Get input dimension. */
    static constexpr unsigned int Dimension = TImage::ImageDimension;

    /** Public typedef definitons. */
    using ImageType = TImage;
    using ImagePointer = typename ImageType::Pointer;
    using PixelType = typename ImageType::PixelType;

    using LabelFilterType = TFilter;
    using LabelFilterPointer = typename LabelFilterType::Pointer;
    using LabelsType = typename LabelFilterType::LabelsType;
    using LabelPixelType = typename LabelFilterType::LabelPixelType;
    using LabelRegionType = typename LabelFilterType::RegionType;
    using LabelIndicesType = typename LabelFilterType::LabelIndicesType;

    using HullFilterType = BinaryImageToConvexHullFilter<ImageType>;
    using HullFilterPointer = typename HullFilterType::Pointer;

    /** Set/Get macro for Label Value. */
    itkSetMacro(LabelFilter, LabelFilterPointer);
    itkGetMacro(LabelFilter, LabelFilterPointer);

#ifdef ITK_USE_CONCEPT_CHECKING
    // Begin concept checking
    itkConceptMacro(InputHasNumericTraitsCheck, (Concept::HasNumericTraits<PixelType>));
    itkConceptMacro(ValidDimension, (Concept::SameDimension<Dimension, 2>));
    // End concept checking
#endif

  protected:
    /**  Declarator and Destructor. */
    LabelImageToConvexHullFilter();
    ~LabelImageToConvexHullFilter() override = default;

    /**  Does the real work. */
    void GenerateData() override;

  private:
    LabelFilterPointer m_LabelFilter;
    HullFilterPointer m_HullFilter;
};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLabelImageToConvexHullFilter.hxx"
#endif

#endif // __itkLabelImageToConvexHullFilter_h