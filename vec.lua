
Vec={}       -- vector class
Vec.mt = {}  -- metatable

Vec.make=function(elts)
    self={}
    setmetatable(self, Vec.mt);
    for i=1,table.getn(elts) do self[i]=elts[i] end
    return self
end

-- shorthand vector constructor
vec = Vec.make

-- Vec.mt.__index = function(self,key)
--     local tab={x=self[1], y=self[2], z=self[3]}
--     return tab[key]
-- end

Vec.mt.__add = function(a,b)
    local ret = {}
    for i=1,table.getn(a) do ret[i]=a[i]+b[i] end
    return Vec.make(ret)
end

Vec.mt.__sub = function(a,b)
    local ret = {}
    for i=1,table.getn(a) do ret[i]=a[i]-b[i] end
    return Vec.make(ret)
end

Vec.mt.__unm = function(v)
    local ret = {}
    for i=1,table.getn(v) do ret[i]=-v[i] end
    return Vec.make(ret)
end

Vec.dot = function(a,b)
    local ret=0.0
    for i=1,table.getn(a) do ret=ret+a[i]*b[i] end
    return ret
end

Vec.scale = function(s,v)
    local ret = {}
    for i=1,table.getn(v) do ret[i]=s*v[i] end
    return Vec.make(ret)
end

Vec.cross = function(a,b)
    return Vec.make {
        a[2]*b[3]-a[3]*b[2],
        a[3]*b[1]-a[1]*b[3],
        a[1]*b[2]-a[2]*b[1]
    }
end

Vec.mt.__tostring = function(v)
    local ret = 'Vec: ' .. v[1] 
    for i=2,table.getn(v) do 
        ret = ret .. ', ' .. v[i]
    end
    return ret
end



