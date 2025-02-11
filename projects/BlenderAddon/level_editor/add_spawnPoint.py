import bpy
import bpy.ops
import os

class SpawnNames():

    # インデックス
    PROTOTYPE = 0   # プロトタイプのオブジェクト名
    INSTANCE = 1    # 量産時のオブジェクト名
    FILENAME = 2    # リソースファイル名

    names = {}
    # names["キー"] = (プロトタイプのオブジェクト名、量産時のオブジェクト名、リソースファイル名)
    names["Enemy"] = ("PrototypeEnemySpawn", "EnemySpawn", "needle/needle.obj")
    names["Player"] = ("PrototypePlayerSpawn", "PlayerSpawn", "player/player.obj")


class MYADDON_OT_add_spawn_symbol(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_spawn_symbol"
    bl_label = "出現ポイントシンボルImport"
    bl_description = "出現ポイントシンボルImport"
    prototype_object_name = "ProttypePlayerSpawn"
    object_name = "PlayerSpawn"

    def load_obj(self, type):

        # 重複ロード防止
        spawn_object = bpy.data.objects.get(SpawnNames.names[type][SpawnNames.PROTOTYPE])
        if spawn_object is not None:
            return {'CANCELLED'}
        print("出現ポイントのシンボルをImportします")

        # 重複ロード防止
        spawn_object = bpy.data.objects.get(MYADDON_OT_add_spawn_symbol.prototype_object_name)
        if spawn_object is not None:
            return {'CANCELLED'}

        # スクリプトが配置されているディレクトリの名前を取得する
        addon_directory = os.path.dirname(__file__)
        #
        relative_path = SpawnNames.names[type][SpawnNames.FILENAME]
        #
        full_path = os.path.join(addon_directory, relative_path)
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
        object.name = SpawnNames.names[type][SpawnNames.PROTOTYPE]
        # 種類設定
        object["type"] = SpawnNames.names[type][SpawnNames.INSTANCE]

        # メモリ上にはおいておくがシーンから外す
        bpy.context.collection.objects.unlink(object)

        return {'FINISHED'}
        

    def execute(self, context):
        # Enemy
        self.load_obj("Enemy")
        # Player
        self.load_obj("Player")

        return {'FINISHED'}

class MYADDON_OT_create_spawn(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_create_spawn"
    bl_label = "出現ポイントシンボルの作成"
    bl_description = "出現ポイントのシンボルを作成します"
    bl_options = {'REGISTER','UNDO'}

    # プロパティ（引数として渡せる）
    type = bpy.props.StringProperty(name = "Type", default = "Player")

    def execute(self, context):
        # 読み込み済みのコピー検索
        spawn_object = bpy.data.objects.get(SpawnNames.names[self.type][SpawnNames.PROTOTYPE])

        # まだ読み込んでいない場合
        if spawn_object is None:
            # 読み込み
            bpy.ops.myaddon.myaddon_ot_add_spawn_symbol('EXEC_DEFAULT')
            # 再建策
            spawn_object = bpy.data.objects.get(SpawnNames.names[self.type][SpawnNames.PROTOTYPE])

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
    
class MYADDON_OT_create_player_symbol(bpy.types.Operator):

    bl_idname = "myaddon.myaddon_ot_spawn_create_player_symbol"
    bl_label = "プレイヤー出現ポイントシンボルの作成"
    bl_description = "プレイヤーの出現シンボル作成"

    def execute(self, context):

        bpy.ops.myaddon.myaddon_ot_create_spawn('EXEC_DEFAULT', type = "Player")

        return {'FINISHED'}
    
class MYADDON_OT_create_enemy_symbol(bpy.types.Operator):

    bl_idname = "myaddon.myaddon_ot_spawn_create_enemy_symbol"
    bl_label = "エネミー出現ポイントシンボルの作成"
    bl_description = "敵の出現シンボル作成"

    def execute(self, context):

        bpy.ops.myaddon.myaddon_ot_create_spawn('EXEC_DEFAULT', type = "Enemy")

        return {'FINISHED'}

