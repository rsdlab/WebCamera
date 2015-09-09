// -*- C++ -*-
/*!
 * @file  WebCamera.h
 * @brief Web Camera RTC with common camera interface version 2.0
 * @date  $Date$
 *
 * $Id$
 */

#ifndef WEBCAMERA_H
#define WEBCAMERA_H

#include "ImgSVC_impl.h"
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>

// Service implementation headers
// <rtc-template block="service_impl_h">


// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

//Include file for OpenCV functions
#include<opencv2/opencv.hpp>

using namespace RTC;
using namespace Img;

//Structure definition for common camera interface
typedef struct CameraParam_ 
{
	cv::Size	imageSize;
	cv::Mat		cameraMatrix;
	cv::Mat		distCoeffs;
	cv::Mat		map1, map2;
}CameraParam;


/*!
 * @class WebCamera
 * @brief Web Camera RTC with common camera interface version 2.0
 *
 */
class WebCamera
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  WebCamera(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~WebCamera();

  // <rtc-template block="public_attribute">
  
  // </rtc-template>

  // <rtc-template block="public_operation">
  
  // </rtc-template>

  /***
   *
   * The initialize action (on CREATED->ALIVE transition)
   * formaer rtc_init_entry() 
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onInitialize();

  /***
   *
   * The finalize action (on ALIVE->END transition)
   * formaer rtc_exiting_entry()
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onFinalize();

  /***
   *
   * The startup action when ExecutionContext startup
   * former rtc_starting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStartup(RTC::UniqueId ec_id);

  /***
   *
   * The shutdown action when ExecutionContext stop
   * former rtc_stopping_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onShutdown(RTC::UniqueId ec_id);

  /***
   *
   * The activated action (Active state entry action)
   * former rtc_active_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  /***
   *
   * The deactivated action (Active state exit action)
   * former rtc_active_exit()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  /***
   *
   * The execution action that is invoked periodically
   * former rtc_active_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

  /***
   *
   * The aborting action when main logic error occurred.
   * former rtc_aborting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onAborting(RTC::UniqueId ec_id);

  /***
   *
   * The error action in ERROR state
   * former rtc_error_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);

  /***
   *
   * The reset action that is invoked resetting
   * This is same but different the former rtc_init_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
  /***
   *
   * The state update action that is invoked after onExecute() action
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStateUpdate(RTC::UniqueId ec_id);

  /***
   *
   * The action that is invoked when execution context's rate is changed
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onRateChanged(RTC::UniqueId ec_id);


 protected:
  // <rtc-template block="protected_attribute">
  
  // </rtc-template>

  // <rtc-template block="protected_operation">
  
  // </rtc-template>

  // Configuration variable declaration
  // <rtc-template block="config_declare">
  /*!
   * 
   * - Name:  camera_id
   * - DefaultValue: 0
   */
  int m_camera_id;
  /*!
   * 
   * - Name:  output_color_format
   * - DefaultValue: RGB
   */
  std::string m_output_color_format;
  /*!
   * 
   * - Name:  camera_param_filename
   * - DefaultValue: NONE
   */
  std::string m_camera_param_filename;
  /*!
   * 
   * - Name:  undistortion_flag
   * - DefaultValue: false
   */
  std::string m_undistortion_flag;
  /*!
  *
  * - Name:  preview_window
  * - DefaultValue: false
  */
  std::string m_preview_window;
  /*!
   * 
   * - Name:  cap_continuous_flag
   * - DefaultValue: false
   */
  std::string m_cap_continuous_flag;
  /*!
   * This parameter can control the compression ratio for JPEG
   * and PNG color format.
   * - Name:  compression_ratio
   * - DefaultValue: 75
   */
  int m_compression_ratio;

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  Img::TimedCameraImage m_CameraImage;
  /*!
   */
  OutPort<Img::TimedCameraImage> m_CameraImageOut;
  
  // </rtc-template>

  // CORBA Port declaration
  // <rtc-template block="corbaport_declare">
  /*!
   */
  RTC::CorbaPort m_CameraCaptureServicePort;
  
  // </rtc-template>

  // Service declaration
  // <rtc-template block="service_declare">
  /*!
   */
  CameraCaptureServiceSVC_impl m_CameraCaptureService;
  
  // </rtc-template>

  // Consumer declaration
  // <rtc-template block="consumer_declare">
  
  // </rtc-template>

 private:
  // <rtc-template block="private_attribute">
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>
  cv::VideoCapture cam_cap;
  cv::Mat src_image, proc_image;
  int width, height, depth, nchannels;
  CameraParam cam_param;

  bool is_alive, is_active;

  // Ready flag for intrinstic matrix
  bool m_readyUndistortion;

  // In preview mode
  bool m_preview;
public:
  RTC::ReturnCode_t initCapture();
  RTC::ReturnCode_t finiCapture();
  RTC::ReturnCode_t captureAndProcess();


  bool isActive() {return is_active;}
  bool isAlive() {return is_alive;}

  std::string m_preview_window_name;
};


extern "C"
{
  DLL_EXPORT void WebCameraInit(RTC::Manager* manager);
};

#endif // WEBCAMERA_H
