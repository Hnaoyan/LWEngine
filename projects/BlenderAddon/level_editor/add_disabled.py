import bpy

#
class MYADDON_OT_add_disabled(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_disabled"
    bl_label = "Disabled 追加"
    bl_description = "['disabled']を追加"
    bl_options = {'REGISTER','UNDO'}

    def execute(self, context):
        context.object["disabled"] = True
        return {'FINISHED'}
    
class OBJECT_PT_disabled(bpy.types.Panel):
    """オブジェクトのファイルネームパネル"""
    bl_idname = "OBJECT_PT_disabled"
    bl_label = "Disabled"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    def draw(self, context):
        #パネルに項目を追加
        if "disabled" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object, '["disabled"]', text=self.bl_label)
        else:
            #プロパティがなければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_disabled.bl_idname)
