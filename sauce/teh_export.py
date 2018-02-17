bl_info = {
    "name": "Teh Engine Model Export",
    "category": "Export",
}

import bpy
import bpy_extras
import bmesh

class TehModelExporter:

    def __init__(self, C):
        self.C = C
        self.objekt = C.active_object

    def mesh(self, f):
        self.C.scene.frame_set(f)
        return self.objekt.to_mesh(self.C.scene, True, 'RENDER')

    def __call__(self, path):
        C = self.C

        # triangulate teh stuff
        bpy.ops.object.modifier_add(type='TRIANGULATE')

        with open(path, 'w', newline='') as fp:
            mesh = self.mesh(C.scene.frame_start)

            # TODO: proper handling
            assert (0 < len(mesh.uv_layers))

            # dota? =op
            fp.write('dota? =op\n')

            # counts
            triangle_count = len(mesh.polygons)
            frame_count = C.scene.frame_end - C.scene.frame_start + 1

            fp.write('teh model has %d triangles and %d frames.\n'
                    % (triangle_count, frame_count))

            # texcoords
            fp.write('teh texcoords are\n')
            for l in mesh.uv_layers[0].data:
                fp.write('%+f\t%+f\n' % tuple(l.uv))

            # vertices
            fp.write('and teh vertices are\n')
            for frame in range(C.scene.frame_start, C.scene.frame_end+1):
                mesh = self.mesh(frame)

                for l in mesh.loops:
                    v = mesh.vertices[l.vertex_index]
                    fp.write('%+f\t%+f\t%+f\n' % tuple(v.co))


class ExportTehModel(bpy.types.Operator, bpy_extras.io_utils.ExportHelper):
    """Export Teh Model"""
    bl_idname = "export_scene.teh"
    bl_label = "Export Teh Model"
    bl_options = {'REGISTER', 'UNDO'}
    filename_ext = ".teh"
    filter_glob = bpy.props.StringProperty(default="*.teh", options={'HIDDEN'})


    def execute(self, C):
        scene = C.scene

        TehModelExporter(C)(self.properties.filepath)

        return {'FINISHED'}

def register():
    bpy.utils.register_class(ExportTehModel)


def unregister():
    bpy.utils.unregister_class(ExportTehModel)


if __name__ == "__main__":
    register()
