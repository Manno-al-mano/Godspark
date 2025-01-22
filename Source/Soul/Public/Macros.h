#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_SPHERE_SMALL(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 10, 12, FColor::Purple, false,  5.f);
#define DRAW_TEXT(text, color) if(GEngine) GEngine->AddOnScreenDebugMessage(1,5.f, color, text)