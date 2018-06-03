#ifndef DECODER_H
#define DECODER_H

#include <list>
#include <vector>
#include <algorithm>

class SampleDecoder {
public:
	SampleDecoder();
	~SampleDecoder();

	double decode(const std::vector< double >& chromosome) const;

private:
};

#endif
