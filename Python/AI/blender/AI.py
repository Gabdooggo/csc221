import bpy
import sys, time, os
#bpy.ops.wm.read_factory_settings(use_empty=True)
Struct = False
Mark = 0
Marked = False
AIMark = 0
crabs = bpy.data.objects.get("AIAddedCube")
if crabs:
    Mark += 1
Cube = bpy.data.objects.get("Cube")  # the default cube’s name
if Cube:
    bpy.data.objects.remove(Cube, do_unlink=True)
bpy.ops.mesh.primitive_cube_add(location=(0,0,1))
if bpy.data.objects.get("Cube"):
    Marked = True
def Start(Struct, Marked, Mark):
    if Marked:
        cube = bpy.context.active_object
        if Struct is False:
            Mark += 1
            cube.name = "AIAddedCube" + str(Mark)
            Name = "AIAddedCube"
        Marked = False
    return
bpy.ops.wm.save_as_mainfile(filepath="/Users/gabrielcallauprellwitz/Desktop/AI_out.blend")
print("AI.py ran")
if "--stay" in sys.argv:
    x = input("Headless: staying alive. Type e to exit.")
    
    try:
        while x != "e":
            x = input()
            time.sleep(0.5)  # low CPU
            Start(Struct, Marked, Mark)
    except KeyboardInterrupt:
        pass    
if x == "e":
    bpy.ops.wm.save_as_mainfile(filepath="/Users/gabrielcallauprellwitz/Desktop/AI_out.blend")
