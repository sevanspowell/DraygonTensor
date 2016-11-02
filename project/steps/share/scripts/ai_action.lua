require "scripts/class"

local action = {}
local action_meta = class(action)

function action:new()
  local self = {}
  self = setmetatable(self, action_meta)
  self:init()
  return self
end 

function action:init()
end

function action:update(deltaTime)
end

-- Returns true if action performed successfully
function action:perform(entity)
end

-- Returns true if the action requires the agent to be in range
function action:requiresInRange()
  return false
end

return action
