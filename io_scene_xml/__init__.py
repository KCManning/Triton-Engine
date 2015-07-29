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

	SelectedOnly = BoolProperty(
		name="Export Selected Objects Only",
		description="Export only selected objects",
		default=False)

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

	ExportVertices = BoolProperty(
		name="Export Mesh Vertices",
		description="Export mesh vertices",
		default=True)

	ExportNormals = BoolProperty(
		name="Export Normals",
		description="Export mesh normals",
		default=True)

	ExportUVCoordinates = BoolProperty(
		name="Export UV Coordinates",
		description="Export mesh UV coordinates, if any",
		default=True)

	ExportTangents = BoolProperty(
		name="Export Tangent Space",
		description="Export normal tangents and bitangent_signs",
		default=False)

	ApplyModifiers = BoolProperty(
		name="Apply Modifiers",
		description="Apply the effects of object modifiers before export, armature modifiers will not export if skin weights or bones are exported",
		default=False)

	ExportSkinWeights = BoolProperty(
		name="Export Skin Weights",
		description="Bind mesh vertices to armature bones, will not export armature modifiers",
		default=True)

	ExportArmatureBones = BoolProperty(
		name="Export Armature Bones",
		description="Export armatures bones, will not export armature modifiers",
		default=True)

	ExportRestPose = BoolProperty(
		name="Export Rest Position",
		description="Export bones in their rest position works only if exporting Armature Bones too.",
		default=True)
		
	ExportPoses = BoolProperty(
		name="Export Poses",
		description="Cuts the active action in each bone into poses marked by pose_markers, "\
			"must export Armature Bones too to work, otherwise poses won't be exported "\
			"usually what you want to go with for multiple bone animations on one armature.",
		default=True)

	#ExportDefaultAnimations = BoolProperty(
	#	name="Export Default Animations",
	#	description="Export object animations. Exports the world matrix of each object relative to it's last matrix each frame, one animation per object, "\
	#		"you usually don't want to have this enabled",
	#	default=False)
		
	#not final
	#ExportActions = BoolProperty(
	#	name="Export Actions",
	#	description="Export each object's active action, while disabling their nla_tracks. doesn't support multiple actions on one"\
	#		" object",
	#	default=False)
		
	#ExportNLATracks = BoolProperty(
	#	name="Export NLA Tracks",
	#	description="Export Each NLA Track on an object as a solo animation set, use this to export multiple actions as animations of non-armature objects",
	#	default=False)

	IncludeFrameRate = BoolProperty(
		name="Include Frame Rate",
		description="Include the AnimTicksPerSecond template which is "\
			"used by some engines to control animation speed",
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
