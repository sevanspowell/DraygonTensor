require "scripts/class"

local affordanceTable = {}
local affordanceTable_meta = class(affordanceTable)

function affordanceTable:new()
  local self = {}
  self = setmetatable(self, affordanceTable_meta)
  self:init()
  return self
end 

function affordanceTable:init()
  self.affordances = {}
end

-- Register an affordance with an entity
function affordanceTable:registerAffordance(entity, affordance, value)
  print("ADD")
  -- If no table for that affordance yet, add one
  if self.affordances[affordance] == nil then
    self.affordances[affordance] = {}
  end

  -- Set affordance value for the entity
  self.affordances[affordance][entity:get_id()] = value
end

-- Remove an affordance from an entity
function affordanceTable:unregisterAffordance(entity, affordance)
  if self.affordances[affordance] ~= nil then
    if self.affordances[affordance][entity:get_id()] ~= nil then
      self.affordances[affordance][entity:get_id()] = nil
    end
  end
end

-- Returns table full of key-value pairs mapping entity to the amount it affords
-- the queried affordance
function affordanceTable:queryAffordance(affordance)
  if self.affordances[affordance] == nil then
    return {}
  else
    return self.affordances[affordance]
  end
end

return affordanceTable
