// -*-C++-*-
/*!
 * @file  ImgSVC_impl.cpp
 * @brief Service implementation code of Img.idl
 *
 */

#include "ImgSVC_impl.h"

/*
 * Example implementational code for IDL interface Img::CameraCaptureService
 */
CameraCaptureServiceSVC_impl::CameraCaptureServiceSVC_impl()
{
  // Please add extra constructor code here.
}


CameraCaptureServiceSVC_impl::~CameraCaptureServiceSVC_impl()
{
  // Please add extra destructor code here.
}


/*
 * Methods corresponding to IDL attributes and operations
 */
void CameraCaptureServiceSVC_impl::take_one_frame()
{
  // Please insert your code here and remove the following warning pragma
#ifndef WIN32
  #warning "Code missing in function <void CameraCaptureServiceSVC_impl::take_one_frame()>"
#endif
}


void CameraCaptureServiceSVC_impl::take_multi_frames(CORBA::Long num)
{
  cap_count = num;
}

void CameraCaptureServiceSVC_impl::start_continuous()
{
  cap_continuous = true;
}

void CameraCaptureServiceSVC_impl::stop_continuous()
{
  cap_continuous = false;
}


// End of example implementational code



