
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg?style=shield)](./LICENSE)

# itkBinaryConvexHullFilter

## Description

This is a set of ITK filters that take a binary or label input nD image and compute the pixel precise 2D convex hull using quick-hull algorithm.

## Requirement

(Minimum)  
[Insight ToolKit (ITK)](https://itk.org/) += 5.0  
[CMake](https://cmake.org/) += 3.0  

 ## itkBinaryImageToConvexHullFilter

To Be Writen

<!-- The filter inherit from ImageToImageFilter and take in input a 2D or 3D binary image and return the corresponding convex hull mask of the binary pixel. If the image is label, the filter take an optional parameter m_LabelValue and, if specified, will only compute the hull of the specified label
object in the image. All label correspondant are respected.  
  
The filter work more efficiently if used with the filter->GetOutput()->SetRequestedRegion(ObjectRegion), where ObjectRegion is the region of the object/label
to process.  
The filter work best with itkLabelGeometryImageFilter class to pre-compute region, indices, and iterate on each label in the image.
See itkBinaryImageToConvexHullFilterTest.cpp and itkBinaryImageToConvexHullFilterTest2.cpp for proper usage of the filter. -->