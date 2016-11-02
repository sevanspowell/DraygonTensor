-- require 'steps'

local script_manager_class = require "scripts/scriptManager"
local fsm_class = require "scripts/fsm"
local state1_class = require "scripts/state1"

local script_manager = nil
local fsm = nil

local affordance_table_class = require "scripts/affordanceTable"

function moveAgentToTarget(entity, targetPosition, radius, moveSpeed, deltaTime)
  local step = moveSpeed * deltaTime
  local currentPosition = Script.get_local_translation(entity)
  local hasReachedTarget = false

  local currentToTarget = targetPosition - currentPosition
  local directionToMove = Vector3.normalize(currentToTarget)
  local deltaPos = directionToMove * step

  Script.set_local_translation(entity, currentPosition + deltaPos)

  if Vector3.magnitude(currentToTarget) < radius then
    hasReachedTarget = true
  end

  return hasReachedTarget 
end

function preinit()
  print("Script: preinit called!")

  cam = Script.spawn_prefab("player", Vector3(0, 1, 2.5))
  -- local location = Vector3(0, 2, 1)
  -- pad = Script.spawn_prefab("waterfountain", location, Quaternion(0.0, 0.0, -0.0, 1.0), Vector3(1, 1, 1))
  -- steps()
  -- Physics.add_plane(Vector3(0, 1, 0), -5)
  floor = Script.spawn_prefab("boxRender", Vector3(0, -0.1/2, 0), Quaternion(0, 0, 0, 1), Vector3(100, 0.1, 100))

  box = Script.spawn_prefab("box", Vector3(0, 5, 0))

  Physics.add_plane(Vector3(0, 1, 0), 0)

end

function init()
  print("Script: init called!")

  script_manager = script_manager_class:new()
  fsm = fsm_class:new()
  state1 = state1_class:new()
  affordance_table = affordance_table_class:new()
end

function shutdown()
  print("Script: shutdown called!")
end

function moveToTarget(entity, currentPosition, target, deltaTime)
  -- Script.set_local_translation(entity, deltaTime*(target - currentPosition))
  Script.set_local_translation(entity, target)
end

keepMoving = true
pushed = false
function update(deltaTime)
  -- Handle messages
  while Script.is_next_message() do
    local msg = Script.get_next_message()
    if (msg.type == "create_component") then
      if (msg.component_type == 'scriptComponents') then
        for path, parameters in pairs(msg.scripts) do
          script_manager:add_script_component(msg.entity, path, parameters)
        end
      elseif msg.component_type == 'affordanceComponent' then
        for affordance, value in pairs(msg.affordances) do
          affordance_table:registerAffordance(msg.entity, affordance, value)
        end
      end
    end
  end

  -- Update script components
  script_manager:update(deltaTime)
  fsm:update(deltaTime)

  for ent, val in pairs(affordance_table:queryAffordance("sit")) do
    print(tostring(ent) .. " affords sitting this much: " .. val)
  end
  for ent, val in pairs(affordance_table:queryAffordance("rest")) do
    print(tostring(ent) .. " affords resting this much: " .. val)
  end


  if (Input.is_key_pressed("1")) then
    affordance_table:registerAffordance(floor, "sit", 0.2)
    print("ADDED")
  end
  if (Input.is_key_pressed("2")) then
    affordance_table:unregisterAffordance(floor, "sit")
    print("REMOVED")
  end
  if (Input.is_key_pressed("[") and pushed == false) then
    print("PUSHED")
    fsm:pushState(state1)
    pushed = true
  end
  if (Input.is_key_pressed("]") and pushed == true) then
    print("POPPED")
    fsm:popState()
    pushed = false
  end
  if (Input.is_key_pressed("p") and keepMoving) then
    keepMoving = not moveAgentToTarget(box, Vector3(5, 0, 0), 1.0, 3.0, deltaTime)
  end
  if (Input.is_key_pressed("i")) then
    Script.set_local_translation(box, Script.get_local_translation(box) + Vector3(0, 0, 10.0*deltaTime));
    -- moveToTarget(box, Script.get_local_translation(box), Vector3(10, 0, -5), 10.0*deltaTime)
  end
  if (Input.is_key_pressed("j")) then
    Script.set_local_translation(box, Script.get_local_translation(box) + Vector3(-10.0*deltaTime, 0, 0));
    -- moveToTarget(box, Script.get_local_translation(box), Vector3(0, 0, -5), 10.0*deltaTime)
  end
  if (Input.is_key_pressed("k")) then
    Script.set_local_translation(box, Script.get_local_translation(box) + Vector3(0, 0, -10.0*deltaTime));
  end
  if (Input.is_key_pressed("l")) then
    Script.set_local_translation(box, Script.get_local_translation(box) + Vector3(10.0*deltaTime, 0, 0));
  end

end

function render()
  --print("Script: render called!")
end
