#pragma once

#include <Graphics/BatchBase.hpp>

template<typename T> class StreamedBatch: public BatchBase<T> {
public:
	StreamedBatch(): BatchBase<T>(GL_STREAM_DRAW) { }
};
