
-- Libraries
local pairs = pairs
local string = string
local gl = gl
local glu = glu
local glut = glut
local io = io
local math = math
local bit32 = bit32
local os = os
local bm = bm

-- Functions
local arshift = bit32.arshift
local abs = math.abs

local band = bit32.band
local bor = bit32.bor

local cos = math.cos

local exp = math.exp

local floor = math.floor

local glBegin = gl.glBegin
local glCallList = gl.glCallList
local glClear = gl.glClear
local glClearColor = gl.glClearColor
local glColor = gl.glColor
local glDepthMask = gl.glDepthMask
local glDisable = gl.glDisable
local glEnable = gl.glEnable
local glEnd = gl.glEnd
local glEndList = gl.glEndList
local glFlush = gl.glFlush
local glGenLists = gl.glGenLists
local glGetBooleanv = gl.glGetBooleanv
local glGetDoublev = gl.glGetDoublev
local glLoadIdentity = gl.glLoadIdentity
local glMaterial = gl.glMaterial
local glMatrixMode = gl.glMatrixMode
local glNewList = gl.glNewList
local glOrtho = gl.glOrtho
local glPixelZoom = gl.glPixelZoom
local glPointSize = gl.glPointSize
local glPopMatrix = gl.glPopMatrix
local glPushMatrix = gl.glPushMatrix
local glRasterPos = gl.glRasterPos
local glReadPixels = gl.glReadPixels
local glRotated = gl.glRotated
local glScaled = gl.glScaled
local glTranslated = gl.glTranslated
local glVertex = gl.glVertex
local glViewport = gl.glViewport

local gluPerspective = glu.gluPerspective
local gluUnProject = glu.gluUnProject

local glutBitmapCharacter = glut.glutBitmapCharacter
local glutFullScreen = glut.glutFullScreen
local glutGet = glut.glutGet
local glutGetModifiers = glut.glutGetModifiers
local glutPostRedisplay = glut.glutPostRedisplay
local glutReshapeWindow = glut.glutReshapeWindow
local glutSwapBuffers = glut.glutSwapBuffers
local glutWireCube = glut.glutWireCube

local load = image.load

local max = math.max
local min = math.min

local open = io.open

local pi = math.pi

local sin = math.sin
local sqrt = math.sqrt

local unpack = table.unpack

-- Boolean values
local GL_FALSE			= 0x0000
local GL_TRUE			= 0x0001

-- Data types
local GL_FLOAT			= 0x1406

-- Primitives
local GL_POINTS			= 0x0000
local GL_LINES			= 0x0001
local GL_QUADS			= 0x0007

-- Matrix Mode
local GL_MODELVIEW		= 0x1700
local GL_PROJECTION		= 0x1701

-- Lines
local GL_LINE_SMOOTH		= 0x0B20

-- Polygons
local GL_POINT			= 0x1B00
local GL_LINE			= 0x1B01
local GL_FILL			= 0x1B02

-- Depth buffer
local GL_DEPTH_TEST		= 0x0B71
local GL_DEPTH_COMPONENT	= 0x1902

-- Lighting
local GL_LIGHTING		= 0x0B50
local GL_DIFFUSE		= 0x1201
local GL_FRONT_AND_BACK		= 0x0408

-- Feedback
local GL_2D			= 0x0600

-- Fog
local GL_LINEAR			= 0x2601

-- Buffers, Pixel Drawing/Reading
local GL_LEFT			= 0x0406
local GL_RIGHT			= 0x0407
local GL_RGBA			= 0x1908

-- Pixel Mode / Transfer
local GL_UNPACK_ALIGNMENT	= 0x0CF5

-- Texture mapping
local GL_TEXTURE_ENV		= 0x2300
local GL_TEXTURE_ENV_MODE	= 0x2200
local GL_TEXTURE_1D		= 0x0DE0
local GL_TEXTURE_WRAP_S		= 0x2802
local GL_TEXTURE_MAG_FILTER	= 0x2800
local GL_TEXTURE_MIN_FILTER	= 0x2801
local GL_TEXTURE_GEN_S		= 0x0C60
local GL_TEXTURE_GEN_MODE	= 0x2500
local GL_TEXTURE_COMPONENTS	= 0x1003
local GL_LINEAR_MIPMAP_LINEAR	= 0x2703
local GL_OBJECT_LINEAR		= 0x2401
local GL_OBJECT_PLANE		= 0x2501
local GL_MODULATE		= 0x2100
local GL_REPEAT			= 0x2901
local GL_S			= 0x2000

-- glPush/PopAttrib bits
local GL_DEPTH_BUFFER_BIT	= 0x00000100
local GL_COLOR_BUFFER_BIT	= 0x00004000

-- Display Lists
local GL_COMPILE		= 0x1300

local GLUT_DOWN			= 0
local GLUT_UP			= 1
local GLUT_LEFT			= 0
local GLUT_WINDOW_X		= 100
local GLUT_WINDOW_Y		= 101
local GLUT_WINDOW_WIDTH		= 102
local GLUT_WINDOW_HEIGHT	= 103
local GLUT_SCREEN_WIDTH		= 200
local GLUT_SCREEN_HEIGHT	= 201
local GLUT_WINDOW_RGBA		= 116
local GLUT_WINDOW_CURSOR	= 122
local GLUT_ACTIVE_SHIFT		= 1
local GLUT_ACTIVE_CTRL		= 2
local GLUT_ACTIVE_ALT		= 4

local browser_path, pager_path
	= "firefox", "firefox"
local mesh_obj = mesh.load("data/cortex.mesh")
local labels = {}
local fovy_deg, camera_distance, znear, zfar, x_angle_deg, y_angle_deg, label_point_size
	= 45.0, 2.0, 1.0, 1e5, 0.0, 0.0, 5.0
local camera_pivot, camera_offset
	= {0, 0, 0}, {0, 0, 0}
local mouse_xi, mouse_yi
	= 0, 0
local axes_color, stripe_color, mesh_color, horizontal_plane_color, coronal_plane_color, sagittal_plane_color, label_color
	= {0, 0, 0, 1}, {1, 0, 0, 1}, {0.9, 0.9, 0.9, 1}, {0.0, 0.0, 0.8, 0.3}, {0.7, 0.0, 0.0, 0.3}, {0.7, 0.4, 0.0, 0.3}, {1.0, 6.0, 0.0, 1.0}
local draw_mode = GL_FILL
local showing_planes = nil
local locked_position = nil
local scene_box = {mesh_obj:get_bounds()}
local kb_cmd_mode = false

local command = ""				-- command being entered by the user
local candy_striping, doing_transparency, drawing_thumbnails, ctrl_is_pressed, shift_is_pressed
	= false, false, true, false, false

local reverse = function(t)
	local len_t = #t
	for i = 1, arshift(len_t, 1) do
		t[i], t[len_t-(i-1)] = t[len_t-(i-1)], t[i]
	end
end

local load_thumbnails = function(orient)
	local bm_thumbnails, bm_names
		= {}, {}
	local bm_max_width, bm_max_height, i
		= 1, 1, 1
	local file = open("thumbnails/" .. orient .. "-labelled/names")
	for filename in file:lines() do
		bm_names[i] = filename:sub(1, -5)
		bm_thumbnails[i] = load("thumbnails/" .. orient .. "-labelled/" .. filename)
		local width, height = bm_thumbnails[i]:get_size()
		bm_max_width = max(width, bm_max_width)
		bm_max_height = max(height, bm_max_height)
		i = i+1
	end
	bm[orient .. "_thumbnails"] = bm_thumbnails
	bm[orient .. "_names"] = bm_names
	bm[orient .. "_max_width"] = bm_max_width
	bm[orient .. "_max_height"] = bm_max_height
end

-- Load coronal thumbnails
load_thumbnails("coronal")
reverse(bm.coronal_names)
reverse(bm.coronal_thumbnails)

-- Load horizontal thumbnails
load_thumbnails("horizontal")

-- Load sagittal thumbnails
load_thumbnails("sagittal")

-- wx, wy, wz are the returned world coordinates
-- Reference: redbook/unproject.c & one of the NeHe demos
-- The name is a bit misleading, since the result can be
-- different if the position is locked.
local get_mouse_location = function()
	if(locked_position) then
		return unpack(locked_position)
	end
	local real_x, real_y
		= mouse_xi, glutGet(GLUT_WINDOW_HEIGHT)-mouse_yi-1
	return gluUnProject(real_x, real_y, glReadPixels(real_x, real_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT))
end

local center = { (scene_box[1]+scene_box[4])/2, (scene_box[2]+scene_box[5])/2, (scene_box[3]+scene_box[6])/2 }
local edges = { scene_box[4]-scene_box[1], scene_box[5]-scene_box[2], scene_box[6]-scene_box[3] }

local get_indexes_from_mouse = function()
	local x, y, z = get_mouse_location()
	return floor((#bm.coronal_names)*(z-scene_box[3])/edges[3]),
	       floor((#bm.horizontal_names)*(y-scene_box[2])/edges[2]),
	       floor((#bm.sagittal_names)*(x-scene_box[1])/edges[1])
end

-------
-- Etc.
-------

local draw_bitmap_string = function(font, s, x, y, z)
	if(x) then
		if(z) then
			glRasterPos(x, y, z)
		else
			glDepthMask(false)
			glRasterPos(x, y)
			glDepthMask(true)
		end
	end
	for i = 1, s:len() do
		glutBitmapCharacter(font, s:byte(i))
	end
end

local show_pos = function(x, y, z)
	draw_bitmap_string("h12", ("(%3.1f, %3.1f, %3.1f) [mm]"):format(x, y, z), x, y, z)
end

local draw_box_list = glGenLists(1)
glNewList(draw_box_list, GL_COMPILE)
glPushMatrix()
glTranslated(center[1], center[2], center[3], 1)
glScaled(edges[1], edges[2], edges[3])
glutWireCube(1.0)
glPopMatrix()
show_pos(scene_box[1], scene_box[2], scene_box[3])
show_pos(scene_box[4], scene_box[5], scene_box[6])
glEndList()

local set_up_3D_viewport_and_matrices = function()
	local w, h = max(1, glutGet(GLUT_WINDOW_WIDTH)), max(1, glutGet(GLUT_WINDOW_HEIGHT))
	local width_of_3d_area = ({ [true] = w-bm.coronal_max_width, [false] = w })[drawing_thumbnails]
	glViewport(0, 0, width_of_3d_area-1, h-1)

	-- Do transformations to implement the camera
	glMatrixMode(GL_PROJECTION)
	glLoadIdentity()
	gluPerspective(fovy_deg, width_of_3d_area/h, znear, zfar)
	glMatrixMode(GL_MODELVIEW)
	glLoadIdentity()
	local x, y, z
		= unpack(camera_offset)
	z = z-camera_distance
	glTranslated(x, y, z)
	glRotated(x_angle_deg, -1.0, 0.0, 0.0)
	glRotated(y_angle_deg,  0.0, 1.0, 0.0)
	x, y, z = unpack(camera_pivot)
	glTranslated(-x, -y, -z)
end

on_display = function()
	if(not(doing_transparency)) then
		glEnable(GL_DEPTH_TEST)
	end
	glClearColor(1.0, 1.0, 1.0, 1.0)
	glClear(bor(GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT))

	set_up_3D_viewport_and_matrices()

	glMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE, unpack(mesh_color))
	glEnable(GL_LIGHTING)
	glPushMatrix()
	if(candy_striping) then
		glEnable(GL_TEXTURE_1D)
		glEnable(GL_TEXTURE_GEN_S)
	end
	mesh_obj:draw()
	if(candy_striping) then
		glDisable(GL_TEXTURE_1D)
		glDisable(GL_TEXTURE_GEN_S)
	end
	glPopMatrix()
	glDisable(GL_LIGHTING)

	glColor(unpack(axes_color))
	glCallList(draw_box_list)

	local x, y, z = get_mouse_location()

	if(locked_position) then

		glDepthMask(false)

		glBegin(GL_QUADS)

		-- coronal
		glColor(unpack(coronal_plane_color))
		glVertex(scene_box[1], scene_box[2], z)
		glVertex(scene_box[1], scene_box[5], z)
		glVertex(scene_box[4], scene_box[5], z)
		glVertex(scene_box[4], scene_box[2], z)

		-- horizontal
		glColor(unpack(horizontal_plane_color))
		glVertex(scene_box[1], y, scene_box[3])
		glVertex(scene_box[1], y, scene_box[6])
		glVertex(scene_box[4], y, scene_box[6])
		glVertex(scene_box[4], y, scene_box[3])

		-- sagittal
		glColor(unpack(sagittal_plane_color))
		glVertex(x, scene_box[2], scene_box[3])
		glVertex(x, scene_box[2], scene_box[6])
		glVertex(x, scene_box[5], scene_box[6])
		glVertex(x, scene_box[5], scene_box[3])

		glEnd()

		-- Draw lines where the planes of section intersect
		glBegin(GL_LINES)
		glColor(0.0, 0.0, 0.0, 1.0)	-- back to black
		glVertex(x, y, scene_box[3])
		glVertex(x, y, scene_box[6])
		glVertex(scene_box[1], y, z)
		glVertex(scene_box[4], y, z)
		glVertex(x, scene_box[2], z)
		glVertex(x, scene_box[5], z)
		glEnd()
		glDepthMask(true)
	end

	glDisable(GL_DEPTH_TEST)
	glPushMatrix()
	glLoadIdentity()
	glMatrixMode(GL_PROJECTION)
	glPushMatrix()
	glLoadIdentity()
	local w, h
		= max(1, glutGet(GLUT_WINDOW_WIDTH)), max(1, glutGet(GLUT_WINDOW_HEIGHT))
	glOrtho(1, w, 1, h, -1, 1)
	local str = nil
	-- Only draw the 3D mouse coords if the mouse is probably over the
	-- surface.  This hack on the next line should be replaced with something
	-- smarter.
	if(x^2+y^2+z^2<(0.75*zfar)^2) then
		str = ("Mouse location: (%4.3f, %4.3f, %4.3f)"):format(x, y, z)
		glRasterPos(10, h-20)
		draw_bitmap_string("h12", str)
	end

	-- More here: print out other info of interest: frame rate, etc.
	if(kb_cmd_mode) then
		glRasterPos(10, 10)
		draw_bitmap_string("h12", "lua> " .. command .. "|")
	end
	glPopMatrix()
	glMatrixMode(GL_MODELVIEW)
	glPopMatrix()

	if(glGetBooleanv(GL_DEPTH_TEST)==GL_TRUE) then
		glEnable(GL_DEPTH_TEST)
	end

	if(#labels>0) then
		glColor(unpack(label_color))

		for _, L in pairs(labels) do
			x, y, z, s = unpack(L)
			draw_bitmap_string("h12", s, x, y, z)
		end

		glPointSize(label_point_size)

		glBegin(GL_POINTS)
		for _, L in pairs(labels) do
			x, y, z, s = unpack(L)
			glVertex(x, y, z)
		end
		glEnd()

		glPointSize(1)
		glColor(0.0, 0.0, 0.0, 1.0)
	end

	if(drawing_thumbnails) then
		-- These have to be done while we still have the GL
		-- matrices and viewport set up from the 3D drawing.
		local icor, ihor, isag
			= get_indexes_from_mouse()

		local wt = bm.coronal_max_width
		glViewport(w-wt-10, 0, wt+10, h)
		glDisable(GL_DEPTH_TEST)

		glMatrixMode(GL_PROJECTION)
		glLoadIdentity()
		glOrtho(1, wt, 1, h, 0, 1)
		glMatrixMode(GL_MODELVIEW)
		glLoadIdentity()

		-- Figure out which section to draw, based on where the mouse is
		-- pointing in 3D.
		glPixelZoom(1.0, -1.0)	-- GL draws images upside down by default.

		-- Draw a white square behind all the images
		glColor(1, 1, 1)
		local x0, y0, x1, y1
			= 1, 1, bm.coronal_max_width, h
		glBegin(GL_QUADS)
		glVertex(x0, y0)
		glVertex(x1, y0)
		glVertex(x1, y1)
		glVertex(x0, y1)
		glEnd()
		glColor(0, 0, 0)

		local cor_img = bm.coronal_thumbnails[icor]
		h = h-1
		if(cor_img) then
			glRasterPos(1, h)
			cor_img:draw_pixels()
		end

		h = h-bm.coronal_max_height-1

		local sag_img = bm.sagittal_thumbnails[isag]
		if(sag_img) then
			glRasterPos(1, h)
			sag_img:draw_pixels()
		end
		h = h-bm.sagittal_max_height-1

		local hor_img = bm.horizontal_thumbnails[ihor]
		if(hor_img) then
			glRasterPos(1, h)
			hor_img:draw_pixels()
		end

		set_up_3D_viewport_and_matrices()	-- put it back so we can calc mouse coords
	end

	glFlush()
	glutSwapBuffers()
end

-- It's not really zooming but moving the camera toward or away from the
-- object being viewed.

local zoom_to_fit = function()
	camera_distance = sqrt(edges[1]^2+edges[2]^2+edges[3]^2)/sin(fovy_deg*pi/360.0)/2.0
	camera_pivot = center
end

bm.set_idle_callback(nil)
zoom_to_fit()

-- Run the given Lua file
local run = function(filename)
	local cmd = loadfile(filename)
	if(cmd) then
		cmd()
	else
		bm.warn("Could not run " .. filename)
	end
end

local restart = function()
	run("bminit.lua")
end

local run_user_selected_lua_script = function()
	run(bm.get_filename())
end

local browse_coronal = function()
	local icor, ihor, isag = get_indexes_from_mouse()
	local name = bm.coronal_names[icor]
	if(name) then
		bm.run_process_in_background(browser_path .. " " .. "\"http://brainmaps.org/index.php?dirname=HBP/m.mulatta/RH04/RH04a/&file=HBP/m.mulatta/RH04/RH04a/" .. name .. "/&win=max\"")
	end
end

local browse_horizontal = function()
	local icor, ihor, isag = get_indexes_from_mouse()
	local name = bm.horizontal_names[ihor]
	if(name) then
		bm.run_process_in_background(browser_path .. " " .. "\"http://brainmaps.org/index.php?dirname=HBP/m.mulatta/RH10/RH10a/&file=HBP/m.mulatta/RH10/RH10a/" .. name .. "/&win=max\"")
	end
end

local browse_sagittal = function()
	local icor, ihor, isag = get_indexes_from_mouse()
	local name = bm.sagittal_names[isag]
	if(name) then
		bm.run_process_in_background(browser_path .. " " .. "\"http://brainmaps.org/index.php?dirname=HBP/m.mulatta/RH12/RH12a/&file=HBP/m.mulatta/RH12/RH12a/" .. name .. "/&win=max\"")
	end
end

-- Launch web browsers on brainmaps.org for the three sections specified by
-- the mouse cursor's projection onto the brain surface.
local browse_all_three = function()
	browse_coronal()
	browse_horizontal()
	browse_sagittal()
end

local toggle_transparency = function()
	doing_transparency = not(doing_transparency)
	if(doing_transparency) then
		mesh_color[4] = 0.5
	else
		mesh_color[4] = 1.0
	end
end

-- This sets up a temporary idle callback that gradually changes the
-- camera focus to the point in 3D space pointed to by the mouse,
-- while reducing the distance between the camera and that point.
local zoom_in = function()
	local dist, src
		= camera_distance/4.0, camera_distance
	local x1, y1, z1 = get_mouse_location()

	if(max(abs(x1), abs(y1), abs(z1))<1e4) then
		local x0, y0, z0 = unpack(camera_pivot)
		local cam_off0 = camera_offset
		local cnt = 0
		bm.set_idle_callback(function()
		        cnt = cnt+1
			if(cnt>60) then
				bm.set_idle_callback(nil)
			end

			local t = cnt/60.0
			-- This polynomial u(t) satisfies u(0)=0, u(1)=1,
			-- u'(0)=0, u'(1)=0, so we get a smoother transition than
			-- we would get by using a linear ramp.
			local u = (3-2*t)*t^2
			local v = 1.0-u
			for i = 1, 3 do
				camera_offset[i] = v*cam_off0[i]
			end
			camera_pivot = {v*x0+u*x1, v*y0+u*y1, v*z0+u*z1}
			camera_distance = v*src+u*dist
		end)
	else
		print("Mouse location is suspiciously large.  Is it off the surface?")
	end
end

local toggle_thumbnails = function()
	drawing_thumbnails = not(drawing_thumbnails)
end

local toggle_position_lock = function()
	if(locked_position) then
		locked_position = nil
	else
		locked_position = {get_mouse_location()}
	end
end

-- Parts of this function were borrowed from the OpenGL texgen demo.
local make_stripe_fun = function(x, y, z, a)
	return function()
		local stripe_texture = {}
		for i = 0, 3 do
			for j = 1, 4 do
				stripe_texture[arshift(i, -2)+j] = stripe_color[j]
			end
		end
		for i = 4, 127 do
			for j = 1, 3 do
				stripe_texture[arshift(i, -2)+j] = mesh_color[j]
			end
			stripe_texture[arshift(i, -2)+4] = 1.0		-- opaque
		end

		gl.glBindTexture(GL_TEXTURE_1D, gl.glGenTextures(1))
		gl.glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT)
		gl.glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
		gl.glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR)

		gl.glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
		gl.gluBuild1DMipmaps(GL_TEXTURE_1D, GL_RGBA, 128, GL_RGBA, GL_FLOAT, stripe_texture)
		gl.glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE)
		gl.glTexGen(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR)
		gl.glTexGen(GL_S, GL_OBJECT_PLANE, x, y, z, a)
		candy_striping = true
	end
end

local cycle_through_draw_styles = (function()
	local draw_mode_switch = {
		[GL_FILL] = GL_LINE,
		[GL_LINE] = GL_POINT,
		[GL_POINT] = GL_FILL
	}
	return function()
		draw_mode = draw_mode_switch[draw_mode]
		if(draw_mode==GL_LINE) then
			glDisable(GL_LINE_SMOOTH)
		else
			glEnable(GL_LINE_SMOOTH)
		end
		gl.glPolygonMode(GL_FRONT_AND_BACK, draw_mode)
	end
end)()

local help = function()
	bm.run_process_in_background(pager_path .. " help.txt")
end

local toggle_full_screen =  (function()
	local fullscreen = false
	local last_width, last_height
	return function()
		fullscreen = not(fullscreen)
		if(fullscreen) then
			last_width, last_height = max(1, glutGet(GLUT_WINDOW_WIDTH)), max(1, glutGet(GLUT_WINDOW_HEIGHT))
			glutFullScreen()
		else
			glutReshapeWindow(last_width, last_height)
		end
	end
end)()

local quit = function()
	os.exit(true)
end

local no_stripes = function()
	candy_striping = false
end

-- Global function add_label() for executing labels.txt
add_label = function(x, y, z, text)
	print(("adding label: %3.3f %3.3f %3.3f %s"):format(x, y, z, text))
	table.insert(labels, {x, y, z, text})
end

local begin_command_mode = function()
	kb_cmd_mode = true
end

local load_labels = function()
	print("load_labels")
	local file, err = open("labels.txt")
	if(err) then
		print("== Result of wget request ==")
		os.execute("wget http://brainmaps.org/labels.txt")
	else
		file:close()
	end
	dofile("labels.txt")
end

local calc_camera_coords = function()
	local theta, phi
		= -y_angle_deg/180.0*pi, -x_angle_deg/180.0*pi
	local cx, cy, cz = unpack(camera_pivot)
	return cx+camera_distance*cos(phi)*sin(theta), cy+camera_distance*sin(phi), cz+camera_distance*cos(phi)*cos(theta)
end

local add_mouse_label = function()
	local x, y, z = get_mouse_location()
	add_label(x, y, z, "*")
end

---------------
-- Menu
---------------

bm.reset_menu()
bm.add_menu_item("Zoom in (i)", zoom_in)
bm.add_menu_item("Zoom to fit (f)", zoom_to_fit)
bm.add_menu_item("Toggle full screen (F)", toggle_full_screen)
bm.add_menu_item("Toggle transparency (T)", toggle_transparency)
bm.add_menu_item("Toggle thumbnails (t)", toggle_thumbnails)
bm.add_menu_item("Toggle position lock (l)", toggle_position_lock)
bm.add_menu_item("Browse coronal (c)", browse_coronal)
bm.add_menu_item("Browse horizontal (h)", browse_horizontal)
bm.add_menu_item("Browse sagittal (s)", browse_sagittal)
bm.add_menu_item("Browse all three (b)", browse_all_three)
bm.add_menu_item("X stripes (x)", make_stripe_fun(2, 0, 0, 0))
bm.add_menu_item("Y stripes (y)", make_stripe_fun(0, 2, 0, 0))
bm.add_menu_item("Z stripes (z)", make_stripe_fun(0, 0, 2, 0))
bm.add_menu_item("Cycle through draw styles (m)", cycle_through_draw_styles)
bm.add_menu_item("No stripes (n)", no_stripes)
bm.add_menu_item("Load labels (L)", load_labels)
bm.add_menu_item("Add label (p)", add_mouse_label)
bm.add_menu_item("Lua prompt (:)", begin_command_mode)
bm.add_menu_item("Run Lua script", run_user_selected_lua_script)
bm.add_menu_item("Help (?)", help)
bm.add_menu_item("Restart (r)", restart)
bm.add_menu_item("Quit (q)", quit)

on_mouse = function(button, state, xi, yi)
	shift_is_pressed = band(glutGetModifiers(), GLUT_ACTIVE_SHIFT)~=0
	ctrl_is_pressed = band(glutGetModifiers(), GLUT_ACTIVE_CTRL)~=0
	if(state==GLUT_DOWN) then
		if(button==GLUT_LEFT) then
			mouse_xi, mouse_yi = xi, yi
		end
	end
end

on_motion = function(xi, yi)
	if(shift_is_pressed) then
		-- move camera closer in or further out
		camera_distance = camera_distance/exp(0.01*(yi-mouse_yi))
	elseif(ctrl_is_pressed) then
		local h = max(1, glutGet(GLUT_WINDOW_HEIGHT))
		-- z=0 at near clipping plane (http://www.opengl.org/resources/faq/technical/glu.htm)
		local z = camera_distance/(zfar-znear)
		glPushMatrix()
		glLoadIdentity()
		local pprev = {gluUnProject(mouse_xi, h-mouse_yi-1, z)}
		local p	= {gluUnProject(xi, h-yi-1, z)}
		glPopMatrix()
		for i = 1, 3 do
			camera_offset[i] = camera_offset[i]+edges[i]*(p[i]-pprev[i])
		end
	else
		y_angle_deg = y_angle_deg+0.5*(xi-mouse_xi)
		x_angle_deg = x_angle_deg-0.5*(yi-mouse_yi)
	end
	mouse_xi = xi
	mouse_yi = yi

	glutPostRedisplay()
end

on_passive_motion = function(xi, yi)
	mouse_xi, mouse_yi = xi, yi;
	glutPostRedisplay()
end

---------------
-- Key bindings
---------------
local key_bindings = {
	['C'] = function()
			print(("camera: %.3f %.3f %.3f"):format(calc_camera_coords()))
		end,
	['L'] = load_labels,
	['m'] = cycle_through_draw_styles,
	['P'] =	function()
			print(("%4.3f %4.3f %4.3f"):format(get_mouse_location()))
		end,
	['p'] = add_mouse_label,
	['?'] = help,
	['r'] = restart,
	['h'] = browse_horizontal,
	['s'] = browse_sagittal,
	['c'] = browse_coronal,
	['b'] = browse_all_three,
	['f'] = zoom_to_fit,
	['i'] = zoom_in,
	['t'] = toggle_thumbnails,
	['F'] = toggle_full_screen,
	['T'] = toggle_transparency,
	['x'] = make_stripe_fun(2, 0, 0, 0),
	['y'] = make_stripe_fun(0, 2, 0, 0),
	['z'] = make_stripe_fun(0, 0, 2, 0),
	['n'] = no_stripes,
	['l'] = toggle_position_lock,
	['q'] = quit,
-- Bring up a lua command prompt, for power users
	[':'] = begin_command_mode
}

on_keyboard = function(key, xi, yi)
	if(kb_cmd_mode) then
		-- If it's Enter, then try to execute the command.
		if(key==13) then
			local loaded_cmd = loadstring(command)
			if(loaded_cmd) then
				local status, result = pcall(loaded_cmd)
				if(status) then
					if(result) then
						print(result)
					end
				else
					bm.warn("Lua error. ", result)
				end
			else
				bm.warn("Lua syntax error.")
			end
			km_cmd_mode = 0
			command = ""

		-- If it's backspace or delete, then try to delete the last
		-- character in the command.
		elseif(key==8 or key==127) then
			command = command:sub(1, -2)

		-- If it's Esc or ctrl-c, then abort the command
		elseif(key==27 or key==3) then
			kb_cmd_mode = false
			command = ""
		else
			-- Otherwise, add the character to the command.
			command = command .. string.char(key)
		end
	else
		local f = key_bindings[string.char(key)]
		if(f) then
			f(xi, yi)
		end
	end
	glutPostRedisplay()
end
