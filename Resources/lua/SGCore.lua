---
--- Created by stuka.
--- DateTime: 12.02.2026 3:06
---

local SGCore = {}

--- @enum SGCore.LogLevel
SGCore.LogLevel = {
    LVL_INFO = 0,
    LVL_DEBUG = 1,
    LVL_WARN = 2,
    LVL_ERROR = 3,
    LVL_CRITICAL = 4
}

--- @class SGCore.Player
--- @field name string
SGCore.Player = {

}

--- @param name string
--- @return SGCore.Player
function SGCore.Player.new(name)
    error("Not implemented. Please, implement this in your native code.")
end

--- @param tag string Log tag.
--- @param level SGCore.LogLevel Log level.
--- @param message string Log message.
function SGCore.log(tag, level, message)
    error("Not implemented. Please, implement this in your native code.")
end

return SGCore