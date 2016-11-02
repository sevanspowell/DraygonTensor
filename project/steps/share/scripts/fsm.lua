-- https://github.com/sploreg/goap
require "scripts/class"

local fsm = {}
local fsm_meta = class(fsm)

function fsm:new()
  local self = {}
  self = setmetatable(self, fsm_meta)
  self:init()
  return self
end 

function fsm:init()
  self.stateStack = {}
end

function fsm:update(deltaTime)
  for i, instance in pairs(self.stateStack) do
    instance:update(deltaTime)
  end
end

function fsm:pushState(state)
  self.stateStack[(#self.stateStack) + 1] = state
end

function fsm:popState()
  table.remove(self.stateStack)
end

return fsm
