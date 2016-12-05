#pragma once

#if GRAPHSIO_LIBRARY
#define GRAPHSIO_API __declspec(dllexport)
#else
#define GRAPHSIO_API __declspec(dllimport)
#endif
