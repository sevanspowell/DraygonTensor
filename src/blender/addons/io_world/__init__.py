bl_info = {
    "name": "Export scene from Blender to DraygonTensor engine",
    "author": "Samuel Evans-Powell",
    "blender": (2,7,0),
    "version": (0, 0, 1),
    "location": "File > Import-Export",
    "description": "Export DraygonTensor engine scene description format",
    "category": "Import-Export"
}

import bpy
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator
import os.path

tab = "  "
luatab = "    "

def getAssetRelativePath(path):
    assetdirname = "assets/"
    
    return path[path.find(assetdirname) + len(assetdirname):]

def writeObjectTexture(obj, folderpath):
    texturepath = folderpath + "/" + obj.active_material.active_texture.name + ".texture"
    
    out = open(texturepath, 'w')
    out.write("{\n")
    out.write(tab + "\"type\": \"2D\",\n")
    out.write(tab + "\"images\": {\n")
    out.write(tab + tab + "\"0\": \"" + getAssetRelativePath(obj.active_material.active_texture.image.filepath) + "\"\n")
    out.write(tab + "}\n")
    out.write("}")
    out.write("\n")
    out.close()
    
    return getAssetRelativePath(texturepath)

def writeObjectMaterial(obj, folderpath, texturepath):
    materialpath = folderpath + "/" + obj.active_material.name + ".material"

    out = open(materialpath, 'w')
    out.write("{\n")
    out.write(tab + "\"shader\": \"" + obj.active_material["shaderpath"] + "\",\n\n")
    
    out.write(tab + "\"textures\": {\n")
    out.write(tab + tab + "\"tex\": {\n")
    out.write(tab + tab + tab + "\"texture\": \"" + texturepath + "\"\n")
    out.write(tab + tab + "}\n")
    out.write(tab + "},\n\n")
    
    out.write(tab + "\"parameters\": {\n")
    out.write(tab + "}\n")
    
    out.write("}")
    out.write("\n")
    out.close()

    return getAssetRelativePath(materialpath)

def writeObjectMesh(obj, folderpath):
    # Output mesh
    meshpath = folderpath + "/" + obj.name + ".obj"
    # Deselect all objects
    bpy.ops.object.select_all(action='DESELECT')
    # Select object to be exported
    obj.select = True
    # Export .obj from selected object only
    bpy.ops.export_scene.obj(filepath=meshpath, axis_forward='-Z', axis_up='Y', use_materials=False, use_normals=True, use_uvs=True, use_triangles=True, use_selection=True)
   
    return getAssetRelativePath(meshpath)

def outputRenderComponent(obj, folderpath, meshpath, materialpath, out):
    out.write(tab + tab + "\"renderComponent\": {\n")
    out.write(tab + tab + tab + "\"mesh\": \"" + meshpath + "\",\n")
    out.write(tab + tab + tab + "\"materials\": {\n")
    out.write(tab + tab + tab + tab + "\"" + materialpath + "\": \"\"\n")
    out.write(tab + tab + tab + "}\n")
    out.write(tab + tab + "}\n")
    

def writePrefab(obj, folderpath, meshpath, materialpath):
    prefabpath = folderpath + "/" + obj.name + ".prefab"
    
    out = open(prefabpath, 'w')
    out.write("{\n")
    out.write(tab + "\"components\": {\n")
    outputRenderComponent(obj, folderpath, meshpath, materialpath, out)
    out.write(tab + "}\n")
    out.write("}")
    out.close()
    
    return getAssetRelativePath(prefabpath)

def writeAll(context, folderpath, levelpath):

    levelout = open(levelpath, 'w')
    levelname = os.path.splitext(os.path.split(levelpath)[1])[0]
    levelout.write("function " + levelname + "()\n")

    
    scene = bpy.context.scene
    for obj in scene.objects: 
        if (obj.type == 'MESH'):
            # Paths are relative to nearest asset/ dir
            reltexturepath = writeObjectTexture(obj, folderpath)
            relmaterialpath = writeObjectMaterial(obj, folderpath, reltexturepath)
            relmeshpath = writeObjectMesh(obj, folderpath)
            relprefabpath = writePrefab(obj, folderpath, relmeshpath, relmaterialpath)
            
            position = "Vector3(" + str(obj.location.x) + ", " + str(obj.location.y) + ", " + str(obj.location.z) + ")"
            #scale = "Vector3(" + str(obj.scale.x) + ", " + str(obj.scale.y) + ", " + str(obj.scale.z) + ")"
            levelout.write(luatab + "Script.spawn_prefab(\"" + os.path.splitext(relprefabpath)[0] + "\", " + position + ")\n")

    levelout.write("end")
        
    return {'FINISHED'}

class ExportDraygonTensorLua(bpy.types.Operator, ExportHelper):
    bl_idname = "export_draygon_tensor.world"
    bl_label = "Export DraygonTensor engine scene description"

    # Select folder, not file
    use_filter_folder = True
    filename_ext = ".lua"
    
    # Filter everything that isn't a folder
    filter_glob = StringProperty(
            default="/",
            options={'HIDDEN'},
            )

    def execute(self, context):
        folderpath = os.path.dirname(self.filepath)
        print(folderpath)
        
        bpy.ops.object.mode_set(mode='OBJECT')
        
        return writeAll(context, folderpath, self.filepath)

def menu_func(self, context):
    self.layout.operator(ExportDraygonTensor.bl_idname, text="Draygon Tensor (.world)")

def register():
    bpy.utils.register_class(ExportDraygonTensorLua)
    bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
    bpy.utils.unregister_class(ExportDraygonTensorLua)
    bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
    register()
    
    #test call
    bpy.ops.export_draygon_tensor.world('INVOKE_DEFAULT')
