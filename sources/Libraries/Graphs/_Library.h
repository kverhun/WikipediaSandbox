#pragma once

#if GRAPHS_LIBRARY
#define GRAPHS_API __declspec(dllexport)
#else
#define GRAPHS_API __declspec(dllimport)
#endif
