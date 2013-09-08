/***************************************************************************
 *   Copyright (C) 1998-2013 by authors (see AUTHORS.txt)                  *
 *                                                                         *
 *   This file is part of LuxRays.                                         *
 *                                                                         *
 *   LuxRays is free software; you can redistribute it and/or modify       *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   LuxRays is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 *   LuxRays website: http://www.luxrender.net                             *
 ***************************************************************************/

#ifndef _SLG_RT_PATHOCL_H
#define	_SLG_RT_PATHOCL_H

#if !defined(LUXRAYS_DISABLE_OPENCL)

#include "slg/engines/pathocl/pathocl.h"

namespace slg {

class RTPathOCLRenderEngine;

//------------------------------------------------------------------------------
// Real-Time Path Tracing GPU-only render threads
//------------------------------------------------------------------------------

class RTPathOCLRenderThread : public PathOCLRenderThread {
public:
	RTPathOCLRenderThread(const u_int index, luxrays::OpenCLIntersectionDevice *device,
			PathOCLRenderEngine *re);
	virtual ~RTPathOCLRenderThread();

	virtual void Interrupt();
	virtual void Stop();

	virtual void BeginEdit();
	virtual void EndEdit(const EditActionList &editActions);

	void SetAssignedIterations(const u_int iters) { assignedIters = iters; }
	u_int GetAssignedIterations() const { return assignedIters; }
	double GetFrameTime() const { return frameTime; }
	u_int GetMinIterationsToShow() const;

	friend class RTPathOCLRenderEngine;

protected:
	void InitDisplayThread();
	void UpdateOCLBuffers();

	virtual void RenderThreadImpl();

	virtual void InitRender();
	virtual void SetKernelArgs();
	
	boost::mutex editMutex;
	EditActionList updateActions;
	volatile double frameTime;
	volatile u_int assignedIters;

	// OpenCL variables
	cl::Buffer *tmpFrameBufferBuff;
	cl::Buffer *mergedFrameBufferBuff;
	cl::Buffer *screenBufferBuff;
};

//------------------------------------------------------------------------------
// Real-Time Path Tracing 100% OpenCL render engine
//------------------------------------------------------------------------------

class RTPathOCLRenderEngine : public PathOCLRenderEngine {
public:
	RTPathOCLRenderEngine(RenderConfig *cfg, Film *flm, boost::mutex *flmMutex);
	virtual ~RTPathOCLRenderEngine();

	virtual RenderEngineType GetEngineType() const { return RTPATHOCL; }
	double GetFrameTime() const { return frameTime; }

	bool WaitNewFrame();

	friend class RTPathOCLRenderThread;

protected:
	virtual PathOCLRenderThread *CreateOCLThread(const u_int index,
		luxrays::OpenCLIntersectionDevice *device);

	virtual void StartLockLess();
	virtual void StopLockLess();
	virtual void UpdateFilmLockLess();

	u_int minIterations;
	u_int displayDeviceIndex;
 
	boost::barrier *frameBarrier;
	double frameTime;
};

}

#endif

#endif	/* _SLG_RT_PATHOCL_H */
