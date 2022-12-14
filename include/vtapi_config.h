/**
 * @file
 * @brief Optional %VTApi configuration for includes. Comment \#includes to make
 * %VTApi more lightweight..
 *
 * @copyright Brno University of Technology &copy; 2011 &ndash; 2012
 *
 * VTApi is distributed under BUT OPEN SOURCE LICENCE (Version 1).
 * This licence agreement provides in essentials the same extent of rights as the terms of GNU GPL version 2 and Software fulfils the requirements of the Open Source software.
 *
 * @authors
 * Petr Chmelar, chmelarp (at) fit.vutbr.cz
 * Vojtech Froml, xfroml00 (at) stud.fit.vutbr.cz
 * Tomas Volf, ivolf (at) fit.vutbr.cz
 *
 */

#ifndef VTAPI_CONFIG_H
#define VTAPI_CONFIG_H

// comment this, if there is no PostGIS extension in the database
#define POSTGIS

// comment this under compilers with no copyfmt/rdbuf capabilities (GCC4.6 @ merlin)
#define COPYRDBUF

// libpqtypes
#include <libpqtypes.h> // tohle se pak poresi configure

// libproc library
// for this you need the libprocps-dev (sometimes libproc-dev, procps-devel) package
#if !(defined(WIN32) || defined(WIN64))
#include <proc/readproc.h>
#endif

// OpenCV header files
// #include <opencv2/opencv.hpp>
// #include <opencv2/highgui/highgui.hpp>
// OpenCV
// #include <opencv2/core/core.hpp>
// #include <opencv2/core/core_c.h>
// #include <opencv2/imgproc/imgproc.hpp>
// This adds the ability to debug image and video
// #include <opencv2/highgui/highgui.hpp>

// GEOS 3.3.3 (http://trac.osgeo.org/geos/) - C wrapper
#ifdef	__cplusplus
extern "C" {
#endif
#include <geos_c.h>
#ifdef	__cplusplus
}
#endif

// postGIS 2.0
#ifdef	__cplusplus
extern "C" {
#endif
#include "../postgres/liblwgeom/liblwgeom.h"
#include "../postgres/cube/cubedata.h"
#ifdef	__cplusplus
}
#endif

#endif	/* VTAPI_CONFIG_H */

