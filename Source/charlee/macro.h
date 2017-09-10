#pragma once

#define DEBUG(type, str) UE_LOG(LogClass, type, TEXT(#str))

#define DEBUG_d(type, var) UE_LOG(LogClass, type, TEXT("%d"), var)