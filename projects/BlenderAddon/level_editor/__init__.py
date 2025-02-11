import bpy
import math
import mathutils
import bpy_extras

import gpu
import gpu_extras.batch

import copy

import json

#アドオン情報
bl_info ={
    "name" : "LevelEditor",
    "author" : "TKfrom",
    "version" : (1, 0),
    "blender" : (3, 3, 1),
    "location" : "",
    "description" : "LevelEditor",
    "warning" : "",
    "wiki_url" : "",
    "tracker_url" : "",
    "category" : "Object"
}

# モジュールのインポート
from .stretch_vertex import MYADDON_OT_stretch_vertex
# スフィア作成
from .create_ico_sphere import MYADDON_OT_create_ico_sphere
# キューブ作成
from .create_cube import MYADDON_OT_create_cube
# コライダー
from .add_collider import MYADDON_OT_add_collider
# ファイルネーム
from .add_filename import MYADDON_OT_add_filename
#from .add_filename import OBJECT_PT_file_name

# 表示
from .add_disabled import MYADDON_OT_add_disabled
from .add_disabled import OBJECT_PT_disabled

# SpawnPoint
from .add_spawnPoint import MYADDON_OT_create_spawn

class DrawCollider:
    # 描画ハンドル
    handle = None

    #3Dビューに登録する関数
    def draw_collider():
        # 動的配列で取得
        #頂点データ
        vertices = {"pos":[]}
        #インデックスデータ
        indicess = []

        #各頂点の、中心からのオフセット
        offsets = [
            [-0.5,-0.5,-0.5], #左下前
            [+0.5,-0.5,-0.5], #右下前
            [-0.5,+0.5,-0.5], #左上前
            [+0.5,+0.5,-0.5], #右上前

            [-0.5,-0.5,+0.5], #左下奥
            [+0.5,-0.5,+0.5], #右下奥
            [-0.5,+0.5,+0.5], #左上奥
            [+0.5,+0.5,+0.5], #右上奥       
        ]

        #立方体のX,Y,Z方向サイズ
        size = [2,2,2]

        #現在シーンのオブジェクトリストを走査
        for object in bpy.context.scene.objects:

            #コライダープロパティがなければ、描画をスキップ
            if not "collider" in object:
                continue

            #中心点、サイズの変数を宣言
            center = mathutils.Vector((0,0,0))
            size = mathutils.Vector((2,2,2))

            #プロパティから値を取得
            center[0]=object["collider_center"][0]
            center[1]=object["collider_center"][1]
            center[2]=object["collider_center"][2]
            size[0]=object["collider_size"][0]
            size[1]=object["collider_size"][1]
            size[2]=object["collider_size"][2]
           #追加前の頂点数
            start = len(vertices["pos"])

            #Boxの8頂点分回す
            for offset in offsets:
                #オブジェクトの中心座標をコピー

                pos = copy.copy(center)
                #中心点を基準に各頂点ごとにずらす
                pos[0]+=offset[0]*size[0]
                pos[1]+=offset[1]*size[1]
                pos[2]+=offset[2]*size[2]
                #ローカル座標からワールド座標に変換
                pos = object.matrix_world @ pos
                #頂点データリストに座標を追加
                vertices['pos'].append(pos)
                #前面を構成する辺の頂点インデックス
                indicess.append([start+0,start+1])
                indicess.append([start+2,start+3])
                indicess.append([start+0,start+2])
                indicess.append([start+1,start+3])
                #奥面を構成する辺の頂点インデックス
                indicess.append([start+4,start+5])
                indicess.append([start+6,start+7])
                indicess.append([start+4,start+6])
                indicess.append([start+5,start+7])
                #前と頂点を繋ぐ辺の頂点インデックス
                indicess.append([start+0,start+4])
                indicess.append([start+1,start+5])
                indicess.append([start+2,start+6])
                indicess.append([start+3,start+7])

        #ビルトインのシェーダを取得
        shader = gpu.shader.from_builtin("UNIFORM_COLOR")

        #バッチを作成（引数：シェーダ、トポロジー、頂点データ、インデックスデータ
        batch = gpu_extras.batch.batch_for_shader(shader,"LINES",vertices,indices = indicess)

        #シェーダのパラメータ設定
        color = [0.5,1.0,1.0,1.0]
        shader.bind()
        shader.uniform_float("color",color)
        #描画
        batch.draw(shader)

class MYADDON_OT_export_scene(bpy.types.Operator, bpy_extras.io_utils.ExportHelper):
    bl_idname = "myaddon.myaddon_ot_export_scene"
    bl_label = "シーン出力"
    bl_description = "シーン情報をExportします"
    #出力するファイルの拡張子
    filename_ext = ".json"

    def write_and_print(self,file,str):
        print(str)

        file.write(str)
        file.write('\n')

    def parse_scene_recursive(self,file,object,level):
        """シーン解析用再帰関数"""

        #深さ文インデントする（タブを挿入
        indent = ''
        for i in range(level):
            indent += "\t"

        # オブジェクト名書き込み
        self.write_and_print(file,indent + object.type + " - " + object.name)
        trans,rot,scale = object.matrix_local.decompose()
        #回転をQuternionからEuler（3軸での回転角）に変換
        rot = rot.to_euler()
        #ラジアンから度数法に変換
        rot.x = math.degrees(rot.x)
        rot.y = math.degrees(rot.y)
        rot.z = math.degrees(rot.z)
        #トランスフォーム情報を表示
        self.write_and_print(file,indent + "T(%f,%f,%f)" % (trans.x, trans.y, trans.z))
        self.write_and_print(file,indent + "R(%f,%f,%f)" % (rot.x, rot.y, rot.z))
        self.write_and_print(file,indent + "S(%f,%f,%f)" % (scale.x, scale.y, scale.z))
        #カスタムプロパティ'file_name'
        if "file_name" in object:
            self.write_and_print(file, indent + "N %s" % object["file_name"])
        if "model_tag" in object:
            self.write_and_print(file, indent + "N %s" % object["model_tag"])            
        #カスタムプロパティ'collision'
        if "collider" in object:
            self.write_and_print(file,indent + "C %s" % object["collider"])
            
            temp_str = indent + "CC %f %f %f"
            temp_str %= (object["collider_center"][0],object["collider_center"][1],object["collider_center"][2])
            self.write_and_print(file,temp_str)

            temp_str = indent + "CS %f %f %f"
            temp_str %= (object["collider_size"][0],object["collider_size"][1],object["collider_size"][2])
            self.write_and_print(file,temp_str)
        self.write_and_print(file,indent + 'END')
        self.write_and_print(file, '')

        for child in object.children:
            self.parse_scene_recursive(file,child,level + 1)

    def parse_scene_recursive_json(self,data_parent,object,level):
        #シーンのオブジェクト1個分のjsonオブジェクト生成
        json_object = dict()
        #オブジェクト種類
        #json_object["type"] = object.type
        if "type" in object:
            json_object["type"] = object["type"]
        else:
            json_object["type"] = object.type
        #オブジェクト名
        json_object["name"] = object.name

        #Todo: その他情報をパック
        #オブジェクトのローカルトランスフォームから
        #平行移動、回転、スケールを抽出
        trans, rot, scale = object.matrix_local.decompose()
        #回転を Quternion から Euler （3軸での回転角）に変換
        rot = rot.to_euler()
        #ラジアンから度数法に変換
        rot.x = math.degrees(rot.x)
        rot.y = math.degrees(rot.y)
        rot.z = math.degrees(rot.z)
        #トランスフォーム情報をディクショナリに登録
        transform = dict()
        transform["translation"] = (trans.x,trans.y,trans.z)
        transform["rotation"] = (rot.x,rot.y,rot.z)
        transform["scaling"] = (scale.x,scale.y,scale.z)
        #まとめて1個分のjsonオブジェクトに登録
        json_object["transform"] = transform

        #カスタムプロパティ'file_name'
        if "file_name" in object:
            json_object["file_name"] = object["file_name"]

        #カスタムプロパティ'model_tag'
        if "model_tag" in object:
            json_object["model_tag"] = object["model_tag"]

        #カスタムプロパティ'collider'
        if "collider" in object:
            collider = dict()
            collider["type"] = object["collider"]
            collider["center"] = object["collider_center"].to_list()
            collider["size"] = object["collider_size"].to_list()
            json_object["collider"] = collider


        #1個分のjsonオブジェクトを親オブジェクトに登録
        data_parent.append(json_object)

        #Todo: 直接の子供リストを走査
        #子ノードがあれば
        if len(object.children) > 0:
            #子ノードリストを作成
            json_object["children"] = list()

            #子ノードへ進む（深さが1上がる）
            for child in object.children:
                self.parse_scene_recursive_json(json_object["children"],child,level + 1)

    def export(self):
        """ファイルに出力"""

        print("シーン情報出力開始... %r" % self.filepath)

        #ファイルをテキスト形式で書き出しようにオープン
        #スコープを抜けると自動的にクローズされる
        with open(self.filepath, "wt") as file:
            #ファイルに文字列を書き込む
            file.write("SCENE")
            for object in bpy.context.scene.objects:
                
                #親オブジェクトがあるものはスキップ
                if(object.parent):
                    continue

                #シーン直下のオブジェクトをルートノード（深さ０）とし、再帰関数で走査
                self.parse_scene_recursive(file,object,0)

    def export_json(self):
        """"JSON形式でファイルに出力"""

        #保存する情報をまとめるdict
        json_object_root = dict()

        #ノード名
        json_object_root["name"] = "scene"
        #オブジェクトリストを作成
        json_object_root["objects"] = list()

        #Todo: シーン内の全オブジェクト操作してパック
            #シーン内の全オブジェクトについて
        for object in bpy.context.scene.objects:
            #親があるものはスキップ
            if(object.parent):
                continue

            #シーン直下のオブジェクトをルートノード（深さ0）とし、再帰関数で走査
            self.parse_scene_recursive_json(json_object_root["objects"],object,0)


        #オブジェクトをJSON文字列にエンコード
        #json_text = json.JSONEncoder().encode(json_object_root)
        json_text = json.dumps(json_object_root, ensure_ascii=False,cls=json.JSONEncoder,indent=4)
        #コンソールに表示
        print(json_text)

        #ファイルをテキスト形式で書き出しようにオープン
        #スコープを抜けると自動的にクローズに
        with open(self.filepath,"wt",encoding = "utf-8") as file:

            #ファイルに文字列を書き込む
            file.write(json_text)
            # #シーン内の全オブジェクトについて
            # for object in bpy.context.scene.objects:
            #     #親があるものはスキップ
            #     if(object.parent):
            #         continue

            #     #シーン直下のオブジェクトをルートノード（深さ0）とし、再帰関数で走査
            #     self.parse_scenerecursive_json(json_object_root["objects"],object,0)



    def execute(self,context):

        print("シーン情報をExportします")
        self.export_json()
        #シーン内の全オブジェクトについて
        #ファイルに出力
        #self.export()

        self.report({'INFO'},"シーン情報をExportしました")
        print("シーン情報をExportしました")

        return {'FINISHED'}

class MYADDON_OT_add_modeltag(bpy.types.Operator):
    bl_idname ="myaddon.myaddon_ot_add_modeltag"
    bl_label = "Modeltag 追加"
    bl_descriptor = "['model_tag']カスタムプロパティを追加します"
    bl_options = {"REGISTER","UNDO"}

    def execute(self,context):
        #['model_path']追加
        context.object["model_tag"]=""

        return {"FINISHED"}


#トップバーの拡張メニュー
class TOPBAR_MT_my_menu(bpy.types.Menu):
    #Blenderクラスを識別するための固有文字列
    bl_idname = "TOPBER_MT_my_menu"
    #ラベルとして表示される文字
    bl_label = "MyMenu"
    #著者の表示
    bl_description = "拡張メニュー by " + bl_info["author"]

    # サブメニューの描画
    def draw(self,context):
        #トップバーの「エディターメニュー」に項目（オペレータ）を追加
        self.layout.operator(MYADDON_OT_stretch_vertex.bl_idname,
            text = MYADDON_OT_stretch_vertex.bl_label)
        self.layout.operator(MYADDON_OT_create_cube.bl_idname,
            text = MYADDON_OT_create_cube.bl_label)
                
        self.layout.operator(MYADDON_OT_create_ico_sphere.bl_idname,
            text = MYADDON_OT_create_ico_sphere.bl_label)
        
        self.layout.operator(MYADDON_OT_export_scene.bl_idname,
            text = MYADDON_OT_export_scene.bl_label)

        self.layout.operator(MYADDON_OT_create_spawn.bl_idname,
            text = MYADDON_OT_create_spawn.bl_label)

        #self.layout.operator(MYADDON_OT_add_collider.bl_idname,

    #既存のメニューにサブメニューを追加
    def submenu(self,context):
        # ID指定でサブメニューを追加
        self.layout.menu(TOPBAR_MT_my_menu.bl_idname)

#パネル ファイル名
class OBJECT_PT_file_name(bpy.types.Panel):
    """オブジェクトのファイルネームパネル"""
    bl_idname = "OBJECT_PT_file_name"
    bl_label = "FileName"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    # サブメニューの描画
    def draw(self,context):
        #パネルに項目を追加
        if "file_name" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object,'["file_name"]',text=self.bl_label)
        else:
            #プロパティがなければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_filename.bl_idname)

#パネル モデル名
class OBJECT_PT_model_tag(bpy.types.Panel):
    """オブジェクトのファイルネームパネル"""
    bl_idname = "OBJECT_PT_model_tag"
    bl_label = "ModelTag"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    # サブメニューの描画
    def draw(self,context):
        #パネルに項目を追加
        if "model_tag" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object,'["model_tag"]',text=self.bl_label)
        else:
            #プロパティがなければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_modeltag.bl_idname)

#パネル コライダー
class OBJECT_PT_collider(bpy.types.Panel):
    bl_idname = "OBJECT_PT_collider"
    bl_label = "Collider"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    #サブメニューの描画
    def draw(self,context):
        #パネルに項目を追加
        if "collider" in context.object:
            #既にプロパティがあれば、プロパティを表示
            self.layout.prop(context.object,'["collider"]',text = "Type")
            self.layout.prop(context.object,'["collider_center"]',text = "Center")
            self.layout.prop(context.object,'["collider_size"]',text = "Size")
        else:
            #プロパティが無ければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_collider.bl_idname)

# Blenderに登録するクラスリスト
classes = {
    MYADDON_OT_stretch_vertex,
    MYADDON_OT_create_ico_sphere,
    MYADDON_OT_create_cube,
    MYADDON_OT_export_scene,
    TOPBAR_MT_my_menu,
    MYADDON_OT_add_filename,
    MYADDON_OT_add_modeltag,
    OBJECT_PT_file_name,
    OBJECT_PT_model_tag,
    MYADDON_OT_add_disabled,
    OBJECT_PT_disabled,
    MYADDON_OT_add_collider,
    OBJECT_PT_collider,
    MYADDON_OT_create_spawn,
}

#メニュー項目描画
def draw_menu_manual(self, context):
    #self : 呼び出し元のクラスインスタンス
    #context : カーソルを合わせた時のポップアップのカスタマイズなど

    #トップバーの「エディターメニュー」に項目（オペレータ）を追加
    self.layout.operator("wm.url_open_preset",text = "Manual", icon ='HELP')




#アドオン有効
def register():
    #クラスに   登録
    for cls in classes:
        bpy.utils.register_class(cls)

    #メニューに項目を追加
    bpy.types.TOPBAR_MT_editor_menus.append(TOPBAR_MT_my_menu.submenu)
    #3Dビューに描画関数を追加
    DrawCollider.handle = bpy.types.SpaceView3D.draw_handler_add(DrawCollider.draw_collider,(),"WINDOW","POST_VIEW")
    
    print("有効化")
    
#アドオン無効
def unregister():
    #メニューから項目を削除
    bpy.types.TOPBAR_MT_editor_menus.remove(TOPBAR_MT_my_menu.submenu)
    #3Dビューから描画関数を削除
    bpy.types.SpaceView3D.draw_handler_remove(DrawCollider.handle,"WINDOW")

    #クラスを削除
    for cls in classes:
        bpy.utils.unregister_class(cls)
    print("無効化")

#TestCode
if __name__ == "__main__":
    register()
