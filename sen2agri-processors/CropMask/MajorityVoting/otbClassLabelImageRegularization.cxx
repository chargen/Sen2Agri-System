/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) CS SI. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

//#include <otbImage.h>
#include "itkAttributeLabelObject.h"
#include "itkLabelMap.h"
#include <itkLabelImageToLabelMapFilter.h>
#include <itkLabelMapToAttributeImageFilter.h>
#include "otbLabelMapWithMajorityClassLabelFilter.h"

namespace otb
{
namespace Wrapper
{


class MajorityVoting : public Application
{
public:
  /** Standard class typedefs. */
  typedef MajorityVoting                    Self;
  typedef Application                   	Superclass;
  typedef itk::SmartPointer<Self>       	Pointer;
  typedef itk::SmartPointer<const Self> 	ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);
  itkTypeMacro(ClassLabelImageRegularization, otb::Application);


  // Typedef list
  typedef unsigned int                              	LabelType;
  //typedef otb::Image<LabelType, 2>                  	ImageType;
  typedef itk::AttributeLabelObject<LabelType, 2, LabelType>           LabelObjectType; //1st attrib : segmentation type, 3rd attrib : classification type
  typedef itk::LabelMap<LabelObjectType>				LabelMapType;
  typedef otb::LabelMapWithMajorityClassLabelFilter<LabelMapType , UInt32ImageType /*ImageType*/ >  	ClassRegularizationFilterType;
  typedef itk::LabelImageToLabelMapFilter<UInt32ImageType /*ImageType*/ , LabelMapType > 		ConverterType;
  typedef itk::LabelMapToAttributeImageFilter< LabelMapType, UInt32ImageType /*ImageType*/ > 		AttributeToImageType;

private:
  void DoInit()
  {
    SetName("MajorityVoting");
    SetDescription("Perform classification regularization using a segmentation");

    // Documentation
    SetDocName("Class label image regularization ");
    SetDocLongDescription("This application performs classification (input 1) regularization using a prior segmentation (input 2)"
			  "For each segment, the most frequent class label is obtained and set to the new classification label image (output)");
    SetDocLimitations("Streaming is not available");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    //AddDocTag(Tags::Filter);
    AddDocTag("Classification");
    AddDocTag("Segmentation");
    AddDocTag("Regularization");

    AddParameter(ParameterType_InputImage,   "inclass",     "Input Image (Classification)");
    SetParameterDescription( "inclass", "Input 1 : classification provided by a label image" );

    AddParameter(ParameterType_InputImage,   "inseg",     "Input Image (Segmentation)");
    SetParameterDescription( "inseg", "Input 2 : segmentation provided by a label image" );

    AddParameter(ParameterType_OutputImage,  "rout",    "Output Image (regularized classification)");
    SetParameterDescription( "rout", "Output label image of the regularized classication" );

    AddParameter(ParameterType_Int, "nodatasegvalue", "The label that corresponds to no data within the segmentation image");
    SetParameterDescription("nodatasegvalue", "The label that corresponds to no data within the segmentation image");
    SetDefaultParameterInt("nodatasegvalue", 0);
    SetMinimumParameterIntValue("nodatasegvalue", 0);
    MandatoryOff("nodatasegvalue");

    AddParameter(ParameterType_Int, "nodataclassifvalue", "The label that corresponds to no data within the classification image");
    SetParameterDescription("nodataclassifvalue", "The label that corresponds to no data within the classification image");
    SetDefaultParameterInt("nodataclassifvalue", 0);
    SetMinimumParameterIntValue("nodataclassifvalue", 0);
    MandatoryOff("nodataclassifvalue");

    // Doc example parameter settings
    SetDocExampleParameterValue("inclass", "classImage.tif");
    SetDocExampleParameterValue("inseg", "classSeg.tif");
    SetDocExampleParameterValue("rout", "regulClassOutput.tif");
    SetDocExampleParameterValue("nodatasegvalue", "0");
    SetDocExampleParameterValue("nodataclassifvalue", "0");

  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    UInt32ImageType* inputC = GetParameterUInt32Image("inclass");
    UInt32ImageType* inputS = GetParameterUInt32Image("inseg");

    //Instanciations
    m_Image2LabelMap = ConverterType::New();
    m_Attribute2Image = AttributeToImageType::New();
    m_ClassRegularization = ClassRegularizationFilterType::New();

    //Settings
    m_ClassRegularization->SetNoDataSegValue(GetParameterInt("nodatasegvalue"));
    m_ClassRegularization->SetNoDataClassifValue(GetParameterInt("nodataclassifvalue"));

    //Pipeline
    m_Image2LabelMap->SetInput(inputS);
    m_ClassRegularization->SetInput(m_Image2LabelMap->GetOutput());
    m_ClassRegularization->SetClassifImage(inputC);
    m_Attribute2Image->SetInput(m_ClassRegularization->GetOutput());
    SetParameterOutputImage("rout", m_Attribute2Image->GetOutput());
    
   }

   //Keep object references as a members of the class, else the pipeline will be broken after exiting DoExecute().
   ConverterType::Pointer m_Image2LabelMap;
   AttributeToImageType::Pointer  m_Attribute2Image;
   ClassRegularizationFilterType::Pointer m_ClassRegularization;


};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::MajorityVoting)