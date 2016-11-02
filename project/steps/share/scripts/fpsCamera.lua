require "scripts/class"

local fps_camera = {}
local fps_camera_meta = class(fps_camera)


function fps_camera:new(entity)
  local self = {}
  self = setmetatable(self, fps_camera_meta)
  self:init(entity)
  return self
end 

function fps_camera:init(entity)
  self.entity = entity
end

function fps_camera:awake()
end

function fps_camera:update(deltaTime)
  local mouseDeltaX, mouseDeltaY = Input.get_mouse_delta_xy()
  local camOrientation = Quaternion.normalize(Script.get_local_orientation(self.entity))
  local yawDelta = Quaternion.create_from_axis_angle(Vector3.unit_y(), -mouseDeltaX * deltaTime * self.sensitivity)
  local pitchDelta = Quaternion.create_from_axis_angle(Vector3.unit_x(), -mouseDeltaY * deltaTime * self.sensitivity)
  camOrientation = (yawDelta * camOrientation * pitchDelta)
  Script.set_local_orientation(self.entity, Quaternion.normalize(camOrientation))

  local temp = Vector4.normalize(Vector4.transform(-Vector4.unit_z(), Matrix4.create_from_quaternion(camOrientation)))
  local camFwd = Vector3(temp:get_x(), temp:get_y(), temp:get_z())
  local temp = Vector4.normalize(Vector4.transform(Vector4.unit_y(), Matrix4.create_from_quaternion(camOrientation)))
  local camUp = Vector3(temp:get_x(), temp:get_y(), temp:get_z())
  local temp = Vector4.normalize(Vector4.transform(Vector4.unit_x(), Matrix4.create_from_quaternion(camOrientation)))
  local camRight = Vector3(temp:get_x(), temp:get_y(), temp:get_z())

  local wishDir = Vector3(0, 0, 0)
  if (Input.is_key_pressed("w")) then
    wishDir = wishDir + camFwd
  end
  if (Input.is_key_pressed("s")) then
    wishDir = wishDir - camFwd
  end
  if (Input.is_key_pressed("a")) then
    wishDir = wishDir - camRight
  end
  if (Input.is_key_pressed("d")) then
    wishDir = wishDir + camRight
  end
  if (Input.is_key_pressed("Spacebar")) then
    wishDir = wishDir + Vector3(0, 1, 0)
  end
  if (Input.is_key_pressed("LeftShift")) then
    wishDir = wishDir + Vector3(0, -1, 0)
  end

  if (Vector3.magnitude(wishDir) > 0) then
    wishDir = Vector3.normalize(wishDir)
    wishDir = wishDir * deltaTime * 10
  end

  Script.set_local_translation(self.entity, Script.get_local_translation(self.entity) + wishDir);
end

return fps_camera
