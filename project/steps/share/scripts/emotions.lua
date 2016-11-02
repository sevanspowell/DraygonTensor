-- Uses https://en.wikipedia.org/wiki/Lövheim_cube_of_emotion as model.
require "scripts/class"

local emotions = {}
local emotions_meta = class(emotions)

function clamp(val, low, high)
  if val < low then
    return low
  elseif val > high then
    return high
  end
end

function emotions:new()
  local self = {}
  self = setmetatable(self, emotions_meta)
  self:init()
  return self
end 

function emotions:init()
  -- serotonin, dopamine, noradrenaline
  self.chemicals = Vector3(1, 1, 1)
  self.mood = 1
  self:updateMood()
end

-- function emotions:update(deltaTime)
-- end

-- Get current mood (float value between 0 and 1)
function emotions:getMood()
  return self.mood
end

-- Dot the current chemical levels with an arbitrary vector to get the
-- proportion the chemical levels are in the direction of the comparitorVec as a
-- number between 0 and 1
function emotions:dotMood(comparitorVec)
  return Vector3.dot(comparitorVec, self.chemicals) / Vector3.dot(comparitorVec, comparitorVec)
end

-- Recalculate mood based on current chemical values
function emotions:updateMood()
  self.mood = Vector3.dot(Vector3(1, 1, 1), self.chemicals) / Vector3.dot(Vector3(1, 1, 1), Vector3(1, 1, 1))
end

-- Move the chemical levels towards some emotion point in the Loveheim cube.
function emotions:affectEmotion(emotionPoint)
  local emotionVector = emotionPoint - self.chemicals
  -- Proportion of emotion vector to apply
  local propApply = 1.0 - (1.0/self.mood)
  -- Clamp proportion between 0.1 and 0.9 so emotion always has some effect
  propApply = clamp(propApply, 0.1, 0.9)
  local scaledEmotionVector = emotionVector * propApply

  self.chemicals = self.chemicals + scaledEmotionVector

  -- Recalc mood
  self:updateMood()
end

return emotions
