# http://blender.stackexchange.com/questions/9233/how-do-i-add-a-panel-to-the-rigid-body-panel - sambler
import bpy
from bpy.props import *
from bpy.types import Panel, UIList

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
        ('Box', 'Box', 'Box'),
        ('Sphere', 'Sphere', 'Sphere')]
    oldname = StringProperty(name="oldname")
    name = StringProperty(name="name", description="Name of this collision shape", get=getCollisionShapeName, set=setCollisionShapeName)
    id = IntProperty()
    shape = EnumProperty(name="shape",
        description="Shape of collision shape",
        items=CollisionShapeTypes)

def createCollisionBox(obj, name):
    if (obj.rigid_body):
        bpy.ops.mesh.primitive_cube_add(
            radius=1,
            location=(0, 0, 0),
            rotation=(0, 0, 0))
            
        colbox = bpy.context.active_object
        colbox.name = name      
        colbox.parent = obj
        colbox.is_col_shape = True
        
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
            
            if (item.shape == 'Box'):
                # Find box in scene
                box = bpy.context.scene.objects[item.name]
                
                row = layout.row()
                row.prop(box, "scale", text="Scale")
                row = layout.row()
                row.prop(box, "location", text="Offset")
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

def register():
    bpy.utils.register_module(__name__)

    bpy.types.Object.rigid_body_collision_shapes = CollectionProperty(type=CollisionShapes)
    bpy.types.Object.rigid_body_collision_shapes_index = IntProperty()
    bpy.types.Object.is_col_shape = BoolProperty(default=False)

def unregister():
    bpy.utils.unregister_module(__name__)
    del bpy.types.Object.rigid_body_collision_shapes
    del bpy.types.Object.rigid_body_collision_shapes_index
    del bpy.types.Object.is_col_shape

if __name__ == "__main__":
    register()
