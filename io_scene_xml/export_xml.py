import bpy #for blender modules
import math
import mathutils #for math stuff 
import bmesh
import os
from math import radians, pi #just for pi, but radians in case it's needed later

Matrix = mathutils.Matrix
Vector = mathutils.Vector
Euler = mathutils.Euler
Quaternion = mathutils.Quaternion

class XMLExporter: #the Exporter referenced in __init__.py
	def __init__(self, Config, Context): #constructor called in __init__.py
		self.Config = Config #gives all the settings set by the user on the export config
		self.Context = Context #the blend file, essentially treat is as data
		self.ExportMatrix = Matrix()
		self.frameBeforeExport = Context.scene.frame_current
		self.activeBeforeExport = Context.active_object
		self.Context.scene.frame_set(0)
		self.modeBeforeExport = self.Context.scene.objects.active.mode
		self.selectedBeforeExport = self.Context.selected_objects
		bpy.ops.object.mode_set(mode='OBJECT')#
		bpy.ops.object.select_all(action='DESELECT')#
		Context.scene.objects.active = None#
	
	def Export(self): #export function called in __init__.py, writes to the XML File
		if(self.Config.CoordinateSystem == 'LEFT_HANDED'):
			self.ExportMatrix *= Matrix.Scale(-1, 4, Vector((1, 0, 0)))
		if(self.Config.UpAxis == 'Y'):
			self.ExportMatrix *= Matrix.Rotation(radians(-90), 4, 'X')
			
		Objects = self.Context.scene.objects
		
		for object in Objects:
			if(object.type == 'MESH'):
				ExportMesh(object, "%s_%s.xml" % (self.Config.filepath[:-4], object.name), self.Config.ExportTangents, self.ExportMatrix, self.Context.scene)
			if(object.type == 'ARMATURE'):
				ExportArmature(object, "%s_%s.xml" % (self.Config.filepath[:-4], object.name), self.ExportMatrix, self.Context.scene)
		
		self.Context.scene.frame_set(self.frameBeforeExport)
		for object in self.selectedBeforeExport:
			object.select = True
		self.Context.scene.objects.active = self.activeBeforeExport
		bpy.ops.object.mode_set(mode=self.modeBeforeExport)
		self.Context.scene.update()

def ExportMesh(mesh, filepath, exporttangents, matrix, scene_context):
	scene_context.objects.active = mesh
	mesh.select = True
	bpy.ops.object.mode_set(mode='OBJECT')
	vertices = []
	UVs = []
	normals = []
	tangents = []
	#set of indices that point to the elements of the other lists
	vertex_indices = []
	vertex_weights = []
	weight_indices = []
	
	if(mesh.data.uv_layers.active is None):
		bpy.ops.object.editmode_toggle()
		bpy.ops.uv.smart_project()
		bpy.ops.object.editmode_toggle()
	
	tempMesh = mesh.to_mesh(scene_context, True, 'PREVIEW')
	bm = bmesh.new()
	bm.from_mesh(tempMesh)
	
	bm.transform(matrix) #sets mesh to correct space
	bmesh.ops.triangulate(bm, faces=bm.faces) #triangulates mesh
	
	bm.to_mesh(tempMesh)
	bm.free()
	del bm
	
	tempMesh.calc_normals_split()
	
	if(exporttangents):
		tempMesh.calc_tangents()
	
	for i, vertex in enumerate(tempMesh.vertices):
		vertices.append(vertex.co)
		vertex_weight = []
		weight_index = []
		totalWeight = 0.0
		
		for group in vertex.groups:
			totalWeight += group.weight
		for group in vertex.groups:
			vertex_weight.append(group.weight/totalWeight)
			poseBoneIndex = mesh.parent.pose.bones.find(mesh.vertex_groups[group.group].name)
			weight_index.append(poseBoneIndex)
		if(len(vertex.groups) > 4):
			nIndex = vertex_weight.index(min(vertex_weight))
			del vertex_weight[nIndex]
			del weight_index[nIndex]
			totalWeight = 0.0
			for weight in vertex_weight:
				totalWeight += weight
			for weight in vertex_weight:
				weight /= totalWeight
		vertex_weights.append(vertex_weight)
		weight_indices.append(weight_index)
	
	for polygon in tempMesh.polygons:
		# indices of values in the current polygon
		face_vertices = []
		face_UVs = []
		face_normals = []
		face_tangents = []
		face_bitangent_signs = []
		
		for loop_index in range(polygon.loop_start, polygon.loop_start + polygon.loop_total):
			face_vertices.append(tempMesh.loops[loop_index].vertex_index)
			UV = tempMesh.uv_layers.active.data[loop_index].uv
			if(UV not in UVs):
				UVs.append(UV)
			face_UVs.append(UVs.index(UV))
			normal = tempMesh.loops[loop_index].normal
			if(normal not in normals):
				normals.append(normal)
			face_normals.append(normals.index(normal))
			if(exporttangents):
				tangent = tempMesh.loops[loop_index].tangent
				bitangent_sign = tempMesh.loops[loop_index].bitangent_sign
				if(tangent not in tangents):
					tangents.append(tangent)
				face_tangents.append(tangents.index(tangent))
				face_bitangent_signs.append(bitangent_sign)
		
		for i in range(0,3):
			vertex_index = [face_vertices[i], face_UVs[i], face_normals[i]]
			if(exporttangents):
				vertex_index.append(face_tangents[i])
				vertex_index.append(face_bitangent_signs[i])
			vertex_indices.append(vertex_index)
	
	MeshFile = File(filepath)
	MeshFile.Open()
	
	MeshFile.Write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")
	MeshFile.Write("<mesh id=\"%s\">" % (mesh.name))
	MeshFile.Indent()
	
	WriteVertexAttributes(MeshFile, "vertices", vertices)
	
	MeshFile.Write("<weights count=\"%d\">" % (len(vertex_weights)))
	MeshFile.Indent()
	
	for element in vertex_weights:
		string = ""
		for sub_element in element:
			string += ("%.6f, " % (sub_element))
		MeshFile.Write(("%d, " % (len(element))) + string[:-2] + ";")
	
	MeshFile.Unindent()
	MeshFile.Write("</weights>")
	
	MeshFile.Write("<weight_indices count=\"%d\">" % (len(weight_indices)))
	MeshFile.Indent()
	
	for element in weight_indices:
		string = ""
		for sub_element in element:
			string += ("%d/" % (sub_element))
		MeshFile.Write(("%d, " % (len(element))) + string[:-1] + ";")
	
	MeshFile.Unindent()
	MeshFile.Write("</weight_indices>")
	
	WriteVertexAttributes(MeshFile, "UVs", UVs)
	normals.reverse()
	WriteVertexAttributes(MeshFile, "normals", normals)
	if(exporttangents):
		WriteVertexAttributes(MeshFile, "tangents", tangents)
	
	#Write vertex indices
	MeshFile.Write("<faces count=\"%d\">" % (len(vertex_indices)))
	MeshFile.Indent()
	
	for index in vertex_indices:
		string = ""
		for element in index:
			string += ("%d/" % (element))
		MeshFile.Write(string[:-1] + ";")
	
	MeshFile.Unindent()
	MeshFile.Write("</faces>")
	
	MeshFile.Unindent()
	MeshFile.Write("</mesh>")
	
	MeshFile.Close()
	
	del tempMesh
	
	bpy.ops.object.mode_set(mode='OBJECT')
	mesh.select = False
	scene_context.objects.active = None
	
def WriteVertexAttributes(file, element_type, elements):
	file.Write("<%s count=\"%d\">" % (element_type, len(elements)))
	file.Indent()
	
	for element in elements:
		string = ""
		for sub_element in element:
			string += ("%.6f, " % (sub_element))
		file.Write(string[:-2] + ";")
	
	file.Unindent()
	file.Write("</%s>" % (element_type))
	
def ExportArmature(armature, filepath, matrix, scene_context):
	scene_context.objects.active = armature
	armature.select = True
	bpy.ops.object.mode_set(mode='OBJECT')
	armature.matrix_world = armature.matrix_world * matrix
	bpy.ops.object.transform_apply(location=False, rotation=True, scale=True)
	
	ArmatureFile = File(filepath)
	ArmatureFile.Open()
	
	ArmatureFile.Write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")
	ArmatureFile.Write("<armature id=\"%s\">" % (armature.name))
	ArmatureFile.Indent()
	
	for bone in armature.data.bones:
		ArmatureFile.Write("<bone id=\"%s\">" % (bone.name))
		ArmatureFile.Indent()
		
		string = ""
		for element in (bone.matrix_local).to_translation():
			string += ("%.6f, " % (element))
		ArmatureFile.Write(string[:-2] + ";")
		
		ArmatureFile.Write("%d;" % (armature.pose.bones.find(bone.parent.name) if bone.parent else -1))
		
		ArmatureFile.Unindent()
		ArmatureFile.Write("</bone>")
	
	ArmatureFile.Unindent()
	ArmatureFile.Write("</armature>")
	
	armature.matrix_world = armature.matrix_world * matrix.inverted()
	bpy.ops.object.transform_apply(location=False, rotation=True, scale=True)
	
	ArmatureFile.Write("<animation count=\"%d\">" % (scene_context.frame_end - scene_context.frame_start + 1))
	ArmatureFile.Indent()
		
	bpy.ops.object.mode_set(mode='POSE')
	
	for bone in armature.pose.bones:
		ArmatureFile.Write("<bone id=\"%s\">" % (bone.name))
		ArmatureFile.Indent()
		
		for frame in range(scene_context.frame_start, scene_context.frame_end + 1):
			scene_context.frame_set(frame)
			
			final_rot_matrix = None
			local_matrix = bone.matrix_channel
			if bone.parent is None:
				final_rot_matrix = local_matrix
			else:
				final_rot_matrix = bone.parent.matrix_channel.inverted() * local_matrix
				
			final_rotation = final_rot_matrix.to_quaternion()
			
			switchZ = -final_rotation.y * 1
			switchY = -final_rotation.z * 1
			final_rotation.y = switchY
			final_rotation.z = switchZ
			
			final_rotation.normalize()
			
			final_tranlation = Vector((0.0, 0.0, 0.0))
			
			final_tranlation += bone.location.x * bone.x_axis
			final_tranlation += bone.location.y * bone.y_axis
			final_tranlation += bone.location.z * bone.z_axis
			
			switchY = final_tranlation.z * 1
			switchZ = final_tranlation.y * 1
			final_tranlation.x = -final_tranlation.x
			final_tranlation.y = switchY
			final_tranlation.z = switchZ
			
			final_scale = final_rot_matrix.to_scale()
			
			switchY = final_scale.z * 1
			switchZ = final_scale.y * 1
			final_scale.y = switchY
			final_scale.z = switchZ
			
			string = ""
			for element in final_tranlation:
				string += ("%.5f, " % (element))
			ArmatureFile.Write(string[:-2] + ";")
			
			string = ""
			for element in final_rotation:
				string += ("%.4f, " % (element))
			ArmatureFile.Write(string[:-2] + ";")
			
			string = ""
			for element in final_scale:
				string += ("%.5f, " % (element))
			ArmatureFile.Write(string[:-2] + ";")
		
		ArmatureFile.Unindent()
		ArmatureFile.Write("</bone>")
		
	ArmatureFile.Unindent()
	ArmatureFile.Write("</animation>")
	
	ArmatureFile.Close()
	
	bpy.ops.object.mode_set(mode='OBJECT')
	armature.select = False
	scene_context.objects.active = None
	scene_context.frame_set(0)
		
class File:
	def __init__(self, Filepath):
		self.filepath = Filepath
		self.File = None
		self.IndentSize = 0
		
	def Open(self):
		if not self.File:
			self.File = open(self.filepath, 'w')
	
	def Close(self):
		self.File.close()
		self.File = None
	
	def Indent(self):
		self.IndentSize += 1
		
	def Unindent(self):
		self.IndentSize -= 1
		
	def Write(self, string):
		self.File.write("%s%s\n" % ("\t" * self.IndentSize, string))