import bpy #for blender modules
import math
import mathutils #for math stuff 
import bmesh
import os
from math import radians, pi #just for pi, but radians in case it's needed later

Matrix = mathutils.Matrix
Vector = mathutils.Vector
Euler = mathutils.Euler

class XMLExporter: #the Exporter referenced in __init__.py
	def __init__(self, Config, Context): #constructor called in __init__.py
		self.Config = Config #gives all the settings set by the user on the export config
		self.Context = Context #the blend file, essentially treat is as data
		self.File = File(self.Config.filepath) #file to be written to
		self.File.Open()
		self.ExportList = [] #list of objects to exported
		self.ExportMatrix = Matrix()
		self.frames_per_second = Context.scene.render.fps
		self.frameBeforeExport = Context.scene.frame_current
		self.Context.scene.frame_set(0)
		self.GenerateExportList()

	def GenerateExportList(self): #fills ExportList with
		if(self.Config.CoordinateSystem == 'LEFT_HANDED'):
			self.ExportMatrix *= Matrix.Scale(-1, 4, Vector((0, 0, 1)))
		if(self.Config.UpAxis == 'Y'):
			self.ExportMatrix *= Matrix.Rotation(radians(-90), 4, 'X')
			
		Objects = None
		if(self.Config.SelectedOnly):
			Objects = self.Context.selected_objects
		else:
			Objects = self.Context.scene.objects
			
		for object in Objects:
			#any of these need mesh data
			condition = [self.Config.ExportVertices, 
				self.Config.ExportNormals, 
				self.Config.ExportUVCoordinates, 
				self.Config.ExportTangents, 
				self.Config.ExportSkinWeights]
			if any(condition):
				if(object.type == 'MESH'):
					mesh = ExporterMesh(self, object)
					self.ExportList.append(mesh)
					
			if(self.Config.ExportArmatureBones):
				if(object.type == 'ARMATURE'):
					armature = ExporterArmature(self, object)
					self.ExportList.append(armature)
					
	def Export(self): #export function called in __init__.py, writes to the XML File
		self.File.Write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")
		self.File.Write("<scene>")
		self.File.Indent()
		if(self.Config.IncludeFrameRate):
			self.File.Write("<FPS>%f</FPS>" % (self.frames_per_second))
		mat_strings = convert_matrix_to_strings(self.ExportMatrix, "Space", "Space meant for export, "\
			"all vertices were exported in Blender's native space, multiply by this space to properly convert", 
			self.Config.CoordinateSystem, self.Config.UpAxis)
		for string in mat_strings:
			self.File.Write(string)
			
		for object in self.ExportList:
			self.File.WriteObject(object)
		
		self.File.Unindent()
		self.File.Write("</scene>")
		self.File.close()
		self.Context.scene.frame_set(self.frameBeforeExport)
		
class ExporterObject:
	def __init__(self, Exporter, object):
		#first six are all properties in Exporter Objects
		self.name = object.name
		self.type = object.type.lower()
		self.world_matrix = object.matrix_world * 1
		#self.animations = [] #set of animations, animation class
		self.local_matrix = None
		self.parent_name = ""
		if(object.parent):
			self.local_matrix = object.matrix_local * 1
			self.parent_name = object.parent.name
			
		#get Animations
		#Exporting default animations
		#if(Exporter.Config.ExportDefaultAnimations):
		#	animation = [] #set of animation_frames
		#	old_transform = None
		#	identity = Matrix() #identity matrix
		#	#iterate through each frame in the scene
		#	for frame in range(Exporter.Context.scene.frame_start, Exporter.Context.scene.frame_end + 1):
		#		Exporter.Context.scene.frame_set(frame)
		#		if(old_transform is not None):
		#			new_transform = old_transform.inverted() * object.matrix_world
		#			if(new_transform != identity):
		#				animation_frame = AnimationFrame(frame, new_transform)
		#				animation.append(animation_frame)
		#			old_transform = object.matrix_world * 1
		#		else:
		#			old_transform = object.matrix_world * 1
		#			animation_frame = AnimationFrame(frame, Matrix())
		#			animation.append(animation_frame)
		#			
		#	if(len(animation) > 1):
		#		nAnimation = ExporterAnimation(object.name, animation, object.name + "_anim", "default")
		#		self.animations.append(nAnimation)
			
		#these are all for default object animations
		#if(object.animation_data is not None):
		#	#checking if track list is not empty
		#	tracks_to_unmute_later = []
		#	if(len(object.animation_data.nla_tracks)):
		#		print(object.name)
		#		#saving nla stack settings before evaluation
		#		for track in object.animation_data.nla_tracks:
		#			if(track.mute == False):
		#				track.mute = True
		#				tracks_to_unmute_later.append(track)
		#				
		#		#Exporting NlaTracks
		#		if(Exporter.Config.ExportNLATracks):
		#			#the evaluation
		#			for track in object.animation_data.nla_tracks:
		#				track.mute = False
		#				
		#				if(len(track.strips)):
		#					animation = [] #set of animation_frames
		#					old_transform = None
		#					identity = Matrix() #identity matrix
		#					#iterate through each frame in the track
		#					for frame in range(int(track.strips[0].frame_start), int(track.strips[len(track.strips) - 1].frame_end)):
		#						Exporter.Context.scene.frame_set(frame)
		#						if(old_transform is not None):
		#							new_transform = old_transform.inverted() * object.matrix_world
		#							if(new_transform != identity):
		#								animation_frame = AnimationFrame(frame, new_transform)
		#								animation.append(animation_frame)
		#							old_transform = object.matrix_world * 1
		#						else:
		#							old_transform = object.matrix_world * 1
		#							animation_frame = AnimationFrame(frame, Matrix())
		#							animation.append(animation_frame)
		#					
		#					if(len(animation) > 1):
		#						nAnimation = ExporterAnimation(object.name, animation, track.name, "nla_track")
		#						self.animations.append(nAnimation)
		#				
		#				track.mute = True
						
			#Exporting Actions, not final
			#if(Exporter.Config.ExportActions):
			#	if(object.animation_data.action is not None):
			#		old_nla_setting = object.animation_data.use_nla * 1
			#		object.animation_data.use_nla = False
			#		
			#		animation = [] #set of animation_frames
			#		Exporter.Context.scene.frame_set(object.animation_data.action.frame_range.x)
			#		old_transform = None
			#		identity = Matrix() #identity matrix
			#		#iterate through each frame in the action
			#		for frame in range(int(object.animation_data.action.frame_range.x), int(object.animation_data.action.frame_range.y)):
			#			Exporter.Context.scene.frame_set(frame)
			#			if(old_transform is not None):
			#				new_transform = old_transform.inverted() * object.matrix_world
			#				if(new_transform != identity):
			#					animation_frame = AnimationFrame(frame, new_transform)
			#					animation.append(animation_frame)
			#				old_transform = object.matrix_world * 1
			#			else:
			#				old_transform = object.matrix_world * 1
			#				animation_frame = AnimationFrame(frame, Matrix())
			#				animation.append(animation_frame)
			#				
			#		if(len(animation) > 1):
			#			nAnimation = ExporterAnimation(object.name, animation, object.animation_data.action.name, "action")
			#			self.animations.append(nAnimation)
			#			
			#		object.animation_data.use_nla = old_nla_setting
					
			#resetting nla stack settings after evaluation
			#for track in tracks_to_unmute_later:
			#	track.mute = False	
				
			Exporter.Context.scene.frame_set(Exporter.frameBeforeExport)

	def to_strings(self): #never call to print an object on its own, only call versions in child classes
		strings = []
	
		#header
		strings.append("<%s id=\"%s\">" % (self.type, self.name))

		##world_matrix
		#mat_strings = convert_matrix_to_strings(self.world_matrix, "world", "matrix is column major")
		#for string in mat_strings:
		#	strings.append("\t%s" % (string))
		#	
		##local matrix
		#if(self.local_matrix is not None):
		#	locmat_strings = convert_matrix_to_strings(self.local_matrix, "local", "matrix is column major", "", "", self.parent_name)
		#	for string in locmat_strings:
		#		strings.append("\t%s" % (string))
				
		return strings
		
	#def animation_to_strings(self):
	#	strings = []
	#	
	#	#animations
	#	if(len(self.animations) > 0):
	#		strings.append("\t<!--multiply non-pose matrices to the object's world matrix, as in apply it before the shader rather than passing it into the shader-->")
	#	for animation in self.animations:
	#		anim_strings = animation.to_strings()
	#		for string in anim_strings:
	#			strings.append("\t%s" % (string))
	#			
	#	return strings
			
class ExporterMesh(ExporterObject): #Mesh to be Exported
	def __init__(self, Exporter, object): #this class, the xml Exporter, the mesh object
		ExporterObject.__init__(self, Exporter, object)
		
		self.faces = [] #list of ExporterFace(s) referencing indices of the other lists here
		self.vertices = [] #lists elements of the mesh, can be a list of normal vertices or ExporterVertex(es)
		self.uvs = []
		self.normals = []
		self.tangents = []
		self.vertex_groups = []
		self.weights = [] #weight values
		self.weight_indices = [] #index to weight values in weights aligned with vertices

		for vertex_group in object.vertex_groups:
			self.vertex_groups.append(vertex_group.name)

		#weights and vertices aren't affected even if the mesh is triangulated, so they go first, triangulation is still needed for face indices
		for vertex in object.data.vertices:
			# Config check
			if(Exporter.Config.ExportSkinWeights and len(object.vertex_groups)):
				#for getting vertex weights
				vertex_weights = []
				totalweight = 0.0
				
				#for calculating total weight
				for group in vertex.groups:
					totalweight += group.weight
					
				#for adjusted weight
				for group in object.vertex_groups:
					weight = 0.0
					for group_element in vertex.groups:
						if(group.index == group_element.group):
							weight = group_element.weight / totalweight
					
					if(weight not in self.weights):
						self.weights.append(weight)
						
					vertex_weights.append(self.weights.index(weight))
				
				self.weight_indices.append(vertex_weights)
			
			if(Exporter.Config.ExportVertices):
				self.vertices.append(vertex.co)

		#setting mesh up to copied and applied modifiers to, or just triangulate
		Deactivated_Modifiers = []
		triangulated = False
		for modifier in object.modifiers:
			if(modifier.type == "TRIANGULATE"):
				triangulated = True
			
			if(modifier.type == "ARMATURE" and modifier.show_viewport and (Exporter.Config.ExportSkinWeights or Exporter.Config.ExportArmatureBones)):
				Deactivated_Modifiers.append(modifier)
				modifier.show_viewport = False
			elif not (Exporter.Config.ApplyModifiers):
				Deactivated_Modifiers.append(modifier)
				modifier.show_viewport = False
			
		triangulate_modifier = None
		if(triangulated == False):
			triangulate_modifier = object.modifiers.new("tris", "TRIANGULATE")
			triangulate_modifier.quad_method = "BEAUTY"

		#tempMesh to apply modifiers to
		tempMesh = object.to_mesh(Exporter.Context.scene, True, 'PREVIEW') # have apply modifier settings in config
		
		#resetting Mesh Object
		if(triangulated == False):
			object.modifiers.remove(triangulate_modifier)
			
		for modifier in Deactivated_Modifiers:
			modifier.show_viewport = True

		#iterating through temporary mesh and storing data
		for polygon in tempMesh.polygons:
			#data to initialize an ExporterFace class object
			vertices = []
			UVs = []
			normals = []
			tangents = []
			bitangent_signs = []
			for loop_index in range(polygon.loop_start, polygon.loop_start + polygon.loop_total):
				#config check
				if(Exporter.Config.ExportVertices):
					vertices.append(tempMesh.loops[loop_index].vertex_index)
				#config check and checks if there's actually a uv layer
				if(Exporter.Config.ExportUVCoordinates and object.data.uv_layers.active):
					#storing for uv coordinates, assigns uv of loop_index to texCoord
					UV = tempMesh.uv_layers.active.data[loop_index].uv
					#for checking if the texCoord is one the loop hasn't passed through before
					if(UV not in self.uvs):
					#adds the texCoord to the texCoordArray if it isn't already there
						self.uvs.append(UV)
					uvIndex = self.uvs.index(UV)
					#adds the index of the latest texCoord to texCoords list
					UVs.append(uvIndex)
				if(Exporter.Config.ExportNormals):
					#calculating normals for sharp edges as well as smooth ones 
					tempMesh.calc_normals_split()
					normal = tempMesh.loops[loop_index].normal
					if(normal not in self.normals):
						self.normals.append(normal)
					normalIndex = self.normals.index(normal)
					normals.append(normalIndex)
				if(Exporter.Config.ExportTangents and object.data.uv_layers.active):
					#calculates a consistent perpendicular tangent to normal vector
					if not Exporter.Config.ExportNormals:
						tempMesh.calc_normals_split()
					tempMesh.calc_tangents()
					tangent = tempMesh.loops[loop_index].tangent
					bitangent_sign = tempMesh.loops[loop_index].bitangent_sign
					if(tangent not in self.tangents):
						self.tangents.append(tangent)
					tangentIndex = self.tangents.index(tangent)
					tangents.append(tangentIndex)
					bitangent_signs.append(bitangent_sign)

			face = ExporterFace(vertices, UVs, normals, tangents, bitangent_signs)
			self.faces.append(face)
		
		del tempMesh
		
	def to_strings(self): #this is in all exporter objects #returns an array of strings, each element representing one line
		strings = [] #resulting string array
		
		#calling ExporterObject's to_string
		obj_strings = ExporterObject.to_strings(self)
		for string in obj_strings:
			strings.append(string) #strings should already be indented

		#vertices
		vert_strings = convert_attribute_to_strings("vertices", "floats", self.vertices)
		for string in vert_strings:
			strings.append("\t%s" % (string))

		#vertex_groups
		vg_strings = convert_attribute_to_strings("vertex_groups", "string", self.vertex_groups)
		for string in vg_strings:
			strings.append("\t%s" % (string))

		#weights
		weight_strings = convert_attribute_to_strings("weights", "single", self.weights)
		for string in weight_strings:
			strings.append("\t%s" % (string))

		#weight_indices
		weight_index_strings = convert_attribute_to_strings("weights_indices", "indices", self.weight_indices, "aligned vertically with vertices and horizontally with vertex_groups", 1)
		for string in weight_index_strings:
			strings.append("\t%s" % (string))

		#UVs
		uv_strings = convert_attribute_to_strings("UVs", "floats", self.uvs)
		for string in uv_strings:
			strings.append("\t%s" % (string))

		#normals
		normal_strings = convert_attribute_to_strings("normals", "floats", self.normals)
		for string in normal_strings:
			strings.append("\t%s" % (string))

		#tangents
		tangent_strings = convert_attribute_to_strings("tangents", "floats", self.tangents)
		for string in tangent_strings:
			strings.append("\t%s" % (string))

		#faces
		face_strings = convert_attribute_to_strings("faces", "indices", self.faces, "indices order: vertex/UV/normal/tangent/bitangent_sign, na means none, either not available or applicable, such as if not exported", 3)
		for string in face_strings:
			strings.append("\t%s" % (string))
			
		##calling animation_to_strings
		#anim_strings = ExporterObject.animation_to_strings(self)
		#for string in anim_strings:
		#	strings.append(string) #strings should already be indented
			
		#footer, it's two lines, just add it at the end of the to_string of each child class
		strings.append("</mesh>")
		return strings
	
class ExporterFace: #class specifically for the ExporterMesh, nested classes don't work
	def __init__(self, vertices, UVs, normals, tangents, bitangent_signs):
		self.vertices = vertices #indices of vertices
		self.UVs = UVs #indices of texCoords
		#the following three make up the tangent space for calculating normal maps
		self.normals = normals #indices of normals
		self.tangents = tangents #indices of tangent vectors
		#Note: bitangent = bitangent_sign * cross(normal, tangent)
		self.bitangent_signs = bitangent_signs #indices of bitangent_signs
		self.indices = [self.vertices, self.UVs, self.normals, self.tangents, self.bitangent_signs]

class ExporterArmature(ExporterObject): #Armature to be Exported
	def __init__(self, Exporter, object): #this class, the xml Exporter, the mesh object
		ExporterObject.__init__(self, Exporter, object)
		
		#self.bones = [] #defining feature of an armature
		#self.bone_names = []
		self.rest_poses = [] #list of pose class objects representing the rest pose of each bone
		self.poses = [] #list of quaternions representing animations per frame, relative to rest quaternion
		
		#for bone in object.data.bones:
		#	self.bones.append(bone.matrix_local)
		#	self.bone_names.append(bone.name)
			
		pose_position_before_export = object.data.pose_position * 1
		if(Exporter.Config.ExportRestPose):
			object.data.pose_position = 'REST'
			for bone in object.data.bones:
				matrix = Matrix.Rotation(-pi/2, 4, 'X') * bone.matrix_local
				#nPose = BonePose(bone.head_local, matrix.to_quaternion(), bone.name)
				nPose = BonePose(matrix.to_translation(), matrix.to_quaternion(), bone.name)
				self.rest_poses.append(nPose)
		
		if(Exporter.Config.ExportPoses and (object.animation_data is not None) and (object.pose_library is not None)):
			object.data.pose_position = 'POSE'
			action_before_export = None
			if(object.animation_data):
				if(object.animation_data.action):
					action_before_export = object.animation_data.action.name * 1
			object.animation_data.action = object.pose_library
			
			for i, pose_marker in enumerate(object.pose_library.pose_markers):
				end_frame = 0
				
				if(i != len(object.pose_library.pose_markers) - 1):
					end_frame = object.pose_library.pose_markers[i + 1].frame
				else:
					end_frame = int(object.pose_library.frame_range.y) + 1
					
				for poseBone in object.pose.bones:
					animation = []
					#from this pose_marker frame to next pose marker's frame
					#bones, btw are in opengl's normal coordinate system different from the rest of blender's coordinate system
					old_matrix = None
					for frame in range(pose_marker.frame, end_frame):
						Exporter.Context.scene.frame_set(frame)
						if(old_matrix != poseBone.matrix):
							matrix = Matrix.Rotation(-pi/2, 4, 'X') * poseBone.matrix #offsets have to be relative to armature
							rmatrix = (Matrix.Rotation(-pi/2, 4, 'X') * poseBone.bone.matrix_local).inverted() * matrix; #rotation quaternions have to be relative to rest quaternion
							#animation_frame = AnimationFrame(frame - pose_marker.frame + 1, matrix.to_quaternion(), poseBone.head - poseBone.bone.head_local) #just testing right now
							animation_frame = AnimationFrame(frame - pose_marker.frame + 1, rmatrix.to_quaternion().normalized(), matrix.to_translation()) #- (Matrix.Rotation(-pi/2, 4, 'X') * poseBone.bone.matrix_local).to_translation())
							animation.append(animation_frame)
							old_matrix = poseBone.matrix * 1
						
					if(len(animation) > 0):
						nAnimation = ExporterAnimation(poseBone.name, animation, pose_marker.name, "pose")
						self.poses.append(nAnimation)
									
			if(action_before_export):
				object.animation_data.action = bpy.data.actions[action_before_export]
		object.data.pose_position = pose_position_before_export
		Exporter.Context.scene.frame_set(0)
		
	def to_strings(self):
		strings = [] #resulting string array
		
		#calling ExporterObject's to_string
		obj_strings = ExporterObject.to_strings(self)
		for string in obj_strings:
			strings.append(string) #strings should already be indented
			
		#bone matrices
		#strings.append("\t<!--these matrices are relative to the armature-->")
		#for bone, name in zip(self.bones, self.bone_names):
		#	bone_strings = convert_matrix_to_strings(bone, "bone", "", "", "", name)
		#	for string in bone_strings:
		#		strings.append("\t%s" % (string))
				
		#rest positions
		#strings.append("\t<rest_pose>")
		#for rest_pose, name in zip(self.rest_poses, self.bone_names):
		#	rest_matrices = convert_matrix_to_strings(rest_pose, "bone", "", "", "", name)
		#	for string in rest_matrices:
		#		strings.append("\t\t%s" % (string))
		#strings.append("\t</rest_pose>")
		
		strings.append("\t<!--quaternions and offsets are set in opengGL's native format, no need to multiply by space matrix-->")
		
		for pose in self.rest_poses:
			pose_strings = pose.to_strings()
			for string in pose_strings:
				strings.append("\t%s" % (string))
			
		
		##calling animation_to_strings
		#strings.append("\t<!--pose matrices are to be applied during the shader phase, they're the pose.bones.matrix_basis(es) in 2.75 version of blender,")
		#strings.append("\t    unless you're doing ragdoll physics, in that case apply inverse_of_matrix_in_frame_before * matrix_of_current_frame to the")
		#strings.append("\t    ragdoll components before sending to shader, but same rule still applies in the shader, apply the matrix as how you would")
		#strings.append("\t    even without ragdolls-->")
		#anim_strings = ExporterObject.animation_to_strings(self)
		#for string in anim_strings:
		#	strings.append(string) #strings should already be indented
		
		for pose in self.poses:
			pose_strings = pose.to_strings()
			for string in pose_strings:
				strings.append("\t%s" % (string))
			
		#footer, it's two lines, just add it at the end of the to_string of each child class
		strings.append("</armature>")
		return strings
		
class BonePose:
	def __init__(self, offset, rotation, boneName):
		self.name = boneName
		self.offset = offset #offset is vec3 to be printed as a vec4 for consistency
		self.rotation = rotation
		
	def to_strings(self):
		strings = []
		strings.append("<rest_pose bone=\"%s\">" % (self.name))
		strings.append("\t%.6f, %.6f, %.6f, %.6f;" % (self.offset.x, self.offset.y, self.offset.z, 0))
		strings.append("\t%.6f, %.6f, %.6f, %.6f;" % (self.rotation.w, self.rotation.x, self.rotation.y, self.rotation.z))
		strings.append("</rest_pose>")
		return strings
		
class AnimationFrame:
	def __init__(self, frame, transform_quat, offset):
		self.frame = frame
		self.transform = transform_quat
		self.offset = offset
		
	def to_strings(self):
		strings = []
		strings.append("<pose_frame frame=\"%d\">" % (self.frame))
		strings.append("\t%.6f, %.6f, %.6f, %.6f;" % (self.offset.x, self.offset.y, self.offset.z, 0))
		strings.append("\t%.6f, %.6f, %.6f, %.6f;" % (self.transform.w, self.transform.x, self.transform.y, self.transform.z))
		strings.append("</pose_frame>")
		return strings
		
class ExporterAnimation:
	#parameters: 	name of object, animation is for
	#				set of animation_frames
	#				name of the action/nla_track/pose, name of default would just be the object name + "_anim"
	#				type of animation:	action, nla_track, pose, default
	def __init__(self, object_name, animation_frames, name, type): 
		self.name = name
		self.type = type
		self.object = object_name
		self.frames = animation_frames
		
	def to_strings(self):
		strings = []
		
		#header
		strings.append("<animation type=\"%s\" name=\"%s\" object=\"%s\">" % (self.type, self.name, self.object))
		
		#frame matrices
		for frame in self.frames:
			#frame_strings = convert_matrix_to_strings(frame, "animation_frame")
			#for string in frame_strings:
			#	strings.append("\t%s" % (string))
			frame_strings = frame.to_strings()
			for string in frame_strings:
				strings.append("\t%s" % (string))
		
		#footer
		strings.append("</animation>")
		return strings
		
class File:
	def __init__(self, Filepath):
		self.filepath = Filepath
		print(Filepath)
		self.File = None
		self.IndentSize = 0
		
	def Open(self):
		if not self.File:
			self.File = open(self.filepath, 'w')
	
	def close(self):
		self.File.close()
		self.File = None
	
	def Indent(self):
		self.IndentSize += 1
		
	def Unindent(self):
		self.IndentSize -= 1
		
	def Write(self, string):
		self.File.write("%s%s\n" % ("\t" * self.IndentSize, string))
		
	def WriteObject(self, object):
		strings = object.to_strings()
		for string in strings:
			self.Write(string)
			
def convert_matrix_to_strings(matrix, type, notes="", coordinateSystem="", upAxis="", name=""): #matrix itself and type aka world, bone, transform, space and any notes, last two are only if space matrix, name only if bone
	strings = []
	strings.append("<matrix type=\"%s\"" % (type))
	if(name != ""):
		strings[0] += (" name=\"%s\"" % (name))
	if(coordinateSystem != ""):
		strings[0] += (" coordinateSystem=\"%s\" upAxis=\"%s\"" % (coordinateSystem, upAxis))
	if(type == "animation_frame"):
		strings[0] += (" frame=\"%d\"" % (matrix.frame))
	strings[0] += ">"
	if(notes != ""):
		strings.append("<!--%s-->" % (notes))
	for i in range(0, 4):
		floats = []
		for j in range(0, 4):
			if(type == "animation_frame"):
				floats.append(matrix.transform[i][j])
			else:
				floats.append(matrix[j][i])
		strings.append("\t%.6f, %.6f, %.6f, %.6f;" % (floats[0], floats[1], floats[2], floats[3]))
	strings[len(strings) - 1] += ";"
	strings.append("</matrix>")
	return strings

#turns an attribute array, like a vertices array into strings
#example convert_attribute_to_strings("vertices", "floats", self.vertices)
#indices would require elementSize
#exmaple convert_attribute_to_strings("faces", "indices", self.faces, "", 3) 3 is the number of vertices in a face in this case
def convert_attribute_to_strings(type, elements, array, notes="", elementSize=0):
	strings = []
	if(len(array)):
		strings.append("<%s count=\"%d\">" % (type, len(array)))
		if(notes != ""):
			strings.append("<!--%s-->" % (notes))
		if(elements == "floats"):
			float_strings = convert_floats_to_strings(array)
			for string in float_strings:
				strings.append("\t%s" % (string))
		if(elements == "indices"):
			index_strings = convert_indices_to_strings(array, elementSize)
			for string in index_strings:
				strings.append("\t%s" % (string))
		if(elements == "string"):
			for string in array:
				strings.append("\t%s;" % (string))
			if(len(strings)):
				strings[len(strings) - 1] += ";"
		if(elements == "single"):
			for float in array:
				strings.append("\t%.6f;" % (float))
			if(len(strings)):
				strings[len(strings) - 1] += ";"
		strings.append("</%s>" % (type))
	return strings

#turns an array of float vectors in an array of strings
def convert_floats_to_strings(array):
	strings = []
	for floats in array:
		string = ""
		for i, float in enumerate(floats):
			string += ("%.6f" % (float))
			if(i != (len(floats) - 1)):
				string += ", "
		string += ";"
		strings.append(string)
	if(len(strings)):
		strings[len(strings) - 1] += ";"
	return strings

def convert_indices_to_strings(array, size):
	strings = []
	for element in array:
		string = ""
		if(size > 1):
			'''
			for i in range(0, size):
				for j, indices in enumerate(element.indices):
					if(len(indices) == size):
						string += ("%d" % (indices[i]))
					else:
						string += "na"
					if(j != (len(element.indices) - 1)):
						string += "/"
				if(i != size - 1):
					string += " "
			'''
			#^v
			for i in range(0, size):
				for j, indices in enumerate(element.indices):
					if(len(indices) == size):
						string += ("%d" % (indices[i]))
					else:
						string += "na"
					if(j != (len(element.indices) - 1)):
						string += "/"
				if(i != size - 1):
					string += ", "
					strings.append(string * 1)
					string = ""
				else:
					string += ";"
					strings.append(string)
			#
		else:
			for i, index in enumerate(element):
				string += ("%d" % (index))
				if(i != (len(element) - 1)):
					string += "/"
			#
			string += ";"
			strings.append(string)
			#v^
		'''
		string += ";"
		strings.append(string)
		'''
	if(len(strings)):
		strings[len(strings) - 1] += ";"
	return strings
	
