// -*- C++ -*-
/*!
 * @file  WebCamera.cpp
 * @brief Web Camera RTC with Common Camera Interface
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
    "description",       "Web Camera RTC with Common Camera Interface",
    "version",           "1.0.0",
    "vendor",            "Kenichi Ohara, Meijo University",
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
    "conf.__widget__.output_color_format", "ordered_list",
    "conf.__widget__.camera_param_filename", "ordered_list",
    "conf.__widget__.undistortion_flag", "radio",
    "conf.__widget__.cap_continuous_flag", "radio",
    // Constraints
    "conf.__constraints__.output_color_format", "(RGB,GRAY)",
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
  return RTC::RTC_OK;
}


RTC::ReturnCode_t WebCamera::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t WebCamera::onExecute(RTC::UniqueId ec_id)
{
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


