#ifndef __itkBinaryImageToConvexHullFilter_h
#define __itkBinaryImageToConvexHullFilter_h

#include "itkImageToImageFilter.h"

namespace itk
{

/** \class BinaryImageToConvexHullFilter
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
template <class TImage> class BinaryImageToConvexHullFilter : public ImageToImageFilter<TImage, TImage>
{
  public:
    ITK_DISALLOW_COPY_AND_ASSIGN(BinaryImageToConvexHullFilter);

    /** Standard class typedef. */
    using Self = BinaryImageToConvexHullFilter;
    using Superclass = ImageToImageFilter<TImage, TImage>;
    using Pointer = SmartPointer<Self>;
    using ConstPointer = SmartPointer<const Self>;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(BinaryImageToConvexHullFilter, ImageToImageFilter);

    /** Get input dimension. */
    static constexpr unsigned int Dimension = TImage::ImageDimension;

    /** Public typedef definitons. */
    using ImageType = TImage;
    using ImagePointer = typename ImageType::Pointer;
    using ImageConstPointer = typename ImageType::ConstPointer;
    using PixelType = typename ImageType::PixelType;
    using IndexType = typename ImageType::IndexType;
    using IndexValueType = typename IndexType::IndexValueType;
    using LabelIndicesType = std::vector<IndexType>;

    /** Set/Get macro for Label Value. */
    itkSetMacro(LabelValue, PixelType);
    itkGetMacro(LabelValue, PixelType);

    /** Set/Get macro for Hull Indices vector. */
    LabelIndicesType GetHullIndices() const;
    void SetHullIndices(LabelIndicesType &);

    /** Set/Get macro for Label Indices vector. */
    LabelIndicesType GetLabelIndices() const;
    void SetLabelIndices(LabelIndicesType &);

#ifdef ITK_USE_CONCEPT_CHECKING
    // Begin concept checking
    itkConceptMacro(InputHasNumericTraitsCheck, (Concept::HasNumericTraits<PixelType>));
    itkConceptMacro(ValidDimension, (Concept::SameDimension<Dimension, 2>));
    // End concept checking
#endif

  protected:
    /**  Declarator and Destructor. */
    BinaryImageToConvexHullFilter();
    ~BinaryImageToConvexHullFilter() override = default;

    /**  Does the real work. */
    void GenerateData() override;

    /** Algorithm methods. */
    IndexValueType LineDistance(IndexType &, IndexType &, IndexType &);
    IndexValueType WhichSide(IndexValueType);
    void FindHull(LabelIndicesType &, IndexType &, IndexType &);
    void DrawHull();
    void QuickHull();
    void GetIndices();

  private:
    PixelType m_LabelValue;
    LabelIndicesType m_LabelIndices;
    LabelIndicesType m_HullIndices;
};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryImageToConvexHullFilter.hxx"
#endif

#endif // __itkBinaryImageToConvexHullFilter_h