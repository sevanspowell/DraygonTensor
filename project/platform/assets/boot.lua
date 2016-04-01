function init()
  print("Script: init called!")
  print("Mysin: " .. mysin(3.14159265359))
end

function shutdown()
  print("Script: shutdown called!")
end

function update(deltaTime)
  print("Script: update called! Delta time: " .. deltaTime)
end

function render()
  print("Script: render called!")
end
