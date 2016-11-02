require "scripts/class"

local ai_manager = {}
local ai_manager_meta = class(ai_manager)

function ai_manager:new()
  local self = {}
  self = setmetatable(self, ai_manager_meta)
  self:init()
  return self
end 

function ai_manager:init()
  self.ai_components = {}
end

function ai_manager:update(deltaTime)
  for i, instance in pairs(self.ai_components) do
    -- print(i)
    instance:update(deltaTime)
  end
end

return ai_manager
