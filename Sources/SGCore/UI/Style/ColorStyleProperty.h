#pragma once

#include <variant>
#include "Style.h" // FIXME: remove unused imports

namespace SGCore::UI
{
	struct ColorStyleProperty final {
		Ref<DynValueNode> alpha;
		Ref<DynValueNode> red;
		Ref<DynValueNode> green;
		Ref<DynValueNode> blue;
	};
}