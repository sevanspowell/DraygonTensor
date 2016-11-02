require "scripts/class"

local state = {}
local state_meta = class(state)

function state:new()
  local self = {}
  self = setmetatable(self, state_meta)
  self:init()
  return self
end 

function state:init()
  self.test = {}
end

function state:update(deltaTime)
  for i, instance in pairs(self.ai_components) do
    -- print(i)
    instance:update(deltaTime)
  end
end

return state
