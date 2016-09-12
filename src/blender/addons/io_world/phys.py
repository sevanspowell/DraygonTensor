# http://blender.stackexchange.com/questions/9233/how-do-i-add-a-panel-to-the-rigid-body-panel - sambler
import bpy
from bpy.props import *
from bpy.types import Panel, UIList

class PHYSICS_UL_rigid_body_collision_shapes(bpy.types.UIList):
    def draw_item(self, context, layout, data, item, icon, active_data, active_propname, index, flt_flag):
        split = layout.split(0.2)
        split.label(str(item.id))
        split.prop(item, "name", text="", emboss=False, translate=False, icon='BORDER_RECT')

class PHYSICS_PT_rigid_body_collisions(bpy.types.Panel):
    bl_label = "Rigid Body Collisions"
    bl_idname = "PHYSICS_PT_rigid_body_collisions"
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

class CollisionShapes(bpy.types.PropertyGroup):
    """Class for collision shapes CollectionProperty"""
    #name = StringProperty()
    id = IntProperty()
    halfsize = bpy.props.FloatVectorProperty(
        name="halfsize", description="", default=(1, 1, 1), min=0, precision=3, size=3)
    

def register():
    bpy.utils.register_module(__name__)

    #bpy.types.Object.rigid_body_collision_shapes = CollectionProperty(
    #    type=CollisionShapes, name="Rigidbody Collision Shapes", description=""
    #)
    bpy.types.Object.rigid_body_collision_shapes = CollectionProperty(type=CollisionShapes)
    bpy.types.Object.rigid_body_collision_shapes_index = IntProperty()

def unregister():
    bpy.utils.unregister_module(__name__)
    del bpy.types.Object.rigid_body_collision_shapes

if __name__ == "__main__":
    register()
    
    ob = bpy.context.object
    item = ob.rigid_body_collision_shapes.add()
    item.id = len(ob.rigid_body_collision_shapes)
    item.name = "Collision Shape " + chr(item.id + 64)
