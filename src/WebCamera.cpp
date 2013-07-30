// -*- C++ -*-
/*!
 * @file  WebCamera.cpp
 * @brief Image Capture Component for General Web Camera
 * @date $Date$
 *
 * $Id$
 */

#include "WebCamera.h"

// Module specification
// <rtc-template block="module_spec">
static const char* webcamera_spec[] =
  {
    "implementation_id", "WebCamera",
    "type_name",         "WebCamera",
    "description",       "Image Capture Component for General Web Camera",
    "version",           "1.0.0",
    "vendor",            "Kenichi Ohara, Osaka University",
    "category",          "Image Processing",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.camera_id", "0",
    "conf.default.output_color_format", "RGB",
    "conf.default.camera_param_filename", "camera.yml",
    "conf.default.undistortion_flag", "false",
    "conf.default.cap_continuous_flag", "false",
    // Widget
    "conf.__widget__.camera_id", "text",
    "conf.__widget__.output_color_format", "radio",
    "conf.__widget__.camera_param_filename", "radio",
    "conf.__widget__.undistortion_flag", "radio",
    "conf.__widget__.cap_continuous_flag", "radio",
    // Constraints
    "conf.__constraints__.output_color_format", "(RGB, GRAY)",
    "conf.__constraints__.undistortion_flag", "(true,false)",
    "conf.__constraints__.cap_continuous_flag", "(true,false)",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
WebCamera::WebCamera(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_CameraImageOut("CameraImage", m_CameraImage),
    m_CameraCaptureServicePort("CameraCaptureService")
    // </rtc-template>
{
	//Initialization of variables
	cap				= NULL;
	cap_img			= NULL;
	nChannels_img	= NULL;
	image			= NULL;
	temp_image		= NULL;
}

/*!
 * @brief destructor
 */
WebCamera::~WebCamera()
{
}



RTC::ReturnCode_t WebCamera::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("CameraImage", m_CameraImageOut);
  
  // Set service provider to Ports
  m_CameraCaptureServicePort.registerProvider("CameraCaptureService", "Img::CameraCaptureService", m_CameraCaptureService);
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  addPort(m_CameraCaptureServicePort);
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("camera_id", m_camera_id, "0");
  bindParameter("output_color_format", m_output_color_format, "RGB");
  bindParameter("camera_param_filename", m_camera_param_filename, "camera.yml");
  bindParameter("undistortion_flag", m_undistortion_flag, "false");
  bindParameter("cap_continuous_flag", m_cap_continuous_flag, "false");
  
  // </rtc-template>
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t WebCamera::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t WebCamera::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t WebCamera::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t WebCamera::onActivated(RTC::UniqueId ec_id)
{
  //Camera Device Open
  if(!cap.open(m_camera_id))
  {
	  RTC_ERROR(("Unable to open video device. [%d]", m_camera_id));
      return RTC::RTC_ERROR;
  }


  //Get and show the camera device information
  cap >> cap_img;
  width = cap_img.cols;
  height = cap_img.rows;
  depth = cap_img.depth();

  std::cout << "Image Size:" << width << "x" << height << std::endl;
  std::cout << "DEPTH:" << depth << std::endl;
  std::cout << "CHANNEL:" << cap_img.channels() << std::endl;
  //For Debug on log file.
  RTC_TRACE(("Capture image size %d x %d", width, height));
  RTC_TRACE(("Depth %d, Channels %d", depth, cap_img.channels()));

  //Check channel of capture device
  channels = (m_output_color_format == "GRAY") ? 1 : 3;
  if(channels > cap_img.channels())
  {
    if(m_output_color_format == "RGB")
	{
		std::cout << "CONVERT: GRAY -> RGB" << std::endl;
    }
  }
  else if(channels < cap_img.channels())
  {
	  std::cout << "CONVERT: BGR -> GRAY" << std::endl;
  }
  else
  {
	  if(m_output_color_format == "RGB")
		  std::cout << "CONVERT: BGR -> RGB" << std::endl;
	  if(m_output_color_format == "GRAY")
		  std::cout << "OUTPUT: GRAY" << std::endl;
  }
  std::cout << "channels = " << channels << std::endl;

  //Load camera parameter
  std::cout << "Load camera parameter from " << m_camera_param_filename << std::endl;
  cv::FileStorage fs(m_camera_param_filename, cv::FileStorage::READ);
  if (!fs.isOpened())
  {
      RTC_ERROR(("Unable to open camera parameter file. [%s]",m_camera_param_filename.c_str()));
      std::cerr << "Could not open camera parameter." << std::endl;
      std::cerr << "Please confirm the filename and set the correct filename to configuration." << std::endl;
      std::cerr << "After setting a correct filename, please restart this component." << std::endl;
      return RTC::RTC_ERROR;
  }
  //Show loaded camera parameter 
  fs["image_width"] >> cam_param.imageSize.width;
  fs["image_height"] >> cam_param.imageSize.height;
  fs["camera_matrix"] >> cam_param.cameraMatrix;
  fs["distortion_coefficients"] >> cam_param.distCoeffs;
	  
  std::cout << "image size = " << cam_param.imageSize.width << " x " << cam_param.imageSize.height << std::endl;
  std::cout << "camera matrix = " << cam_param.cameraMatrix << std::endl;
  std::cout << "distortion coefficients = " << cam_param.distCoeffs << std::endl;
  std::cout << std::endl;
  //Set distortion coefficients to make rectify map
  CameraParam *cp = &cam_param;
  cv::initUndistortRectifyMap(cp->cameraMatrix, cp->distCoeffs, cv::Mat(),
                              getOptimalNewCameraMatrix(cp->cameraMatrix,
                              cp->distCoeffs, cp->imageSize, 1, cp->imageSize, 0),
                              cp->imageSize, CV_16SC2, cp->map1, cp->map2);


  //Set default continuous_flag, which is the capture mode setting flag
  m_CameraCaptureService.cap_continuous = m_cap_continuous_flag;

  std::cout << "Capture start..." << std::endl;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t WebCamera::onDeactivated(RTC::UniqueId ec_id)
{
  std::cout << "Capture Stop" << std::endl;

  //Release the device handler and allocated buffer
  cap.release();
  cap_img.release();
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t WebCamera::onExecute(RTC::UniqueId ec_id)
{
  //Capture type selection based on cap_continuous status
  if(m_CameraCaptureService.cap_continuous){;}  // do capture process
  else if(m_CameraCaptureService.cap_count > 0) --m_CameraCaptureService.cap_count;
  else
  {
	  RTC_DEBUG(("Waiting capture command from continuous_flag or Service Port."));
	  std::cout<< "Waiting for the capture command..." << std::endl;
	  return RTC::RTC_OK;
  }

  //Check channel status for captured image
  channels = (m_output_color_format == "GRAY") ? 1 : 3;

  //Image capture
  cap >> cap_img;
  if(cap_img.empty())
  {
	  RTC_WARN(("Image data is empty."));
	  return RTC::RTC_OK;
  }

  //***************************************************************************************
  //***************************************************************************************
  //Following part is the template for the common camera interface
  //Don't change following part.
  //***************************************************************************************
  //***************************************************************************************
  //Set time stamp for a captured time.
  setTimestamp(m_CameraImage);
  m_CameraImage.data.captured_time = m_CameraImage.tm;

  //Color conversion process
  //General output data type in the protocol of common camera interface is RGB.
  //So, if the image type of captured image is not RGB, channel number should be converted to RGB.
  if(channels > cap_img.channels())
  {
    if(m_output_color_format == "RGB")
	{
	  cv::cvtColor(cap_img, nChannels_img, CV_GRAY2RGB);
    }
    image = nChannels_img;
    channels = 3;
  }
  else if(channels < cap_img.channels())
  {
	  if(m_output_color_format == "GRAY")
	  {
		  cv::cvtColor(cap_img, nChannels_img, CV_BGR2GRAY);
		  image = nChannels_img;
		  channels = 1;
	  }
  }
  else
  {
    if(m_output_color_format == "RGB")
	{
		cv::cvtColor(cap_img, nChannels_img, CV_BGR2RGB);
		image = nChannels_img;
    }
	else
	{
		image = cap_img;
    }
  }

  // copy camera intrinsic matrix to Data Port
  m_CameraImage.data.intrinsic.matrix_element[0] = cam_param.cameraMatrix.at<double>(0, 0);
  m_CameraImage.data.intrinsic.matrix_element[1] = cam_param.cameraMatrix.at<double>(0, 1);
  m_CameraImage.data.intrinsic.matrix_element[2] = cam_param.cameraMatrix.at<double>(1, 1);
  m_CameraImage.data.intrinsic.matrix_element[3] = cam_param.cameraMatrix.at<double>(0, 2);
  m_CameraImage.data.intrinsic.matrix_element[4] = cam_param.cameraMatrix.at<double>(1, 2);
  m_CameraImage.data.intrinsic.distortion_coefficient.length(cam_param.distCoeffs.rows);

  //Copy undistortion matrix
  if(m_undistortion_flag)
  {
    cv::undistort(image, temp_image, cam_param.cameraMatrix, cam_param.distCoeffs, cam_param.cameraMatrix);
    //Copy distortion coefficient to the output parameter
    for(int j = 0; j < 5; ++j)
		m_CameraImage.data.intrinsic.distortion_coefficient[j] = 0.0;
    image = temp_image;
  }
  else
  {
    //Copy distortion coefficient to Data Port
    for(int j = 0; j < 5; ++j)
      m_CameraImage.data.intrinsic.distortion_coefficient[j] = cam_param.distCoeffs.at<double>(0, j);
  }

  //Copy Image parameter to Outport data based on TimedCameraImage structure
  m_CameraImage.data.image.width = width;
  m_CameraImage.data.image.height = height;
  m_CameraImage.data.image.format
    = (m_output_color_format == "RGB") ? Img::CF_RGB :
      (m_output_color_format == "GRAY") ? Img::CF_GRAY : Img::CF_UNKNOWN;
  m_CameraImage.data.image.raw_data.length(width * height * channels);
  for(int i(0); i<height; ++i){
    memcpy(&m_CameraImage.data.image.raw_data[i*width*channels],&image.data[i*image.step], sizeof(unsigned char)*width*channels);
  }

  //Output the captureing data
  m_CameraImageOut.write();
  //***************************************************************************************
  //End of the template part
  //***************************************************************************************

  return RTC::RTC_OK;
}


RTC::ReturnCode_t WebCamera::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t WebCamera::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t WebCamera::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t WebCamera::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t WebCamera::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void WebCameraInit(RTC::Manager* manager)
  {
    coil::Properties profile(webcamera_spec);
    manager->registerFactory(profile,
                             RTC::Create<WebCamera>,
                             RTC::Delete<WebCamera>);
  }
  
};


