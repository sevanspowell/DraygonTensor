require "scripts/class"

local script_manager = {}
local script_manager_meta = class(script_manager)

function script_manager:new()
  local self = {}
  self = setmetatable(self, script_manager_meta)
  self:init()
  return self
end 

function script_manager:init()
  self.script_components = {}
end

function script_manager:add_script_component(entity, scriptPath, parameters)
  local class = require (scriptPath)
  local instance = class:new(entity)

  -- Give instance script parameters
  for name, value in pairs(parameters) do
    instance[name] = value
  end

  -- Call instance awake method, if possible
  if instance.awake ~= nil then
    instance:awake()
  end
  
  -- Add instance to manager
  self.script_components[(#self.script_components) + 1] = instance
end

function script_manager:update(deltaTime)
  for i, instance in pairs(self.script_components) do
    -- print(i)
    instance:update(deltaTime)
  end
end

return script_manager
