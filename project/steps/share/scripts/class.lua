function class(members)
    members = members or {}

    local mt = {
        __metatable = members;
        __index     = members;
    }

    local function new(_, init)
        return setmetatable(init or {}, mt)
    end

    members.new  = members.new  or new

    return mt
end
