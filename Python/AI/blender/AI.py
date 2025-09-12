import bpy
import sys, time, os
#bpy.ops.wm.read_factory_settings(use_empty=True)
Mark = 0
Marked = False
AIMark = 0
crabs = bpy.data.objects.get("AIAddedCube") 
Cube = bpy.data.objects.get("Cube")  # the default cube’s name
if Cube:
    bpy.data.objects.remove(Cube, do_unlink=True)
bpy.ops.mesh.primitive_cube_add(location=(0,0,1))
if bpy.data.objects.get("Cube"):
    Marked = True
if Marked:
    Mark += 1
    cube = bpy.context.active_object
    cube.name = "AIAddedCube" + str(Mark)
    Name = "AIAddedCube"
    Marked = False
bpy.ops.wm.save_as_mainfile(filepath="/Users/gabrielcallauprellwitz/Desktop/AI_out.blend")
print("AI.py ran")
if "--stay" in sys.argv:
    x = input("Headless: staying alive. Type e to exit.")
    
    try:
        while x != "e":
            x = input()
            time.sleep(0.5)  # low CPU
    except KeyboardInterrupt:
        pass    
if x == "e":
    bpy.ops.wm.save_as_mainfile(filepath="/Users/gabrielcallauprellwitz/Desktop/AI_out.blend")
