require "scripts/class"

local state1 = {}
local state1_meta = class(state1)

function state1:new()
  local self = {}
  self = setmetatable(self, state1_meta)
  self:init()
  return self
end 

function state1:init()
end

function state1:update(deltaTime)
  -- for i, instance in pairs(self.stateStack) do
  --   instance:update(deltaTime)
  -- end
  print('state1 ' .. deltaTime)
end

return state1
