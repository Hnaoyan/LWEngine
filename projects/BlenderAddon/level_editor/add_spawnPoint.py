import bpy
import bpy.ops
import os

class MYADDON_OT_add_spawn_symbol(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_spawn_symbol"
    bl_label = "出現ポイントシンボルImport"
    bl_description = "出現ポイントシンボルImport"
    prototype_object_name = "ProttypePlayerSpawn"
    object_name = "PlayerSpawn"

    def execute(self, context):
        print("出現ポイントのシンボルをImportします")

        # 重複ロード帽子
        spawn_object = bpy.data.objects.get(MYADDON_OT_add_spawn_symbol.prototype_object_name)
        if spawn_object is not None:
            return {'CANCELLED'}

        # スクリプトが配置されているディレクトリの名前を取得する
        addon_directory = os.path.dirname(__file__)
        #
        relative_path = "player/player.obj"
        #
        full_path = os.path.join(addon_directory,relative_path)
        # オブジェクトインポート
        bpy.ops.wm.obj_import('EXEC_DEFAULT',
                              filepath = full_path, display_type = 'THUMBNAIL',
                              forward_axis = 'Z', up_axis = 'Y')
        # 回転を適用
        bpy.ops.object.transform_apply(location = False,
                                        rotation = True,
                                        scale = False,
                                        property = False,
                                        isolate_users = False)
        # アクティブなオブジェクトを取得
        object = bpy.context.active_object
        # 名前を変更
        object.name = MYADDON_OT_add_spawn_symbol.object_name
        # 種類設定
        object["type"] = MYADDON_OT_add_spawn_symbol.object_name

        # メモリ上にはおいておくがシーンから外す
        bpy.context.collection.objects.unlink(object)

        return {'FINISHED'}

class MYADDON_OT_create_spawn(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_create_spawn"
    bl_label = "出現ポイントシンボルの作成"
    bl_description = "出現ポイントのシンボルを作成します"
    bl_options = {'REGISTER','UNDO'}

    def execute(self, context):
        # 読み込み済みのコピー検索
        spawn_object = bpy.data.objects.get(MYADDON_OT_add_spawn_symbol.prototype_object_name)

        # まだ読み込んでいない場合
        if spawn_object is None:
            # 読み込み
            bpy.ops.myaddon.myaddon_ot_add_spawn_symbol('EXEC_DEFAULT')
            # 再建策
            spawn_object = bpy.data.objects.get(MYADDON_OT_add_spawn_symbol.prototype_object_name)

        print("出現ポイントの作成")
        # 選択解除
        bpy.ops.object.select_all(action = 'DESELECT')

        # 非表示オブジェクト複製
        object = spawn_object.copy()

        # シーンにリンク
        bpy.context.collection.objects.link(object)

        # 名前変更
        object.name = MYADDON_OT_add_spawn_symbol.object_name

        return {'FINISHED'}