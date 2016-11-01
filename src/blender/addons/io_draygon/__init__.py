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

# http://blender.stackexchange.com/questions/9233/how-do-i-add-a-panel-to-the-rigid-body-panel - sambler
import bpy
from bpy.props import *
from bpy.types import Panel, UIList
import copy
import math
from mathutils import Euler, Vector, Quaternion

def ensureUniqueName(name):
    i = 1
    originalName = name
    while (name in bpy.context.scene.objects):
        name = originalName + "_" + str(i)
    
    return name

def setCollisionShapeName(self, value):
    # Don't do anything if current name == new name
    if "name" in self:
        if self["name"] == value:
            return None
        else:
            # Rename associated collision geometry too
            if "name" in self:
                # If another collision shape already exists with this name, rename this one
                value = ensureUniqueName(value)
                bpy.context.scene.objects[self.name].name = value
          
    if "name" in self:
        self["oldname"] = self["name"]
    else:
        self["oldname"] = ""
          
    self["name"] = value
    return None
        

def getCollisionShapeName(self):
    if ("name" in self):
        return self["name"]
    else:
        return ""
    
def updateCollisionShape(self, context):
    if "name" in self:
        self["oldname"] = self["name"]

# TODO Inherit box collision shape and sphere collision shape from this??
class CollisionShapes(bpy.types.PropertyGroup):
    """Class for collision shapes CollectionProperty"""
    CollisionShapeTypes = [
        ('box', 'box', 'box'),
        ('Sphere', 'Sphere', 'Sphere')]
    oldname = StringProperty(name="oldname")
    name = StringProperty(name="name", description="Name of this collision shape", get=getCollisionShapeName, set=setCollisionShapeName)
    id = IntProperty()
    shape = EnumProperty(name="shape",
        description="Shape of collision shape",
        items=CollisionShapeTypes)
    isInvMass = BoolProperty(default=False)
    mass = FloatProperty()
    isInvInertiaTensor = BoolProperty(default=False)
    inertiaTensor = FloatVectorProperty(size=3)

def createCollisionBox(obj, name):
    if (obj.rigid_body):
        bpy.ops.mesh.primitive_cube_add(
            radius=1,
            location=(0, 0, 0),
            rotation=(0, 0, 0))
            
        colbox = bpy.context.active_object
        colbox.name = name      
        colbox.parent = obj
        colbox.rigid_body_is_col_shape = True
        
        return colbox

class PHYSICS_UL_rigid_body_collision_shapes(bpy.types.UIList):
    def draw_item(self, context, layout, data, item, icon, active_data, active_propname, index, flt_flag):
        split = layout.split(0.2)
        split.label(str(item.id))
        split.prop(item, "name", text="", emboss=False, translate=False, icon='BORDER_RECT')

class PHYSICS_PT_rigid_body_collisions(bpy.types.Panel):
    bl_label = "Game Rigid Body Collisions"
    bl_idname = "PHYSICS_PT_game_rigid_body_collisions"
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "physics"
    bl_default_closed = True

    @classmethod
    def poll(cls, context):
        obj = context.object
        return (obj and obj.rigid_body and
                obj.rigid_body.type == 'ACTIVE' and
                (not context.scene.render.use_game_engine))

    def draw(self, context):
        layout = self.layout

        ob = context.object

        row = layout.row()
        row.template_list("PHYSICS_UL_rigid_body_collision_shapes", "", ob, "rigid_body_collision_shapes", ob, "rigid_body_collision_shapes_index", rows=3)
        col = row.column(align=True)
        col.operator("rigid_body_props.add_col_shape", icon='ZOOMIN', text="")
        col.operator("rigid_body_props.remove_col_shape", icon='ZOOMOUT', text="")
        
        layout.operator("rigid_body_props.add_col_shape", icon='ZOOMIN', text="New")

        ob = bpy.context.object
        if (len(ob.rigid_body_collision_shapes) >= 1):
            item = ob.rigid_body_collision_shapes[ob.rigid_body_collision_shapes_index]
            layout.prop(item, "shape", text="Shape")
            
            if (item.shape == 'box'):
                # Find box in scene
                box = bpy.context.scene.objects[item.name]
                
                row = layout.row()
                row.prop(box, "scale", text="Scale")
                row = layout.row()
                row.prop(box, "location", text="Offset")
                row = layout.row()
                row.prop(item, "isInvMass", text="Use inverse mass?")
                row = layout.row()
                row.prop(item, "mass", text="Mass")
                row = layout.row()
                row.prop(item, "isInvInertiaTensor", text="Use inverse inertia tensor?")
                row = layout.row()
                row.prop(item, "inertiaTensor", text="Inertia tensor: ")
            elif (item.shape == 'Sphere'):
                row = layout.row()
                #row.prop(item, "radius", text="Radius")
                row = layout.row()
                row.prop(item, "location", text="Offset")

class PHYSICS_OT_add_rigid_body_collision_shape(bpy.types.Operator):
    bl_label = ""
    bl_description = "Add a Collision Shape to the Rigid Body"
    bl_idname = "rigid_body_props.add_col_shape"

    def execute(self, context):
        ob = bpy.context.object
        item = ob.rigid_body_collision_shapes.add()
        item.id = len(ob.rigid_body_collision_shapes)
        name = "Collision Shape " + chr(item.id + 64)
        name = ensureUniqueName(name)
        colbox = createCollisionBox(ob, name)
        item.name = colbox.name
 
        bpy.ops.object.select_all(action='DESELECT')
        ob.select = True
        bpy.context.scene.objects.active = ob
        
        return {'FINISHED'}

class PHYSICS_OT_remove_rigid_body_collision_shape(bpy.types.Operator):
    bl_label = ""
    bl_description = "Remove Selected Collision Shape"
    bl_idname = "rigid_body_props.remove_col_shape"

    @classmethod
    def poll(cls, context):
        colShapes = bpy.context.object.rigid_body_collision_shapes
        return bool(colShapes.items())

    def execute(self, context):
        bpy.ops.object.mode_set(mode='OBJECT')
        colShapes = bpy.context.object.rigid_body_collision_shapes
        i = bpy.context.object.rigid_body_collision_shapes_index
        bpy.ops.object.select_all(action='DESELECT')
        bpy.context.scene.objects[colShapes[i].name].select = True
        bpy.ops.object.delete()
        colShapes.remove(i)

        return {'FINISHED'}

class PHYSICS_PT_rigid_body_dynamics(bpy.types.Panel):
    bl_label = "Game Rigid Body Dynamics"
    bl_idname = "PHYSICS_PT_game_rigid_body_dynamics"
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "physics"
    bl_default_closed = True

    @classmethod
    def poll(cls, context):
        obj = context.object
        return (obj and obj.rigid_body and
                obj.rigid_body.type == 'ACTIVE' and
                (not context.scene.render.use_game_engine))

    def draw(self, context):
        layout = self.layout

        ob = context.object
        rb = ob.rigid_body

        # row = layout.row()
        # row.prop(ob, "rigid_body_use_inv_mass", text="Use Inverse Mass?")
        # row = layout.row()
        # if ob.rigid_body_use_inv_mass == True:
        #     row.prop(ob, "rigid_body_inv_mass", text="Inverse Mass")
        # else:
        #     row.prop(rb, "mass", text="Mass")
        # row = layout.row()
        # row.prop(ob, "rigid_body_use_inv_inertia_tensor", text="Use Inverse Inertia Tensor?")
        
        #row.prop(ob, "rigid_body_inertia_tensor", text="Inertia Tensor")


tab = "  "
luatab = "    "

def getAssetRelativePath(path):
    assetdirname = "assets/"
    
    return path[path.find(assetdirname) + len(assetdirname):]

def writeObjectTexture(obj, folderpath):
    import shutil
    texturepath = folderpath + "/" + obj.active_material.active_texture.name + ".texture"
    
    # Make a copy of the source image and save it in the assets directory
    img = obj.active_material.active_texture.image
    newimgpath = ""
    if (len(os.path.splitext(img.name)[1]) > 0):
        newimgpath = folderpath + "/" + img.name
    else:
        ext = os.path.splitext(img.filepath)[1]
        newimgpath = folderpath + "/" + img.name + ext

    try:
        shutil.copyfile(bpy.path.abspath(img.filepath), newimgpath)
    except shutil.Error as e:
        # Ignore same file error
        print("Warning: %s" % e)
    
    out = open(texturepath, 'w')
    out.write("{\n")
    out.write(tab + "\"type\": \"2D\",\n")
    out.write(tab + "\"images\": [\n")
    out.write(tab + tab + "\"" + getAssetRelativePath(newimgpath) + "\"\n")
    out.write(tab + "]\n")
    out.write("}")
    out.write("\n")
    out.close()
    
    return getAssetRelativePath(texturepath)

def writeObjectMaterial(obj, folderpath, texturepath):
    materialpath = folderpath + "/" + obj.active_material.name + ".material"

    out = open(materialpath, 'w')
    out.write("{\n")
    # out.write(tab + "\"shader\": \"" + obj.active_material["shaderpath"] + "\",\n\n")
    out.write(tab + "\"shader\": \"" + "simple.shader" + "\",\n\n")
    
    out.write(tab + "\"textures\": [\n")
    out.write(tab + tab + "{\n")
    out.write(tab + tab + tab + "\"name\": \"tex\",\n")
    out.write(tab + tab + tab + "\"texture\": \"" + texturepath + "\"\n")
    out.write(tab + tab + "}\n")
    out.write(tab + "],\n\n")
    
    out.write(tab + "\"parameters\": [\n")
    out.write(tab + "]\n")
    
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
    location = copy.copy(obj.location)
    orientation = copy.copy(obj.rotation_euler)
    # orientation = copy.copy(obj.rotation_quaternion)
    scale = copy.copy(obj.scale)
    
    obj.location = Vector((0.0, 0.0, 0.0))
    obj.rotation_euler = Euler((0.0, 0.0, 0.0), 'XYZ')
    #obj.rotation_quaternion = Quaternion((1.0, 0.0, 0.0, 0.0))
    # obj.scale = Vector((1.0, 1.0, 1.0))
    
    # Export .obj from selected object only
    bpy.ops.export_scene.obj(filepath=meshpath, axis_forward='-Z', axis_up='Y', use_materials=False, use_normals=True, use_uvs=True, use_triangles=True, use_selection=True)
    
    obj.location = location
    obj.rotation_euler = orientation
    # obj.rotation_quaternion = orientation
    # obj.scale = scale
    
    return getAssetRelativePath(meshpath)

def outputRenderComponent(obj, folderpath, meshpath, materialpath, out):
    out.write(tab + tab + "\"renderComponent\": {\n")
    out.write(tab + tab + tab + "\"mesh\": \"" + meshpath + "\",\n")
    out.write(tab + tab + tab + "\"materials\": [\n")
    out.write(tab + tab + tab + tab + "\"" + materialpath + "\"\n")
    out.write(tab + tab + tab + "]\n")
    out.write(tab + tab + "}")

def outputPhysicsComponent(obj, folderpath, out):
    out.write(tab + tab + "\"physicsComponent\": {\n")
    out.write(tab + tab + tab + "\"restitution\": " + str(obj.rigid_body.restitution) + ",\n")
    out.write(tab + tab + tab + "\"damping\": " + str(obj.rigid_body.linear_damping) + ",\n")
    out.write(tab + tab + tab + "\"angularDamping\": " + str(obj.rigid_body.angular_damping) + ",\n")
    # TODO inverse mass
    #if (obj.rigid_body.use_inv_mass):
        #out.write(tab + tab + tab + "\"invMass\": " + str(obj.rigid_body.inv_mass) + ",\n")
    # out.write(tab + tab + tab + "\"mass\": " + str(obj.rigid_body.mass) + ",\n")
    # out.write(tab + tab + tab + "\"invInertiaTensor\": " + "[1.0, 1.0, 1.0],\n")
    out.write(tab + tab + tab + "\"collisionShapes\": [\n")
    out.write(tab + tab + tab + tab + "{\n")
    
    i = 0
    for shape in obj.rigid_body_collision_shapes:
        colobj = bpy.context.scene.objects[shape.name]
        out.write(tab + tab + tab + tab + tab + "\"name\": \"" + shape.name + "\",\n")
        out.write(tab + tab + tab + tab + tab + "\"type\": \"" + shape.shape + "\",\n")
        if (shape.isInvMass):
            out.write(tab + tab + tab + tab + tab + "\"invMass\": " + str(shape.mass) + ",\n")
        else:
            out.write(tab + tab + tab + tab + tab + "\"mass\": " + str(shape.mass) + ",\n")
        if (shape.isInvInertiaTensor):
            out.write(tab + tab + tab + tab + tab + "\"invInertiaTensor\": [" + str(shape.inertiaTensor[0]) + ", " + str(shape.inertiaTensor[1]) + ", " + str(shape.inertiaTensor[2]) + "],\n")
        else:
            out.write(tab + tab + tab + tab + tab + "\"inertiaTensor\": [" + str(shape.inertiaTensor[0]) + ", " + str(shape.inertiaTensor[1]) + ", " + str(shape.inertiaTensor[2]) + "],\n")

        if (shape.shape == "box"):
            out.write(tab + tab + tab + tab + tab + "\"halfSize\": [" + str(colobj.scale[0] / 2.0) + ", " + str(colobj.scale[1] / 2.0) + ", " + str(colobj.scale[2] / 2.0) + "],\n")
                
        out.write(tab + tab + tab + tab + tab + "\"offset\": [" + str(colobj.location[0]) + ", " + str(colobj.location[2]) + ", " + str(-colobj.location[1]) + "]\n")
        out.write(tab + tab + tab + tab + "}")
        if (i < (len(obj.rigid_body_collision_shapes) - 1)):
            out.write(",\n")
        else:
            out.write("\n")
        i = i + 1

    out.write(tab + tab + tab + "]\n")
    out.write(tab + tab + "}")

def writePrefab(obj, folderpath, meshpath, materialpath):
    prefabpath = folderpath + "/" + obj.name + ".prefab"
    
    out = open(prefabpath, 'w')
    out.write("{\n")
    out.write(tab + "\"components\": {\n")
    outputRenderComponent(obj, folderpath, meshpath, materialpath, out)
    if (obj.rigid_body):
        out.write(",\n")
        outputPhysicsComponent(obj, folderpath, out)
    out.write("\n")
    out.write(tab + "}\n")
    out.write("}")
    out.close()
    
    return getAssetRelativePath(prefabpath)

def writeAll(context, folderpath, levelpath):

    levelout = open(levelpath, 'w')
    levelname = os.path.splitext(os.path.split(levelpath)[1])[0]
    levelout.write("-- GENERATED BY BLENDER SCRIPT\n")
    levelout.write("function " + levelname + "(offset)\n")

    
    scene = bpy.context.scene
    for obj in scene.objects: 
        if (obj.type == 'MESH'):
            # Skip collision shapes
            if ("rigid_body_is_col_shape" in obj and obj.rigid_body_is_col_shape == True):
                continue
            # Paths are relative to nearest asset/ dir
            reltexturepath = writeObjectTexture(obj, folderpath)
            relmaterialpath = writeObjectMaterial(obj, folderpath, reltexturepath)
            relmeshpath = writeObjectMesh(obj, folderpath)
            relprefabpath = writePrefab(obj, folderpath, relmeshpath, relmaterialpath)
            
            # Get position, orientation and scale
            position = "Vector3(" + str(obj.location.x) + ", " + str(obj.location.z) + ", " + str(-obj.location.y) + ")"
            # scale = "Vector3(" + str(obj.scale.x) + ", " + str(obj.scale.y) + ", " + str(obj.scale.z) + ")"
            scale = "Vector3(1, 1, 1)"
            #orientation = obj.rotation_quaternion
            orientation = obj.rotation_euler.to_quaternion()

            orientation = "Quaternion(" + str(orientation[1]) + ", " + str(orientation[3]) + ", " + str(-orientation[2]) + ", " + str(orientation[0]) + ")"
            
            levelout.write(luatab + "local location = " + position + "\n")
            levelout.write(luatab + "if offset ~= nil then\n")
            levelout.write(luatab + luatab + "location = location + offset\n")
            levelout.write(luatab + "end\n")
            # Output spawn prefab call to level file
            levelout.write(luatab + "Script.spawn_prefab(\"" + os.path.splitext(relprefabpath)[0] + "\", location" + ", " + orientation + ", " + scale + ")\n")

    # Finish level file
    levelout.write("end")
    levelout.close()
    
    return {'FINISHED'}

class ExportDraygonTensorLua(bpy.types.Operator, ExportHelper):
    bl_idname = "export_draygon_tensor.world"
    bl_label = "Export DraygonTensor engine scene description"
    bl_options = {'PRESET'}

    # Select folder, not file
    use_filter_folder = True
    filename_ext = ".lua"
    
    # Filter everything that isn't a folder
    # filter_glob = StringProperty(
    #         default="/",
    #         options={'HIDDEN'},
    #         )

    def execute(self, context):
        folderpath = os.path.dirname(self.filepath)
        print(folderpath)
        
        bpy.ops.object.mode_set(mode='OBJECT')
        
        return writeAll(context, folderpath, self.filepath)

def menu_func(self, context):
    self.layout.operator(ExportDraygonTensorLua.bl_idname, text="Draygon Tensor (.world)")

def register():
    bpy.utils.register_module(__name__)

    bpy.types.Object.rigid_body_collision_shapes = CollectionProperty(type=CollisionShapes)
    bpy.types.Object.rigid_body_collision_shapes_index = IntProperty()
    bpy.types.Object.rigid_body_is_col_shape = BoolProperty(default=False)
    # bpy.utils.register_class(ExportDraygonTensorLua)
    bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
    bpy.utils.unregister_module(__name__)
    del bpy.types.Object.rigid_body_collision_shapes
    del bpy.types.Object.rigid_body_collision_shapes_index
    del bpy.types.Object.rigid_body_is_col_shape
    # bpy.utils.unregister_class(ExportDraygonTensorLua)
    bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
    register()
