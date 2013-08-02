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
  m_cap_continuous = false;
	m_cap_count = 0;
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
	m_cap_count = 1;
}

void CameraCaptureServiceSVC_impl::take_multi_frames(CORBA::Long num)
{
	m_cap_count = num;
}

void CameraCaptureServiceSVC_impl::start_continuous()
{
	m_cap_continuous = true;
}

void CameraCaptureServiceSVC_impl::stop_continuous()
{
	m_cap_continuous = false;
}



// End of example implementational code



