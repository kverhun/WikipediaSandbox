#pragma once

#if GRAPHCLUSTERIZATION_LIBRARY
#define GRAPHCLUSTERIZATION_API __declspec(dllexport)
#else
#define GRAPHCLUSTERIZATION_API __declspec(dllimport)
#endif
