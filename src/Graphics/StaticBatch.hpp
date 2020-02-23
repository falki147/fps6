#pragma once

#include <Graphics/BatchBase.hpp>

template<typename T> class StaticBatch: public BatchBase<T> {
public:
	StaticBatch(): BatchBase<T>(GL_STATIC_DRAW) { }
};
