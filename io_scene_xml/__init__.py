bl_info = {
	"name": "XML Format",
	"author": "John Paul Aglubat",
	"version": (0, 0, 1),
	"blender": (2, 75, 0),
	"location": "File > Export > XML (.xml)",
	"description": "Export mesh vertices, UV's, "
				   "vertex weights, armatures, and animations.",
	"wiki_url": ""
				"Scripts/Import-Export/XML_Exporter",
	"category": "Import-Export"}


import bpy
from bpy.props import BoolProperty
from bpy.props import EnumProperty
from bpy.props import StringProperty


class ExportXML(bpy.types.Operator):
	bl_idname = "export_scene.xml"
	bl_label = "Export XML"
	
	filepath = StringProperty(subtype='FILE_PATH')
	
	#Export Settings

	CoordinateSystem = EnumProperty(
		name="Coordinate System",
		description="Use the selected coordinate system for export",
		items=(('LEFT_HANDED', "Left-Handed", "Use a Y up, Z forward system or a Z up, -Y forward system"),
			   ('RIGHT_HANDED', "Right-Handed", "Use a Y up, -Z forward system or a Z up, Y forward system")),
		default='LEFT_HANDED')

	UpAxis = EnumProperty(
		name="Up Axis",
		description="The selected axis points upward",
		items=(('Y', "Y", "The Y axis points up"),
			   ('Z', "Z", "The Z axis points up")),
		default='Y')
		
	ExportTangents = BoolProperty(
		name="Export Tangent Space",
		description="Export normal tangents and bitangent_signs",
		default=True)
		
	def execute(self, context):
		self.filepath = bpy.path.ensure_ext(self.filepath, ".xml")

		from . import export_xml
		Exporter = export_xml.XMLExporter(self, context)
		Exporter.Export()
		return {'FINISHED'}

	def invoke(self, context, event):
		if not self.filepath:
			self.filepath = bpy.path.ensure_ext(bpy.data.filepath, ".xml")
		context.window_manager.fileselect_add(self)
		return {'RUNNING_MODAL'}

def menu_func(self, context):
	self.layout.operator(ExportXML.bl_idname, text="XML (.xml)")

def register():
	bpy.utils.register_module(__name__)

	bpy.types.INFO_MT_file_export.append(menu_func)


def unregister():
	bpy.utils.unregister_module(__name__)

	bpy.types.INFO_MT_file_export.remove(menu_func)


if __name__ == "__main__":
	register()
