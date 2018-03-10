import bpy
import sys

argv = sys.argv
argv = argv[argv.index("--") + 1:]

path_out = argv[0]

bpy.ops.export_scene.teh(filepath=path_out)

