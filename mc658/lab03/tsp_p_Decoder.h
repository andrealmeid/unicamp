/*
 * SampleDecoder.h
 *
 * Any decoder must have the format below, i.e., implement the method decode(std::vector< double >&)
 * returning a double corresponding to the fitness of that vector. If parallel decoding is to be
 * used in the BRKGA framework, then the decode() method _must_ be thread-safe; the best way to
 * guarantee this is by adding 'const' to the end of decode() so that the property will be checked
 * at compile time.
 *
 * The chromosome inside the BRKGA framework can be changed if desired. To do so, just use the
 * first signature of decode() which allows for modification. Please use double values in the
 * interval [0,1) when updating, thus obeying the BRKGA guidelines.
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#ifndef TSP_P_DECODER_H
#define TSP_P_DECODER_H

#include <list>
#include <vector>
#include <algorithm>
#include "tsp_p.h"


class tsp_p_Decoder {
public:
	tsp_p_Decoder();
	~tsp_p_Decoder();

	double decode(const std::vector< double >& chromosome) const;

private:
};

#endif
