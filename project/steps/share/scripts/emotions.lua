-- Uses https://en.wikipedia.org/wiki/Lövheim_cube_of_emotion as model.
require "scripts/class"

local emotions = {}
local emotions_meta = class(emotions)

emotions.emotionVectors = {}
emotions.emotionVectors["Ashamed"] = Vector3(0, 0, 0)
emotions.emotionVectors["Distressed"] = Vector3(0, 0, 1)
emotions.emotionVectors["Scared"] = Vector3(0, 1, 0)
emotions.emotionVectors["Angry"] = Vector3(0, 1, 1)
emotions.emotionVectors["Disgusted"] = Vector3(1, 0, 0)
emotions.emotionVectors["Suprised"] = Vector3(1, 0, 1)
emotions.emotionVectors["Joyful"] = Vector3(1, 1, 0)
emotions.emotionVectors["Excited"] = Vector3(1, 1, 1)

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

  self.emotionLookupTable = {}
  self.emotionLookupTable[0] = {}
  self.emotionLookupTable[0][0] = {}
  self.emotionLookupTable[0][0][0] = "Ashamed" 
  self.emotionLookupTable[0][0][1] = "Distressed"
  self.emotionLookupTable[0][1] = {}
  self.emotionLookupTable[0][1][0] = "Scared" 
  self.emotionLookupTable[0][1][1] = "Angry"
  self.emotionLookupTable[1] = {}
  self.emotionLookupTable[1][0] = {}
  self.emotionLookupTable[1][0][0] = "Disgusted" 
  self.emotionLookupTable[1][0][1] = "Suprised"
  self.emotionLookupTable[1][1] = {}
  self.emotionLookupTable[1][1][0] = "Joyful" 
  self.emotionLookupTable[1][1][1] = "Excited"

end

-- function emotions:update(deltaTime)
-- end

-- Get current mood (float value between 0 and 1)
function emotions:getMood()
  return self.mood
end

-- -- Dot the current chemical levels with an arbitrary vector to get the
-- -- proportion the chemical levels are in the direction of the comparitorVec as a
-- -- number between 0 and 1
-- function emotions:dotMood(comparitorVec)
--   return Vector3.dot(comparitorVec, self.chemicals) / Vector3.dot(comparitorVec, comparitorVec)
-- end

-- Returns how close I am to the given emotion point, 0 is close, 1 is far away
-- NOTE: Assumes the emotion point is on a corner of the cube
function emotions:howClose(emotionPoint)
  local toChemical = self.chemicals - emotionPoint
  -- create a vector from the emotion point to the point furthest from the
  -- emotion point on the cube
  local furthestAway = Vector3(1, 1, 1) - emotionPoint
  -- Create vector from emotionPoint to furthest
  local toFurthest = furthestAway - emotionPoint

  return 1.0 - ((Vector3.dot(toChemical, toFurthest)) / (Vector3.dot(toFurthest, toFurthest)))
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

function emotions:getCurrentEmotion()
  local serotonin = 1
  local dopamine = 1
  local noradrenaline = 1
  if self.chemicals:get_x() < 0.5 then
    serotonin = 0
  end
  if self.chemicals:get_y() < 0.5 then
    dopamine = 0
  end
  if self.chemicals:get_z() < 0.5 then
    noradrenaline = 0
  end

  return self.emotionLookupTable[serotonin][dopamine][noradrenaline]
end

return emotions
