
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg?style=shield)](./LICENSE)

# itkBinaryConvexHullFilter

## Description

This an ITK filter that take a binary or label input nD image and compute the pixel-precise 2D convex hull using quick-hull algorithm.

## Requirement

(Minimum)  
[Insight ToolKit (ITK)](https://itk.org/) += 5.0  
[CMake](https://cmake.org/) += 3.0  

 ## itkBinaryImageToConvexHullFilter

The filter inherit from ImageToImageFilter and take in input an nD binary or labelled image and return the corresponding convex hull. By default, the filter will consider all pixel which is not the background (=0) to compute the convex hull.
In the case of a labelled image it is possible to define the **LabeValue** to identify a specific label object in the input to process. In this case it is also possible to provide the **LabelIndices** and/or provide the object region using the **filter->GetOutput()->SetRequestedRegion(ObjectRegion)**. Both indices and region can be computed by the **itkLabelGeometryImageFilter**.

## Install

'''
git clone https://github.com/StRigaud/itkConvexHull2D.git  
mkdir itkConvexHull2D/build  
cd itkConvexHull2D/build  
ccmake -DCMAKE_BUILD_TYPE=Release ..  
make  
make test  
'''

All tests should pass sucessfully.

## Usage

See **itkBinaryImageToConvexHullFilterTest** and **itkBinaryImageToConvexHullFilterTest2** for example on how to use the filter.
