dofile("gl.lua")
dofile("glu.lua")
dofile("glut.lua")
dofile("vec.lua")

if(bm.os()=="windows") then
    bm.pager_path='C:\\windows\\system32\\notepad.exe'
    bm.browser_path='\"C:\\Program Files\\Internet Explorer\\iexplore.exe\"'
elseif(bm.os()=='mac') then
    bm.pager_path='/usr/bin/open'
    bm.browser_path='/usr/bin/open'
else 
    bm.browser_path="firefox"
    bm.pager_path=bm.browser_path
end
bm.shadowing=true
bm.candy_striping=false
bm.the_meshes = {}
bm.fovy_deg=45.0;
bm.log_camera_distance=math.log(2.)
bm.znear=1.0;
bm.zfar=1e5;
bm.y_angle_deg=-90.0;
bm.x_angle_deg=0.0;
bm.camera_pivot = Vec.make {0,0,0}
bm.camera_offset = Vec.make {0,0,0}
bm.mouse_xi = 0
bm.mouse_yi = 0
bm.updating_mouse_location = true
bm.axes_scale=10.
bm.axes_visible=true
bm.axes_color = {0,0,0, 1}
bm.stripe_color = { 1, 0, 0, 1 }
bm.mesh_color = { 0.9, 0.9, 0.9, 1 }
bm.horizontal_plane_color = { 0.0, 0.0, 0.8, 0.3 }
bm.coronal_plane_color = { 0.7, 0.0, 0.0, 0.3 }
bm.sagittal_plane_color = { 0.7, 0.4, 0.0, 0.3 }
bm.draw_mode=GL_FILL;
bm.doing_transparency = false
bm.showing_planes = nil
bm.label_point_size=5.0
bm.label_color = { 1.,6.,0., 1. }

bm.heads_up_string = ''
bm.command='' 				-- command being entered by the user
bm.drawing_thumbnails=true
bm.x_is_pressed=false  
bm.shift_is_pressed=false

reverse = function(tbl)
	local ret={}
	for i=1,table.getn(tbl) do
		ret[i]=tbl[table.getn(tbl)-(i-1)]
	end
	return ret
end

load_thumbnails = function(orient)
	bm[orient .. '_thumbnails'] = {}
	bm[orient .. '_names'] = {}
	bm[orient .. '_max_width'] = 1
	bm[orient .. '_max_height'] = 1
	local file=io.open('thumbnails/' .. orient .. '-labelled/names')
	local i=1
	for filename in file:lines() do 
		bm[orient .. '_names'][i] = string.sub(filename,1,-5)
		local img = 
			image.load('thumbnails/' .. orient .. '-labelled/' .. filename)
		bm[orient .. '_thumbnails'][i] = img
		local width,height = img:get_size()
		bm[orient .. '_max_width'] = 
			math.max(width, bm[orient .. '_max_width'])
		bm[orient .. '_max_height'] = 
			math.max(height, bm[orient .. '_max_height'])
		i=i+1
	end
end

-- Load coronal thumbnails
load_thumbnails('coronal')
bm.coronal_names=reverse(bm.coronal_names)
bm.coronal_thumbnails=reverse(bm.coronal_thumbnails)

-- Load horizontal thumbnails
load_thumbnails('horizontal')

-- Load sagittal thumbnails
load_thumbnails('sagittal')

bm.get_coronal_index_from_mouse = function()
    local x,y,z=bm.get_mouse_location()
	local zlo=17.0
	local zhi=70.0
    return math.floor(table.getn(bm.coronal_names)*(z-zlo)/(zhi-zlo))
end

bm.get_horizontal_index_from_mouse = function()
    local x,y,z=bm.get_mouse_location()
	local ylo=37.0
	local yhi=80.0
    return math.floor(table.getn(bm.horizontal_names)*(y-ylo)/(yhi-ylo))
end

bm.get_sagittal_index_from_mouse = function()
    local x,y,z=bm.get_mouse_location()
	local xhi=24.0
	local xlo=76.0
    return math.floor(table.getn(bm.sagittal_names)*(x-xlo)/(xhi-xlo))
end

bm.browse_coronal=function()
    local index = bm.get_coronal_index_from_mouse()
    local name = bm.coronal_names[index]
    if(name) then
        local url = 
            '"http://brainmaps.org/index.php?dirname=HBP/m.mulatta/RH04/RH04a/&file=HBP/m.mulatta/RH04/RH04a/' .. name .. '/&win=max"';
        bm.run_process_in_background(bm.browser_path .. " " .. url)
    end
end

bm.browse_horizontal=function()
    local index = bm.get_horizontal_index_from_mouse()
    local name = bm.horizontal_names[index]
    if(name) then
        local url = 
            '"http://brainmaps.org/index.php?dirname=HBP/m.mulatta/RH10/RH10a/&file=HBP/m.mulatta/RH10/RH10a/' .. name .. '/&win=max"';
        bm.run_process_in_background(bm.browser_path .. " " .. url)
    end
end

bm.browse_sagittal=function()
    local index = bm.get_sagittal_index_from_mouse()
    local name = bm.sagittal_names[index]
    if(name) then 
        local url = 
            '"http://brainmaps.org/index.php?dirname=HBP/m.mulatta/RH12/RH12a/&file=HBP/m.mulatta/RH12/RH12a/' .. name .. '/&win=max"';
        bm.run_process_in_background(bm.browser_path .. " " .. url)
    end
end

-- Launch web browsers on brainmaps.org for the three sections specified by 
-- the mouse cursor's projection onto the brain surface.
bm.browse_all_three = function()
    bm.browse_coronal()
    bm.browse_horizontal()
    bm.browse_sagittal()
end

bm.help = function()
    bm.run_process_in_background(bm.pager_path .. " help.txt")
end

map=function(f,t) 
    r={}  
    for k,v in t do r[k]=f(v) end 
    return r 
end

-- Set up the near and far cutting planes to only view a slice of the scene.
bm.slice=function(dist,thickness)
    bm.znear=dist-thickness/2.
    bm.zfar=dist+thickness/2.
end

-- Set the near and far cutting planes back to reasonable default values.
bm.unslice=function()
    bm.znear=1.
    bm.zfar=1e6
end

bm.list_functions=function() 
    for key,value in bm do 
        -- Hmm... Why are these things being included in the bm table? -ijt
        if(not(key=="base" or key=="table" or key=="io" or key=="string" or
key=="math" or key=="debug" or key=="loadlib" or key=="bm")) then
            print(key) 
        end
    end 
end

bm.spin=function()
    local t=0
    bm.set_idle_callback(function() t=t+1; bm.set_y_angle_deg(t) end)
end


-- Parts of this function were borrowed from the OpenGL texgen demo. 
bm.set_up_candy_stripes = function(coeffs)
    local stripe_texture={}
	local stripe_size=4
	
	for i=0,stripe_size-1 do
		for j=1,4 do stripe_texture[4*i+j]=bm.stripe_color[j] end
	end
    for i=stripe_size,(128-1) do
        for j=1,3 do stripe_texture[4*i+j]=bm.mesh_color[j] end
		stripe_texture[4*i+4]=1.0  -- opaque
    end

    local texlen = table.getn(stripe_texture)/4;
    local texname=gl.glGenTextures(1)
	print(string.format("texlen: %i\n",texlen))
    gl.glBindTexture(GL_TEXTURE_1D,texname)
    gl.glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT)
    gl.glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    gl.glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, 
        GL_LINEAR_MIPMAP_LINEAR)

    gl.glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    gl.gluBuild1DMipmaps(GL_TEXTURE_1D,GL_RGBA, texlen,GL_RGBA,GL_FLOAT,
        stripe_texture)
    gl.glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    gl.glTexGen(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    gl.glTexGen(GL_S, GL_OBJECT_PLANE, unpack(coeffs));
end

bm.enable_candy_stripes=function() bm.candy_striping=true end

bm.disable_candy_stripes=function() bm.candy_striping=false end


-- Use the box's center for the camera's center of attention, and its size to
-- determine the scale of the axes. 
bm.view_box = function(box)
    local k
    local cx,cy,cz,dx,dy,dz,r,d,theta
    local xlo,ylo,zlo
    local xhi,yhi,zhi

    xlo=box[1]; ylo=box[2]; zlo=box[3];
    xhi=box[4]; yhi=box[5]; zhi=box[6];
    cx=0.5*(xlo+xhi)
    cy=0.5*(ylo+yhi)
    cz=0.5*(zlo+zhi)
    dx=xhi-cx
    dy=yhi-cy
    dz=zhi-cz
    r=math.sqrt(dx*dx+dy*dy+dz*dz)
    theta=bm.fovy_deg/2.0*math.pi/180.0
    d=r/math.sin(theta)
    -- print(string.format("camera distance: %g",d))
    bm.log_camera_distance=math.log(d)
    bm.camera_pivot = Vec.make {cx,cy,cz}
    
    local scale=0.0
    local f=function(x) scale=math.max(scale,math.abs(x)) end
    f(xlo); f(ylo); f(zlo);
    f(xhi); f(yhi); f(zhi);
    if(scale>0.5) then scale=math.ceil(scale) end
    bm.axes_scale=scale
end

-- It's not really zooming but moving the camera toward or away from the
-- object being viewed.
bm.zoom_to_fit = function()
    local boxes = {}
    local i=1
    for k,mesh in bm.the_meshes do 
        boxes[i]= { mesh:get_bounds() }
        i=i+1
    end
    bm.scene_box = bm.calc_max_bounds(boxes)
    bm.view_box(bm.scene_box)
end

-- This sets up a temporary idle callback that gradually changes the 
-- camera focus to the point in 3D space pointed to by the mouse,
-- while reducing the distance between the camera and that point.
bm.zoom_in=function()
    local x1,y1,z1=bm.get_mouse_location()
    local camx,camy,camz=bm.calc_camera_coords()
    local cx,cy,cz,dx,dy,dz,dist
    cx,cy,cz = unpack(bm.camera_pivot)
    dx = cx-camx
    dy = cy-camy
    dz = cz-camz
    dist = math.sqrt(dx*dx+dy*dy+dz*dz)
    -- It's OK to replace 4. with anything above 1.
    local ld1 = math.log(dist/4.)  -- target log-distance 
    local ld0 = bm.log_camera_distance  -- initial log-distnace
	local d1 = math.exp(ld1)
	local d0 = math.exp(ld0)
    ld1 = math.max(ld1,math.log(2.*bm.znear))

    if(math.max(math.abs(x1),math.abs(y1),math.abs(z1)) < 1e4) then
        local x0,y0,z0
        local cam_off0 = bm.camera_offset
        x0,y0,z0=unpack(bm.camera_pivot)
        local t=0.0
        local f=function()
            local x,y,z,d
            t=t+1./(1.5*60.); -- bad: we should use elapsed time here.
            if(t>=1.0) then bm.set_idle_callback(nil) end
            if(true) then
                -- This polynomial u(t) satisfies u(0)=0, u(1)=1,
                -- u'(0)=0, u'(1)=0, so we get a smoother transition than 
                -- we would get by using a linear ramp.
                u=-2*t*t*t+3*t*t
            else 
                -- Less pleasant linear version 
                u=t
            end
            v=1.-u
            x=v*x0+u*x1
            y=v*y0+u*y1
            z=v*z0+u*z1
            d=v*d0+u*d1
            bm.camera_offset=Vec.scale(v,cam_off0)
            bm.camera_pivot=Vec.make {x,y,z}
            bm.log_camera_distance=math.log(d)
        end
        bm.set_idle_callback(f)
    else
        print("Mouse location is suspiciously large.  Is it off the surface?")
    end
end

bm.load_labels=function()
    -- server_name and label_string should be defined in bmconf.lua
    print("load_labels")
    local label_string=bm.http_get(bm.server_name,bm.label_filename)
    print("== Result of http request ==")
    print(label_string)
    local cmd=loadstring(label_string)
    if(cmd) then 
        cmd() 
    else 
        print(
[[The labels file is invalid.  It should consist of a sequence of lua statements 
like add_label(x,y,z, "label text").]])
    end
end

bm.toggle_shadow=function() 
    -- bm.set_shadow_visible(not(bm.get_shadow_visible())) 
    bm.shadowing=not(bm.shadowing)
end

bm.labels={}
bm.add_label=function(x,y,z, text)
    print(string.format("adding label: %3.3f %3.3f %3.3f %s",x,y,z,text))
    bm.labels[table.getn(bm.labels)+1]={x,y,z,text}
end

bm.on_help=function()
    print ""
    print ""
    print "== Brainmaps Help ==" 
    print "Hold the shift key while dragging up and down to move the camera in and out."
    print "Hold the `x' key while dragging to pan."
    print ""
    print ""
end

bm.begin_command_mode=function()
	bm.keyboard_mode = 'command'
end


-------
-- Etc.
-------

bm.cycle_through_draw_styles = function()
    bm.draw_mode=(bm.draw_mode==GL_FILL and GL_LINE 
        or (bm.draw_mode==GL_LINE and GL_POINT or GL_FILL));
    local decide = bm.draw_mode==GL_LINE and gl.glDisable or gl.glEnable
    decide(GL_LINE_SMOOTH)
    gl.glPolygonMode(GL_FRONT_AND_BACK,bm.draw_mode);
end

bm.draw_bitmap_string=function(font,s,x,y,z)
    if(x) then gl.glRasterPos(x,y,z) end
    for i=1,string.len(s) do 
        glut.glutBitmapCharacter(font, string.byte(s,i)); 
    end
end

in_2d_do = function(do_this)
    local w,h
    w=math.max(1,glut.glutGet(GLUT_WINDOW_WIDTH))
    h=math.max(1,glut.glutGet(GLUT_WINDOW_HEIGHT))
    local have_depth=gl.glGetDoublev(GL_DEPTH_TEST)
    gl.glDisable(GL_DEPTH_TEST)
    gl.glMatrixMode(GL_MODELVIEW); gl.glPushMatrix(); gl.glLoadIdentity()
    gl.glMatrixMode(GL_PROJECTION); gl.glPushMatrix(); gl.glLoadIdentity()
    gl.glOrtho(1,w, 1,h, -1,1);
    do_this()
    gl.glMatrixMode(GL_PROJECTION); gl.glPopMatrix()
    gl.glMatrixMode(GL_MODELVIEW); gl.glPopMatrix()
    if(have_depth==1) then gl.glEnable(GL_DEPTH_TEST) end
end

bm.draw_heads_up_display=function()
    local x,y,z,mag;

    x,y,z=bm.get_mouse_location()
    mag=math.sqrt(x*x+y*y+z*z);
    in_2d_do(function ()
        local h=math.max(1,glut.glutGet(GLUT_WINDOW_HEIGHT))
        local str 
        -- Only draw the 3D mouse coords if the mouse is probably over the
        -- surface.  This hack on the next line should be replaced with something
        -- smarter. 
        if(mag<0.75*bm.zfar) then
            str = string.format("% 4.3f % 4.3f % 4.3f",x,y,z);
        else 
            str = string.format("background")
        end

        if(bm.prevkey) then str = str .. ', key = ' .. bm.prevkey end

        gl.glRasterPos(10,h-20)
        if(str) then bm.draw_bitmap_string(GLUT_BITMAP_8_BY_13,str) end

        gl.glRasterPos(10,h-40)
        if(bm.heads_up_string) then 
            bm.draw_bitmap_string(GLUT_BITMAP_8_BY_13, bm.heads_up_string)
        end

        -- More here: print out other info of interest: frame rate, etc. 
        gl.glRasterPos(10,10)
        if(bm.keyboard_mode=='command') then
                bm.draw_bitmap_string(GLUT_BITMAP_8_BY_13, 'lua> ' .. bm.command .. '|')
        end
    end)
end

bm.set_up_3D_viewport_and_matrices=function()
    local w,h=glut.glutGetWindowSize()
    local wt = bm.coronal_max_width
    local width_of_3d_area = bm.drawing_thumbnails and w-wt or w
    gl.glViewport(0,0,width_of_3d_area-1,h-1)

    -- Do transformations to implement the camera 
    gl.glMatrixMode(GL_PROJECTION);
    gl.glLoadIdentity();
    glu.gluPerspective(bm.fovy_deg, width_of_3d_area/h, bm.znear, bm.zfar);
    gl.glMatrixMode(GL_MODELVIEW);
    gl.glLoadIdentity();
    gl.glTranslated(unpack(bm.camera_offset))
    gl.glTranslated(0., 0., -math.exp(bm.log_camera_distance)); 
    gl.glRotated(bm.x_angle_deg, -1.,0.,0.);
    gl.glRotated(bm.y_angle_deg,  0.,1.,0.);
    gl.glTranslated(unpack(-bm.camera_pivot))
end

bm.draw_3D_view=function()

    if(not(bm.doing_transparency)) then gl.glEnable(GL_DEPTH_TEST) end
    gl.glClearColor(1.,1.,1.,1.);
    gl.glClear(bitlib.bor(GL_COLOR_BUFFER_BIT,GL_DEPTH_BUFFER_BIT));

    bm.set_up_3D_viewport_and_matrices()

    -- for m in the_meshes do m:draw() end  <- This fails.  Why?
    -- table.foreach(the_meshes,function(m) m:draw() end) <- fails too

    gl.glMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE, unpack(bm.mesh_color))
    gl.glEnable(GL_LIGHTING)
    gl.glPushMatrix()
    if(bm.candy_striping) then 
        gl.glEnable(GL_TEXTURE_1D) 
        gl.glEnable(GL_TEXTURE_GEN_S)
    end
    for k,mesh in bm.the_meshes do mesh:draw() end
    if(bm.candy_striping) then 
        gl.glDisable(GL_TEXTURE_1D) 
        gl.glDisable(GL_TEXTURE_GEN_S)
    end
    gl.glPopMatrix()
    gl.glDisable(GL_LIGHTING)

    if (false) then
        if(bm.shadowing) then 
            gl.glPushMatrix()
            gl.glColor(0.5,0.5,0.5,1.0)
            gl.glScaled(1,0,1)
            for k,mesh in bm.the_meshes do mesh:draw() end
            gl.glPopMatrix()
        end
    end

    if(bm.axes_visible) then 
        local draw_string=function(x,y,z,s) bm.draw_bitmap_string("8x13",s,x,y,z) end
        gl.glColor(unpack(bm.axes_color))
        local b=bm.scene_box
        gl.glPushMatrix()
        gl.glTranslate(0.5*(b[1]+b[4]), 0.5*(b[2]+b[5]), 0.5*(b[3]+b[6]))
        gl.glScale(b[4]-b[1], b[5]-b[2], b[6]-b[3])
        glut.glutWireCube(1.0)
        gl.glPopMatrix()
        local str
        str=string.format("(%3.1f, %3.1f, %3.1f) [mm]",b[1],b[2],b[3]); 
        draw_string(b[1],b[2],b[3],str);
        str=string.format("(%3.1f, %3.1f, %3.1f) [mm]",b[4],b[5],b[6]); 
        draw_string(b[4],b[5],b[6],str);
    end

    -- Label and draw the axes. */
--     local as=bm.axes_scale;
--     if(bm.axes_visible) then 
--         gl.glColor(0.,0.,0.,1.);
--         gl.glPushMatrix();
--         gl.glScaled(as,as,as);
--         local str
--         str=string.format("X=%g [mm]",as); draw_string(1.,0.,0.,str);
--         str=string.format("Y=%g [mm]",as); draw_string(0.,1.,0.,str);
--         str=string.format("Z=%g [mm]",as); draw_string(0.,0.,1.,str);
--         -- str=string.format("X=-%g [mm]",as); draw_string(-1.,0.,0.,str);
--         -- str=string.format("Y=-%g [mm]",as); draw_string(0.,-1.,0.,str);
--         -- str=string.format("Z=-%g [mm]",as); draw_string(0.,0.,-1.,str);
--         gl.glBegin(GL_LINES);
--         gl.glVertex( 1., 0., 0.); gl.glVertex( 0., 0., 0.);
--         gl.glVertex( 0., 1., 0.); gl.glVertex( 0., 0., 0.);
--         gl.glVertex( 0., 0., 1.); gl.glVertex( 0., 0., 0.);
--         gl.glEnd();
--         gl.glPopMatrix();
--     end

    -- Draw the planes of section, 
    -- fixme: The numbers in here should be computed from the mesh
    -- rather than ad-hoc.
    if (bm.locked_position) then
        local x,y,z = unpack(bm.locked_position)

        -- Make it so we read from the z-buffer but do not write to it.
        gl.glDepthMask(GL_FALSE)

        -- coronal
        gl.glColor(unpack(bm.coronal_plane_color))
        gl.glTranslated(0,0,z) 
        gl.glRect(0,0, 100,100) 
        gl.glTranslated(0,0,-z) 

        -- horizontal
        gl.glColor(unpack(bm.horizontal_plane_color))
        gl.glPushMatrix()
        gl.glTranslated(50,y,40)
        gl.glRotated(-90, 1,0,0)
        gl.glRect(-50,-40, 50,40)
        gl.glPopMatrix()
        
        -- sagittal
        gl.glColor(unpack(bm.sagittal_plane_color))
        gl.glPushMatrix()
        gl.glTranslated(x,50,40)
        gl.glRotated(-90, 0,1,0)
        gl.glRect(-40,-50, 40,50)
        gl.glPopMatrix()

        gl.glDepthMask(GL_TRUE)
        gl.glColor(0,0,0)  -- back to black

        -- Draw lines where the planes of section intersect
        gl.glBeginEnd(GL_LINES, 
            function()
                gl.glVertex(x,y,0)         
                gl.glVertex(x,y,80)         

                gl.glVertex(0,y,z)         
                gl.glVertex(100,y,z)         

                gl.glVertex(x,0,z)         
                gl.glVertex(x,100,z)         
            end)
    end

    bm.draw_heads_up_display();

    gl.glColor(unpack(bm.label_color)) 
    for i,L in bm.labels do
        x,y,z,s=unpack(L)
        bm.draw_bitmap_string(GLUT_BITMAP_HELVETICA_12,s,x,y,z)
        gl.glPointSize(bm.label_point_size)
        gl.glBegin(GL_POINTS)
        gl.glVertex(x,y,z)
        gl.glEnd()
        gl.glPointSize(1)
    end
    gl.glColor(0.,0.,0.,1.)
end

bm.draw_thumbnails = function()
    -- These have to be done while we still have the GL 
    -- matrices and viewport set up from the 3D drawing.
    local icor = bm.get_coronal_index_from_mouse()
    local ihor = bm.get_horizontal_index_from_mouse()
    local isag = bm.get_sagittal_index_from_mouse()

    local w,h=glut.glutGetWindowSize()
    local wt = bm.coronal_max_width
    gl.glViewport(w-wt-10,0,wt+10,h)
    gl.glDisable(GL_DEPTH_TEST)

    gl.glMatrixMode(GL_PROJECTION)
    gl.glLoadIdentity()
    gl.glOrtho(1,wt, 1,h, 0,1)
    gl.glMatrixMode(GL_MODELVIEW)
    gl.glLoadIdentity()

    -- Figure out which section to draw, based on where the mouse is
    -- pointing in 3D.
    gl.glPixelZoom(1.,-1.)  -- GL draws images upside down by default.  

    -- Draw a white square behind all the images
    gl.glColor(1,1,1)
    gl.glBegin(GL_QUADS)
        local x0,x1, y0,y1
        x0 = 1
        x1 = bm.coronal_max_width
        y0 = 1
        y1 = h
        gl.glVertex(x0,y0)
        gl.glVertex(x1,y0)
        gl.glVertex(x1,y1)
        gl.glVertex(x0,y1)
    gl.glEnd()
    gl.glColor(0,0,0)

    local cor_img=bm.coronal_thumbnails[icor]
    local y = h-1
    local x = 1
    if(cor_img) then
        gl.glRasterPos(x,y)
        cor_img:draw_pixels() 
    end
    y=y-bm.coronal_max_height

    local sag_img=bm.sagittal_thumbnails[isag]
    if(sag_img) then
        gl.glRasterPos(x,y)
        sag_img:draw_pixels() 
    end
    y=y-bm.sagittal_max_height

    local hor_img=bm.horizontal_thumbnails[ihor]
    if(hor_img) then
        gl.glRasterPos(x,y)
        hor_img:draw_pixels() 
    end

    gl.glPixelZoom(1.,1.)  
end

on_display=function()
    bm.draw_3D_view()
    if(bm.drawing_thumbnails) then 
        bm.draw_thumbnails() 
        bm.set_up_3D_viewport_and_matrices()  -- put it back so we can calc mouse coords
    end
    glut.glutSwapBuffers();
end

on_idle=nil
 
bm.calc_max_bounds = function(boxlist)
    if(table.getn(boxlist)==0) then
        error("calc_max_bounds needs at least one box in the list")
    end
    local bigbox = boxlist[1]
    for i=2,table.getn(boxlist) do
        local box=boxlist[i]
        for j=1,3 do bigbox[j] = math.min(box[j],bigbox[j]) end
        for j=4,6 do bigbox[j] = math.max(box[j],bigbox[j]) end
    end
    return bigbox
end

bm.calc_camera_coords=function()
    local phi,theta,r,x,y,z,cx,cy,cz
    theta = (180-bm.y_angle_deg/180.)*math.pi
    phi = - bm.x_angle_deg/180.*math.pi
    cx,cy,cz = unpack(bm.camera_pivot)
    r = math.exp(bm.log_camera_distance)
    x = cx+r*math.sin(theta)*math.cos(phi)    
    y = cy+r*math.sin(phi)
    z = cz+r*math.cos(theta)*math.cos(phi)
    return x,y,z
end

mag=function(v) 
    local x,y,z=unpack(v)
    return math.sqrt(x*x+y*y+z*z)
end

normalized=function(v)
    len = mag(v)
    r={}
    if(len~=0.0) then
        for i=1,3 do r[i]=v[i]/len end
        return r
    else
        return v
    end
end

-- The x key is held down while clicking and dragging to pan around.
-- I would have used ctrl, but clicking with ctrl held down brings
-- up the right-mouse-button menu on my Mac laptop.
-- I would have used alt, but it has no effect on this same laptop.
-- In most other respects, it is a nice laptop, and is quite popular,
-- especially among neuroscientists.
on_mouse = function(button,state,xi,yi)
    bm.shift_is_pressed=
        bitlib.band(glut.glutGetModifiers(),GLUT_ACTIVE_SHIFT)~=0
    if(button==GLUT_LEFT) then
        if(state==GLUT_DOWN) then
            bm.mouse_xi=xi;
            bm.mouse_yi=yi;
        else 

        end
    end
end

on_motion=function(xi, yi)
    if(bm.shift_is_pressed) then
        -- move camera closer in or further out 
        bm.log_camera_distance=
            bm.log_camera_distance-0.01*(yi-bm.mouse_yi);
    elseif(bm.x_is_pressed) then
        local w,h
        h = math.max(1,glut.glutGet(GLUT_WINDOW_HEIGHT))
        -- z=0 at near clipping plane (http://www.opengl.org/resources/faq/technical/glu.htm)
        local z = math.exp(bm.log_camera_distance)/(bm.zfar-bm.znear)
        gl.glPushMatrix()
        gl.glLoadIdentity()
        local pprev = Vec.make { glu.gluUnProject(bm.mouse_xi, h-bm.mouse_yi-1, z) }
        local p     = Vec.make { glu.gluUnProject(xi, h-yi-1, z) }
        gl.glPopMatrix()
        local delta = p - pprev
        local fudge=70  -- FIXME: get this from a principle instead of hacking
        bm.camera_offset = bm.camera_offset + Vec.scale(fudge,(p-pprev))
    else 
        bm.y_angle_deg=bm.y_angle_deg+0.5*(xi-bm.mouse_xi);
        bm.x_angle_deg=bm.x_angle_deg-0.5*(yi-bm.mouse_yi);
        bm.x_angle_deg=math.max(-90.,math.min(90.,bm.x_angle_deg));
    end
    bm.mouse_xi=xi;
    bm.mouse_yi=yi;

    glut.glutPostRedisplay();
end

on_passive_motion=function(xi, yi)
    bm.mouse_xi=xi;
    bm.mouse_yi=yi;

    -- bm.get_mouse_location(&mouse_wx, &mouse_wy, &mouse_wz);
    if(bm.updating_mouse_location) then glut.glutPostRedisplay() end
end

-- wx, wy, wz are the returned world coordinates 
-- Reference: redbook/unproject.c & one of the NeHe demos
-- The name is a bit misleading, since the result can be
-- different if the position is locked.
bm.get_mouse_location=function()
    if(bm.locked_position) then
        return unpack(bm.locked_position)
    else
        local real_y,z
        real_y = glut.glutGet(GLUT_WINDOW_HEIGHT) - bm.mouse_yi - 1;
        z = gl.glReadPixels(bm.mouse_xi,real_y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT)
        local wx,wy,wz = glu.gluUnProject(bm.mouse_xi, real_y, z)
        return wx,wy,wz  
    end
end

on_keyboard=function(key,xi,yi)
        -- bm.prevkey=string.format('%i',key)
	if(bm.keyboard_mode=='command') then
		-- If it's Enter, then try to execute the command.
		if(key==13) then
			local loaded_cmd=loadstring(bm.command)
			if(loaded_cmd) then 
				local status,result=pcall(loaded_cmd)
				if(status) then
					if(result~=nil) then print(result) end
				else
					bm.warn("Lua error. ",result)
				end
			else
				bm.warn("Lua syntax error.")
			end
			bm.keyboard_mode='default'
			bm.command=''
			
                -- If it's backspace or delete, then try to delete the last
                -- character in the command.
		elseif(key==8 or key==127) then
			bm.command = string.sub(bm.command,1,-2)
		
		-- If it's Esc or ctrl-c, then abort the command
		elseif(key==27 or key==3) then
			bm.keyboard_mode='default'
			bm.command=''
		else

			-- Otherwise, add the character to the command.
			bm.command = bm.command .. string.char(key)		
		end
	else
		if(string.char(key)=='x') then 
			bm.x_is_pressed=true
		else
			f=bm.key_bindings[string.char(key)]
			if(f) then 
				f(xi,yi) 
				glut.glutPostRedisplay()
			end
		end
	end
	glut.glutPostRedisplay()
end

on_keyboard_up=function(key,xi,yi)
    if(string.char(key)=='x') then bm.x_is_pressed=false end
end

-- Run the given Lua file
run = function(filename)
	local cmd=loadfile(filename)
	if(cmd) then 
		cmd() 
	else 
		bm.warn('Could not run ' .. filename)
	end
end

restart=function() run('bminit.lua') end

run_user_selected_lua_script = function()
    on_filename = run;
    bm.get_filename()
end

toggle_transparency = function()
	bm.doing_transparency = not(bm.doing_transparency)
	if(bm.doing_transparency) then
		bm.mesh_color[4] = 0.5		
		gl.glDisable(GL_DEPTH_TEST)
	else
		bm.mesh_color[4] = 1.0
		gl.glEnable(GL_DEPTH_TEST)
	end
end

toggle_thumbnails = function()
	bm.drawing_thumbnails = not(bm.drawing_thumbnails)
end

bm.toggle_position_lock = function()
    if(bm.locked_position) then
        bm.locked_position = nil
    else 
        local x,y,z = bm.get_mouse_location()
        bm.locked_position = {x,y,z}
    end
end


---------------
-- Key bindings 
---------------

bm.key_bindings={}

bm.bind_key = function(key, f)
    bm.key_bindings[key] = f
end

bm.bind_key("c", 
    function() 
        print(string.format("camera: %.3f %.3f %.3f",bm.calc_camera_coords()))
    end)

-- bm.bind_key("l", bm.load_labels)

bm.bind_key("m", bm.cycle_through_draw_styles)

bm.bind_key("P", 
    function() 
        print(string.format("% 4.3f % 4.3f % 4.3f",bm.get_mouse_location())) 
    end)
bm.bind_key("p", 
    function() 
        x,y,z=bm.get_mouse_location()
        bm.add_label(x,y,z, "*")
    end)

bm.bind_key("?", bm.help)
bm.bind_key("r", restart)

bm.bind_key('h', bm.browse_horizontal)
bm.bind_key('s', bm.browse_sagittal)
bm.bind_key('c', bm.browse_coronal)
bm.bind_key("b", bm.browse_all_three)

bm.bind_key("Z", bm.zoom_to_fit)
bm.bind_key("z", bm.zoom_in)
bm.bind_key("t", toggle_thumbnails)

bm.bind_key("l", bm.toggle_position_lock)

-- Bring up a lua command prompt, for power users
bm.bind_key(":", bm.begin_command_mode)

---------------
-- Menu
---------------

make_stripe_fun = function(axis)
    return function() 
        bm.set_up_candy_stripes(axis)
        bm.enable_candy_stripes() 
    end
end

do_x_stripes = make_stripe_fun({2,0,0,0})
do_y_stripes = make_stripe_fun({0,2,0,0})
do_z_stripes = make_stripe_fun({0,0,2,0})

bm.reset_menu()
bm.add_menu_item("Zoom in (z)", bm.zoom_in)
bm.add_menu_item("Zoom to fit (Z)", bm.zoom_to_fit)
-- bm.add_menu_item("Toggle shadow", bm.toggle_shadow)
bm.add_menu_item("Toggle transparency", toggle_transparency)
bm.add_menu_item("Toggle thumbnails", toggle_thumbnails)
bm.add_menu_item("Toggle position lock (l)", bm.toggle_position_lock)
bm.add_menu_item("Browse coronal (c)", bm.browse_coronal)
bm.add_menu_item("Browse horizontal (h)", bm.browse_horizontal)
bm.add_menu_item("Browse sagittal (s)", bm.browse_sagittal)
bm.add_menu_item("Browse all three (b)", bm.browse_all_three)
bm.add_menu_item("X stripes", do_x_stripes)
bm.add_menu_item("Y stripes", do_y_stripes)
bm.add_menu_item("Z stripes", do_z_stripes)
bm.add_menu_item("Cycle through draw styles (m)", bm.cycle_through_draw_styles)
bm.add_menu_item("No stripes", bm.disable_candy_stripes)
bm.add_menu_item("Lua prompt (:)", bm.begin_command_mode)
bm.add_menu_item("Run Lua script", run_user_selected_lua_script);
bm.add_menu_item("Help (?)", bm.help)
bm.add_menu_item("Restart (r)", restart)
-- bm.add_menu_item("Help", bm.on_help) <- rewrite the help...
bm.add_menu_item("", function() end);
bm.add_menu_item("Quit", function() os.exit(0) end)


-- Load the mesh and view it.  
-- 
bm.server_name="brainmaps.org"
bm.label_filename="labels.txt"
bm.the_meshes = { mesh.load("data/cortex.mesh") }
bm.set_idle_callback(nil)
bm.zoom_to_fit()
glut.glutReshapeWindow(1000,650)



