---
--- Created by stuka.
--- DateTime: 12.02.2026 3:06
---

SGCore = {}

SGCore.LogLevel = {
    LVL_INFO = 0,
    LVL_DEBUG = 1,
    LVL_WARN = 2,
    LVL_ERROR = 3,
    LVL_CRITICAL = 4
}

function SGCore.log(tag, level, message)
    SGCore_log_impl(tag, level, message)
end

return SGCore