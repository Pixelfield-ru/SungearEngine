#pragma once

#include <variant>
#include "Style.h" // FIXME: remove unused imports

namespace SGCore::UI
{
	struct ColorStyleProperty final {
		Ref<StyleMathNode> alpha;
		Ref<StyleMathNode> red;
		Ref<StyleMathNode> green;
		Ref<StyleMathNode> blue;
	};
}