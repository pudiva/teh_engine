bl_info = {
    "name": "Teh Engine Model Export",
    "category": "Export",
}

import bpy
import bpy_extras
import bmesh
import os
import sqlite3

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

        if os.path.exists(path): os.remove(path)

        teh = sqlite3.connect(path)
        teh.execute('CREATE TABLE triangles (i INTEGER, a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY (i))')
        teh.execute('CREATE TABLE vertices (f INTEGER, i INTEGER, x FLOAT, y FLOAT, z FLOAT, PRIMARY KEY (f, i))')
        teh.commit()

        mesh = self.mesh(C.scene.frame_start)

        triangles = [(p.index, *p.vertices) for p in mesh.polygons]
        teh.executemany('INSERT INTO triangles VALUES (?, ?, ?, ?)', triangles)
        teh.commit()

        # vertices
        for f in range(C.scene.frame_start, C.scene.frame_end+1):
            mesh = self.mesh(f)

            vertices = [(f, v.index, *v.co) for v in mesh.vertices]
            teh.executemany('INSERT INTO vertices VALUES (?, ?, ?, ?, ?)', vertices)
            teh.commit()

#        with open(path, 'w', newline='') as f:
#            mesh = self.mesh(C.scene.frame_start)
#
#            # indices
#            triangle_count = len(mesh.polygons)
#            index_count = 3 * triangle_count
#
#            f.write('triangle count: %d\n' % triangle_count)
#            f.write('index count: %d\n' % index_count)
#            f.write('indices:\n')
#            
#            for p in mesh.polygons:
#                a, b, c = p.vertices
#                f.write('%d\t%d\t%d\n' %(a, b, c))
#
#            # vertices
#            frame_count = C.scene.frame_end - C.scene.frame_start + 1
#            vertices_per_frame = len(mesh.vertices)
#            vertex_count = vertices_per_frame * frame_count
#
#            f.write('vertices per frame: %d\n' % vertices_per_frame)
#            f.write('frame count: %d\n' % frame_count)
#            f.write('vertex count: %d\n' % vertex_count)
#            f.write('vertices:\n')
#
#            for frame in range(C.scene.frame_start, C.scene.frame_end+1):
#                mesh = self.mesh(frame)
#
#                for v in mesh.vertices:
#                    x, y, z = v.co
#                    f.write('%+f\t%+f\t%+f\n' % (x, y, z))
#
#            # texcoords
#            # TODO


class ExportTehModel(bpy.types.Operator, bpy_extras.io_utils.ExportHelper):
    """Export Teh Model"""
    bl_idname = "export_scene.teh_model"
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
