/***
 * hesperus: Submesh.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Submesh.h"

#include <hesp/materials/Material.h>
#include <hesp/math/matrices/RBTMatrix.h>
#include "BoneHierarchy.h"
#include "ConfiguredBone.h"
#include "ConfiguredPose.h"
#include "Skeleton.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Submesh::Submesh(const std::vector<unsigned int>& vertIndices, const std::vector<ModelVertex>& vertices,
				 const Material_Ptr& material, const std::vector<TexCoords>& texCoords)
:	m_vertIndices(vertIndices), m_vertices(vertices), m_vertArray(vertices.size() * 3), m_material(material)
{
	if(m_material->uses_texcoords())
	{
		// Construct the tex coord array.
		m_texCoordArray.resize(texCoords.size() * 2);

		int texCoordCount = static_cast<int>(texCoords.size());
		for(int i=0, offset=0; i<texCoordCount; ++i, offset+=2)
		{
			m_texCoordArray[offset] = texCoords[i].u;
			m_texCoordArray[offset+1] = texCoords[i].v;
		}
	}
}

//#################### PUBLIC METHODS ####################
void Submesh::render() const
{
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, &m_vertArray[0]);

	if(m_material->uses_texcoords())
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_DOUBLE, 0, &m_texCoordArray[0]);
	}

	m_material->apply();

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_vertIndices.size()), GL_UNSIGNED_INT, &m_vertIndices[0]);

	glPopAttrib();
	glPopClientAttrib();
}

void Submesh::skin(const Skeleton_CPtr& skeleton)
{
	/*
	Linear Blend Skinning Algorithm:

	P = (\sum_i w_i * M_i * M_{0,i}^{-1}) * P_0 / (sum i w_i)

	Each M_{0,i}^{-1} matrix gets P_0 (the rest vertex) into its corresponding bone's coordinate frame.
	We construct matrices M_n * M_{0,n}^-1 for each n in advance to avoid repeating calculations.
	I refer to these in the code as the 'skinning matrices'.
	*/

	BoneHierarchy_CPtr boneHierarchy = skeleton->bone_hierarchy();
	ConfiguredPose_CPtr pose = skeleton->get_pose();
	int boneCount = boneHierarchy->bone_count();

	// Construct the skinning matrices.
	std::vector<RBTMatrix_CPtr> skinningMatrices(boneCount);
	for(int i=0; i<boneCount; ++i)
	{
		skinningMatrices[i] = pose->bones(i)->absolute_matrix() * skeleton->to_bone_matrix(i);
	}

	// Build the vertex array.
	RBTMatrix_Ptr m = RBTMatrix::zeros();		// used as an accumulator for \sum_i w_i * M_i * M_{0,i}^{-1}

	int vertCount = static_cast<int>(m_vertices.size());
	for(int i=0, offset=0; i<vertCount; ++i, offset+=3)
	{
		const Vector3d& p0 = m_vertices[i].position();
		const std::vector<BoneWeight>& boneWeights = m_vertices[i].bone_weights();
		int boneWeightCount = static_cast<int>(boneWeights.size());

		Vector3d p;
		if(boneWeightCount != 0)
		{
			double boneWeightSum = 0;

			for(int j=0; j<boneWeightCount; ++j)
			{
				int boneIndex = boneWeights[j].bone_index();
				double boneWeight = boneWeights[j].weight();
				boneWeightSum += boneWeight;
				m->add_scaled(skinningMatrices[boneIndex], boneWeight);
			}

			// Note: This is effectively p = m*p0 (if we think of p0 as (p0.x, p0.y, p0.z, 1)).
			p = m->apply_to_point(p0);
			p /= boneWeightSum;

			// Reset the accumulator matrix ready for the next vertex.
			m->reset_to_zeros();
		}
		else
		{
			// If this vertex is unaffected by the armature (i.e. no bone weights have been assigned to it),
			// use its rest position as its real position (it's the best we can do).
			p = p0;
		}

		m_vertArray[offset] = p.x;
		m_vertArray[offset+1] = p.y;
		m_vertArray[offset+2] = p.z;
	}
}

}
