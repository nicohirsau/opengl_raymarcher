#include <math.h>

#include "Mantaray/Core/Vector.h"

namespace MR {
    float clamp(float value, float min, float max) {
        if (value < min)
            return min;
        if (value > max)
            return max;
        return value;
    }
}