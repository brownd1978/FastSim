/**
 * @file MemUtil/MemAllocator.cc
 *
 * Implementation of class MemAllocator.
 * 
 * @see MemUtil/MemAllocator.h
 * @author Andrei A. Salnikov
 * @date   May 24, 2001
 * Copyright (C) BINP, 2001 
 *
 * $Id: MemAllocator.cc 517 2010-01-15 08:24:18Z stroili $
 */

// ----- includes ------

// Experiment.h must be in any implementation file
#include "BaBar/BaBar.hh"

// standard C headers

// standard C++ headers

// this class header
#include "MemUtil/MemAllocator.hh"

// other packages headers

// ----- methods definition ------

MemAllocator MemAllocator::instance_ ;

// default constructor
MemAllocator::MemAllocator()
{
}

// destructor
MemAllocator::~MemAllocator()
{
}


// Local Variables: ***
// mode: c++ ***
// End: ***
