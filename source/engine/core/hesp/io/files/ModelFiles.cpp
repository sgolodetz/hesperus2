/***
 * hesperus: ModelFiles.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ModelFiles.h"

#include <iostream>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>
#include <hesp/images/ImageLoader.h>
#include <hesp/io/util/DirectoryFinder.h>
#include <hesp/io/util/LineIO.h>
#include <hesp/materials/BasicMaterial.h>
#include <hesp/materials/TextureMaterial.h>
#include <hesp/math/matrices/RBTMatrix.h>
#include <hesp/models/Animation.h>
#include <hesp/models/Bone.h>
#include <hesp/models/BoneHierarchy.h>
#include <hesp/models/Mesh.h>
#include <hesp/models/Model.h>
#include <hesp/models/Pose.h>
#include <hesp/models/Skeleton.h>
#include <hesp/models/Submesh.h>
#include <hesp/textures/TextureFactory.h>
#include <hesp/xml/XMLParser.h>
namespace bf = boost::filesystem;

namespace {

//#################### CONSTANTS ####################
const double SCALE = 1.0/10;	// the models are built such that 10 units in Blender corresponds to 1 unit in the game

}

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads a set of materials from the specified Ogre materials file.

@param filename	The name of the file
@return			The set of materials as a std::map, indexed by name
*/
std::map<std::string,Material_Ptr> ModelFiles::load_materials(const std::string& filename)
{
	std::map<std::string,Material_Ptr> ret;

	std::ifstream is(filename.c_str(), std::ios_base::binary);
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	NamedMaterial_Ptr material;
	while(material = read_material(is))
	{
		ret.insert(std::make_pair(material->first, material->second));
	}

	return ret;
}

/**
Loads a mesh from the specified Ogre mesh file.

@param filename		The name of the file
@param materials	The set of materials referenced in the file
@return				The mesh
*/
Mesh_Ptr ModelFiles::load_mesh(const std::string& filename, const std::map<std::string,Material_Ptr>& materials)
try
{
	XMLLexer_Ptr lexer(new XMLLexer(filename));
	XMLParser parser(lexer);
	XMLElement_CPtr root = parser.parse();
	XMLElement_CPtr meshElt = root->find_unique_child("mesh");

	XMLElement_CPtr submeshesElt = meshElt->find_unique_child("submeshes");

	std::vector<XMLElement_CPtr> submeshElts = submeshesElt->find_children("submesh");
	int submeshCount = static_cast<int>(submeshElts.size());

	std::vector<Submesh_Ptr> submeshes;
	for(int i=0; i<submeshCount; ++i)
	{
		const XMLElement_CPtr& submeshElt = submeshElts[i];

		std::string materialName = submeshElt->attribute("material");

		// Lookup the material in the materials map. Use a default material and output an error if it's missing.
		Material_Ptr material;
		std::map<std::string,Material_Ptr>::const_iterator jt = materials.find(materialName);
		if(jt != materials.end())
		{
			material = jt->second;
		}
		else
		{
			material.reset(new BasicMaterial(Colour3d(1,1,1), Colour3d(1,1,1), Colour3d(1,1,1), 1, Colour3d(1,1,1), true));
			std::cerr << "Missing material: " << materialName << std::endl;
		}

		// Read in the vertex indices for the triangles in the mesh.
		XMLElement_CPtr facesElt = submeshElt->find_unique_child("faces");
		std::vector<XMLElement_CPtr> faceElts = facesElt->find_children("face");
		int faceCount = static_cast<int>(faceElts.size());

		std::vector<unsigned int> vertIndices;
		vertIndices.reserve(faceCount * 3);

		for(int j=0; j<faceCount; ++j)
		{
			const XMLElement_CPtr& faceElt = faceElts[j];
			unsigned int v1 = lexical_cast<unsigned int>(faceElt->attribute("v1"));
			unsigned int v2 = lexical_cast<unsigned int>(faceElt->attribute("v2"));
			unsigned int v3 = lexical_cast<unsigned int>(faceElt->attribute("v3"));
			vertIndices.push_back(v1);
			vertIndices.push_back(v2);
			vertIndices.push_back(v3);
		}

		// Read in the vertex positions and normals.
		XMLElement_CPtr geometryElt = submeshElt->find_unique_child("geometry");
		XMLElement_CPtr vertexbufferElt = geometryElt->find_unique_child("vertexbuffer");
		if(!vertexbufferElt->has_attribute("positions") || vertexbufferElt->attribute("positions") != "true" ||
		   !vertexbufferElt->has_attribute("normals") || vertexbufferElt->attribute("normals") != "true")
		{
			throw Exception("Both vertex positions and normals are required to be present - did you make sure to export them?");
		}

		std::vector<XMLElement_CPtr> vertexElts = vertexbufferElt->find_children("vertex");
		int vertCount = static_cast<int>(vertexElts.size());

		std::vector<ModelVertex> vertices;
		vertices.reserve(vertCount);

		for(int j=0; j<vertCount; ++j)
		{
			const XMLElement_CPtr& vertexElt = vertexElts[j];
			XMLElement_CPtr positionElt = vertexElt->find_unique_child("position");
			XMLElement_CPtr normalElt = vertexElt->find_unique_child("normal");
			Vector3d position = extract_vector3d(positionElt) * SCALE;
			Vector3d normal = extract_vector3d(normalElt);

			vertices.push_back(ModelVertex(position, normal));
		}

		// Read in the texture coordinates (if present).
		bool useTexture = vertexbufferElt->has_attribute("texture_coords") && vertexbufferElt->attribute("texture_coords") == "1";

		std::vector<TexCoords> texCoords;
		if(useTexture)
		{
			texCoords.reserve(vertCount);
			for(int j=0; j<vertCount; ++j)
			{
				const XMLElement_CPtr& vertexElt = vertexElts[j];
				XMLElement_CPtr texcoordElt = vertexElt->find_unique_child("texcoord");
				texCoords.push_back(extract_texcoords(texcoordElt));
			}
		}

		// Read in the vertex bone assignments.
		XMLElement_CPtr boneassignmentsElt = submeshElt->find_unique_child("boneassignments");
		std::vector<XMLElement_CPtr> vertexboneassignmentElts = boneassignmentsElt->find_children("vertexboneassignment");
		int boneassignmentCount = static_cast<int>(vertexboneassignmentElts.size());

		for(int j=0; j<boneassignmentCount; ++j)
		{
			const XMLElement_CPtr& vbaElt = vertexboneassignmentElts[j];

			int vertIndex = lexical_cast<int>(vbaElt->attribute("vertexindex"));
			if(vertIndex < 0 || vertIndex >= vertCount)
				throw Exception("Invalid vertex index in bone assignment " + lexical_cast<std::string>(j));

			int boneIndex = lexical_cast<int>(vbaElt->attribute("boneindex"));
			double weight = lexical_cast<double>(vbaElt->attribute("weight"));

			vertices[vertIndex].add_bone_weight(BoneWeight(boneIndex, weight));
		}

		submeshes.push_back(Submesh_Ptr(new Submesh(vertIndices, vertices, material, texCoords)));
	}

	return Mesh_Ptr(new Mesh(submeshes));
}
catch(bad_lexical_cast&)
{
	throw Exception("An element attribute was not of the correct type");
}

/**
Loads the model with the specified name.

@param name	The name of the model
@return		The model
*/
Model_Ptr ModelFiles::load_model(const std::string& name)
{
	bf::path modelsDir = DirectoryFinder::instance().determine_models_directory();
	bf::path materialsPath = modelsDir / (name + ".material");
	bf::path meshPath = modelsDir / (name + ".mesh.xml");
	bf::path skeletonPath = modelsDir / (name + ".skeleton.xml");

	std::map<std::string,Material_Ptr> materials = load_materials(materialsPath.file_string());
	Mesh_Ptr mesh = load_mesh(meshPath.file_string(), materials);
	Skeleton_Ptr skeleton = load_skeleton(skeletonPath.file_string());

	return Model_Ptr(new Model(mesh, skeleton));
}

/**
Loads a skeleton from the specified Ogre skeleton file.

@param filename	The name of the file
@return			The skeleton
*/
Skeleton_Ptr ModelFiles::load_skeleton(const std::string& filename)
try
{
	XMLLexer_Ptr lexer(new XMLLexer(filename));
	XMLParser parser(lexer);
	XMLElement_CPtr root = parser.parse();
	XMLElement_CPtr skeletonElt = root->find_unique_child("skeleton");

	// Load in the bones.
	XMLElement_CPtr bonesElt = skeletonElt->find_unique_child("bones");

	std::vector<XMLElement_CPtr> boneElts = bonesElt->find_children("bone");
	int boneCount = static_cast<int>(boneElts.size());

	std::vector<Bone_Ptr> bones(boneCount);
	for(int i=0; i<boneCount; ++i)
	{
		// Extract all the necessary bits of information from the XML tree below each bone, then construct the bone itself.
		const XMLElement_CPtr& boneElt = boneElts[i];

		int id = lexical_cast<int>(boneElt->attribute("id"));

		std::string name = boneElt->attribute("name");

		XMLElement_CPtr positionElt = boneElt->find_unique_child("position");
		Vector3d position = extract_vector3d(positionElt) * SCALE;

		XMLElement_CPtr rotationElt = boneElt->find_unique_child("rotation");

		double rotationAngle = lexical_cast<double>(rotationElt->attribute("angle"));

		XMLElement_CPtr axisElt = rotationElt->find_unique_child("axis");
		Vector3d rotationAxis = extract_vector3d(axisElt);

		bones[id].reset(new Bone(name, position, rotationAxis, rotationAngle));
	}

	// Construct the bone hierarchy.
	BoneHierarchy_Ptr boneHierarchy(new BoneHierarchy(bones));

	// Load in the bone parents.
	XMLElement_CPtr bonehierarchyElt = skeletonElt->find_unique_child("bonehierarchy");
	std::vector<XMLElement_CPtr> boneparentElts = bonehierarchyElt->find_children("boneparent");
	int boneparentCount = static_cast<int>(boneparentElts.size());
	for(int i=0; i<boneparentCount; ++i)
	{
		const XMLElement_CPtr& boneparentElt = boneparentElts[i];
		std::string childName = boneparentElt->attribute("bone");
		std::string parentName = boneparentElt->attribute("parent");
		Bone_Ptr child = boneHierarchy->bones(childName);
		Bone_Ptr parent = boneHierarchy->bones(parentName);
		child->set_parent(parent);
	}

	// Load in the animations (if any).
	std::map<std::string,Animation_CPtr> animations;

	if(skeletonElt->has_child("animations"))
	{
		XMLElement_CPtr animationsElt = skeletonElt->find_unique_child("animations");
		std::vector<XMLElement_CPtr> animationElts = animationsElt->find_children("animation");
		int animationCount = static_cast<int>(animationElts.size());

		for(int i=0; i<animationCount; ++i)
		{
			const XMLElement_CPtr& animationElt = animationElts[i];

			// Load in the tracks for the bones.
			XMLElement_CPtr tracksElt = animationElt->find_unique_child("tracks");
			std::vector<XMLElement_CPtr> trackElts = tracksElt->find_children("track");
			int trackCount = static_cast<int>(trackElts.size());
			int keyframeCount = 0;

			typedef std::vector<RBTMatrix_Ptr> Track;
			std::map<std::string,Track> tracks;

			for(int j=0; j<trackCount; ++j)
			{
				const XMLElement_CPtr& trackElt = trackElts[j];

				std::string bone = trackElt->attribute("bone");

				// Read in the bone keyframes for this particular bone.
				XMLElement_CPtr keyframesElt = trackElt->find_unique_child("keyframes");
				std::vector<XMLElement_CPtr> keyframeElts = keyframesElt->find_children("keyframe");
				keyframeCount = static_cast<int>(keyframeElts.size());

				Track track(keyframeCount);
				for(int k=0; k<keyframeCount; ++k)
				{
					const XMLElement_CPtr& keyframeElt = keyframeElts[k];

					XMLElement_CPtr translateElt = keyframeElt->find_unique_child("translate");
					Vector3d translation = extract_vector3d(translateElt) * SCALE;

					XMLElement_CPtr rotateElt = keyframeElt->find_unique_child("rotate");
					double rotateAngle = lexical_cast<double>(rotateElt->attribute("angle"));
					XMLElement_CPtr axisElt = rotateElt->find_unique_child("axis");
					Vector3d rotateAxis = extract_vector3d(axisElt);

					// TODO: Make use of scale here as well if necessary.

					track[k] = RBTMatrix::from_axis_angle_translation(rotateAxis, rotateAngle, translation);
				}
				tracks.insert(std::make_pair(bone,track));
			}

			// Check that each track has the same number of bone keyframes.
			for(std::map<std::string,Track>::const_iterator kt=tracks.begin(), kend=tracks.end(); kt!=kend; ++kt)
			{
				const Track& track = kt->second;
				if(track.size() != keyframeCount) throw Exception("Bad track length");
			}

			// Use the tracks to create the *model* keyframes (note: these are distinct from the bone keyframes!).
			std::vector<Pose_CPtr> keyframes(keyframeCount);
			for(int j=0; j<keyframeCount; ++j)
			{
				std::vector<RBTMatrix_CPtr> boneMatrices(boneCount);
				for(int k=0; k<boneCount; ++k)
				{
					Bone_CPtr bone = boneHierarchy->bones(k);
					std::map<std::string,Track>::const_iterator kt = tracks.find(bone->name());
					if(kt != tracks.end())
					{
						// If there's an animation track for this bone, use the track matrix.
						const Track& track = kt->second;
						boneMatrices[k] = track[j];
					}
					else
					{
						// Otherwise, use the identity matrix.
						boneMatrices[k] = RBTMatrix::identity();
					}
				}

				keyframes[j].reset(new Pose(boneMatrices));
			}

			std::string name = animationElt->attribute("name");
			double length = lexical_cast<double>(animationElt->attribute("length"));
			Animation_CPtr animation(new Animation(length, keyframes));
			animations.insert(std::make_pair(name, animation));
		}
	}

	return Skeleton_Ptr(new Skeleton(boneHierarchy, animations));
}
catch(bad_lexical_cast&)
{
	throw Exception("An element attribute was not of the correct type");
}

//#################### LOADING SUPPORT METHODS ####################
/**
Extracts (u,v) texture coordinates from the specified XML element.

@param elt	The XML element
@return		The (u,v) texture coordinates
*/
TexCoords ModelFiles::extract_texcoords(const XMLElement_CPtr& elt)
{
	double u = lexical_cast<double>(elt->attribute("u"));
	double v = lexical_cast<double>(elt->attribute("v"));
	return TexCoords(u,v);
}

/**
Extracts a 3D vector from the specified XML element.

@param elt	The XML element
@return		The 3D vector
*/
Vector3d ModelFiles::extract_vector3d(const XMLElement_CPtr& elt)
{
	double x = lexical_cast<double>(elt->attribute("x"));
	double y = lexical_cast<double>(elt->attribute("y"));
	double z = lexical_cast<double>(elt->attribute("z"));
	return Vector3d(x,y,z);
}

ModelFiles::NamedMaterial_Ptr ModelFiles::read_material(std::istream& is)
{
	// Note:	We don't make use of most of the information for each material - we're only interested in certain bits,
	//			which is why the code might look strange.

	std::string line;
	if(!LineIO::portable_getline(is, line)) return NamedMaterial_Ptr();

	NamedMaterial_Ptr ret;

	if(line.substr(0,9) != "material ") throw Exception("Expected material");
	std::string name = line.substr(9);

	LineIO::read_checked_line(is, "{");

	bool done = false;
	int bracketCount = 1;
	for(;;)
	{
		LineIO::read_trimmed_line(is, line, "material");

		if(line == "{")
		{
			++bracketCount;
		}
		if(line == "}")
		{
			--bracketCount;
			if(bracketCount == 0) break;
		}
		else if(line == "technique")
		{
			Material_Ptr material = read_technique(is);
			if(done)
			{
				std::cerr << "Ignoring extra techniques for material " << name << std::endl;
			}
			else
			{
				ret.reset(new NamedMaterial(name, material));
				done = true;
			}
		}
	}

	return ret;
}

Material_Ptr ModelFiles::read_pass(std::istream& is)
{
	Material_Ptr ret;

	LineIO::read_checked_trimmed_line(is, "{");

	std::string line;

	bool useTexture = false;
	Colour3d ambient, diffuse, emissive, specular;
	double specularExponent = 0;
	std::string textureFilename;

	int bracketCount = 1;
	for(;;)
	{
		LineIO::read_trimmed_line(is, line, "pass");

		if(line == "{")
		{
			++bracketCount;
		}
		else if(line == "}")
		{
			--bracketCount;
			if(bracketCount == 0) break;
		}
		else if(line == "ambient")
		{
			// TODO
		}
		else if(line == "diffuse")
		{
			// TODO
		}
		else if(line == "emissive")
		{
			// TODO
		}
		else if(line == "specular")
		{
			// TODO
		}
		else if(line == "texture_unit")
		{
			useTexture = true;

			LineIO::read_checked_trimmed_line(is, "{");

			LineIO::read_trimmed_line(is, line, "texture");
			if(line.substr(0,8) != "texture ") throw Exception("Expected texture");
			textureFilename = line.substr(8);

			LineIO::read_checked_trimmed_line(is, "}");
		}
	}

	if(useTexture)
	{
		bf::path modelsDir = DirectoryFinder::instance().determine_models_directory();
		Texture_Ptr texture = TextureFactory::create_texture24(ImageLoader::load_image24((modelsDir / textureFilename).file_string()));
		ret.reset(new TextureMaterial(texture));
	}
	else
	{
		// FIXME: Load the material colours in properly above.
#if 0
		ret.reset(new BasicMaterial(ambient, diffuse, specular, specularExponent, emissive, true));
#else
		ret.reset(new BasicMaterial(Colour3d(0.5,0.5,0.5), Colour3d(0.8,0.8,0.64), Colour3d(0.5,0.5,0.5), 12.5, Colour3d(0,0,0), true));
#endif
	}

	return ret;
}

Material_Ptr ModelFiles::read_technique(std::istream& is)
{
	Material_Ptr ret;

	LineIO::read_checked_trimmed_line(is, "{");

	std::string line;

	bool done = false;
	int bracketCount = 1;
	for(;;)
	{
		LineIO::read_trimmed_line(is, line, "technique");

		if(line == "{")
		{
			++bracketCount;
		}
		if(line == "}")
		{
			--bracketCount;
			if(bracketCount == 0) break;
		}
		else if(line == "pass")
		{
			Material_Ptr material = read_pass(is);
			if(done)
			{
				std::cerr << "Ignoring extra passes for technique" << std::endl;
			}
			else
			{
				ret = material;
				done = true;
			}
		}
	}

	return ret;
}

}
