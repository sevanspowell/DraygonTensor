local chair = {}
local chair_meta = class(chair)

function chair:new(entity)
  print("New called")
  local self = {}
  self = setmetatable(self, chair_meta)
  self:init(entity)
  return self
end

function chair:init(entity)
  print("Init called")
  self.entity = entity 
end

function chair:awake()
end

function chair:update(deltaTime)
  -- print(self.entity)
  -- print(deltaTime)
end

return chair
