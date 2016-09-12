# http://blender.stackexchange.com/questions/9233/how-do-i-add-a-panel-to-the-rigid-body-panel - sambler
import bpy
from bpy.props import *
from bpy.types import Panel, UIList

# TODO Inherit box collision shape and sphere collision shape from this??
class CollisionShapes(bpy.types.PropertyGroup):
    """Class for collision shapes CollectionProperty"""
    CollisionShapeTypes = [
        ('Box', 'Box', 'Box'),
        ('Sphere', 'Sphere', 'Sphere')]
    #name = StringProperty()
    id = IntProperty()
    shape = EnumProperty(name="shape",
        description="Shape of collision shape",
        items=CollisionShapeTypes)
    halfsize = bpy.props.FloatVectorProperty(
        name="halfsize", description="", default=(1, 1, 1), min=0, precision=3, size=3)
    offset = bpy.props.FloatVectorProperty(
        name="offset", description="", default=(0, 0, 0), min=0, precision=3, size=3)

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
        row.template_list("PHYSICS_UL_rigid_body_collision_shapes", "", ob, "rigid_body_collision_shapes", ob, "rigid_body_collision_shapes_index", rows=1)
        col = row.column(align=True)
        col.operator("rigid_body_props.add_col_shape", icon='ZOOMIN', text="")
        col.operator("rigid_body_props.remove_col_shape", icon='ZOOMOUT', text="")
        
        layout.operator("rigid_body_props.add_col_shape", icon='ZOOMIN', text="New")

        ob = bpy.context.object
        if (len(ob.rigid_body_collision_shapes) >= 1):
            item = ob.rigid_body_collision_shapes[ob.rigid_body_collision_shapes_index]
            layout.prop(item, "shape", text="Shape")
            
            if (item.shape == 'Box'):
                row = layout.row()
                row.prop(item, "halfsize", text="Half Size")
                row = layout.row()
                row.prop(item, "offset", text="Offset")
            elif (item.shape == 'Sphere'):
                row = layout.row()
                #row.prop(item, "radius", text="Radius")
                row = layout.row()
                row.prop(item, "offset", text="Offset")

class PHYSICS_OT_add_rigid_body_collision_shape(bpy.types.Operator):
    bl_label = ""
    bl_description = "Add a Collision Shape to the Rigid Body"
    bl_idname = "rigid_body_props.add_col_shape"

    def execute(self, context):
        #colShapes = bpy.context.object.rigid_body_collision_shapes
        #pp = bpy.context.scene.palette_props
        #new_index = 0
        #if colShapes.items():
            #new_index = pp.current_color_index + 1
            #new_index = bpy.context.object.rigid_body_collision_shapes_index + 1
        #pp.colors.add()
        #colShapes.add()

        #last = colShapes.__len__() - 1

        #colShapes.move(last, new_index)
        #bpy.context.object.rigid_body_collision_shapes_index = new_index
        #sample()
        #update_panels()
            
        ob = bpy.context.object
        item = ob.rigid_body_collision_shapes.add()
        item.id = len(ob.rigid_body_collision_shapes)
        item.name = "Collision Shape " + chr(item.id + 64)
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
        colShapes = bpy.context.object.rigid_body_collision_shapes
        i = bpy.context.object.rigid_body_collision_shapes_index
        colShapes.remove(i)

        #if bpy.context.object.rigid_body_collision_shapes_index >= colShapes.__len__():
            #pp.index = pp.current_color_index = pp.colors.__len__() - 1
        return {'FINISHED'}

# class RigidBodyCollisionShapesPanel:
#     bl_space_type = 'PROPERTIES'
#     bl_region_type = 'WINDOW'
#     bl_context = "physics"
    
# class PHYSICS_PT_box(RigidBodyCollisionShapesPanel, Panel):
#     bl_label = "Box"
    
#     @classmethod
#     def poll(cls, context):
#         psys = context.particle_system
#         settings = particle_get_settings(context)

#         if settings is None:
#             return False
#         if settings.is_fluid:
#             return False
#         if particle_panel_poll(PARTICLE_PT_emission, context):
#             return psys is None or not context.particle_system.point_cache.use_external
#         return False

#     def draw(self, context):
#         layout = self.layout

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
