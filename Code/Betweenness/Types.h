#pragma once
/**
 * Types.h
 * Holds definition of floating number
 */

#ifdef NUM_SINGLE
	typedef float btw_num_t;
	#define MPI_TYPE MPI_FLOAT
#endif

#ifdef NUM_DOUBLE
	typedef double btw_num_t;
	#define MPI_TYPE MPI_DOUBLE
#endif

