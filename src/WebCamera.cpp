// -*- C++ -*-
/*!
 * @file  WebCamera.cpp
 * @brief Web Camera RTC with common camera interface version 2.0
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
    "description",       "Web Camera RTC with common camera interface version 2.0",
    "version",           "2.0.0",
    "vendor",            "Kenichi Ohara, Meijo University",
    "category",          "ImageProcessing",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.camera_id", "0",
    "conf.default.output_color_format", "RGB",
    "conf.default.camera_param_filename", "NULL",
    "conf.default.undistortion_flag", "false",
    "conf.default.cap_continuous_flag", "true",
    "conf.default.compression_ratio", "75",
    "conf.default.frame_width", "640",
    "conf.default.frame_height", "480",
    // Widget
    "conf.__widget__.camera_id", "text",
    "conf.__widget__.output_color_format", "radio",
    "conf.__widget__.camera_param_filename", "text",
    "conf.__widget__.undistortion_flag", "radio",
    "conf.__widget__.cap_continuous_flag", "radio",
    "conf.__widget__.compression_ratio", "slider.1",
    // Constraints
    "conf.__constraints__.output_color_format", "(RGB,GRAY,JPEG,PNG)",
    "conf.__constraints__.undistortion_flag", "(true,false)",
    "conf.__constraints__.cap_continuous_flag", "(true,false)",
    "conf.__constraints__.compression_ratio", "0<=x<=100",
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
  bindParameter("camera_param_filename", m_camera_param_filename, "..\\..\\camera.yml");
  bindParameter("undistortion_flag", m_undistortion_flag, "false");
  bindParameter("cap_continuous_flag", m_cap_continuous_flag, "false");
  bindParameter("compression_ratio", m_compression_ratio, "75");
  bindParameter("frame_width", m_frame_width, "640");
  bindParameter("frame_height", m_frame_height, "480");
  // </rtc-template>
  
  isFileLoad = false;
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
  //Open camera device
  if(!cam_cap.open(m_camera_id))
    {
      RTC_ERROR(("Unable to open selected video device ID:[%d].", m_camera_id));
      return RTC::RTC_ERROR;
    }
  
  cam_cap.set(CV_CAP_PROP_FRAME_WIDTH, m_frame_width);
  cam_cap.set(CV_CAP_PROP_FRAME_HEIGHT, m_frame_height);	
  //Get and show the camera device properties
  cam_cap >> src_image;
  width = src_image.cols;
  height = src_image.rows;
  depth = src_image.depth();
  
  std::cout << "Image size:" << width << " x " << height << std::endl;
  std::cout << "Depth     :" << depth << std::endl;
  std::cout << "Channles  :" << src_image.channels() << std::endl;
  
  //Check channels of camera device
  nchannels = (m_output_color_format == "GRAY") ? 1 : 3;
  
  if(nchannels > src_image.channels())
    {
      if(m_output_color_format == "RGB" || m_output_color_format == "JPEG" || m_output_color_format == "PNG")
	{
	  std::cout << "Convert GRAY image to RGB color image" <<std::endl;
	}
    }
  else if( nchannels < src_image.channels() )
    {
      std::cout<< "Convert RGB color image to GRAY image" <<std::endl;
    }
  else
    {
      if(m_output_color_format == "RGB" || m_output_color_format == "JPEG" || m_output_color_format == "PNG")
	{
	  std::cout << "Convert BGR color image to RGB color image" << std::endl;			
	}
      else if(m_output_color_format == "GRAY")
	{
			std::cout << "Gray image" <<std::endl;
	}
    }
  
  //Load camera parameter
  //If camera parameter file could not be found, whole camera parameters are set to zero.
  std::cout<<"Loading camera parameter file: "<< m_camera_param_filename << std::endl;
  
  cv::FileStorage fs(m_camera_param_filename, cv::FileStorage::READ);
  if(fs.isOpened())
    {
      isFileLoad = true;
      fs["image_width"] >> cam_param.imageSize.width;
      fs["image_height"] >> cam_param.imageSize.height;
      fs["camera_matrix"] >> cam_param.cameraMatrix;
      fs["distortion_coefficients"] >> cam_param.distCoeffs;
      
      std::cout << "=================================================" << std::endl;
      std::cout << "Camera Parameter" <<std::endl;
      std::cout << "=================================================" << std::endl;
      
      std::cout << "Image size: " << cam_param.imageSize.width << "x" << cam_param.imageSize.height << std::endl;
      std::cout << "Camera Matrix: "<< cam_param.cameraMatrix << std::endl;
      std::cout << "Distortion coefficients: " << cam_param.distCoeffs << std::endl;
      
      //Set distortion coefficient to make rectify map
      CameraParam *param;
      param = &cam_param;
      cv::initUndistortRectifyMap(param->cameraMatrix, param->distCoeffs, cv::Mat(),
				  cv::getOptimalNewCameraMatrix(param->cameraMatrix, param->distCoeffs, param->imageSize, 1, param->imageSize, 0),
				  param->imageSize, CV_16SC2, param->map1, param->map2);
      
      //Set default capture mode
      m_CameraCaptureService.m_cap_continuous = coil::toBool(m_cap_continuous_flag, "true", "false");

      std::cout << "Capture mode: " << m_cap_continuous_flag << std::endl;
      std::cout << "Capture start!!" << std::endl;
    }
  else
    {
      RTC_ERROR(( "Unable to open selected camera parameter file: %s", m_camera_param_filename.c_str() ));
      RTC_ERROR(( "Camera parameters are set to zero" ));
      std::cout << "Unable to open selected camera parameter file: " << m_camera_param_filename.c_str() << std::endl;
      std::cout << "This program sets camera parameter as all zero." << std::endl;

      isFileLoad = false;
      
      cam_param.imageSize.width = width;
      cam_param.imageSize.height = height;
      //Clear camera intrinsic parameter to zero
      cam_param.cameraMatrix.at<double>(0,0) = 0.0;
      cam_param.cameraMatrix.at<double>(0,1) = 0.0;
      cam_param.cameraMatrix.at<double>(0,2) = 0.0;
      cam_param.cameraMatrix.at<double>(1,0) = 0.0;
      cam_param.cameraMatrix.at<double>(1,1) = 0.0;
      cam_param.cameraMatrix.at<double>(1,2) = 0.0;
      cam_param.cameraMatrix.at<double>(2,0) = 0.0;
      cam_param.cameraMatrix.at<double>(2,1) = 0.0;
      cam_param.cameraMatrix.at<double>(2,2) = 1.0;
      
      //Clear distortion parameter to zero
      
      cam_param.distCoeffs.at<double>(0) = 0.0;
      cam_param.distCoeffs.at<double>(1) = 0.0;
      cam_param.distCoeffs.at<double>(2) = 0.0;
      cam_param.distCoeffs.at<double>(3) = 0.0;
      cam_param.distCoeffs.at<double>(4) = 0.0;
      
      return RTC::RTC_OK;
    }
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t WebCamera::onDeactivated(RTC::UniqueId ec_id)
{
  std::cout<<"Capture stop!!" <<std::endl;
  
  //Release the device handler and allocated image buffer
  src_image.release();
  cam_cap.release();
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t WebCamera::onExecute(RTC::UniqueId ec_id)
{
  //Capture mode select
  if(m_CameraCaptureService.m_cap_continuous || (m_CameraCaptureService.m_cap_count > 0))
    {
      if( m_CameraCaptureService.m_cap_count > 0) --m_CameraCaptureService.m_cap_count;
      
      //Check image channels for captured image via camera
      nchannels = ( m_output_color_format == "GRAY") ? 1 : 3;
      
      //Image Capture
      cam_cap >> src_image;
      if( src_image.empty() )
	{
	  RTC_WARN(("Capture image data is empty!!"));
	  return RTC::RTC_OK;
	}
      
      //***********************************************************************************
      //***********************************************************************************
      //Following part is the template for common camera interface
      //Don't change following part
      //***********************************************************************************
      //***********************************************************************************
      
      //Set timestamp
      setTimestamp(m_CameraImage);
      m_CameraImage.data.captured_time = m_CameraImage.tm;
      
      //Color conversion process
      cv::Mat proc_image;
      if(nchannels > src_image.channels())
	{
	  if( m_output_color_format == "RGB" || m_output_color_format == "JPEG" || m_output_color_format == "PNG")
	    cv::cvtColor(src_image, proc_image, cv::COLOR_GRAY2RGB/*CV_GRAY2RGB*/);
	  nchannels = 3;
	}
      else if( nchannels < src_image.channels() )
	{
	  if( m_output_color_format == "GRAY" )
	    {
	      cv::cvtColor(src_image, proc_image, cv::COLOR_BGR2GRAY/*CV_BGR2GRAY*/);
      nchannels = 1;
    }
	}
      else
	{
	  if( m_output_color_format == "RGB" /*|| m_output_color_format == "JPEG" || m_output_color_format == "PNG"*/)
	    cv::cvtColor(src_image, proc_image, cv::COLOR_BGR2RGB/*CV_BGR2RGB*/);
	  else
	    proc_image = src_image;
	}
      //Set camera parameter to output structure data
      m_CameraImage.data.intrinsic.matrix_element[0] = cam_param.cameraMatrix.at<double>(0,0);
      m_CameraImage.data.intrinsic.matrix_element[1] = cam_param.cameraMatrix.at<double>(0,2);
      m_CameraImage.data.intrinsic.matrix_element[2] = cam_param.cameraMatrix.at<double>(1,1);
      m_CameraImage.data.intrinsic.matrix_element[3] = cam_param.cameraMatrix.at<double>(1,2);
      m_CameraImage.data.intrinsic.matrix_element[4] = cam_param.cameraMatrix.at<double>(2,2);


      //Copy undistortion matrix
      m_CameraImage.data.intrinsic.distortion_coefficient.length(cam_param.distCoeffs.rows);
      cv::Mat distortion_temp;
      if(coil::toBool(m_undistortion_flag, "true", "false"))
	{
	  if(isFileLoad)
	    {
	      distortion_temp = proc_image.clone();
	      cv::undistort(distortion_temp, proc_image, cam_param.cameraMatrix, cam_param.distCoeffs, cam_param.cameraMatrix);
	      
	    }
	  //Copy distortion coefficient to output parameter
	  for(int j(0); j < 5; ++j)
	    m_CameraImage.data.intrinsic.distortion_coefficient[j] = 0.0; 
	}
      else
	{
	  //Copy distortion coefficient to output parameter
	  for(int j(0); j < 5; ++j)
	    m_CameraImage.data.intrinsic.distortion_coefficient[j] = cam_param.distCoeffs.at<double>(j);
	}
      
      //Copy image parameter to output data based on TimedCameraImage structure
      m_CameraImage.data.image.width = width;
      m_CameraImage.data.image.height = height;
      
      //Transmission image data creation based on selected image compression mode
      if( m_output_color_format == "RGB")
	{
	  m_CameraImage.data.image.format = Img::CF_RGB;
	  m_CameraImage.data.image.raw_data.length( width * height * nchannels);
	  for( int i(0); i< height; ++i )
	    memcpy(&m_CameraImage.data.image.raw_data[ i * width * nchannels], &proc_image.data[ i * proc_image.step ], width * nchannels);
	}
      else if(m_output_color_format == "JPEG")
	{
	  m_CameraImage.data.image.format = Img::CF_JPEG;
	  //Jpeg encoding using OpenCV image compression function
	  std::vector<int> compression_param = std::vector<int>(2); 
	  compression_param[0] = CV_IMWRITE_JPEG_QUALITY;
	  compression_param[1] = m_compression_ratio;
	  //Encode raw image data to jpeg data
	  std::vector<uchar> compressed_image;
	  cv::imencode(".jpg", proc_image, compressed_image, compression_param);
	  //Copy encoded jpeg data to Outport Buffer
	  m_CameraImage.data.image.raw_data.length(compressed_image.size());
	  memcpy(&m_CameraImage.data.image.raw_data[0], &compressed_image[0], sizeof(unsigned char) * compressed_image.size());
	}
      else if(m_output_color_format == "PNG")
	{
	  m_CameraImage.data.image.format = Img::CF_PNG;
	  //Jpeg encoding using OpenCV image compression function
	  std::vector<int> compression_param = std::vector<int>(2); 
	  compression_param[0] = CV_IMWRITE_PNG_COMPRESSION;
	  compression_param[1] = (int)((double)m_compression_ratio/10.0);
	  if(compression_param[1] == 10)
	    compression_param[1] = 9;
	  std::cout<<"PNG compression ratio: "<<compression_param[1] << "\r";
	  
	  
	  //Encode raw image data to jpeg data
	  std::vector<uchar> compressed_image;
	  cv::imencode(".png", proc_image, compressed_image, compression_param);
	  //Copy encoded jpeg data to Outport Buffer
	  m_CameraImage.data.image.raw_data.length(compressed_image.size());
	  memcpy(&m_CameraImage.data.image.raw_data[0], &compressed_image[0], sizeof(unsigned char) * compressed_image.size());
	}
      else
	{
	  m_CameraImage.data.image.format = Img::CF_GRAY;
	  
	  std::cout<<"Selected image compression mode is not defined. Please confirm correct compression mode!"<<std::endl;
	  m_CameraImage.data.image.raw_data.length( width * height * nchannels);
	  for( int i(0); i< height; ++i )
	    memcpy(&m_CameraImage.data.image.raw_data[ i * width * nchannels], &proc_image.data[ i * proc_image.step ], width * nchannels);			
	}
      //Output image data via OutPort
      m_CameraImageOut.write();
      //***********************************************************************************
      //End of the template part
      //***********************************************************************************
    }
  else
    {
      RTC_DEBUG( ("Waiting capture mode command via ServicePort") );
      std::cout << "Waiting capture mode command via ServicePort" << std::endl;
      return RTC::RTC_OK;
    }
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


